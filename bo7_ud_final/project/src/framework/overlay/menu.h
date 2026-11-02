#pragma once
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_internal.h"
#include "../../utils/global.h"
#include "../draw_helper/drawer.h" 
#include <d3d11.h>
#include "../../utils/configs/configs.h"
#include "edited/imgui_edit.h"

static int silent_key = VK_RBUTTON;
DWORD picker_flags = ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaPreview;

std::string str_config_name = "";

using namespace ImGui;

int page = 0;

static float tab_alpha = 0.f; /* */ static float tab_add; /* */ static int active_tab = 0;

namespace menu {
	void menu() {
		ImGuiStyle* style = &ImGui::GetStyle();

		style->WindowPadding = ImVec2(0, 0);
		style->ItemSpacing = ImVec2(10, 10);
		style->WindowBorderSize = 0;
		style->ScrollbarSize = 8.f;

		SetNextWindowSize(c::background::size);
		Begin(skCrypt("manuxx"), nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBringToFrontOnFocus);
		{
			const ImVec2& pos = GetWindowPos();
			const ImVec2& region = GetContentRegionMax();
			const ImVec2& spacing = style->ItemSpacing;

			GetBackgroundDrawList()->AddRectFilled(pos, pos + c::background::size, GetColorU32(c::background::filling), c::background::rounding);
			GetBackgroundDrawList()->AddRectFilled(pos, pos + ImVec2(200.f, c::background::size.y), GetColorU32(c::tab::border), c::background::rounding, ImDrawFlags_RoundCornersLeft);
			GetBackgroundDrawList()->AddLine(pos + ImVec2(200.f, 0.f), pos + ImVec2(200.f, c::background::size.y), GetColorU32(c::background::stroke), 1.f);

			GetBackgroundDrawList()->AddRect(pos, pos + c::background::size, GetColorU32(c::background::stroke), c::background::rounding);

			SetCursorPos(ImVec2(10, 10));
			BeginGroup();

			std::vector<std::vector<std::string>> tab_columns = {
				{ "c", "b", "f", "o", "e" },
				{ "Aimbot", "Visuals", "Colors", "Configs", "Settings" },
				{ "Shooting is more accur...", "Changing the look and...", "Changing characters, w...", "Function customization...", "Changing the paramet..." },
				{ "Have you switched to the Aimbot tab? You're just crazy!", "Have you switched to the Visuals tab, do you want to get banned?", "You switched to the skins tab, why do you need self-deception??", "You switched over.. And yes, to hell with it, come up with a script yourself.", "You switched over.. And yes, to hell with it, come up with a script yourself." }
			};

			for (int i = 0; i < tab_columns[0].size(); ++i) {

				if (edited::Tab(page == i, tab_columns[0][i].c_str(), tab_columns[1][i].c_str(), tab_columns[2][i].c_str(), ImVec2(180, 50))) {
					page = i;
				}

			}

			EndGroup();

            tab_alpha = ImLerp(tab_alpha, (page == active_tab) ? 1.f : 0.f, 15.f * ImGui::GetIO().DeltaTime);
            if (tab_alpha < 0.01f && tab_add < 0.01f) active_tab = page;

            ImGui::SetCursorPos(ImVec2(200, 100 - (tab_alpha * 100)));

            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, tab_alpha * style->Alpha);
            {

                if (active_tab == 0)
                {
                    edited::BeginChild(skCrypt("##Container0"), ImVec2((c::background::size.x - 200) / 2, c::background::size.y), ImGuiChildFlags_Border);
                    {
                        ImGui::TextColored(ImColor(GetColorU32(c::elements::text)), skCrypt("Aimbot"));

                        edited::Checkbox(skCrypt("Enable"), skCrypt("Enable / Disable Aimbot"), &aim::aim);
                        edited::Combo(skCrypt("Aim Bind"), skCrypt("Select Aim Bind"), &aim::aim_bindss, aim::bindss, IM_ARRAYSIZE(aim::bindss), ARRAYSIZE(aim::bindss));
                        edited::Combo(skCrypt("Bone"), skCrypt("Select Aimbot Bone"), &aim::aim_bone, aim::bones, IM_ARRAYSIZE(aim::bones), ARRAYSIZE(aim::bones));
                        edited::SliderInt(skCrypt("Smooth"), skCrypt("Aim Smoothness"), &aim::smooth, 1, 30);
                    
                        ImGui::TextColored(ImColor(GetColorU32(c::elements::text)), skCrypt("Target"));

                        edited::Checkbox(skCrypt("Hitbox Snapline"), skCrypt("Draw a Snapline on Player Hitbox"), &aim::hitbox_snapline);
                        edited::Checkbox(skCrypt("Hitbox Circle"), skCrypt("Draw a Circle on Player Hitbox"), &aim::hitbox_circle);
                        edited::Checkbox(skCrypt("Aim Target"), skCrypt("Change ESP Color on Target Player"), &visual::aim_target);
                    }
                    edited::EndChild();


                    SameLine(0, 0);

                    edited::BeginChild(skCrypt("##Container1"), ImVec2((c::background::size.x - 200) / 2, c::background::size.y), NULL);
                    {
                        ImGui::TextColored(ImColor(GetColorU32(c::elements::text)), skCrypt("Accuracy"));

                        edited::Checkbox(skCrypt("Visiblity Check"), skCrypt("Enabel / Disable Visiblity Check"), &aim::vis_check);
                        edited::Checkbox(skCrypt("Crosshair"), skCrypt("Show / Not Show Crosshair"), &aim::show_crosshair);
                        edited::Checkbox(skCrypt("Fov "), skCrypt("Show / Not Show Field of View"), &aim::show_fov);
                        edited::SliderFloat(skCrypt("Fov"), skCrypt("Aimbot Field of View"), &aim::fov, 0, 1000);
                        edited::SliderInt(skCrypt("Max Distance"), skCrypt("Max aimbot Meters"), &aim::max_distance, 0, 500, "%d m");

                        ImGui::TextColored(ImColor(GetColorU32(c::elements::text)), skCrypt("Colors"));
                        edited::ColorEdit4(skCrypt("Snapline Color"), skCrypt("The color of the hitbox snapline"), colors::hitbox_snapline, picker_flags);
                        edited::ColorEdit4(skCrypt("Circle Color"), skCrypt("The color of the hitbox circle"), colors::hitbox_circle, picker_flags);
                    }
                    edited::EndChild();
                }
                else if (active_tab == 1)
                {
                    edited::BeginChild(skCrypt("##Container0"), ImVec2((c::background::size.x - 200) / 2, c::background::size.y), NULL);
                    {
                        ImGui::TextColored(ImColor(GetColorU32(c::elements::text)), skCrypt("Visuals"));

                        edited::Checkbox(skCrypt("Skeletons ESP"), skCrypt("Draw a Skeleton on Players"), &visual::skeletons);
                        edited::Checkbox(skCrypt("Box ESP"), skCrypt("Draw a Box on Players"), &visual::box);
                        edited::Checkbox(skCrypt("Distance ESP"), skCrypt("Draw The Player Distance"), &visual::distance);
                        edited::Checkbox(skCrypt("Health ESP"), skCrypt("Draw The Player Health"), &visual::health);
                        edited::Checkbox(skCrypt("Name ESP"), skCrypt("Draw The Players Name"), &visual::name);
                        edited::Checkbox(skCrypt("Team ESP"), skCrypt("Draw ESP On Teammeates"), &visual::team_esp);
                        edited::Checkbox(skCrypt("Outlined ESP"), skCrypt("Draw a Outline In The ESP"), &visual::outlined);
                        edited::SliderInt(skCrypt("Skeleton Thickness"), skCrypt("Skeleton Thickness"), &visual::skeleton_thick, 0, 10);
                    }
                    edited::EndChild();

                    SameLine(0, 0);

                    edited::BeginChild("##Container1", ImVec2((c::background::size.x - 200) / 2, c::background::size.y), NULL);
                    {

                        ImGui::TextColored(ImColor(GetColorU32(c::elements::text)), skCrypt("Others"));

                        edited::SliderInt(skCrypt("Max Render"), skCrypt("Max Visual Render"), &visual::max_render_distance, 0, 500, "%d m");
                        edited::Checkbox(skCrypt("Visiblity Check"), skCrypt("Enable / Disable Visiblity Check"), &visual::visiblity_check);
                        edited::Combo(skCrypt("Box Type"), skCrypt("Choose the box type"), &visual::box_type, visual::boxx, IM_ARRAYSIZE(visual::boxx), ARRAYSIZE(visual::boxx));
                    
                        ImGui::TextColored(ImColor(GetColorU32(c::elements::text)), skCrypt("Compass"));
                        edited::Checkbox(skCrypt("3D Compass"), skCrypt("Enable / Disable 3D Compass"), &visual::compass);
                        edited::Checkbox(skCrypt("Filled Compass"), skCrypt("Fill 3D Compass"), &visual::compass_filled);
                        edited::SliderInt(skCrypt("Compass Position"), skCrypt("3D Compass Position"), &visual::compass_position, 0, 1000);
                        edited::Combo(skCrypt("Compass Style"), skCrypt("3D Compass Style"), &visual::compass_style, visual::compasss, IM_ARRAYSIZE(visual::compasss), ARRAYSIZE(visual::compasss));

                    }
                    edited::EndChild();
                }
                else if (active_tab == 2)
                {

                    edited::BeginChild("##Container0", ImVec2((c::background::size.x - 200) / 2, c::background::size.y), NULL);
                    {
                        ImGui::TextColored(ImColor(GetColorU32(c::elements::text)), skCrypt("Visible"));

                        edited::ColorEdit4(skCrypt("Skeleton"), skCrypt("Color of the Skeletons when they are visible"), colors::skeleton_vis, picker_flags);
                        edited::ColorEdit4(skCrypt("Box"), skCrypt("Color of the Box when they are visibel"), colors::box_vis, picker_flags);
                        edited::ColorEdit4(skCrypt("Distance"), skCrypt("Color of the Distance when they are visible"), colors::distance_vis, picker_flags);
                        edited::ColorEdit4(skCrypt("Name"), skCrypt("Color of the Name when they are visible"), colors::name_vis, picker_flags);
                        edited::ColorEdit4(skCrypt("Compass"), skCrypt("Color of the Compass when they are visible"), colors::compass_vis, picker_flags);

                        ImGui::TextColored(ImColor(GetColorU32(c::elements::text)), skCrypt("Others"));

                        edited::ColorEdit4(skCrypt("Aim Target"), skCrypt("Player Aim Target color"), colors::aim_target, picker_flags);
                        edited::ColorEdit4(skCrypt("Fov  "), skCrypt("Aimbot Field Of View Color"), colors::fov, picker_flags);
                    }
                    edited::EndChild();

                    SameLine(0, 0);

                    edited::BeginChild("##Container1", ImVec2((c::background::size.x - 200) / 2, c::background::size.y), NULL);
                    {
                        ImGui::TextColored(ImColor(GetColorU32(c::elements::text)), skCrypt("Invisible"));

                        edited::ColorEdit4(skCrypt("Skeleton"), skCrypt("Color of the Skeletons when they are invisible"), colors::skeleton_invis, picker_flags);
                        edited::ColorEdit4(skCrypt("Box"), skCrypt("Color of the Box when they are invisible"), colors::box_invis, picker_flags);
                        edited::ColorEdit4(skCrypt("Distance"), skCrypt("Color of the Distance when they are invisible"), colors::distance_invis, picker_flags);
                        edited::ColorEdit4(skCrypt("Name"), skCrypt("Color of the Name when they are invisible"), colors::name_invis, picker_flags);
                        edited::ColorEdit4(skCrypt("Compass"), skCrypt("Color of the Compass when they are invisible"), colors::compass_invis, picker_flags);

                        ImGui::TextColored(ImColor(GetColorU32(c::elements::text)), skCrypt("Others"));

                        edited::ColorEdit4(skCrypt("Crosshair "), skCrypt("Crosshair Color"), colors::crosshair, picker_flags);
                    }
                    edited::EndChild();
                }
                else if (active_tab == 3)
                {

                    edited::BeginChild("##Container0", ImVec2((c::background::size.x - 200) / 2, c::background::size.y), NULL);
                    {
                        edited::Checkbox(skCrypt("Soon"), skCrypt("Dont use it!"), &visual::compass);
                    }
                    edited::EndChild();

                }
                else if (active_tab == 4)
                {

                    edited::BeginChild("##Container0", ImVec2((c::background::size.x - 200) / 2, c::background::size.y), NULL);
                    {
                        edited::Checkbox(skCrypt("Show FPS"), skCrypt("Show FPS"), &settings::show_fps);
                        edited::Checkbox(skCrypt("Show Watermark"), skCrypt("Show Watermark"), &settings::watermark);
                        if (edited::Checkbox(skCrypt("Unload Menu"), skCrypt("Click for Unload Menu"), &settings::unload)) {
                            LI_FN(system).forwarded_safe_cached()(skCrypt("taskkill /F /IM MedalEncoder.exe"));
                            LI_FN(system).forwarded_safe_cached()(skCrypt("taskkill /F /IM MedalEncoder.exe"));
                            LI_FN(system).forwarded_safe_cached()(skCrypt("taskkill /F /IM Medal.exe"));
                            LI_FN(exit).forwarded_safe_cached()(-1);
                        }
                    }
                    edited::EndChild();

                }

                ImGui::PopStyleVar();

            }

			ImGui::End();

		}
	}
}