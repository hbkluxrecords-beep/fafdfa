#pragma once
#include "../helper/visual_help.h"
#include "../../world/engine/engine.h"
#include "../../world/thread/thread.h"
#include <unordered_map>
#include <sstream>
#include <vector>
#include <atomic>
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

struct EspCache {
    bool isDead = false;
    Vector3 lastDeathPos = { 0, 0, 0 };
};

static std::unordered_map<int, EspCache> espCache;

struct Scoreboard {
    int status;
    int isBot;
    int kills;
    int deaths;
};

auto ConvertDistanceToString(float dist) -> std::string
{
    std::stringstream strs;
    strs << dist;
    std::string temp_str = strs.str();
    const char* text = (const char*)temp_str.c_str();
    return (std::string)text + "M";
}

bool get_bbox(Vector3 ent_position, Vector4& box) {
    Vector2 headScreen, feetScreen;
    Vector3 headPos = ent_position;
    headPos.z += 68.0f;

    Vector3 feetPos = ent_position;
    if (!engine::client::w2s(headPos, headScreen) || !engine::client::w2s(feetPos, feetScreen)) {
        return false;
    }

    float height = abs(headScreen.y - feetScreen.y);
    float width = height * 0.45f;

    if (height <= 0 || width <= 0) {
        return false;
    }

    box.x = headScreen.x - width / 2;
    box.y = headScreen.y;
    box.z = width;
    box.w = height;

    return true;
}

template<typename EntityType>
bool IsEspValid(int index, const EntityType& ent)
{
    if (index == ent.local_player) return false;

    Vector3 currentPos = engine::player::get_position(ent.player);

    if (currentPos.x == 0.0f && currentPos.y == 0.0f && currentPos.z == 0.0f)
        return false;

    if (!ent.bone_ptr)
        return false;

    if (engine::player::team_id_check(ent.player) == engine::player::team_id_check(ent.local_player) && !visual::team_esp)
        return false;

    Scoreboard scoreboard;
    scoreboard.status = 0;
    scoreboard.isBot = 0;
    scoreboard.kills = 0;
    scoreboard.deaths = 0;

    uintptr_t scoreboard_address = offsets::scoreboard + index * offsets::scoreboard_size;
    if (scoreboard_address != 0) {
        scoreboard = request->read<Scoreboard>(scoreboard_address);
    }

    if (scoreboard.status == 2 || scoreboard.status == 3)
    {
        return false;
    }

    if (scoreboard.isBot != 0 && scoreboard.kills == 0 && scoreboard.deaths > 0)
        return false;

    return true;
}

