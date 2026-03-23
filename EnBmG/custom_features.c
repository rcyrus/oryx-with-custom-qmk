// custom_features.c — Dream Layout implementations
// #included at the end of keymap.c (same compilation unit)

// ═══════════════════════════════════════════════════════════════════
// Chordal Hold — opposite-hand rule for home row mods
// ═══════════════════════════════════════════════════════════════════

const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM =
    LAYOUT_moonlander(
        'L', 'L', 'L', 'L', 'L', 'L', '*',   '*', 'R', 'R', 'R', 'R', 'R', 'R',
        'L', 'L', 'L', 'L', 'L', 'L', '*',   '*', 'R', 'R', 'R', 'R', 'R', 'R',
        'L', 'L', 'L', 'L', 'L', 'L', '*',   '*', 'R', 'R', 'R', 'R', 'R', 'R',
        'L', 'L', 'L', 'L', 'L', 'L',             'R', 'R', 'R', 'R', 'R', 'R',
        'L', 'L', 'L', 'L', 'L', '*',             '*', 'R', 'R', 'R', 'R', 'R',
                       '*', '*', '*',             '*', '*', '*'
    );

// Allow same-hand shortcuts (Cmd+Z/X/C/V/S/W/Q/T on macOS, Ctrl+Z/X/C/V in Emacs)
bool get_chordal_hold(uint16_t tap_hold_keycode, keyrecord_t *tap_hold_record,
                      uint16_t other_keycode, keyrecord_t *other_record) {
    switch (tap_hold_keycode) {
        // Left GUI (A) — macOS Cmd shortcuts
        case HRM_A:
            switch (other_keycode) {
                case KC_Z: case KC_X: case KC_C: case KC_V:
                case HRM_S: case HRM_D: case HRM_F:
                case KC_Q: case KC_W: case KC_E: case KC_R: case KC_T:
                    return true;
            }
            break;
        // Left Ctrl (F) — Emacs C-x, C-c, C-a, C-s, C-z etc.
        case HRM_F:
            switch (other_keycode) {
                case KC_Z: case KC_X: case KC_C: case KC_V:
                case HRM_A: case HRM_S: case HRM_D:
                case KC_Q: case KC_W: case KC_E: case KC_R: case KC_T:
                case KC_G: case KC_B:
                    return true;
            }
            break;
        // Left Shift (D) — capitalize left-hand letters
        case HRM_D:
            switch (other_keycode) {
                case HRM_A: case HRM_S: case HRM_F:
                case KC_Z: case KC_X: case KC_C: case KC_V: case KC_B:
                case KC_Q: case KC_W: case KC_E: case KC_R: case KC_T:
                case KC_G:
                    return true;
            }
            break;
        // Right Ctrl (J) — Emacs chords with right-hand letters
        case HRM_J:
            switch (other_keycode) {
                case HRM_K: case HRM_L: case HRM_SC:
                case KC_N: case KC_M: case KC_COMM: case KC_DOT:
                case KC_Y: case KC_U: case KC_I: case KC_O: case KC_P:
                case KC_H:
                    return true;
            }
            break;
        // Right Shift (K) — capitalize right-hand letters
        case HRM_K:
            switch (other_keycode) {
                case HRM_J: case HRM_L: case HRM_SC:
                case KC_N: case KC_M: case KC_COMM: case KC_DOT:
                case KC_Y: case KC_U: case KC_I: case KC_O: case KC_P:
                case KC_H:
                    return true;
            }
            break;
    }
    return get_chordal_hold_default(tap_hold_record, other_record);
}

// ═══════════════════════════════════════════════════════════════════
// Flow Tap — disable hold during fast typing
// ═══════════════════════════════════════════════════════════════════

bool is_flow_tap_key(uint16_t keycode) {
    if ((get_mods() & (MOD_MASK_CG | MOD_BIT_LALT)) != 0) {
        return false;  // Don't interfere with active hotkeys
    }
    switch (get_tap_keycode(keycode)) {
        case KC_SPC:
        case KC_A ... KC_Z:
        case KC_DOT:
        case KC_COMM:
        case KC_SCLN:
            return true;
    }
    return false;
}

// ═══════════════════════════════════════════════════════════════════
// Per-Key Tapping Term — pinkies need more time
// ═══════════════════════════════════════════════════════════════════

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case HRM_A: case HRM_SC:   // Pinkies
            return TAPPING_TERM + 40;
        case HRM_S: case HRM_L:    // Ring fingers
            return TAPPING_TERM + 20;
        case NAV_SPC: case SYM_ENT: // Thumb layer-taps
            return TAPPING_TERM - 20;
        default:
            return TAPPING_TERM;
    }
}

// ═══════════════════════════════════════════════════════════════════
// Combos
// ═══════════════════════════════════════════════════════════════════

