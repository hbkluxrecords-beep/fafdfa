#pragma once
#include "../engine/engine.h"
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <atomic>

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

std::vector<game_info> game_info_s_temp;
std::vector<game_info> game_info_s;
std::vector<entity_s> ent_t;
std::vector<entity_s> ent_t_temp;
std::vector<decrypt_game> decrypt_t;
std::vector<decrypt_game> decrypt_t_temp;
std::mutex decrypt_mutex;
std::mutex ent_mutex;
std::mutex bones_mutex;

void cache_00_optimized() {
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL);
    decrypt_game cached_game{};
    bool has_valid_cache = false;
    constexpr auto CACHE_INTERVAL = std::chrono::seconds(30);
    auto last_cache_time = std::chrono::steady_clock::now() - CACHE_INTERVAL;

    for (;;) {
        if (!engine::client::is_user_in_game()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            continue;
        }

        auto now = std::chrono::steady_clock::now();
        if (now - last_cache_time >= CACHE_INTERVAL) {
            cached_game.client_info = decrypt_client_info();
            cached_game.client_base = decrypt_client_base(cached_game.client_info);
            cached_game.bone_base = decrypt_bone_base();
            bool is_valid = (cached_game.client_info > 0 && cached_game.client_base > 0 && cached_game.bone_base > 0);

            {
                std::lock_guard<std::mutex> lock(decrypt_mutex);
                if (is_valid) {
                    decrypt_t.clear();
                    decrypt_t.push_back(cached_game);
                    has_valid_cache = true;
                }
                else {
                    decrypt_t_temp.clear();
                    decrypt_t_temp.push_back(cached_game);
                    decrypt_t.swap(decrypt_t_temp);
                    has_valid_cache = false;
                }
            }
            last_cache_time = now;
        }

        std::this_thread::sleep_for(has_valid_cache ? std::chrono::seconds(5) : std::chrono::seconds(2));
    }
}

void cache_01_optimized() {
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
    bool ingame = false;
    int player_count = 0;
    int index = -1;
    uintptr_t local_player = 0;
    constexpr auto STATE_UPDATE_INTERVAL = std::chrono::seconds(1);
    auto last_update = std::chrono::steady_clock::now() - STATE_UPDATE_INTERVAL;
    constexpr auto ENT_UPDATE_INTERVAL = std::chrono::milliseconds(8);
    auto last_ent_update = std::chrono::steady_clock::now() - ENT_UPDATE_INTERVAL;

    std::vector<entity_s> ent_temp_local;
    ent_temp_local.reserve(150);
    const auto player_size = offsets::player::size;

    for (;;) {
        auto now = std::chrono::steady_clock::now();

        if (now - last_update >= STATE_UPDATE_INTERVAL) {
            ingame = engine::client::is_user_in_game();
            if (ingame) {
                player_count = engine::client::player_count();
                decrypt_game game_copy;
                {
                    std::lock_guard<std::mutex> lock(decrypt_mutex);
                    if (!decrypt_t.empty()) {
                        game_copy = decrypt_t[0];
                        index = engine::player::local_player_index(game_copy.client_base, game_copy.client_info);
                        local_player = (index >= 0) ? game_copy.client_base + (index * offsets::player::size) : 0;
                    }
                }
            }
            else {
                player_count = 0;
                index = -1;
                local_player = 0;
            }
            last_update = now;
        }

        if (!ingame || player_count <= 0 || index < 0 || local_player == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        if (now - last_ent_update < ENT_UPDATE_INTERVAL) {
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
            continue;
        }

        decrypt_game game_copy;
        {
            std::lock_guard<std::mutex> lock(decrypt_mutex);
            if (decrypt_t.empty()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(16));
                continue;
            }
            game_copy = decrypt_t[0];
        }

        ent_temp_local.clear();
        if (ent_temp_local.capacity() < static_cast<size_t>(player_count))
            ent_temp_local.reserve(player_count + 16);

        for (int i = 0; i < player_count; ++i) {
            auto player = game_copy.client_base + (i * player_size);
            auto bone_index = get_bone_index(i);
            auto bone_ptr = engine::bones::bone_pointer(game_copy.bone_base, bone_index);
            if (!bone_ptr) continue;

            entity_s ent_new{};
            ent_new.player = player;
            ent_new.local_player = local_player;
            ent_new.is_visible = engine::player::is_visible(i, game_copy.client_info);
            ent_new.bone_ptr = bone_ptr;
            ent_new.bone_pos = engine::bones::retrieve_bone_position_vec(game_copy.client_info);
            ent_new.i = i;
            ent_temp_local.push_back(ent_new);
        }

        {
            std::lock_guard<std::mutex> lock(ent_mutex);
            ent_t.swap(ent_temp_local);
        }

        last_ent_update = now;
        std::this_thread::sleep_for(std::chrono::milliseconds(4));
    }
}

void cache_02_optimized() {
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
    constexpr auto REFDEF_INTERVAL = std::chrono::milliseconds(8);
    uintptr_t cached_ref_def_addr = 0;
    auto last_addr_update = std::chrono::steady_clock::now();
    auto last_read = std::chrono::steady_clock::now();
    constexpr auto ADDR_CACHE_DURATION = std::chrono::seconds(1);

    for (;;) {
        auto now = std::chrono::steady_clock::now();

        if (now - last_addr_update >= ADDR_CACHE_DURATION) {
            cached_ref_def_addr = decrypt_refdef->retrieve_ref_def();
            last_addr_update = now;
        }

        if (now - last_read >= REFDEF_INTERVAL && cached_ref_def_addr != 0) {
            try {
                decrypt_refdef->ref_def_nn = request->read<ref_def_t>(cached_ref_def_addr);
                last_read = now;
            }
            catch (...) {}
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(4));
    }
}

inline void start_optimized_cache_threads() {
    std::thread(cache_00_optimized).detach();
    std::thread(cache_01_optimized).detach();
    std::thread(cache_02_optimized).detach();
}
