// custom_features.h — Dream Layout defines and declarations
// Included from keymap.c. All custom QMK features for Moonlander.
#pragma once

// ═══════════════════════════════════════════════════════════════════
// Layer Names
// ═══════════════════════════════════════════════════════════════════
enum layers {
    _BASE = 0,
    _NAV,       // 1 — hold Space
    _SYM,       // 2 — hold Enter
    _FUNC,      // 3 — Tri Layer (Nav + Sym)
    _MOUSE,     // 4 — combo Z+X
    _SYSTEM,    // 5 — MO(5)
    _GAMING,    // 6 — TG(6)
};

// ═══════════════════════════════════════════════════════════════════
// Key Aliases
// ═══════════════════════════════════════════════════════════════════

// Thumb layer-taps
#define NAV_SPC  LT(_NAV, KC_SPC)
#define SYM_ENT  LT(_SYM, KC_ENT)

// Home Row Mods — GASC order (GUI, Alt, Shift, Ctrl)
// Left hand
#define HRM_A  LGUI_T(KC_A)
#define HRM_S  LALT_T(KC_S)
#define HRM_D  LSFT_T(KC_D)
#define HRM_F  LCTL_T(KC_F)
// Right hand (mirrored)
#define HRM_J  RCTL_T(KC_J)
#define HRM_K  RSFT_T(KC_K)
#define HRM_L  LALT_T(KC_L)    // LALT not RALT — avoids AltGr
#define HRM_SC RGUI_T(KC_SCLN)

// ═══════════════════════════════════════════════════════════════════
// Combo IDs
// ═══════════════════════════════════════════════════════════════════
enum combo_events {
    CMB_ESC,       // J + K = Escape
    CMB_TAB,       // D + F = Tab
    CMB_BSPC,      // K + L = Backspace
    CMB_MOUSE,     // Z + X = Mouse layer (momentary)
    CMB_COUNT
};

// ═══════════════════════════════════════════════════════════════════
// LED Color Palette — Elemental Spectrum Theme
// ═══════════════════════════════════════════════════════════════════

// Shared
#define C_OFF   {0,   0,   0}
#define C_WHITE {0,   0, 255}

// Nav (Forest Green H=85)
#define C_NAV1  {85, 255, 255}   // Primary: arrows
#define C_NAV2  {85, 255, 180}   // Secondary: Home/End/PgUp/PgDn
#define C_NAVM  {85, 150, 255}   // Modifier: plain mods
#define C_NAVA  {106, 255, 220}  // Accent: macOS shortcuts

// Sym (Amber H=32)
#define C_SYM1  {32, 255, 255}   // Primary: numbers
#define C_SYM2  {21, 255, 220}   // Secondary: brackets
#define C_SYM3  {43, 255, 200}   // Accent: operators
#define C_SYMM  {32, 200, 180}   // Lesser symbols

// Func (Amethyst H=191)
#define C_FUN1  {191, 255, 255}  // Primary: F-keys
#define C_FUN2  {213, 255, 220}  // Secondary: media
#define C_FUN3  {234, 255, 200}  // Accent: volume

// Mouse (Coral H=234)
#define C_MOU1  {234, 255, 255}  // Primary: movement
#define C_MOU2  {234, 200, 255}  // Secondary: buttons
#define C_MOU3  {245, 255, 220}  // Accent: scroll

// System (Ember H=0)
#define C_SYS1  {0,  255, 255}   // Danger: bootloader
#define C_SYS2  {21, 255, 220}   // Secondary: RGB
#define C_SYS3  {0,  200, 180}   // Audio

// Gaming (Plasma Lime H=64)
#define C_GAM1  {64, 255, 255}   // Primary: WASD
#define C_GAM2  {85, 255, 220}   // Numbers
#define C_GAMM  {64, 150, 255}   // Modifiers
#define C_GAM3  {85, 200, 180}   // F-keys
#define C_GAM4  {64, 200, 140}   // Auxiliary