const uint16_t PROGMEM jk_esc[]    = {HRM_J, HRM_K, COMBO_END};
const uint16_t PROGMEM df_tab[]    = {HRM_D, HRM_F, COMBO_END};
const uint16_t PROGMEM kl_bspc[]   = {HRM_K, HRM_L, COMBO_END};
const uint16_t PROGMEM zx_mouse[]  = {KC_Z,  KC_X,  COMBO_END};

combo_t key_combos[CMB_COUNT] = {
    [CMB_ESC]   = COMBO(jk_esc,   KC_ESC),
    [CMB_TAB]   = COMBO(df_tab,   KC_TAB),
    [CMB_BSPC]  = COMBO(kl_bspc,  KC_BSPC),
    [CMB_MOUSE] = COMBO(zx_mouse, MO(_MOUSE)),
};

// Disable combos on gaming layer
bool combo_should_trigger(uint16_t combo_index, combo_t *combo,
                          uint16_t keycode, keyrecord_t *record) {
    if (layer_state_is(_GAMING)) return false;
    return true;
}

// ═══════════════════════════════════════════════════════════════════
// Key Overrides
// ═══════════════════════════════════════════════════════════════════

// Shift + Backspace = Delete
const key_override_t delete_key_override =
    ko_make_basic(MOD_MASK_SHIFT, KC_BSPC, KC_DEL);

const key_override_t *key_overrides[] = {
    &delete_key_override,
    NULL
};

// ═══════════════════════════════════════════════════════════════════
// Leader Key — Emacs prefix sequences and utilities
// ═══════════════════════════════════════════════════════════════════

void leader_end_user(void) {
    // Leader → x → s = C-x C-s (save buffer)
    if (leader_sequence_two_keys(KC_X, KC_S)) {
        SEND_STRING(SS_LCTL("x") SS_LCTL("s"));
    }
    // Leader → x → f = C-x C-f (find file)
    else if (leader_sequence_two_keys(KC_X, KC_F)) {
        SEND_STRING(SS_LCTL("x") SS_LCTL("f"));
    }
    // Leader → x → b = C-x b (switch buffer)
    else if (leader_sequence_two_keys(KC_X, KC_B)) {
        SEND_STRING(SS_LCTL("x") "b");
    }
    // Leader → x → k = C-x k (kill buffer)
    else if (leader_sequence_two_keys(KC_X, KC_K)) {
        SEND_STRING(SS_LCTL("x") "k");
    }
    // Leader → x → 0 = C-x 0 (close window)
    else if (leader_sequence_two_keys(KC_X, KC_0)) {
        SEND_STRING(SS_LCTL("x") "0");
    }
    // Leader → x → 1 = C-x 1 (only window)
    else if (leader_sequence_two_keys(KC_X, KC_1)) {
        SEND_STRING(SS_LCTL("x") "1");
    }
    // Leader → x → 2 = C-x 2 (split horizontal)
    else if (leader_sequence_two_keys(KC_X, KC_2)) {
        SEND_STRING(SS_LCTL("x") "2");
    }
    // Leader → x → 3 = C-x 3 (split vertical)
    else if (leader_sequence_two_keys(KC_X, KC_3)) {
        SEND_STRING(SS_LCTL("x") "3");
    }
    // Leader → g = C-x g (magit-status)
    else if (leader_sequence_one_key(KC_G)) {
        SEND_STRING(SS_LCTL("x") "g");
    }
    // Leader → c → c = C-c C-c (context action in many modes)
    else if (leader_sequence_two_keys(KC_C, KC_C)) {
        SEND_STRING(SS_LCTL("c") SS_LCTL("c"));
    }
}

// ═══════════════════════════════════════════════════════════════════
// Repeat Key — alternate repeat definitions
// ═══════════════════════════════════════════════════════════════════

uint16_t get_alt_repeat_key_keycode_user(uint16_t keycode, uint8_t mods) {
    switch (keycode) {
        case KC_LEFT:  return KC_RGHT;
        case KC_RGHT:  return KC_LEFT;
        case KC_UP:    return KC_DOWN;
        case KC_DOWN:  return KC_UP;
        case KC_PGUP:  return KC_PGDN;
        case KC_PGDN:  return KC_PGUP;
        case KC_HOME:  return KC_END;
        case KC_END:   return KC_HOME;
    }
    return KC_TRNS;
}

// ═══════════════════════════════════════════════════════════════════
// Caps Word — custom continuation keys
// ═══════════════════════════════════════════════════════════════════

bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        case KC_A ... KC_Z:
        case KC_MINS:           // - becomes _ during caps word
            add_weak_mods(MOD_BIT(KC_LSFT));
            return true;
        case KC_1 ... KC_0:
        case KC_BSPC:
        case KC_DEL:
        case KC_UNDS:
            return true;        // Continue caps word without shifting
        default:
            return false;       // Deactivate caps word
    }
}

// Caps Word LED indicator using Moonlander status LEDs
void caps_word_set_user(bool active) {
    STATUS_LED_1(active);
    STATUS_LED_4(active);
}
