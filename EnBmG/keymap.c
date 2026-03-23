// keymap.c — Dream Layout for Moonlander
// Oryx layout ID: EnBmG | Firmware v25
//
// This file merges Oryx-generated scaffolding with custom QMK features.
// The keymaps[] array is fully custom. Oryx sync will require conflict resolution.
// All custom feature implementations are in custom_features.c (#included at end).

#include QMK_KEYBOARD_H
#include "version.h"
#define MOON_LED_LEVEL LED_LEVEL
#ifndef ZSA_SAFE_RANGE
#define ZSA_SAFE_RANGE SAFE_RANGE
#endif

#include "custom_features.h"

// ═══════════════════════════════════════════════════════════════════
// Oryx-generated keycodes (kept for compatibility)
// ═══════════════════════════════════════════════════════════════════
enum custom_keycodes {
  RGB_SLD = ZSA_SAFE_RANGE,
  HSV_0_255_255,
  HSV_86_255_128,
  HSV_172_255_255,
};

// ═══════════════════════════════════════════════════════════════════
// Keymaps — 7 Layers
// ═══════════════════════════════════════════════════════════════════

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  // ─── Layer 0: Base ─────────────────────────────────────────────
  // QWERTY with GASC home row mods, LT(Nav) on Space, LT(Sym) on Enter
  [_BASE] = LAYOUT_moonlander(
    KC_EQL,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_ESC,                                           KC_DEL,   KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,
    KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     CW_TOGG,                                          QK_LEAD,  KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_BSLS,
    KC_MEH,   HRM_A,    HRM_S,    HRM_D,    HRM_F,    KC_G,     TG(_GAMING),                                                          QK_REP,   KC_H,     HRM_J,    HRM_K,    HRM_L,    HRM_SC,   KC_QUOT,
    KC_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,                                         KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  KC_RSFT,
    KC_LCTL,  KC_LALT,  KC_GRV,   KC_LBRC,  KC_RBRC,  KC_LGUI,                                                                                          MO(_SYSTEM), KC_UP,  KC_DOWN,  KC_LEFT,  KC_RGHT,  KC_RALT,
    NAV_SPC,  KC_BSPC,  KC_LCTL,                      KC_HYPR,  KC_TAB,   SYM_ENT
  ),

  // ─── Layer 1: Nav (hold Space) ────────────────────────────────
  // Right: HJKL arrows, Home/End/PgUp/PgDn
  // Left: plain modifiers for combining with arrows (Shift+arrow = select)
  [_NAV] = LAYOUT_moonlander(
    _______, _______, _______, _______, _______, _______, _______,                                          _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______,                                          _______, _______, KC_HOME, KC_PGUP, KC_PGDN, KC_END,  _______,
    _______, KC_LGUI, KC_LALT, KC_LSFT, KC_LCTL, _______, _______,                                                          _______, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______,
    _______, G(KC_Z), G(KC_X), G(KC_C), G(KC_V), _______,                                          _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______,                                                                                          _______, _______, _______, _______, _______, _______,
    _______, _______, _______,                   _______, _______, _______
  ),

  // ─── Layer 2: Sym (hold Enter) ────────────────────────────────
  // Right: numpad (789/456/123/0)
  // Left: symbols and plain modifiers
  [_SYM] = LAYOUT_moonlander(
    _______, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, _______,                                          _______, KC_EQL,  KC_7,    KC_8,    KC_9,    KC_PLUS, _______,
    _______, KC_CIRC, KC_AMPR, KC_LCBR, KC_RCBR, KC_PIPE, _______,                                          _______, KC_BSLS, KC_4,    KC_5,    KC_6,    KC_0,    _______,
    _______, KC_LGUI, KC_LALT, KC_LSFT, KC_LCTL, KC_TILD, _______,                                                          _______, KC_ASTR, KC_1,    KC_2,    KC_3,    KC_MINS, _______,
    _______, KC_GRV,  KC_UNDS, KC_LPRN, KC_RPRN, _______,                                          KC_COMM, KC_DOT,  _______, _______, _______, _______,
    _______, _______, _______, KC_LABK, KC_RABK, _______,                                                                                          _______, _______, _______, _______, _______, _______,
    _______, _______, _______,                   _______, _______, _______
  ),

  // ─── Layer 3: Func (Tri Layer: Nav + Sym) ─────────────────────
  // F-keys, media, volume
  [_FUNC] = LAYOUT_moonlander(
    _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   _______,                                          _______, KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,
    _______, _______, _______, _______, _______, _______, _______,                                          _______, _______, _______, KC_BRIU, _______, _______, KC_F12,
    _______, KC_LGUI, KC_LALT, KC_LSFT, KC_LCTL, _______, _______,                                                          _______, _______, KC_MPRV, KC_VOLD, KC_VOLU, KC_MNXT, _______,
    _______, _______, _______, _______, _______, _______,                                          _______, _______, _______, _______, KC_MPLY, _______,
    _______, _______, _______, _______, _______, _______,                                                                                          _______, _______, _______, _______, _______, _______,
    _______, _______, _______,                   _______, _______, _______
  ),

  // ─── Layer 4: Mouse (combo Z+X) ──────────────────────────────
  // Right: mouse movement, buttons, scroll
  // Left: modifiers
  [_MOUSE] = LAYOUT_moonlander(
    _______, _______, _______, _______, _______, _______, _______,                                          _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______,                                          _______, _______, MS_WHLU, MS_UP,   MS_WHLD, _______, _______,
    _______, KC_LGUI, KC_LALT, KC_LSFT, KC_LCTL, _______, _______,                                                          _______, _______, MS_LEFT, MS_DOWN, MS_RGHT, _______, _______,
    _______, _______, _______, _______, _______, _______,                                          _______, MS_BTN1, MS_BTN3, MS_BTN2, _______, _______,
    _______, _______, _______, _______, _______, _______,                                                                                          _______, MS_ACL0, MS_ACL1, MS_ACL2, _______, _______,
    MS_BTN1, MS_BTN2, _______,                   _______, _______, _______
  ),

  // ─── Layer 5: System (MO(5) on right bottom) ─────────────────
  // RGB, audio, bootloader, DT tuning
  [_SYSTEM] = LAYOUT_moonlander(
    AU_TOGG, _______, _______, _______, _______, _______, QK_BOOT,                                          QK_BOOT, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______,                                          _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______,                                                          _______, RGB_TOG, RGB_HUI, RGB_SAI, RGB_VAI, RGB_SPI, _______,
    _______, _______, _______, _______, _______, _______,                                          RGB_MOD, RGB_HUD, RGB_SAD, RGB_VAD, RGB_SPD, _______,
    _______, _______, _______, DT_DOWN, DT_UP,   DT_PRNT,                                                                                          _______, _______, _______, _______, _______, _______,
    _______, _______, _______,                   _______, _______, _______
  ),

  // ─── Layer 6: Gaming (TG(6)) ─────────────────────────────────
  // Full standard keyboard — no HRM, no LT, no combos
  // Left hand: explicit WASD + modifiers. Right hand: auxiliary.
  [_GAMING] = LAYOUT_moonlander(
    KC_ESC,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_F1,                                          KC_F3,    KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,
    KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_F2,                                          KC_F4,    KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_BSLS,
    KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     TG(_GAMING),                                                        KC_F5,    KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,
    KC_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,                                         KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  KC_RSFT,
    KC_LCTL,  KC_LALT,  KC_LGUI,  KC_LEFT,  KC_RGHT,  KC_F5,                                                                                          KC_F9,    KC_UP,    KC_DOWN,  KC_LBRC,  KC_RBRC,  KC_RALT,
    KC_SPC,   KC_BSPC,  KC_LCTL,                      KC_RCTL,  KC_TAB,   KC_ENT
  ),
};

