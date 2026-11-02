#pragma once
#include "../framework/imgui/imgui.h"


namespace settings
{
	bool menu = true;
	bool performance;
	bool watermark = true;
	bool show_fps = true;

	bool unload = false; // DONT TOUCH!
}

namespace visual {
	bool skeletons = true;
	bool weapon;
	bool name;
	bool aim_target;
	bool team_esp;
	bool box = true;
	bool outlined;
	bool snapline;
	bool distance;
	bool health = false;
	bool visiblity_check = true;
	bool compass;
	bool compass_filled;
	int compass_position;

	int skeleton_thick;
	int max_render_distance = 150;

	int box_type = 0;
	const char* boxx [ 3 ] { "Box", "Cornered", "Filled" };

	int snap_pos = 0;
	const char* snapss [ 3 ] { "Top", "Center", "Bottom" };

	int compass_style = 0;
	const char* compasss [ 2 ] { "Crushed", "Pointed"};
}

namespace radarr {
	bool b_radar = false;
	float radar_zoom = 24.70;
	float radar_BlipSize = 8.0f;
	float radar_size = 340.10;

	float flScale = 10.f, flRadarSize = 200, flBlipSize = 8.0f;
	ImVec2 vRadarPosition, vBlipPosition;
}

namespace aim {
	bool aim = false;
	bool advanced_aim = false;
	bool show_fov;
	bool show_crosshair;
	float fov = 200;
	int max_distance = 500;
	int smooth = 1;
	int smooth_x = 1;
	int smooth_y = 1;
	bool is_aiming;
	bool vis_check;

	float aim_delay = 0.1f;
	float switch_delay = 0.2f;

	bool skip_knocked;
	bool aim_teammate;
	bool skip_vehicle;

	int aim_bone;
	const char* bones [ 4 ] { "Head", "Neck", "Chest", "Pelvis" };


	int aim_bindss = 0;
	const char* bindss [ 5 ] { "Right Button", "Left Button", "Center Button", "Mouse X1", "Mouse X2" };
	int current_bind;

	bool hitbox_snapline;
	bool hitbox_circle;
}

namespace colors {
	float skeleton_vis [ 4 ] = { 1.0f, 0.0f, 0.0f, 1.f };
	float box_vis [ 4 ] = { 1.0f, 0.0f, 0.0f, 1.f };
	float distance_vis [ 4 ] = { 1.0f, 0.0f, 0.0f, 1.f };
	float name_vis [ 4 ] = { 1.0f, 0.0f, 0.0f, 1.f };
	float snapline_vis [ 4 ] = { 1.0f, 0.0f, 0.0f, 1.f };
	float weapon_vis [ 4 ] = { 1.0f, 0.0f, 0.0f, 1.f };
	float compass_vis [ 4 ] = { 1.0f, 0.0f, 0.0f, 1.f };


	float skeleton_invis [ 4 ] = { 1.0f, 1.0f, 1.0f, 1.f };
	float box_invis [ 4 ] = { 1.0f, 1.0f, 1.0f, 1.f };
	float distance_invis [ 4 ] = { 1.0f, 1.0f, 1.0f, 1.f };
	float name_invis [ 4 ] = { 1.0f, 1.0f, 1.0f, 1.f };
	float snapline_invis [ 4 ] = { 1.0f, 1.0f, 1.0f, 1.f };
	float weapon_invis [ 4 ] = { 1.0f, 1.0f, 1.0f, 1.f };
	float compass_invis [ 4 ] = { 1.0f, 1.0f, 1.0f, 1.f };



	float aim_target [ 4 ] = { 1.0f, 0.0f, 1.0f, 1.f };
	float crosshair [ 4 ] = { 1.0f, 1.0f, 0.0f, 1.0f };
	float fov [ 4 ] = { 1.0f, 1.0f, 0.0f, 1.0f };

	float hitbox_snapline [ 4 ] = { 1.0f, 1.0f, 1.0f, 1.f };
	float hitbox_circle [ 4 ] = { 1.0f, 1.0f, 1.0f, 1.f };
}

