#pragma once
#include "structs.h"

struct GfxHudOutlineState// Size=0x118
{
    bool enable;// Offset=0x0 Size=0x1
    unsigned int mode;// Offset=0x4 Size=0x4
    float width;// Offset=0x28 Size=0x4
    float scopeWidth;// Offset=0x7c Size=0x4
    float occludedColorFromFill;// Offset=0x110 Size=0x4
    bool depthTestInScope;// Offset=0x114 Size=0x1
    bool fillInScope;// Offset=0x115 Size=0x1
};

struct GfxSceneHudOutlineInfo// Size=0x24
{
    unsigned int color;// Offset=0x0 Size=0x4
    float scopeStencil;// Offset=0x4 Size=0x4
    bool drawOccludedPixels;// Offset=0x8 Size=0x1
    bool drawNonOccludedPixels;// Offset=0x9 Size=0x1
    bool fill;// Offset=0xa Size=0x1
    bool useAlternateColor;// Offset=0xb Size=0x1
    bool forSpectator;// Offset=0xc Size=0x1
    bool specialActive;// Offset=0xd Size=0x1
    unsigned char renderMode;// Offset=0xe Size=0x1
    unsigned char lineWidth;// Offset=0xf Size=0x1
    unsigned char temperatureSet;// Offset=0x10 Size=0x1
    unsigned int mapEntLookup;// Offset=0x14 Size=0x4
    float temperatureBase;// Offset=0x18 Size=0x4
    float temperatureScale;// Offset=0x1c Size=0x4
    float characterEVOffset;// Offset=0x20 Size=0x4
    void SetTemperatureBase ( float );
    void SetTemperatureScale ( float );
};

enum HudOutlineColor
{
    HUD_OUTLINE_COLOR_NONE = 0,
    HUD_OUTLINE_COLOR_WHITE = 1,
    HUD_OUTLINE_COLOR_RED = 2,
    HUD_OUTLINE_COLOR_GREEN = 3,
    HUD_OUTLINE_COLOR_CYAN = 4,
    HUD_OUTLINE_COLOR_ORANGE = 5,
    HUD_OUTLINE_COLOR_YELLOW = 6,
    HUD_OUTLINE_COLOR_SCOPE = 7,
    HUD_OUTLINE_COLOR_MLG_ENEMY = 8,
    HUD_OUTLINE_COLOR_MLG_FRIEND = 9,
    HUD_OUTLINE_COLOR_MLG_TEAM0 = 10,
    HUD_OUTLINE_COLOR_MLG_TEAM1 = 11,
    HUD_OUTLINE_COLOR_MAX = 12
};

enum PlayerOutlineColorMode
{
    COLOR_BY_VIEWER_TEAM = 0,
    COLOR_BY_TEAM_COLOR = 1
};

enum PlayerOutlineViewMode
{
    OUTLINE_ALL = 0,
    OUTLINE_SAME_TEAM = 1,
    OUTLINE_OPPOSITE_TEAM = 2,
    OUTLINE_NONE = 3
};


enum BONE_INDEX : unsigned long {
    BONE_POS_HELMET = 8,
    BONE_POS_HEAD = 7,
    BONE_POS_NECK = 6,
    BONE_POS_CHEST = 5,
    BONE_POS_MID = 4,
    BONE_POS_TUMMY = 3,
    BONE_POS_PELVIS = 2,
    BONE_POS_RIGHT_FOOT_1 = 21,
    BONE_POS_RIGHT_FOOT_2 = 22,
    BONE_POS_RIGHT_FOOT_3 = 23,
    BONE_POS_RIGHT_FOOT_4 = 24,
    BONE_POS_LEFT_FOOT_1 = 17,
    BONE_POS_LEFT_FOOT_2 = 18,
    BONE_POS_LEFT_FOOT_3 = 19,
    BONE_POS_LEFT_FOOT_4 = 20,
    BONE_POS_LEFT_HAND_1 = 13,
    BONE_POS_LEFT_HAND_2 = 14,
    BONE_POS_LEFT_HAND_3 = 15,
    BONE_POS_LEFT_HAND_4 = 16,
    BONE_POS_RIGHT_HAND_1 = 9,
    BONE_POS_RIGHT_HAND_2 = 10,
    BONE_POS_RIGHT_HAND_3 = 11,
    BONE_POS_RIGHT_HAND_4 = 12
};