void actor_loop() {
    if (!aim::is_aiming)
        engine::aimm::reset_lock();

    for (const auto& ent : ent_t) {
        if (!IsEspValid(ent.i, ent)) continue;

        Vector3 ent_position = engine::player::get_position(ent.player);
        auto ent_distance = engine::utils::units_to_m(engine::player::get_position(ent.local_player).distance_to(ent_position));
        bool is_target = (ent.i == current_target_index);

        Vector4 bbox{};
        get_bbox(ent_position, bbox);

        ImColor skeleton_color, box_color, distance_color, compass_color, name_color, snapline_color;
        skeleton_color = engine::utils::get_visibility_color(ent.is_visible, is_target, colors::skeleton_vis, colors::skeleton_invis, colors::aim_target);
        box_color = engine::utils::get_visibility_color(ent.is_visible, is_target, colors::box_vis, colors::box_invis, colors::aim_target);
        compass_color = engine::utils::get_visibility_color(ent.is_visible, is_target, colors::compass_vis, colors::compass_invis, colors::aim_target);
        name_color = engine::utils::get_visibility_color(ent.is_visible, is_target, colors::name_vis, colors::name_invis, colors::aim_target);
        distance_color = engine::utils::get_visibility_color(ent.is_visible, is_target, colors::distance_vis, colors::distance_invis, colors::aim_target);
        snapline_color = engine::utils::get_visibility_color(ent.is_visible, is_target, colors::snapline_vis, colors::snapline_invis, colors::aim_target);

        if (bbox.x <= 0 || bbox.y <= 0 || bbox.z <= 0 || bbox.w <= 0) continue;

        if (ent_distance < visual::max_render_distance) {
            if (visual::box) {
                if (visual::box_type == 0) {
                    if (visual::outlined) {
                        draw.draw_box(bbox.x, bbox.y, bbox.z, bbox.w, ImColor(0, 0, 0, 255), 2.5f);
                    }
                    draw.draw_box(bbox.x, bbox.y, bbox.z, bbox.w, box_color, 1.f);
                }
                else if (visual::box_type == 1) {
                    if (visual::outlined) {
                        draw.draw_cornered_box(bbox.x, bbox.y, bbox.z, bbox.w, ImColor(0, 0, 0, 255), 2.5f);
                    }
                    draw.draw_cornered_box(bbox.x, bbox.y, bbox.z, bbox.w, box_color, 1.f);
                }
                else if (visual::box_type == 1) {
                    draw.rect_filled(bbox.x, bbox.y, bbox.z, bbox.w, ImColor(box_color.Value.x, box_color.Value.y, box_color.Value.z, 100 / 255.f), 0.f, 0);
                }
            }

            if (visual::health) {
                help.draw_health_bar(engine::player::get_name_entry(ent.i).health, bbox);
            }

            if (visual::distance) {
                draw.string(ImGui::GetFontSize(), bbox.x + bbox.z / 2 - (ImGui::CalcTextSize(ConvertDistanceToString((int)ent_distance).c_str()).x / 2), bbox.y + bbox.w, distance_color, false, true, ConvertDistanceToString((int)ent_distance).c_str());
            }

            if (visual::name) {
                draw.string(ImGui::GetFontSize(), bbox.x + bbox.z / 2 - (ImGui::CalcTextSize(engine::player::get_name_entry(ent.i).name).x / 2), bbox.y - ImGui::GetFontSize(), name_color, false, true, engine::player::get_name_entry(ent.i).name);
            }

            if (visual::skeletons) {
                auto bone_positions = engine::bones::get_multiple_bones(ent.bone_ptr, ent.bone_pos, { 7, 2, 5, 6, 10, 11, 12, 14, 15, 16, 18, 19, 22, 23 });

                help.draw_bone(bone_positions[3], bone_positions[0], ent_position, skeleton_color);
                help.draw_bone(bone_positions[1], bone_positions[3], ent_position, skeleton_color);
                help.draw_bone(bone_positions[1], bone_positions[10], ent_position, skeleton_color);
                help.draw_bone(bone_positions[10], bone_positions[11], ent_position, skeleton_color);
                help.draw_bone(bone_positions[1], bone_positions[12], ent_position, skeleton_color);
                help.draw_bone(bone_positions[12], bone_positions[13], ent_position, skeleton_color);
                help.draw_bone(bone_positions[3], bone_positions[4], ent_position, skeleton_color);
                help.draw_bone(bone_positions[4], bone_positions[5], ent_position, skeleton_color);
                help.draw_bone(bone_positions[5], bone_positions[6], ent_position, skeleton_color);
                help.draw_bone(bone_positions[3], bone_positions[7], ent_position, skeleton_color);
                help.draw_bone(bone_positions[7], bone_positions[8], ent_position, skeleton_color);
                help.draw_bone(bone_positions[8], bone_positions[9], ent_position, skeleton_color);
            }

            if (visual::compass) {
                if (visual::compass_style == 0) {
                    help.draw_compass_crushed(ImVec2(bbox.x, bbox.y), compass_color, visual::compass_filled, false, 0);
                }
                else if (visual::compass_style == 1) {
                    help.draw_compass_pointed(ImVec2(bbox.x, bbox.y), compass_color, visual::compass_filled, false, 0);
                }
            }
        }

        if (aim::aim) {
            if (aim::vis_check && !ent.is_visible) continue;

            Vector3 bone_position;

            if (aim::aim_bone == 0)
                bone_position = engine::bones::get_bone(ent.bone_ptr, ent.bone_pos, 7);
            else if (aim::aim_bone == 1)
                bone_position = engine::bones::get_bone(ent.bone_ptr, ent.bone_pos, 6);
            else if (aim::aim_bone == 2)
                bone_position = engine::bones::get_bone(ent.bone_ptr, ent.bone_pos, 5);
            else if (aim::aim_bone == 3)
                bone_position = engine::bones::get_bone(ent.bone_ptr, ent.bone_pos, 2);

            Vector2 bone_screen;
            if (engine::client::w2s(bone_position, bone_screen)) {
                if (aim::hitbox_circle) {
                    ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(bone_screen.x, bone_screen.y), 60.f / ent_distance, ImColor(colors::hitbox_circle[0], colors::hitbox_circle[1], colors::hitbox_circle[2], colors::hitbox_circle[3]), 128, 2.f);
                }
                if (aim::hitbox_snapline) {
                    ImGui::GetBackgroundDrawList()->AddLine(ImVec2(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2), ImVec2(bone_screen.x, bone_screen.y), ImColor(colors::hitbox_snapline[0], colors::hitbox_snapline[1], colors::hitbox_snapline[2], colors::hitbox_snapline[3]), 1.f);
                }
                engine::aimm::get_target_in_fov(bone_screen, ent.i);
            }
        }
    }
    engine::aimm::lock_on_target();
}