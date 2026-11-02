#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../imgui/imgui.h"
#include "../../utils/math/vectors.h"
#include <map>

ImFont* details;
ImFont* tab_font;
ImFont* fontCalibri;
ImFont* fontArial;
ImFont* fontSegoeUI;

using namespace ImGui;


class drawer {
public:

	void draw_line ( const ImVec2& from, const ImVec2& to, ImColor color, float thickness )
	{
		auto window = ImGui::GetBackgroundDrawList ( );;
		window->AddLine ( from, to, color, thickness );
	}
	void draw_box ( const float x, const float y, const float width, const float height, const ImColor color, float thickness )
	{
		draw_line ( ImVec2 ( x, y ), ImVec2 ( x + width, y ), color, thickness );
		draw_line ( ImVec2 ( x, y ), ImVec2 ( x, y + height ), color, thickness );
		draw_line ( ImVec2 ( x, y + height ), ImVec2 ( x + width, y + height ), color, thickness );
		draw_line ( ImVec2 ( x + width, y ), ImVec2 ( x + width, y + height ), color, thickness );
	}

	auto rect_filled ( float x0, float y0, float x1, float y1, ImColor color, float rounding, int rounding_corners_flags ) -> VOID
	{
		ImGui::GetForegroundDrawList ( )->AddRectFilled ( ImVec2 ( x0, y0 ), ImVec2 ( x1, y1 ), color, rounding, rounding_corners_flags );
	}

	auto rect ( float x0, float y0, float x1, float y1, ImColor color, float rounding, int rounding_corners_flags ) -> VOID
	{
		ImGui::GetForegroundDrawList ( )->AddRect ( ImVec2 ( x0, y0 ), ImVec2 ( x1, y1 ), color, rounding, rounding_corners_flags );
	}

	void rect_filled_1 ( const Vector2& pos, const Vector2& size, const ImColor& col )
	{
		ImGui::GetBackgroundDrawList ( )->AddRectFilled ( { pos.x, pos.y }, { pos.x + size.x, pos.y + size.y }, col );
	}

	void rect_1 ( const Vector2& pos, const Vector2& size, const ImColor& col )
	{
		ImGui::GetBackgroundDrawList ( )->AddRect ( { pos.x, pos.y }, { pos.x + size.x, pos.y + size.y }, col );
	}


	__forceinline void string ( float fontSize, int x, int y, ImColor color, bool bCenter, bool stroke, std::string text )
	{
		if ( text.empty ( ) ) {
			return;
		}

		if ( bCenter ) {
			ImVec2 textSize = ImGui::CalcTextSize ( text.c_str ( ) );
			x -= textSize.x / 2;
			y -= textSize.y / 2;
		}

		if ( stroke ) {
			ImVec4 black = ImVec4 ( 0, 0, 0, 1 );
			ImColor black_color = ImColor ( black );
			ImGui::GetBackgroundDrawList ( )->AddText ( ImGui::GetFont ( ), fontSize, ImVec2 ( x + 1, y + 1 ), black_color, text.c_str ( ) );
			ImGui::GetBackgroundDrawList ( )->AddText ( ImGui::GetFont ( ), fontSize, ImVec2 ( x - 1, y - 1 ), black_color, text.c_str ( ) );
			ImGui::GetBackgroundDrawList ( )->AddText ( ImGui::GetFont ( ), fontSize, ImVec2 ( x + 1, y - 1 ), black_color, text.c_str ( ) );
			ImGui::GetBackgroundDrawList ( )->AddText ( ImGui::GetFont ( ), fontSize, ImVec2 ( x - 1, y + 1 ), black_color, text.c_str ( ) );
		}

		ImGui::GetBackgroundDrawList ( )->AddText ( ImGui::GetFont ( ), fontSize, ImVec2 ( x, y ), color, text.c_str ( ) );
	}