// ═══════════════════════════════════════════════════════════════════
// LED Maps — Elemental Spectrum Theme
// ═══════════════════════════════════════════════════════════════════
//
// LED index order: column-major, top-to-bottom, per half
// Left  0-35:  Col0(0-4) Col1(5-9) Col2(10-14) Col3(15-19) Col4(20-24)
//              Col5(25-28) Inner(29-31) Special+Thumb(32-35)
// Right 36-71: ColR(36-40) Col1(41-45) Col2(46-50) Col3(51-55) Col4(56-60)
//              Col5(61-64) Inner(65-67) Special+Thumb(68-71)

extern rgb_config_t rgb_matrix_config;

RGB hsv_to_rgb_with_value(HSV hsv) {
  RGB rgb = hsv_to_rgb(hsv);
  float f = (float)rgb_matrix_config.hsv.v / UINT8_MAX;
  return (RGB){ f * rgb.r, f * rgb.g, f * rgb.b };
}

void keyboard_post_init_user(void) {
  rgb_matrix_enable();
}

const uint8_t PROGMEM ledmap[][RGB_MATRIX_LED_COUNT][3] = {

    // ─── Base: keep original Tokyo Night color scheme ────────────
    [_BASE] = {
        // Left half (0-35)
        {207,251,255}, {207,251,255}, {204,181,240}, {207,251,255}, {207,251,255},
        {204,181,240}, {225,251,255}, {225,251,255}, {225,251,255}, {207,251,255},
        {247,251,255}, {247,251,255}, {247,251,255}, {225,251,255}, {207,251,255},
        {141,251,255}, {141,251,255}, {247,251,255}, {225,251,255}, {207,251,255},
        {168,251,255}, {141,251,255}, {247,251,255}, {225,251,255}, {207,251,255},
        {168,251,255}, {141,251,255}, {247,251,255}, {225,251,255},
        {168,251,255}, {141,251,255}, {247,251,255},
        {138,251,255}, {138,251,255}, {138,251,255}, {138,251,255},
        // Right half (36-71)
        {207,251,255}, {207,251,255}, {207,251,255}, {204,181,240}, {207,251,255},
        {225,251,255}, {225,251,255}, {225,251,255}, {225,251,255}, {207,251,255},
        {247,251,255}, {247,251,255}, {247,251,255}, {225,251,255}, {207,251,255},
        {141,251,255}, {141,251,255}, {247,251,255}, {225,251,255}, {207,251,255},
        {168,251,255}, {141,251,255}, {247,251,255}, {225,251,255}, {207,251,255},
        {168,251,255}, {141,251,255}, {247,251,255}, {225,251,255},
        {168,251,255}, {141,251,255}, {247,251,255},
        {138,251,255}, {138,251,255}, {138,251,255}, {138,251,255},
    },

    // ─── Nav: Forest Green ───────────────────────────────────────
    [_NAV] = {
        // Left: Col0(=,Tab,MEH,LSft,LCtl)
        C_OFF, C_OFF, C_OFF, C_OFF, C_OFF,
        // Col1(1,Q,A=GUI,Z=CmdZ,Hypr)
        C_OFF, C_OFF, C_NAVM, C_NAVA, C_OFF,
        // Col2(2,W,S=Alt,X=CmdX,`)
        C_OFF, C_OFF, C_NAVM, C_NAVA, C_OFF,
        // Col3(3,E,D=Sft,C=CmdC,[)
        C_OFF, C_OFF, C_NAVM, C_NAVA, C_OFF,
        // Col4(4,R,F=Ctl,V=CmdV,])
        C_OFF, C_OFF, C_NAVM, C_NAVA, C_OFF,
        // Col5(5,T,G,B)
        C_OFF, C_OFF, C_OFF, C_OFF,
        // Inner(Esc,CW,TG)
        C_OFF, C_OFF, C_OFF,
        // Special+Thumb(GUI,Space[held],Bspc,Ctrl)
        C_OFF, C_NAV1, C_OFF, C_OFF,
        // Right: ColR(-,\,',RSft,RAlt)
        C_OFF, C_OFF, C_OFF, C_OFF, C_OFF,
        // Col1(0,P=End,;,/,Right)
        C_OFF, C_NAV2, C_OFF, C_OFF, C_OFF,
        // Col2(9,O=PgDn,L=Right,.,Left)
        C_OFF, C_NAV2, C_NAV1, C_OFF, C_OFF,
        // Col3(8,I=PgUp,K=Up,,Down)
        C_OFF, C_NAV2, C_NAV1, C_OFF, C_OFF,
        // Col4(7,U=Home,J=Down,M,Up)
        C_OFF, C_NAV2, C_NAV1, C_OFF, C_OFF,
        // Col5(6,Y,H=Left,N)
        C_OFF, C_OFF, C_NAV1, C_OFF,
        // Inner(Del,Lead,Rep)
        C_OFF, C_OFF, C_OFF,
        // Special+Thumb(MO5,Hypr,Tab,Enter)
        C_OFF, C_OFF, C_OFF, C_OFF,
    },

    // ─── Sym: Amber/Gold ─────────────────────────────────────────
    [_SYM] = {
        // Left: Col0(=,Tab,MEH,LSft,LCtl)
        C_OFF, C_OFF, C_OFF, C_OFF, C_OFF,
        // Col1(!=!,Q=^,A=GUI,Z=`,Hypr)
        C_SYM2, C_SYM2, C_SYMM, C_SYMM, C_OFF,
        // Col2(@,W=&,S=Alt,X=_,`)
        C_SYM2, C_SYM2, C_SYMM, C_SYMM, C_OFF,
        // Col3(#,E={,D=Sft,C=(,<)
        C_SYM2, C_SYM2, C_SYMM, C_SYM2, C_SYM2,
        // Col4($,R=},F=Ctl,V=),>)
        C_SYM2, C_SYM2, C_SYMM, C_SYM2, C_SYM2,
        // Col5(%,T=|,G=~,___)
        C_SYM2, C_SYM2, C_SYM2, C_OFF,
        // Inner
        C_OFF, C_OFF, C_OFF,
        // Thumb
        C_OFF, C_OFF, C_OFF, C_OFF,
        // Right: ColR(-,\,',RSft,RAlt)
        C_OFF, C_OFF, C_OFF, C_OFF, C_OFF,
        // Col1(+=+,P=0,;=-,/,Right)
        C_SYM3, C_SYM1, C_SYM3, C_OFF, C_OFF,
        // Col2(9,O=6,L=3,.,Left)
        C_SYM1, C_SYM1, C_SYM1, C_OFF, C_OFF,
        // Col3(8,I=5,K=2,,Down)
        C_SYM1, C_SYM1, C_SYM1, C_OFF, C_OFF,
        // Col4(7,U=4,J=1,M=.,Up)
        C_SYM1, C_SYM1, C_SYM1, C_SYM1, C_OFF,
        // Col5(=,Y=\,H=*,N=,)
        C_SYM3, C_SYMM, C_SYM3, C_SYM1,
        // Inner
        C_OFF, C_OFF, C_OFF,
        // Thumb
        C_OFF, C_OFF, C_OFF, C_OFF,
    },

    // ─── Func: Amethyst/Purple ───────────────────────────────────
    [_FUNC] = {
        // Left: Col0
        C_OFF, C_OFF, C_OFF, C_OFF, C_OFF,
        // Col1(F1)
        C_FUN1, C_OFF, C_SYMM, C_OFF, C_OFF,
        // Col2(F2)
        C_FUN1, C_OFF, C_SYMM, C_OFF, C_OFF,
        // Col3(F3)
        C_FUN1, C_OFF, C_SYMM, C_OFF, C_OFF,
        // Col4(F4)
        C_FUN1, C_OFF, C_SYMM, C_OFF, C_OFF,
        // Col5(F5)
        C_FUN1, C_OFF, C_OFF, C_OFF,
        // Inner
        C_OFF, C_OFF, C_OFF,
        // Thumb
        C_OFF, C_OFF, C_OFF, C_OFF,
        // Right: ColR(-,\,',RSft,RAlt)
        C_OFF, C_OFF, C_OFF, C_OFF, C_OFF,
        // Col1(F10,___,Next,Play,___)
        C_FUN1, C_OFF, C_FUN2, C_FUN2, C_OFF,
        // Col2(F9,___,VolU,___,___)
        C_FUN1, C_OFF, C_FUN3, C_OFF, C_OFF,
        // Col3(F8,BriU,VolD,___,___)
        C_FUN1, C_FUN2, C_FUN3, C_OFF, C_OFF,
        // Col4(F7,___,Prev,___,___)
        C_FUN1, C_OFF, C_FUN2, C_OFF, C_OFF,
        // Col5(F6,___,___,___)
        C_FUN1, C_OFF, C_OFF, C_OFF,
        // Inner
        C_OFF, C_OFF, C_OFF,
        // Thumb
        C_OFF, C_OFF, C_OFF, C_FUN1,
    },

    // ─── Mouse: Coral/Pink ───────────────────────────────────────
    [_MOUSE] = {
        // Left: mostly off, mods on home row
        C_OFF, C_OFF, C_OFF, C_OFF, C_OFF,
        C_OFF, C_OFF, C_MOU2, C_OFF, C_OFF,
        C_OFF, C_OFF, C_MOU2, C_OFF, C_OFF,
        C_OFF, C_OFF, C_MOU2, C_OFF, C_OFF,
        C_OFF, C_OFF, C_MOU2, C_OFF, C_OFF,
        C_OFF, C_OFF, C_OFF, C_OFF,
        C_OFF, C_OFF, C_OFF,
        C_OFF, C_MOU2, C_MOU2, C_OFF,
        // Right: movement on home row, buttons below
        C_OFF, C_OFF, C_OFF, C_OFF, C_OFF,
        C_OFF, C_OFF, C_OFF, C_OFF, C_OFF,
        C_OFF, C_MOU3, C_MOU1, C_OFF, C_OFF,
        C_OFF, C_MOU1, C_MOU1, C_OFF, C_OFF,
        C_OFF, C_MOU3, C_MOU1, C_MOU2, C_OFF,
        C_OFF, C_OFF, C_MOU1, C_MOU2,
        C_OFF, C_OFF, C_OFF,
        C_OFF, C_OFF, C_OFF, C_OFF,
    },

    // ─── System: Ember/Red ───────────────────────────────────────
    [_SYSTEM] = {
        // Left: boot on inner top, audio toggle, DT on bottom
        C_SYS3, C_OFF, C_OFF, C_OFF, C_OFF,
        C_OFF, C_OFF, C_OFF, C_OFF, C_OFF,
        C_OFF, C_OFF, C_OFF, C_OFF, C_OFF,
        C_OFF, C_OFF, C_OFF, C_OFF, C_SYS2,
        C_OFF, C_OFF, C_OFF, C_OFF, C_SYS2,
        C_OFF, C_OFF, C_OFF, C_OFF,
        C_SYS1, C_OFF, C_OFF,
        C_SYS2, C_OFF, C_OFF, C_OFF,
        // Right: RGB on home row, boot on inner top
        C_OFF, C_OFF, C_OFF, C_OFF, C_OFF,
        C_OFF, C_OFF, C_SYS2, C_OFF, C_OFF,
        C_OFF, C_OFF, C_SYS2, C_OFF, C_OFF,
        C_OFF, C_OFF, C_SYS2, C_OFF, C_OFF,
        C_OFF, C_OFF, C_SYS2, C_SYS2, C_OFF,
        C_OFF, C_OFF, C_SYS2, C_SYS2,
        C_SYS1, C_OFF, C_OFF,
        C_OFF, C_OFF, C_OFF, C_OFF,
    },

    // ─── Gaming: Neon Lime ───────────────────────────────────────
    [_GAMING] = {
        // Left: Col0(Esc,Tab,Caps,LSft,LCtl)
        C_GAM3, C_GAM1, C_GAMM, C_GAMM, C_GAMM,
        // Col1(1,Q,A,Z,LAlt)
        C_GAM2, C_GAM1, C_GAM1, C_GAM1, C_GAMM,
        // Col2(2,W,S,X,LGui)
        C_GAM2, C_GAM1, C_GAM1, C_GAM1, C_GAMM,
        // Col3(3,E,D,C,Left)
        C_GAM2, C_GAM1, C_GAM1, C_GAM1, C_GAM4,
        // Col4(4,R,F,V,Right)
        C_GAM2, C_GAM1, C_GAM1, C_GAM1, C_GAM4,
        // Col5(5,T,G,B)
        C_GAM2, C_GAM1, C_GAM1, C_GAM1,
        // Inner(F1,F2,TG)
        C_GAM3, C_GAM3, C_WHITE,
        // Thumb(F5,Space,Bspc,Ctrl)
        C_GAM3, C_GAM1, C_GAM1, C_GAMM,
        // Right: Col0(-,\,',RSft,RAlt)
        C_GAM4, C_GAM4, C_GAM4, C_GAM4, C_GAM4,
        // Col1(0,P,;,/,])
        C_GAM4, C_GAM4, C_GAM4, C_GAM4, C_GAM4,
        // Col2(9,O,L,.,])
        C_GAM4, C_GAM4, C_GAM4, C_GAM4, C_GAM4,
        // Col3(8,I,K,,Down)
        C_GAM4, C_GAM4, C_GAM4, C_GAM4, C_GAM4,
        // Col4(7,U,J,M,Up)
        C_GAM4, C_GAM4, C_GAM4, C_GAM4, C_GAM4,
        // Col5(6,Y,H,N)
        C_GAM4, C_GAM4, C_GAM4, C_GAM4,
        // Inner(F3,F4,F5)
        C_GAM3, C_GAM3, C_GAM3,
        // Thumb(F9,RCtl,Tab,Enter)
        C_GAM3, C_GAM4, C_GAM4, C_GAM4,
    },
};

