#pragma once
#include <cstdint>
#include <iostream>
#include <math.h>
#include "../../utils/kernel/driver.h"
#include "../../utils/math/vectors.h"
#include "lele_core.h"
#include "../helper/classes/classes.h"
#include "../../utils/global.h"
#include "../../utils/mouse/mouse.h"
#include "../../framework/imgui/imgui.h"
#include <chrono>
#include "offsets_final.h"

#define M_PI       3.14159265358979323846 

std::atomic<int> current_target_index { -1 };


class entity_s {
public:
    Vector3 bone_pos;
    uint64_t bone_ptr;
    bool is_visible;
    unsigned long long player;
    unsigned long long local_player;
    int i;
};

class decrypt_game {
public:
    uintptr_t client_info;
    uintptr_t client_base;
    int index;
    unsigned long long local_player;
    uintptr_t bone_base;
};

class game_info {
public:
    bool is_ingame;
    int player_count;
    uintptr_t client_info;
    uintptr_t client_base;
    int index;
    unsigned long long local_player;
    uintptr_t bone_base;
    Vector3 bone_pos;
};



namespace engine {

    namespace utils {
        void vec_to_angle ( const Vector3 vec, angle_t& angle )
        {
            if ( vec.x == 0 && vec.y == 0 )
            {
                angle.x = vec.z > 0.0f ? -90.f : 90.f;
                angle.y = 0;
            }
            else
            {
                angle.x = static_cast< float >( atan2 ( -vec.z, std::hypot ( vec.x, vec.y ) ) * ( 180.f / M_PI ) );
                angle.y = static_cast< float >( atan2 ( vec.y, vec.x ) * ( 180.f / M_PI ) );


                angle.y = fmodf ( angle.y + 360.f, 360.f );
            }
            angle.z = 0;
        }

        auto units_to_m ( float units ) -> float {
            return units * 0.0254;
        }

        ImColor get_visibility_color ( bool is_visible, bool is_target, const float visible_color [ 4 ], const float invisible_color [ 4 ], const float target_color [ 4 ] ) {
            if ( is_target && visual::aim_target) {
                return ImColor ( target_color [ 0 ], target_color [ 1 ], target_color [ 2 ], target_color [ 3 ] );
            }
            else if ( is_visible && visual::visiblity_check ) {
                return ImColor ( visible_color [ 0 ], visible_color [ 1 ], visible_color [ 2 ], visible_color [ 3 ] );
            }
            else {
                return ImColor ( invisible_color [ 0 ], invisible_color [ 1 ], invisible_color [ 2 ], invisible_color [ 3 ] );
            }
        }
    }


    namespace client {
        inline auto is_user_in_game ( ) -> bool
        {
            return request->read<int> ( request->image_base + offsets::game_mode ) > 1;
        }

        inline auto player_count ( ) -> int
        {
            int playercount = request->read<int> ( request->image_base + offsets::game_mode );

            return playercount;
        }

        auto retrieve_camera_posistion ( ) -> Vector3
        {
            auto player_camera = request->read<uintptr_t> ( request->image_base + offsets::camera_base );
            if ( !player_camera )
                return {};

            Vector3 received = request->read<Vector3> ( player_camera + offsets::camera_pos );
            return received;
        }
        auto get_angles ( ) -> Vector2
        {
            auto camera_angle = request->read<uintptr_t> ( request->image_base + offsets::camera_base );
            if ( !camera_angle )
                return {};

            Vector2 received = request->read<Vector2> ( camera_angle + offsets::camera_pos + 0xC );
            return received;
        }

        auto world_to_screen ( Vector3 world_location, Vector2& out, Vector3 camera_pos, int screen_width, int screen_height, Vector2 fov, Vector3 matricies [ 3 ] ) -> bool
        {
            auto local = world_location - camera_pos;


            auto trans = Vector3 {
                local.Dot ( matricies [ 1 ] ),
                local.Dot ( matricies [ 2 ] ),
                local.Dot ( matricies [ 0 ] )
            };

            if ( trans.z < 0.01f ) {
                return false;
            }

            out.x = ( ( float ) screen_width / 2.0 ) * ( 1.0 - ( trans.x / fov.x / trans.z ) );
            out.y = ( ( float ) screen_height / 2.0 ) * ( 1.0 - ( trans.y / fov.y / trans.z ) );

            if ( out.x < 1 || out.y < 1 || ( out.x > decrypt_refdef->ref_def_nn.width ) || ( out.y > decrypt_refdef->ref_def_nn.height ) ) {
                return false;
            }

            return true;
        }