	auto draw_cornered_box ( float X, float Y, float W, float H, const ImU32& color, float thickness ) -> VOID
	{
		auto vList = ImGui::GetForegroundDrawList ( );

		float lineW = ( W / 3 );
		float lineH = ( H / 3 );

		auto col = ImGui::GetColorU32 ( color );

		vList->AddLine ( ImVec2 ( X, Y - thickness / 2 ), ImVec2 ( X, Y + lineH ), col, thickness );
		vList->AddLine ( ImVec2 ( X - thickness / 2, Y ), ImVec2 ( X + lineW, Y ), col, thickness );

		vList->AddLine ( ImVec2 ( X + W - lineW, Y ), ImVec2 ( X + W + thickness / 2, Y ), col, thickness );
		vList->AddLine ( ImVec2 ( X + W, Y - thickness / 2 ), ImVec2 ( X + W, Y + lineH ), col, thickness );

		vList->AddLine ( ImVec2 ( X, Y + H - lineH ), ImVec2 ( X, Y + H + ( thickness / 2 ) ), col, thickness );
		vList->AddLine ( ImVec2 ( X - thickness / 2, Y + H ), ImVec2 ( X + lineW, Y + H ), col, thickness );

		vList->AddLine ( ImVec2 ( X + W - lineW, Y + H ), ImVec2 ( X + W + thickness / 2, Y + H ), col, thickness );
		vList->AddLine ( ImVec2 ( X + W, Y + H - lineH ), ImVec2 ( X + W, Y + H + ( thickness / 2 ) ), col, thickness );

	}

	ImVec4 from_hsb ( float hue, float saturation, float brightness )
	{
		float h = hue == 1.0f ? 0 : hue * 6.0f; 
		float f = h - static_cast< int >( h );     
		float p = brightness * ( 1.0f - saturation ); 
		float q = brightness * ( 1.0f - saturation * f );
		float t = brightness * ( 1.0f - ( saturation * ( 1.0f - f ) ) ); 

		if ( h < 1 )
		{
			return ImVec4 ( brightness, t, p, 1.0f );
		}
		if ( h < 2 )
		{
			return ImVec4 ( q, brightness, p, 1.0f );
		}
		if ( h < 3 )
		{
			return ImVec4 ( p, brightness, t, 1.0f );
		}
		if ( h < 4 )
		{
			return ImVec4 ( p, q, brightness, 1.0f );
		}
		if ( h < 5 )
		{
			return ImVec4 ( t, p, brightness, 1.0f );
		}
		return ImVec4 ( brightness, p, q, 1.0f );
	}
};

drawer draw;

const char* keys [ ] = {
"None",
"Mouse 1",
"Mouse 2",
"CN",
"Mouse 3",
"Mouse 4",
"Mouse 5",
"-",
"Back",
"Tab",
"-",
"-",
"CLR",
"Enter",
"-",
"-",
"Shift",
"CTL",
"Menu",
"Pause",
"Caps",
"KAN",
"-",
"JUN",
"FIN",
"KAN",
"-",
"ESC",
"CON",
"NCO",
"ACC",
"MAD",
"Space",
"PGU",
"PGD",
"End",
"Home",
"Left",
"Up",
"Right",
"Down",
"SEL",
"PRI",
"EXE",
"PRI",
"INS",
"DEL",
"HEL",
"0",
"1",
"2",
"3",
"4",
"5",
"6",
"7",
"8",
"9",
"-",
"-",
"-",
"-",
"-",
"-",
"-",
"A",
"B",
"C",
"D",
"E",
"F",
"G",
"H",
"I",
"J",
"K",
"L",
"M",
"N",
"O",
"P",
"Q",
"R",
"S",
"T",
"U",
"V",
"W",
"X",
"Y",
"Z",
"WIN",
"WIN",
"APP",
"-",
"SLE",
"Numpad 0",
"Numpad 1",
"Numpad 2",
"Numpad 3",
"Numpad 4",
"Numpad 5",
"Numpad 6",
"Numpad 7",
"Numpad 8",
"Numpad 9",
"MUL",
"ADD",
"SEP",
"MIN",
"Delete",
"DIV",
"F1",
"F2",
"F3",
"F4",
"F5",
"F6",
"F7",
"F8",
"F9",
"F10",
"F11",
"F12",
"F13",
"F14",
"F15",
"F16",
"F17",
"F18",
"F19",
"F20",
"F21",
"F22",
"F23",
"F24",
"-",
"-",
"-",
"-",
"-",
"-",
"-",
"-",
"NUM",
"SCR",
"EQU",
"MAS",
"TOY",
"OYA",
"OYA",
"-",
"-",
"-",
"-",
"-",
"-",
"-",
"-",
"-",
"Shift",
"Shift",
"Ctrl",
"Ctrl",
"Alt",
"Alt"
};