// ═══════════════════════════════════════════════════════════════════
// RGB Layer Indicators
// ═══════════════════════════════════════════════════════════════════

void set_layer_color(int layer) {
  for (int i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
    HSV hsv = {
      .h = pgm_read_byte(&ledmap[layer][i][0]),
      .s = pgm_read_byte(&ledmap[layer][i][1]),
      .v = pgm_read_byte(&ledmap[layer][i][2]),
    };
    if (!hsv.h && !hsv.s && !hsv.v) {
        rgb_matrix_set_color(i, 0, 0, 0);
    } else {
        RGB rgb = hsv_to_rgb_with_value(hsv);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
  }
}

bool rgb_matrix_indicators_user(void) {
  if (rawhid_state.rgb_control) {
      return false;
  }
  if (!keyboard_config.disable_layer_led) {
    switch (biton32(layer_state)) {
      case _BASE:    set_layer_color(_BASE);    break;
      case _NAV:     set_layer_color(_NAV);     break;
      case _SYM:     set_layer_color(_SYM);     break;
      case _FUNC:    set_layer_color(_FUNC);    break;
      case _MOUSE:   set_layer_color(_MOUSE);   break;
      case _SYSTEM:  set_layer_color(_SYSTEM);  break;
      case _GAMING:  set_layer_color(_GAMING);  break;
      default:
        if (rgb_matrix_get_flags() == LED_FLAG_NONE) {
          rgb_matrix_set_color_all(0, 0, 0);
        }
    }
  } else {
    if (rgb_matrix_get_flags() == LED_FLAG_NONE) {
      rgb_matrix_set_color_all(0, 0, 0);
    }
  }
  return true;
}

// ═══════════════════════════════════════════════════════════════════
// Process Record — Oryx-generated cases kept, custom cases at end
// ═══════════════════════════════════════════════════════════════════

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case QK_MODS ... QK_MODS_MAX:
      // Oryx: mouse/consumer keys with modifiers
      if (IS_MOUSE_KEYCODE(QK_MODS_GET_BASIC_KEYCODE(keycode)) || IS_CONSUMER_KEYCODE(QK_MODS_GET_BASIC_KEYCODE(keycode))) {
        if (record->event.pressed) {
          add_mods(QK_MODS_GET_MODS(keycode));
          send_keyboard_report();
          wait_ms(2);
          register_code(QK_MODS_GET_BASIC_KEYCODE(keycode));
          return false;
        } else {
          wait_ms(2);
          del_mods(QK_MODS_GET_MODS(keycode));
        }
      }
      break;
    case RGB_SLD:
      if (rawhid_state.rgb_control) { return false; }
      if (record->event.pressed) { rgblight_mode(1); }
      return false;
    case HSV_0_255_255:
      if (rawhid_state.rgb_control) { return false; }
      if (record->event.pressed) { rgblight_mode(1); rgblight_sethsv(0,255,255); }
      return false;
    case HSV_86_255_128:
      if (rawhid_state.rgb_control) { return false; }
      if (record->event.pressed) { rgblight_mode(1); rgblight_sethsv(86,255,128); }
      return false;
    case HSV_172_255_255:
      if (rawhid_state.rgb_control) { return false; }
      if (record->event.pressed) { rgblight_mode(1); rgblight_sethsv(172,255,255); }
      return false;
  }
  return true;
}

// ═══════════════════════════════════════════════════════════════════
// Custom Feature Implementations (combos, chordal hold, leader, etc.)
// ═══════════════════════════════════════════════════════════════════
#include "custom_features.c"
