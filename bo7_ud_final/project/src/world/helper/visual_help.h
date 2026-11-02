#pragma once
#include "../../framework/draw_helper/drawer.h"
#include "../../utils/math/vectors.h"
#include "../../world/engine/engine.h"

class helper {
public:
    void draw_bone ( Vector3 from, Vector3 to, Vector3 m_location, ImU32 col )
    {
        if ( distance_3d ( m_location, from ) > 118 )
            return;

        if ( distance_3d ( m_location, to ) > 118 )
            return;

        if ( distance_3d ( from, to ) > 39 )
            return;

        Vector2 W2S_from;
        if ( !engine::client::w2s ( from, W2S_from ) )
            return;

        Vector2 W2S_to;
        if ( !engine::client::w2s ( to, W2S_to ) )
            return;

        ImGui::GetForegroundDrawList()->AddLine ( ImVec2 ( W2S_from.x, W2S_from.y ), ImVec2 ( W2S_to.x, W2S_to.y ), col, visual::skeleton_thick );

    }

    auto draw_health_bar ( int i_health, Vector4 bbox ) -> VOID
    {
        const auto factor = static_cast< float >( i_health ) / 127.f;
        const float hue = ( factor * 120.f ) / 360.f;
        auto color = draw.from_hsb ( hue, 1.f, 1.f ); 

        draw.rect_1 ( { bbox.x - 5 - 1, bbox.y - 1 }, { 2 + 2, bbox.w + 2 }, { 10, 10, 10 } );
        draw.rect_1 ( { bbox.x - 5 - 1 + 1, bbox.y - 1 + 1 }, { 2 - 2, bbox.w - 2 }, { 10, 10, 10 } );
        draw.rect_filled_1 ( { bbox.x - 5, bbox.y }, { 2, bbox.w }, { 50, 50, 50 } );
        draw.rect_filled_1 ( { bbox.x - 5, bbox.y + bbox.w * ( 1.f - factor ) }, { 2, bbox.w - bbox.w * ( 1.f - factor ) }, ImColor (color) );
    }


    static void draw_compass_crushed ( ImVec2 pos, ImColor color, bool filled = true, bool drawDistance = false, int distance = 0 ) {
        const auto ARROW_SIZE = 12.0f;
        const float WIDTH_FACTOR = 1.39626f; 
        const float OUTLINE_THICKNESS = 2.0f;

        auto Center = ImVec2 ( GetSystemMetrics ( SM_CXSCREEN ) / 2, GetSystemMetrics ( SM_CYSCREEN ) / 2 );
        auto Direction = ImVec2 ( pos.x - Center.x, pos.y - Center.y );

        auto Length = sqrtf ( Direction.x * Direction.x + Direction.y * Direction.y );
        Direction.x /= Length;
        Direction.y /= Length;

        auto Angle = atan2f ( Direction.y, Direction.x );
        ImVec2 ArrowPos = ImVec2 ( Center.x + Direction.x * ( visual::compass_position + 10 ), Center.y + Direction.y * ( visual::compass_position + 10 ) );

        ImVec2 point1 = ImVec2 ( ArrowPos.x + cosf ( Angle ) * ARROW_SIZE, ArrowPos.y + sinf ( Angle ) * ARROW_SIZE );
        ImVec2 point2 = ImVec2 ( ArrowPos.x + cosf ( Angle + WIDTH_FACTOR ) * ARROW_SIZE, ArrowPos.y + sinf ( Angle + WIDTH_FACTOR ) * ARROW_SIZE );
        ImVec2 point3 = ImVec2 ( ArrowPos.x + cosf ( Angle - WIDTH_FACTOR ) * ARROW_SIZE, ArrowPos.y + sinf ( Angle - WIDTH_FACTOR ) * ARROW_SIZE );

        ImDrawList* drawList = ImGui::GetBackgroundDrawList ( );
        if ( visual::compass_filled ) {
            drawList->AddTriangle ( point1, point2, point3, color, OUTLINE_THICKNESS );
            drawList->AddTriangleFilled ( point1, point2, point3, ImColor ( color.Value.x, color.Value.y, color.Value.z, 50.f / 255.f ) );

        }
        else {
            drawList->AddTriangle ( point1, point2, point3, color, OUTLINE_THICKNESS );
        }
    }

    static void draw_compass_pointed ( ImVec2 pos, ImColor color, bool filled = true, bool drawDistance = false, int distance = 0 ) {
        const auto ARROW_SIZE = 10.0f;

        auto Center = ImVec2 ( GetSystemMetrics ( SM_CXSCREEN ) / 2, GetSystemMetrics ( SM_CYSCREEN ) / 2 );
        auto Direction = ImVec2 ( pos.x - Center.x, pos.y - Center.y );

        auto Length = sqrtf ( Direction.x * Direction.x + Direction.y * Direction.y );
        Direction.x /= Length;
        Direction.y /= Length;

        auto Angle = atan2f ( Direction.y, Direction.x );
        ImVec2 ArrowPos;

        ArrowPos = ImVec2 ( Center.x + Direction.x * ( visual::compass_position + 10 ), Center.y + Direction.y * ( visual::compass_position + 10 ) );

        if ( !visual::compass_filled ) {
            ImGui::GetBackgroundDrawList ( )->AddTriangle (
                ImVec2 ( ArrowPos.x + cosf ( Angle ) * ARROW_SIZE, ArrowPos.y + sinf ( Angle ) * ARROW_SIZE ),
                ImVec2 ( ArrowPos.x + cosf ( Angle + 2.35619f ) * ARROW_SIZE, ArrowPos.y + sinf ( Angle + 2.35619f ) * ARROW_SIZE ),
                ImVec2 ( ArrowPos.x + cosf ( Angle - 2.35619f ) * ARROW_SIZE, ArrowPos.y + sinf ( Angle - 2.35619f ) * ARROW_SIZE ), color, 2 );
        }
        else {
            ImGui::GetBackgroundDrawList ( )->AddTriangle (
                ImVec2 ( ArrowPos.x + cosf ( Angle ) * ARROW_SIZE, ArrowPos.y + sinf ( Angle ) * ARROW_SIZE ),
                ImVec2 ( ArrowPos.x + cosf ( Angle + 2.35619f ) * ARROW_SIZE, ArrowPos.y + sinf ( Angle + 2.35619f ) * ARROW_SIZE ),
                ImVec2 ( ArrowPos.x + cosf ( Angle - 2.35619f ) * ARROW_SIZE, ArrowPos.y + sinf ( Angle - 2.35619f ) * ARROW_SIZE ), color, 2 );

            ImGui::GetBackgroundDrawList ( )->AddTriangleFilled (
                ImVec2 ( ArrowPos.x + cosf ( Angle ) * ARROW_SIZE, ArrowPos.y + sinf ( Angle ) * ARROW_SIZE ),
                ImVec2 ( ArrowPos.x + cosf ( Angle + 2.35619f ) * ARROW_SIZE, ArrowPos.y + sinf ( Angle + 2.35619f ) * ARROW_SIZE ),
                ImVec2 ( ArrowPos.x + cosf ( Angle - 2.35619f ) * ARROW_SIZE, ArrowPos.y + sinf ( Angle - 2.35619f ) * ARROW_SIZE ), ImColor ( color.Value.x, color.Value.y, color.Value.z, 50.f / 255.f ) );
        }
    }
};

helper help;