        auto w2s ( Vector3 world_position, Vector2& screen_position ) -> bool
        {
            return world_to_screen ( world_position, screen_position, retrieve_camera_posistion ( ), decrypt_refdef->ref_def_nn.width, decrypt_refdef->ref_def_nn.height, decrypt_refdef->ref_def_nn.tan_half_fov, decrypt_refdef->ref_def_nn.axis );
        }

        angle_t get_local_angle ( ) {
            auto refdef = decrypt_refdef->ref_def_nn;
            angle_t local_angles;
            engine::utils::vec_to_angle ( refdef.axis [ 0 ], local_angles );
            local_angles.z = 0;

            return local_angles;
        }

        uintptr_t get_stance ( uintptr_t player ) {
            return request->read<uintptr_t> ( player + offsets::player::stance );
        }

    }

    namespace bones {
        auto get_bone ( const uintptr_t pointer, const Vector3& bone_position, const int Bone ) -> Vector3
        {
            Vector3 position = request->read<Vector3> ( pointer + ( ( uint64_t ) Bone * 0x20 ) + 0x10 );
            position.x += bone_position.x;
            position.y += bone_position.y;
            position.z += bone_position.z;
            return position;
        }

        std::vector<Vector3> get_multiple_bones ( const uintptr_t pointer, const Vector3& bone_position, const std::vector<int>& bone_indices ) {
            std::vector<Vector3> bone_positions;
            bone_positions.reserve ( bone_indices.size ( ) );

            for ( const auto& bone : bone_indices ) {
                bone_positions.push_back ( get_bone ( pointer, bone_position, bone ) );
            }

            return bone_positions;
        }

        auto retrieve_bone_position_vec ( const uintptr_t Client_Information ) -> Vector3
        {
            Vector3 information = request->read<Vector3> ( Client_Information + offsets::bone::bone_base );
            return information;
        }


        auto bone_pointer ( uint64_t base, uint64_t index ) -> uint64_t
        {
            auto bone = request->read<uintptr_t> ( base + ( index * offsets::bone::size ) + offsets::bone::offset );
            return bone;
        }

        bool bones_to_screen ( const Vector3* BonePosArray, Vector2* ScreenPosArray, const long Count )
        {
            for ( long i = 0; i < Count; ++i )
            {
                if ( !client::w2s ( BonePosArray [ i ], ScreenPosArray [ i ] ) )
                    return false;
            }
            return true;
        }
    }

    namespace player {
        inline auto is_player_valid ( QWORD player_address ) -> bool
        {
            bool response = request->read<bool> ( player_address + offsets::player::valid );
            return response;
        }

        inline auto local_player_index ( QWORD client_base, QWORD client_info ) -> int
        {
            auto index = request->read<uintptr_t> ( client_info + offsets::local_index );
            int read_return = request->read<int> ( index + offsets::local_index_pos );
            return read_return;
        }

        inline auto get_position ( QWORD player_address ) -> Vector3
        {
            auto local_position = request->read<uintptr_t> ( player_address + offsets::player::pos );
            Vector3 final_pos = request->read<Vector3> ( local_position + 0x70);
            return final_pos;
        }

        int is_visible ( uint32_t p_index, uintptr_t client_info )
        {
            auto sighted_enemy_fools = request->read<client_bits_t> (client_info + offsets::visible_bit);
            auto bitmask = 0x80000000 >> ( p_index & 0x1F );
            return sighted_enemy_fools.array [ p_index >> 5 ] & bitmask;
        }

        uint8_t team_id_check ( uintptr_t player ) {
            return request->read<uint8_t> ( player + offsets::player::team );
        }

        uint64_t get_name_list ( ) {
            auto ptr = request->read<uint64_t> ( request->image_base + offsets::name_array );
            return ptr + offsets::name_array_pos;
        }

        name_entry get_name_entry ( uint32_t index ) {
            return request->read<name_entry> ( get_name_list ( ) + ( index * offsets::name_size ) );
        }
    }


    namespace aimm {
        constexpr float init_center_dist = FLT_MAX;
        float min_center_dist = init_center_dist;
        Vector2 lock_position;

        __forceinline void reset_lock ( )
        {
            min_center_dist = init_center_dist;
        }


        void get_target_in_fov ( Vector2 target_pos, int entity_index )
        {
            const float x = target_pos.x - ( decrypt_refdef->ref_def_nn.width / 2 );
            const float y = target_pos.y - ( decrypt_refdef->ref_def_nn.height / 2 );
            float center_dis = sqrt ( pow ( y, 2 ) + pow ( x, 2 ) );

            if ( center_dis < min_center_dist && center_dis <= aim::fov )
            {
                min_center_dist = center_dis;
                lock_position = target_pos;
                current_target_index = entity_index;
            }
        }

        void get_target_in_fov_closest ( entity_s ent, int entity_index )
        {
            const float screen_center_x = decrypt_refdef->ref_def_nn.width / 2;
            const float screen_center_y = decrypt_refdef->ref_def_nn.height / 2;

            float closest_bone_distance = FLT_MAX;
            Vector2 closest_bone_position;

            std::vector<int> bones = { 7, 6, 5, 2 };

            for ( int bone : bones ) {
                Vector3 bone_world_position = engine::bones::get_bone ( ent.bone_ptr, ent.bone_pos, bone );

                Vector2 bone_screen_position;
                if ( !engine::client::w2s ( bone_world_position, bone_screen_position ) ) {
                    continue;
                }

                float x = bone_screen_position.x - screen_center_x;
                float y = bone_screen_position.y - screen_center_y;
                float distance = sqrt ( pow ( x, 2 ) + pow ( y, 2 ) );

                if ( distance < closest_bone_distance && distance <= aim::fov ) {
                    closest_bone_distance = distance;
                    closest_bone_position = bone_screen_position;
                }
            }

            if ( closest_bone_distance < min_center_dist ) {
                min_center_dist = closest_bone_distance;
                lock_position = closest_bone_position;
                current_target_index = entity_index;
            }
        }

        float lerp ( float start, float end, float t ) {
            return start + t * ( end - start );
        }


        void lock_on_target ( )
        {
            if ( GetAsyncKeyState ( aim::current_bind ) )
            {
                if ( min_center_dist != init_center_dist )
                {
                    float target_x = 0;
                    float target_y = 0;
                    auto center_x = decrypt_refdef->ref_def_nn.width / 2;
                    auto center_y = decrypt_refdef->ref_def_nn.height / 2;

                    if ( lock_position.x != 0 )
                    {
                        float smooth_factor_x = aim::advanced_aim ? aim::smooth_x : aim::smooth;

                        if ( lock_position.x > center_x )
                        {
                            target_x = -( center_x - lock_position.x );
                            target_x /= smooth_factor_x;
                            if ( target_x + center_x > center_x * 2 )
                                target_x = 0;
                        }
                        if ( lock_position.x < center_x )
                        {
                            target_x = lock_position.x - center_x;
                            target_x /= smooth_factor_x;
                            if ( target_x + center_x < 0 )
                                target_x = 0;
                        }
                    }
                    if ( lock_position.y != 0 )
                    {
                        float smooth_factor_y = aim::advanced_aim ? aim::smooth_y : aim::smooth;
                        if ( lock_position.y > center_y )
                        {
                            target_y = -( center_y - lock_position.y );
                            target_y /= smooth_factor_y;
                            if ( target_y + center_y > center_y * 2 )
                                target_y = 0;
                        }
                        if ( lock_position.y < center_y )
                        {
                            target_y = lock_position.y - center_y;
                            target_y /= smooth_factor_y;
                            if ( target_y + center_y < 0 )
                                target_y = 0;
                        }
                    }
                    if ( target_x != 0 && target_y != 0 )
                        move_mouse( target_x, target_y );
                }
            }
            else
            {
                aim::is_aiming = false;
            }
        }

    }
  
}