# QMK Custom Firmware Guidebook

> **Audience:** LLM agents helping Rajeev design efficient keyboard layouts, particularly those configuring emacs and zellij keybindings in parallel sessions.
>
> **Hardware:** ZSA Moonlander Mark I (Rev A), 72-key split ergonomic keyboard with hot-swappable MX switches, per-key RGB, thumb clusters, and STM32F303 MCU.
>
> **Workflow:** This repo uses [oryx-with-custom-qmk](https://github.com/poulainpi/oryx-with-custom-qmk) — a GitHub Action that merges Oryx graphical layout changes with custom QMK C code, builds firmware, and produces a flashable `.bin`. Layout ID: `EnBmG`, geometry: `moonlander/reva`, firmware: v25.

---

## Table of Contents

1. [Workflow Constraints](#1-workflow-constraints)
2. [Moonlander Hardware Reference](#2-moonlander-hardware-reference)
3. [Tap-Hold System](#3-tap-hold-system)
4. [Home Row Mods](#4-home-row-mods)
5. [Layers](#5-layers)
6. [Combos](#6-combos)
7. [Key Overrides](#7-key-overrides)
8. [Caps Word](#8-caps-word)
9. [Repeat Key](#9-repeat-key)
10. [Leader Key](#10-leader-key)
11. [Tap Dance](#11-tap-dance)
12. [One Shot Keys](#12-one-shot-keys)
13. [Macros and Send String](#13-macros-and-send-string)
14. [Autocorrect](#14-autocorrect)
15. [Mouse Keys](#15-mouse-keys)
16. [OS Detection](#16-os-detection)
17. [Unicode](#17-unicode)
18. [Swap Hands](#18-swap-hands)
19. [Emacs-Specific Patterns](#19-emacs-specific-patterns)
20. [Terminal Multiplexer Patterns](#20-terminal-multiplexer-patterns)
21. [Oryx Feature Compatibility Matrix](#21-oryx-feature-compatibility-matrix)
22. [Resource Index](#22-resource-index)

---

## 1. Workflow Constraints

### Branch Model

- **`oryx` branch**: Receives Oryx-generated code exclusively. Managed by the GitHub Action. Never commit directly.
- **`main` branch**: User's custom QMK code. The workflow merges `oryx` into `main` using `git merge -Xignore-all-space`.

### Files Oryx Overwrites

Each workflow run regenerates three files in the `EnBmG/` directory:

| File | What Oryx Generates |
|------|---------------------|
| `keymap.c` | Full keymap arrays, LED maps, `process_record_user()` switch cases, tap dance definitions |
| `config.h` | `ORYX_CONFIGURATOR`, `FIRMWARE_VERSION`, USB defines, feature-specific defines |
| `rules.mk` | Feature enable flags Oryx needs (e.g., `TAP_DANCE_ENABLE`, `AUTO_SHIFT_ENABLE`) |

### Safe Customization Patterns

1. **Add code at the END of files**, separated by blank lines from Oryx-generated content. This minimizes merge conflicts.
2. **For maximum safety, create separate files** (e.g., `custom_features.c`, `custom_features.h`) in the layout folder and `#include` them from `keymap.c`.
3. **Custom keycodes**: Start from `SAFE_RANGE` (Oryx provides a `ZSA_SAFE_RANGE` alias).
4. **In `process_record_user()`**: Add new `case` statements at the end of the existing `switch` block.
5. **In `rules.mk`**: Append new feature enables at the bottom.
6. **In `config.h`**: Append new `#define` lines at the bottom.

### What Agents Must Not Do

- Never modify the middle of Oryx-generated code — edits in the middle of `keymaps[]`, `ledmap[]`, or the existing `process_record_user()` cases will cause merge conflicts on the next Oryx update.
- Never redefine `raw_hid_receive()` — the Oryx module owns this function for Keymapp communication.
- Always check `rawhid_state.rgb_control` before overriding RGB behavior.

---

## 2. Moonlander Hardware Reference

| Spec | Value |
|------|-------|
| Keys | 72 total (36 per half) |
| MCU | STM32F303 (ARM Cortex-M4) |
| Matrix | 12 rows x 7 columns |
| RGB LEDs | 72 per-key (IS31FL3731, max brightness 175) |
| Status LEDs | 6 (3 per side), controlled via `ML_LED_1()` through `ML_LED_6()` |
| Audio | DAC additive driver |
| EEPROM | I2C 24LC128 (Rev A) |
| USB | VID `0x3297`, PID `0x1969` (Rev A) |
| Dynamic keymap layers | 8 max |
| Thumb cluster | 4 keys per side, tiltable |
| Enabled features | Audio, bootmagic, command, console, deferred_exec, extrakey, mousekey, nkro, rgb_matrix, swap_hands |

### Physical Layout

The Moonlander uses `LAYOUT_moonlander` (72 keys) or `LAYOUT_halfmoon` (36 keys, one-handed). The thumb cluster on each side has 4 keys, but only the 1-2 innermost are comfortably reachable for most hand sizes. Place your most-used keys on the inner thumb positions.

---

## 3. Tap-Hold System

Tap-hold is QMK's dual-function key system. A key performs one action when tapped and a different action when held. This underpins Mod-Tap (`MT`), Layer-Tap (`LT`), and all home row mod implementations. No enable flag needed — it is built into QMK core.

### Configuration Defines

| Define | Default | Purpose |
|--------|---------|---------|
| `TAPPING_TERM` | 200ms | Threshold separating tap from hold |
| `QUICK_TAP_TERM` | `TAPPING_TERM` | Window for auto-repeat on double-tap-and-hold. Set to `0` to always activate hold. |
| `PERMISSIVE_HOLD` | off | Another key tapped (pressed+released) during hold triggers hold action even within `TAPPING_TERM` |
| `HOLD_ON_OTHER_KEY_PRESS` | off | Hold triggers immediately when any other key is pressed. **Incompatible with home row mods.** |
| `RETRO_TAPPING` | off | Send tap keycode when releasing after `TAPPING_TERM` expires with no other key pressed |
| `CHORDAL_HOLD` | off | Built-in opposite-hands rule (successor to Achordion). Hold only triggers when second key is on opposite hand. |
| `FLOW_TAP_TERM` | none | Disables hold behavior during fast typing if next key follows within this window |
| `SPECULATIVE_HOLD` | off | Applies modifiers immediately on keydown, cancels if the key turns out to be a tap |

### Decision Mode Hierarchy

1. **Default** — Only timing matters. Other keys are ignored.
2. **PERMISSIVE_HOLD** — Another key being *tapped* (press+release) during hold triggers hold action.
3. **HOLD_ON_OTHER_KEY_PRESS** — Another key being *pressed* (even without release) triggers hold. Takes precedence over `PERMISSIVE_HOLD`.

### Per-Key Configuration

Every major define has a `_PER_KEY` variant enabling a callback:

```c
#define TAPPING_TERM_PER_KEY
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LGUI_T(KC_A):
        case RGUI_T(KC_SCLN):
            return TAPPING_TERM + 40;  // Pinkies need more time
        case LALT_T(KC_S):
        case LALT_T(KC_L):
            return TAPPING_TERM + 20;  // Ring fingers
        default:
            return TAPPING_TERM;
    }
}
```

### Dynamic Tapping Term (Runtime Tuning)

```c
// rules.mk
DYNAMIC_TAPPING_TERM_ENABLE = yes

// config.h
#define DYNAMIC_TAPPING_TERM_INCREMENT 5

// Keycodes: DT_PRNT (print current value), DT_UP, DT_DOWN
```

### Chordal Hold (Built-in, February 2025)

The modern replacement for Achordion. Define a hand-assignment matrix, and hold only activates when the second key is on the opposite hand:

```c
#define CHORDAL_HOLD

const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM =
    LAYOUT_moonlander(
        'L', 'L', 'L', 'L', 'L', 'L', 'L',   'R', 'R', 'R', 'R', 'R', 'R', 'R',
        'L', 'L', 'L', 'L', 'L', 'L', 'L',   'R', 'R', 'R', 'R', 'R', 'R', 'R',
        'L', 'L', 'L', 'L', 'L', 'L', 'L',   'R', 'R', 'R', 'R', 'R', 'R', 'R',
        'L', 'L', 'L', 'L', 'L', 'L',             'R', 'R', 'R', 'R', 'R', 'R',
        'L', 'L', 'L', 'L', 'L', '*',             '*', 'R', 'R', 'R', 'R', 'R',
                       '*', '*', '*',             '*', '*', '*'
    );
```

Use `'*'` for keys that should be exempt from the opposite-hand rule (thumb clusters, inner column keys).

Per-chord customization for same-hand shortcuts (Ctrl+Z/X/C/V):

```c
bool get_chordal_hold(uint16_t tap_hold_keycode, keyrecord_t* tap_hold_record,
                      uint16_t other_keycode, keyrecord_t* other_record) {
    switch (tap_hold_keycode) {
        case LCTL_T(KC_F):
            switch (other_keycode) {
                case KC_Z: case KC_X: case KC_C: case KC_V:
                    return true;  // Allow same-hand Ctrl+C, Ctrl+V
            }
            break;
    }
    return get_chordal_hold_default(tap_hold_record, other_record);
}
```

### Flow Tap (May 2025)

Disables hold functions during fast typing. Only activates hold if there is a brief pause before the keypress:

```c
#define FLOW_TAP_TERM 150  // Recommended starting value (75-200ms range)

bool is_flow_tap_key(uint16_t keycode) {
    if ((get_mods() & (MOD_MASK_CG | MOD_BIT_LALT)) != 0) {
        return false;  // Don't interfere with active hotkeys
    }
    switch (get_tap_keycode(keycode)) {
        case KC_SPC:
        case KC_A ... KC_Z:
        case KC_DOT:
        case KC_COMM:
            return true;
    }
    return false;
}
```

### Anti-Patterns

- Setting `TAPPING_TERM` below 150ms — constant accidental holds.
- Enabling `HOLD_ON_OTHER_KEY_PRESS` with home row mods — breaks normal typing rolls.
- Forgetting `QUICK_TAP_TERM 0` — leads to unwanted auto-repeat ("camelllllCase").
- Using `RETRO_TAPPING` with GUI/Alt — triggers OS-level menu activations.

---

## 4. Home Row Mods

Home row mods place the four modifiers (GUI, Alt, Shift, Ctrl) on the home row alpha keys as mod-taps. Both hands mirror each other.

### Modifier Orderings

| Order | Pinky | Ring | Middle | Index | Best For |
|-------|-------|------|--------|-------|----------|
| **GASC** | GUI | Alt | Shift | Ctrl | Windows/Linux (most popular) |
| **GACS** | GUI | Alt | Ctrl | Shift | Miryoku layout; minimizes accidental capitals |
| **CAGS** | Ctrl | Alt | GUI | Shift | macOS (accounts for Cmd frequency) |

### Recommended Configuration

```c
// config.h
#define TAPPING_TERM 200
#define QUICK_TAP_TERM 0
#define CHORDAL_HOLD
#define TAPPING_TERM_PER_KEY

// keymap.c — GASC order example for QWERTY
// Left hand
#define GUI_A  LGUI_T(KC_A)
#define ALT_S  LALT_T(KC_S)
#define SFT_D  LSFT_T(KC_D)
#define CTL_F  LCTL_T(KC_F)

// Right hand (mirrored)
#define CTL_J  RCTL_T(KC_J)
#define SFT_K  RSFT_T(KC_K)
#define ALT_L  LALT_T(KC_L)      // LALT, NOT RALT — RALT is AltGr on non-US layouts
#define GUI_SC RGUI_T(KC_SCLN)
```

### The RALT/AltGr Problem

On non-US layouts, `RALT` becomes AltGr. **Always use `LALT_T()` for the right-hand Alt mod-tap** unless AltGr is specifically needed.

### Tuning Process

1. Start at 220-250ms to avoid accidental mods during learning.
2. Use `DT_DOWN` keycode to progressively lower `TAPPING_TERM`.
3. When accidental holds appear, bump back up 10-20ms.
4. Give pinkies +30-50ms over index fingers via per-key tapping term.
5. Budget ~2 months to build reliable muscle memory.

### Common Problems and Solutions

| Problem | Cause | Solution |
|---------|-------|----------|
| Accidental capitals ("THe") | Shift held too long during roll | Enable `CHORDAL_HOLD` or lower `TAPPING_TERM` |
| "camelllllCase" on repeat | `QUICK_TAP_TERM` too high | Set `QUICK_TAP_TERM 0` |
| Modifier won't activate | `TAPPING_TERM` too low | Increase `TAPPING_TERM` or enable `PERMISSIVE_HOLD` |
| Same-hand shortcuts fail (Ctrl+C) | Bilateral combinations blocking | Add exceptions in `get_chordal_hold()` |
| Slow modifier response | Waiting full `TAPPING_TERM` | Use `SPECULATIVE_HOLD` or `achordion_eager_mod()` |

### Alternative Approaches

- **Callum-style one-shot mods**: Modifiers on a separate layer's home row as one-shot keys. No timers involved. Tap layer key, tap mod, release layer, press target key. Consistent and predictable but requires more keypresses.
- **Bottom row mods**: Place mod-taps on ZXCV row instead of home row. Less misfire-prone, harder to reach.
- **Thumb cluster mods**: Place Ctrl, Shift, Alt, GUI on the 4 thumb keys. Most ergonomic for Moonlander users — eliminates home row misfire issues entirely.

---

## 5. Layers

QMK uses a stack-based layer system. Multiple layers can be active simultaneously. The firmware scans from the highest-numbered active layer downward, using the first non-`KC_TRNS` (transparent) key it finds.

### Layer Keycodes

| Keycode | Behavior |
|---------|----------|
| `MO(layer)` | Momentary — active while held |
| `LT(layer, kc)` | Hold for layer, tap for keycode |
| `TG(layer)` | Toggle layer on/off |
| `TO(layer)` | Activate layer exclusively |
| `TT(layer)` | Hold = momentary, tap 5x = toggle |
| `DF(layer)` | Change default layer (volatile) |
| `PDF(layer)` | Change default layer (persisted to EEPROM) |
| `OSL(layer)` | One-shot layer — active for one keypress |
| `LM(layer, mod)` | Momentary layer with modifier(s) |
| `QK_LLCK` | Layer Lock — lock current momentary layer |

### Limitations

- `LT()` and `LM()` support layers 0-15 only.
- `LT()` keycode argument is limited to Basic Keycode set (max 0xFF).

### Recommended Layer Organization

| Layer | Name | Purpose | Activation |
|-------|------|---------|------------|
| 0 | Base | Primary alpha layout | Default |
| 1 | Nav | Arrows, Home/End, PgUp/PgDn | `MO(1)` or `LT(1, kc)` on thumb |
| 2 | Num | Number row, numpad | `MO(2)` or `LT(2, kc)` on thumb |
| 3 | Sym | Symbols, brackets, operators | `MO(3)` or Tri Layer (Nav+Num) |
| 4 | Func | F1-F12, F13-F24 | `MO(4)` |
| 5 | Media | Volume, brightness, playback | `MO(5)` or combo |
| 6 | Gaming | Plain keys (no mod-taps) | `TG(6)` |

### Tri Layer

Automatically activates a third layer when two designated layers are active simultaneously:

```makefile
# rules.mk
TRI_LAYER_ENABLE = yes
```

```c
// config.h
#define TRI_LAYER_LOWER_LAYER 1
#define TRI_LAYER_UPPER_LAYER 2
#define TRI_LAYER_ADJUST_LAYER 3
```

Keycodes: `TL_LOWR`, `TL_UPPR`

### Layer State Callback

```c
layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
        case _NAV:
            rgblight_setrgb(0x00, 0x00, 0xFF);
            break;
        case _NUM:
            rgblight_setrgb(0xFF, 0x00, 0x00);
            break;
        default:
            rgblight_setrgb(0x00, 0xFF, 0xFF);
            break;
    }
    return state;
}
```

### Layer Functions (for custom code)

```c
layer_on(layer);
layer_off(layer);
layer_move(layer);        // Activate one, deactivate all others
layer_clear();
layer_invert(layer);      // Toggle
layer_state_is(layer);    // Check if active
get_highest_layer(layer_state);
```

### Best Practices

- Layer 0 should always be the base alpha layout.
- Always have a way back to layer 0 from every layer.
- Use `MO()` for temporary layers, `TG()` for persistent toggles.
- Arrange layers in a tree — don't enter the same layer from multiple parents.

### Symbol Layer Design Principles (from getreuer)

1. Place most-used symbols on home row positions.
2. Avoid pinkies for keys often double-tapped (`==`, `++`, `//`).
3. Optimize common bigrams as "inward rolls" (keystrokes moving toward center).
4. `.`, `,`, and `_` are frequent enough to arguably belong on the base layer.

---

## 6. Combos

Press multiple keys simultaneously to produce a different keycode or action.

### Enable

```makefile
# rules.mk
COMBO_ENABLE = yes
```

### Basic Usage

```c
const uint16_t PROGMEM jk_esc[] = {KC_J, KC_K, COMBO_END};
const uint16_t PROGMEM df_tab[] = {KC_D, KC_F, COMBO_END};

combo_t key_combos[] = {
    COMBO(jk_esc, KC_ESC),
    COMBO(df_tab, KC_TAB),
};
```

### Custom Action Combos

```c
enum combo_events { EM_EMAIL, BSPC_LSFT_CLEAR };

const uint16_t PROGMEM email_combo[] = {KC_E, KC_M, COMBO_END};
combo_t key_combos[] = {
    [EM_EMAIL] = COMBO_ACTION(email_combo),
};

void process_combo_event(uint16_t combo_index, bool pressed) {
    switch (combo_index) {
        case EM_EMAIL:
            if (pressed) SEND_STRING("user@example.com");
            break;
    }
}
```

### Configuration

| Define | Default | Purpose |
|--------|---------|---------|
| `COMBO_TERM` | 50ms | Recognition window |
| `COMBO_MUST_HOLD_MODS` | off | Extend window for modifier combos |
| `COMBO_MUST_PRESS_IN_ORDER` | off | Enforce key press sequence |
| `COMBO_ONLY_FROM_LAYER` | off | Reference a single layer for combo positions |
| `EXTRA_SHORT_COMBOS` | off | Limit to 6-key combos (saves memory) |

### Per-Combo Configuration

```c
uint16_t get_combo_term(uint16_t combo_index, combo_t *combo) { ... }
bool get_combo_must_hold(uint16_t combo_index, combo_t *combo) { ... }
bool get_combo_must_tap(uint16_t combo_index, combo_t *combo) { ... }
```

### Layer-Aware Combos

```c
bool combo_should_trigger(uint16_t combo_index, combo_t *combo,
                          uint16_t keycode, keyrecord_t *record) {
    if (layer_state_is(_GAMING)) return false;
    return true;
}
```

Runtime keycodes: `CM_ON`, `CM_OFF`, `CM_TOGG`

### Gotchas

- Every key array **must** end with `COMBO_END`.
- Mod-Tap and Layer-Tap keys work but require the full keycode (e.g., `LSFT_T(KC_A)`), not just the base key.
- Accidental misfires are common with low `COMBO_TERM`. Tune carefully.

---

## 7. Key Overrides

Intercept modifier+key combinations and replace them with different outputs.

### Enable

```makefile
# rules.mk
KEY_OVERRIDE_ENABLE = yes
```

### Basic Usage

```c
// Shift+Backspace = Delete
const key_override_t delete_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_BSPC, KC_DEL);

const key_override_t *key_overrides[] = {
    &delete_key_override,
    NULL
};
```

### Advanced Initializer

```c
const key_override_t next_track_override =
    ko_make_with_layers_negmods_and_options(
        MOD_MASK_CTRL,           // Trigger modifiers
        KC_MPLY,                 // Trigger key
        KC_MNXT,                 // Replacement key
        ~0,                      // All layers
        MOD_MASK_SA,             // Negative mods (don't activate with shift/alt)
        ko_option_no_reregister_trigger);
```

### config.h Defines

```c
#define KEY_OVERRIDE_REPEAT_DELAY 500
```

Runtime keycodes: `KO_TOGG`, `KO_ON`, `KO_OFF`

### Gotchas

- A key override only activates if the trigger key is the *last* non-modifier key pressed down.
- Programs detecting lone modifier taps (Windows Start menu on GUI release) may trigger falsely with `suppressed_mods`. Use `DUMMY_MOD_NEUTRALIZER_KEYCODE` to mitigate.

---

## 8. Caps Word

A modern Caps Lock replacement that auto-capitalizes letters, converts `-` to `_`, then disables on word boundary (space, enter, etc.). Ideal for `SCREAMING_SNAKE_CASE`.

### Enable

```makefile
# rules.mk
CAPS_WORD_ENABLE = yes
```

Keycode: `CW_TOGG`

### Configuration

```c
#define CAPS_WORD_IDLE_TIMEOUT 5000          // Auto-disable after idle (default 5000ms)
#define BOTH_SHIFTS_TURNS_ON_CAPS_WORD       // Press both shifts simultaneously
#define DOUBLE_TAP_SHIFT_TURNS_ON_CAPS_WORD  // Double-tap left shift
#define CAPS_WORD_INVERT_ON_SHIFT            // Shift produces lowercase during Caps Word
```

### Customization

```c
// Control which keys continue Caps Word
bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        case KC_A ... KC_Z:
        case KC_MINS:
            add_weak_mods(MOD_BIT(KC_LSFT));
            return true;
        case KC_1 ... KC_0:
        case KC_BSPC:
        case KC_DEL:
        case KC_UNDS:
            return true;
        default:
            return false;  // Deactivates Caps Word
    }
}

// LED indicator
void caps_word_set_user(bool active) {
    if (active) { ML_LED_1(true); }
    else        { ML_LED_1(false); }
}
```

API: `caps_word_on()`, `caps_word_off()`, `caps_word_toggle()`, `is_caps_word_on()`

### Gotchas

- Conflicts with Command feature (both use Left Shift + Right Shift). Disable Command or redefine its activation.
- Works on keycodes, not characters. Non-US keyboard layouts may behave unexpectedly.
- Works even when Caps Lock is remapped to Ctrl at OS level (relevant for emacs users).

---

## 9. Repeat Key

Repeats the last pressed key. Also supports an alternate repeat key for "opposite" actions.

### Enable

```makefile
# rules.mk
REPEAT_KEY_ENABLE = yes
```

Keycodes: `QK_REP` (repeat), `QK_AREP` (alternate repeat)

### Custom Alternate Definitions

```c
uint16_t get_alt_repeat_key_keycode_user(uint16_t keycode, uint8_t mods) {
    if ((mods & MOD_MASK_CTRL)) {
        switch (keycode) {
            case KC_Y: return C(KC_Z);  // Ctrl+Y alt → Ctrl+Z (undo/redo)
            case KC_Z: return C(KC_Y);
        }
    }
    switch (keycode) {
        case KC_LEFT: return KC_RGHT;
        case KC_RGHT: return KC_LEFT;
        case KC_UP:   return KC_DOWN;
        case KC_DOWN: return KC_UP;
    }
    return KC_TRNS;
}
```

### Exclude Keys from Memory

```c
bool remember_last_key_user(uint16_t keycode, keyrecord_t* record,
                            uint8_t* remembered_mods) {
    switch (keycode) {
        case KC_BSPC:
            return false;  // Don't remember backspace
    }
    return true;
}
```

API: `get_last_keycode()`, `get_last_mods()`, `get_repeat_key_count()` (positive = repeat, negative = alt repeat)

---

## 10. Leader Key

Vim-style sequential key sequences. Press a leader key, then a sequence of up to 5 keys to trigger actions.

### Enable

```makefile
# rules.mk
LEADER_ENABLE = yes
```

Keycode: `QK_LEAD`

### Usage

```c
void leader_end_user(void) {
    if (leader_sequence_one_key(KC_F)) {
        SEND_STRING("QMK is awesome.");
    } else if (leader_sequence_two_keys(KC_D, KC_D)) {
        SEND_STRING(SS_LCTL("a") SS_LCTL("c"));  // Select all + copy
    } else if (leader_sequence_two_keys(KC_A, KC_S)) {
        tap_code16(LGUI(KC_S));  // Cmd+S / Ctrl+S
    }
}
```

### Configuration

```c
#define LEADER_TIMEOUT 350              // Sequence timeout (default 300ms)
#define LEADER_PER_KEY_TIMING           // Reset timer on each keypress
#define LEADER_NO_TIMEOUT               // Wait indefinitely after leader key
```

### Gotchas

- Maximum sequence length is 5 keys.
- With `LEADER_NO_TIMEOUT`, sequences require a final non-matching key or timeout to complete.
- When the leader key is far from sequence keys, use `LEADER_NO_TIMEOUT` to avoid timeouts during hand repositioning.

---

## 11. Tap Dance

A single key performs different actions based on tap count and whether it is held.

### Enable

```makefile
# rules.mk
TAP_DANCE_ENABLE = yes
```

### Simple Example

```c
enum { TD_ESC_CAPS };
tap_dance_action_t tap_dance_actions[] = {
    [TD_ESC_CAPS] = ACTION_TAP_DANCE_DOUBLE(KC_ESC, KC_CAPS),
};
// In keymap: TD(TD_ESC_CAPS)
```

### Built-in Actions

| Action | Behavior |
|--------|----------|
| `ACTION_TAP_DANCE_DOUBLE(kc1, kc2)` | Tap = kc1, double tap = kc2 |
| `ACTION_TAP_DANCE_LAYER_MOVE(kc, layer)` | Tap = keycode, double tap = switch layer |
| `ACTION_TAP_DANCE_LAYER_TOGGLE(kc, layer)` | Tap = keycode, double tap = toggle layer |
| `ACTION_TAP_DANCE_FN_ADVANCED(on_each, on_finish, on_reset)` | Full custom control |

### Advanced State Detection

```c
typedef enum {
    TD_NONE, TD_UNKNOWN,
    TD_SINGLE_TAP, TD_SINGLE_HOLD,
    TD_DOUBLE_TAP, TD_DOUBLE_HOLD,
    TD_DOUBLE_SINGLE_TAP,
    TD_TRIPLE_TAP, TD_TRIPLE_HOLD
} td_state_t;

td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
        else return TD_SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->interrupted) return TD_DOUBLE_SINGLE_TAP;
        else if (state->pressed) return TD_DOUBLE_HOLD;
        else return TD_DOUBLE_TAP;
    }
    if (state->count == 3) {
        if (state->interrupted || !state->pressed) return TD_TRIPLE_TAP;
        else return TD_TRIPLE_HOLD;
    }
    return TD_UNKNOWN;
}
```

### Best Practices

- Place tap dances on infrequently-typed keys (semicolon, slash, quote, etc.).
- Avoid tap dance on keys commonly double-tapped in normal text.
- Give tap dance keys a longer per-key tapping term (+75ms).
- The existing Oryx layout already has 3 tap dances: tilde/grave (DANCE_0), question/slash (DANCE_1), quote/double-quote (DANCE_2).

---

## 12. One Shot Keys

One-shot modifiers (`OSM`) and layers (`OSL`) stay active for exactly one keypress then release. Tapping multiple times locks the modifier.

### Keycodes

- `OSM(MOD_LCTL)` — one-shot left Ctrl
- `OSL(layer)` — one-shot layer
- Pre-built: `OS_LCTL`, `OS_LSFT`, `OS_LALT`, `OS_LGUI`, `OS_MEH`, `OS_HYPR`
- `OS_TOGG` / `OS_ON` / `OS_OFF` — control one-shot state

### Configuration

```c
#define ONESHOT_TAP_TOGGLE 5   // Taps to lock (default 5)
#define ONESHOT_TIMEOUT 5000   // Auto-release timeout in ms
```

### Callbacks

```c
void oneshot_mods_changed_user(uint8_t mods) { /* LED indicator logic */ }
void oneshot_locked_mods_changed_user(uint8_t mods) { /* Locked indicator */ }
void oneshot_layer_changed_user(uint8_t layer) { /* Layer indicator */ }
```

---

## 13. Macros and Send String

### Custom Keycodes Pattern

```c
enum custom_keycodes {
    MY_MACRO = SAFE_RANGE,
    MY_EMAIL,
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MY_MACRO:
            if (record->event.pressed) {
                SEND_STRING("Hello, World!");
            }
            return false;
        case MY_EMAIL:
            if (record->event.pressed) {
                SEND_STRING("user@example.com");
            }
            return false;
    }
    return true;
}
```

### Send String Injection Macros

| Macro | Purpose |
|-------|---------|
| `SS_TAP(X_keyname)` | Tap a non-character key |
| `SS_DOWN(X_keyname)` | Press (without release) |
| `SS_UP(X_keyname)` | Release |
| `SS_DELAY(ms)` | Pause |
| `SS_LCTL("text")` | Send text with Ctrl held |
| `SS_LSFT("text")` | Send text with Shift held |
| `SS_LALT("text")` | Send text with Alt held |
| `SS_LGUI("text")` | Send text with GUI held |

### Dynamic Macros (Runtime Recording)

```makefile
# rules.mk
DYNAMIC_MACRO_ENABLE = yes
```

Keycodes: `DM_REC1`, `DM_REC2`, `DM_PLY1`, `DM_PLY2`, `DM_RSTP`

### Gotchas

- `SEND_STRING` assumes US ANSI layout.
- Never store passwords or sensitive data — firmware is extractable.
- Use `SS_DELAY()` for timing-sensitive sequences.
- Backslashes require escaping as `\\`.

---

## 14. Autocorrect

Automatically corrects typing errors using a trie data structure.

### Enable

```makefile
# rules.mk
AUTOCORRECT_ENABLE = yes
```

### Dictionary Format

Create `autocorrect_dictionary.txt`:

```
:thier        -> their
fitler        -> filter
lenght        -> length
```

Use `:` prefix for word-boundary matching to avoid false triggers on substrings.

Generate the trie:

```bash
qmk generate-autocorrect-data autocorrect_dictionary.txt
```

### Keycodes

`AC_ON`, `AC_OFF`, `AC_TOGG`

### Customization

```c
// Handle Mod-Tap keycodes
bool process_autocorrect_user(uint16_t *keycode, keyrecord_t *record,
                               uint8_t *typo_buffer_size, uint8_t *mods) {
    switch (*keycode) {
        case QK_MOD_TAP ... QK_MOD_TAP_MAX:
            *keycode &= 0xFF;  // Mask to base keycode
            return true;
    }
    return true;
}
```

### Gotchas

- Only matches 8-bit keycodes. Mod-Tap keys need manual handling (mask with `& 0xFF`).
- Modifier keys (Ctrl, Alt, but not Shift) reset the buffer.
- Without word-break markers, corrections can trigger inside correctly-spelled words.

---

## 15. Mouse Keys

Emulate mouse movement, buttons, and scrolling from the keyboard.

### Enable

```makefile
# rules.mk
MOUSEKEY_ENABLE = yes
```

Already enabled by default on the Moonlander.

### Keycodes

| Category | Keycodes |
|----------|----------|
| Movement | `MS_UP`, `MS_DOWN`, `MS_LEFT`, `MS_RGHT` |
| Buttons | `MS_BTN1` through `MS_BTN8` |
| Scroll | `MS_WHLU`, `MS_WHLD`, `MS_WHLL`, `MS_WHLR` |
| Acceleration | `MS_ACL0`, `MS_ACL1`, `MS_ACL2` |

### Acceleration Modes (mutually exclusive)

1. **Accelerated (default)** — cursor accelerates while held
2. **Kinetic** (`#define MK_KINETIC_SPEED`) — smooth quadratic acceleration
3. **Constant 3-speed** (`#define MK_3_SPEED`) — fixed speeds via acceleration keys
4. **Combined** (`#define MK_COMBINED`) — accelerated + momentary constant-speed overrides
5. **Inertia** (`#define MOUSEKEY_INERTIA`) — quadratic acceleration, glides on release

### Tuning (Accelerated Mode)

```c
#define MOUSEKEY_DELAY          10
#define MOUSEKEY_INTERVAL       20
#define MOUSEKEY_MOVE_DELTA     8
#define MOUSEKEY_MAX_SPEED      10
#define MOUSEKEY_TIME_TO_MAX    30
#define MOUSEKEY_WHEEL_DELAY    10
#define MOUSEKEY_WHEEL_INTERVAL 80
#define MOUSEKEY_WHEEL_DELTA    1
#define MOUSEKEY_WHEEL_MAX_SPEED 8
#define MOUSEKEY_WHEEL_TIME_TO_MAX 40
```

---

## 16. OS Detection

Heuristically detects the host OS based on USB setup behavior.

### Enable

```makefile
# rules.mk
OS_DETECTION_ENABLE = yes
```

### Usage

```c
bool process_detected_host_os_user(os_variant_t detected_os) {
    switch (detected_os) {
        case OS_MACOS:
            // Swap GUI and Ctrl for macOS conventions
            break;
        case OS_WINDOWS:
        case OS_LINUX:
            // Standard modifier layout
            break;
    }
    return true;
}
```

Detected values: `OS_UNSURE`, `OS_LINUX`, `OS_WINDOWS`, `OS_MACOS`, `OS_IOS`

### Gotchas

- Not available during `keyboard_post_init_user()` — detection takes time after boot. Use the callback.
- Heuristic-based, not 100% reliable. Always provide a manual fallback (e.g., a keycode to toggle OS mode).
- Some KVM switches prevent re-detection. Enable `OS_DETECTION_KEYBOARD_RESET`.

---

## 17. Unicode

Type Unicode characters via OS-specific input sequences.

### Enable (choose one tier)

```makefile
UNICODE_ENABLE = yes          # Basic: UC(codepoint) up to U+7FFF
UNICODEMAP_ENABLE = yes       # Map: UM(index) for any codepoint via lookup table
UCIS_ENABLE = yes             # Interactive input by name
```

### Unicode Map Example

```c
enum unicode_names { GRIN, TJOY, SMILE };
const uint32_t PROGMEM unicode_map[] = {
    [GRIN]  = 0x1F600,
    [TJOY]  = 0x1F602,
    [SMILE] = 0x1F60A,
};
// Use UM(GRIN) in keymap; UP(GRIN, TJOY) for shift-aware pairs
```

### Configuration

```c
#define UNICODE_SELECTED_MODES UNICODE_MODE_MACOS, UNICODE_MODE_WINCOMPOSE
```

Mode cycling keycodes: `UC_NEXT`, `UC_PREV`, `UC_MAC`, `UC_LINX`, `UC_WIN`, `UC_WINC`

### Gotchas

- Host OS must be configured separately (macOS: enable "Unicode Hex Input" keyboard; Windows: install WinCompose; Linux: uses IBus).
- Basic `UNICODE_ENABLE` only supports codepoints up to U+7FFF. Use `UNICODEMAP_ENABLE` for emoji.
- Only one of the three tiers can be enabled at a time.
- Supports `UNICODE_MODE_EMACS` for direct emacs unicode input.

---

## 18. Swap Hands

Mirrors the keyboard layout for one-handed typing.

### Enable

```makefile
# rules.mk
SWAP_HANDS_ENABLE = yes
```

Already enabled by default on the Moonlander.

### Keycodes

| Keycode | Description |
|---------|-------------|
| `SH_T(kc)` | Tap: send kc; Hold: swap hands |
| `SH_MON` | Swap while held (momentary) |
| `SH_TOGG` | Toggle swap on/off |
| `SH_OS` | One-shot: swap for one keypress |

The `hand_swap_config` matrix is provided by the Moonlander's keyboard definition — you don't need to define it yourself.

---

## 19. Emacs-Specific Patterns

### The Core Problem

"Emacs pinky" — repetitive strain from constant Ctrl usage with the pinky. The Moonlander's split layout and thumb clusters solve this ergonomically.

### Pattern 1: Ctrl on Thumb Cluster (Highest Impact)

Place Ctrl under the thumb. This single change eliminates emacs pinky entirely.

```c
// Example thumb cluster assignment
KC_SPACE,  KC_BSPC,  KC_LCTL,    // Left thumb: Space, Backspace, Ctrl
KC_HYPR,   KC_TAB,   KC_ENTER    // Right thumb: Hyper, Tab, Enter
```

### Pattern 2: Home Row Mods with Emacs

With Ctrl on home row (e.g., `LCTL_T(KC_F)`), common emacs chords become single-hand gestures:
- `C-f` (forward char) = hold F, release (since F itself is Ctrl)
- Solution: use `get_chordal_hold()` exceptions or put Ctrl on the opposite hand's home row instead.

### Pattern 3: Hyper and Super Keys

Bind unused keys to Hyper (`Ctrl+Shift+Alt+GUI`) and Super (`Ctrl+Shift+Alt`) for emacs bindings that never conflict with other applications:

```c
// In keymap
KC_MEH,   // Ctrl+Shift+Alt (already on the existing layout)
KC_HYPR,  // Ctrl+Shift+Alt+GUI (already on the existing layout)
```

These are perfect for emacs keybindings that need to be globally unique.

### Pattern 4: Dedicated Emacs Chord Keys

Assign common emacs chords to single keys or combos:

```c
// Macros for common emacs operations
case EMC_SAVE:    // C-x C-s
    if (record->event.pressed) {
        SEND_STRING(SS_LCTL("x") SS_LCTL("s"));
    }
    return false;
case EMC_FIND:    // C-x C-f
    if (record->event.pressed) {
        SEND_STRING(SS_LCTL("x") SS_LCTL("f"));
    }
    return false;
case EMC_EVAL:    // C-x C-e
    if (record->event.pressed) {
        SEND_STRING(SS_LCTL("x") SS_LCTL("e"));
    }
    return false;
```

### Pattern 5: Leader Key for Emacs Prefix Sequences

QMK's Leader Key mirrors emacs's prefix key concept:

```c
void leader_end_user(void) {
    // Leader + x + s = C-x C-s (save)
    if (leader_sequence_two_keys(KC_X, KC_S)) {
        SEND_STRING(SS_LCTL("x") SS_LCTL("s"));
    }
    // Leader + x + f = C-x C-f (find file)
    else if (leader_sequence_two_keys(KC_X, KC_F)) {
        SEND_STRING(SS_LCTL("x") SS_LCTL("f"));
    }
    // Leader + b = C-x b (switch buffer)
    else if (leader_sequence_one_key(KC_B)) {
        SEND_STRING(SS_LCTL("x") "b");
    }
}
```

### Pattern 6: Caps Word Compatibility

Caps Word works even when Caps Lock is remapped to Ctrl at the OS level. This is significant for emacs users who commonly remap Caps Lock to Ctrl — `BOTH_SHIFTS_TURNS_ON_CAPS_WORD` gives a clean activation path that doesn't conflict.

### Pattern 7: OS Detection for Modifier Swapping

Emacs on macOS often uses different modifier conventions. Use OS Detection to auto-swap:

```c
bool process_detected_host_os_user(os_variant_t detected_os) {
    switch (detected_os) {
        case OS_MACOS:
            // macOS: GUI for app shortcuts, Ctrl for emacs
            break;
        case OS_LINUX:
            // Linux: Ctrl for both app and emacs shortcuts
            break;
    }
    return true;
}
```

---

## 20. Terminal Multiplexer Patterns

### Zellij

Zellij uses a modal design with `Ctrl+<key>` prefixes:
- `Ctrl+P` — Pane mode
- `Ctrl+T` — Tab mode
- `Ctrl+N` — Resize mode
- `Ctrl+S` — Scroll mode
- `Ctrl+O` — Session mode
- `Ctrl+H` — Move mode

This maps naturally to QMK layers or dedicated keys:

```c
// Option A: Dedicated zellij layer with single-key mode entry
// On a zellij layer, each key sends the Ctrl+key combo
[_ZELLIJ] = LAYOUT_moonlander(
    _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______,
    _______, _______, C(KC_S), _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, C(KC_P), _______,
                      _______, C(KC_T), _______,
    // ... right half
),

// Option B: Combos for frequently-used zellij modes
const uint16_t PROGMEM zellij_pane[] = {KC_Z, KC_P, COMBO_END};
const uint16_t PROGMEM zellij_tab[]  = {KC_Z, KC_T, COMBO_END};
combo_t key_combos[] = {
    COMBO(zellij_pane, C(KC_P)),
    COMBO(zellij_tab,  C(KC_T)),
};
```

### tmux

tmux uses `Ctrl+B` as prefix (configurable). With Ctrl on the thumb cluster, the prefix becomes trivial. Some users dedicate a single key:

```c
// Single key for tmux prefix
case TMUX_PREFIX:
    if (record->event.pressed) {
        SEND_STRING(SS_LCTL("b"));
    }
    return false;

// Or a combo
const uint16_t PROGMEM tmux_prefix[] = {KC_T, KC_B, COMBO_END};
combo_t key_combos[] = {
    COMBO(tmux_prefix, C(KC_B)),
};
```

### General Advice for Terminal Multiplexers

- Place frequently-used prefix keys on easily reachable positions. On a Moonlander, a thumb key or combo is ideal.
- If using zellij's modal design, consider a dedicated QMK layer that sends the `Ctrl+<key>` sequences with single keypresses.
- If using both emacs and a terminal multiplexer, be aware of Ctrl key conflicts. Zellij's `Ctrl+S` conflicts with emacs's incremental search. Consider rebinding one or the other, or using a QMK key override to send a different binding in certain contexts.

---

## 21. Oryx Feature Compatibility Matrix

| QMK Feature | Safe with Oryx? | Notes |
|---|---|---|
| Tap Dance | Yes | Oryx supports natively; code additions work alongside |
| Combos | Yes | Not in Oryx UI; add via code |
| Leader Key | Yes | Not in Oryx UI; code-only |
| Key Overrides | Yes | Not in Oryx UI; enable via `rules.mk` |
| Caps Word | Yes | Not in Oryx UI; enable via `rules.mk` |
| Repeat Key | Yes | Not in Oryx UI; code-only |
| Auto Shift | Yes | Oryx supports natively (already enabled in this layout) |
| Dynamic Macros | Yes | Oryx supports natively |
| Send String / Macros | Yes | Used in `process_record_user()` |
| Tri Layer | Yes | Layer management is compatible |
| One Shot Keys | Yes | Standard keycodes |
| Magic Keycodes | Yes | Standard keycodes |
| Autocorrect | Yes | Code-only, no Oryx conflict |
| Mouse Keys | Yes | Already enabled on Moonlander |
| Swap Hands | Yes | Already enabled on Moonlander |
| OS Detection | Yes | Code-only |
| Unicode | Yes | Code-only, requires host OS configuration |
| Chordal Hold | Yes | Code-only, config.h + keymap.c |
| Flow Tap | Yes | Code-only, config.h |
| Achordion | Yes | Commonly used with this workflow |
| Programmable Button | Yes | Independent HID page |
| Deferred Execution | Yes | Already enabled on Moonlander |
| Secure | Caution | May block Keymapp pairing when locked |
| Raw HID (custom) | **CONFLICT** | Oryx module owns `raw_hid_receive()` |
| RGB overrides | Caution | Must check `rawhid_state.rgb_control` before overriding |

---

## 22. Resource Index

### Official Documentation

| Resource | URL | Use For |
|----------|-----|---------|
| QMK Docs Home | https://docs.qmk.fm/ | Complete reference for all features |
| Keycodes Reference | https://docs.qmk.fm/keycodes | Full keycode listing |
| Tap-Hold Config | https://docs.qmk.fm/tap_hold | TAPPING_TERM, PERMISSIVE_HOLD, CHORDAL_HOLD, FLOW_TAP |
| Mod-Tap | https://docs.qmk.fm/mod_tap | MT() keycodes, limitations, workarounds |
| Layers | https://docs.qmk.fm/feature_layers | MO, LT, TG, TO, TT, DF, layer functions |
| Key Overrides | https://docs.qmk.fm/features/key_overrides | ko_make_basic, advanced overrides |
| Combos | https://docs.qmk.fm/features/combo | Chording, COMBO_TERM tuning |
| Caps Word | https://docs.qmk.fm/features/caps_word | SCREAMING_SNAKE_CASE, activation methods |
| Repeat Key | https://docs.qmk.fm/features/repeat_key | QK_REP, QK_AREP, alternate definitions |
| Leader Key | https://docs.qmk.fm/features/leader_key | Sequential key sequences |
| Tap Dance | https://docs.qmk.fm/tap_dance | Multi-tap actions |
| One Shot Keys | https://docs.qmk.fm/one_shot_keys | Sticky modifiers and layers |
| Macros | https://docs.qmk.fm/feature_macros | SEND_STRING, SS_* macros |
| Autocorrect | https://docs.qmk.fm/features/autocorrect | Typo correction trie |
| Mouse Keys | https://docs.qmk.fm/features/mouse_keys | Acceleration modes, tuning |
| OS Detection | https://docs.qmk.fm/features/os_detection | Host OS detection |
| Unicode | https://docs.qmk.fm/features/unicode | Input modes, unicode map |
| Swap Hands | https://docs.qmk.fm/features/swap_hands | One-handed typing |
| Tri Layer | https://docs.qmk.fm/features/tri_layer | Lower+Upper=Adjust |
| Layer Lock | https://docs.qmk.fm/features/layer_lock | QK_LLCK |
| Dynamic Macros | https://docs.qmk.fm/feature_dynamic_macros | Runtime macro recording |
| Custom Functions | https://docs.qmk.fm/custom_quantum_functions | process_record_user, callbacks |
| Config Options | https://docs.qmk.fm/config_options | All config.h and rules.mk options |

### Community Guides (High Authority)

| Resource | URL | Use For |
|----------|-----|---------|
| Home Row Mods Guide | https://precondition.github.io/home-row-mods | Definitive HRM reference — modifier orderings, tuning, per-key config |
| Pascal Getreuer's Posts | https://getreuer.info/posts/keyboards/ | Best-in-class feature guides (Achordion, symbol layers, triggers) |
| Achordion | https://getreuer.info/posts/keyboards/achordion/index.html | Bilateral combinations (now superseded by Chordal Hold for new builds) |
| Symbol Layer Design | https://getreuer.info/posts/keyboards/symbol-layer/index.html | Principles for programming symbol placement |
| Keyboard FAQs | https://getreuer.info/posts/keyboards/faqs/index.html | Common questions answered authoritatively |
| Tap Flow | https://getreuer.info/posts/keyboards/tap-flow/index.html | Flow Tap implementation and tuning |
| Macro Triggers | https://getreuer.info/posts/keyboards/triggers/index.html | Advanced macro patterns, deferred execution |
| Callum-Style Mods | https://github.com/callum-oakley/qmk_firmware/tree/master/users/callum | Timer-free one-shot mod approach |
| Bilateral Combinations | https://sunaku.github.io/home-row-mods.html | Sunaku's advanced HRM technique |
| Callum Layers Writeup | https://blog.ffff.lt/posts/callum-layers/ | Detailed explanation of the Callum approach |

### ZSA-Specific

| Resource | URL | Use For |
|----------|-----|---------|
| ZSA Blog | https://blog.zsa.io/ | Moonlander features, Oryx updates |
| Oryx Configurator | https://configure.zsa.io/ | Visual layout editing |
| ZSA QMK Fork | https://github.com/zsa/qmk_firmware | Firmware source (use firmware25 branch) |
| ZSA QMK Modules | https://github.com/zsa/qmk_modules | Oryx module, keycolors module |
| oryx-with-custom-qmk | https://github.com/poulainpi/oryx-with-custom-qmk | This workflow's upstream repo |
| Oryx+QMK Blog Post | https://blog.zsa.io/oryx-custom-qmk-features/ | Tutorial for this workflow |
| Keymapp | https://www.zsa.io/flash | Firmware flashing tool |
| ZSA Getting Started | https://www.zsa.io/101 | New user orientation |

### Layout Databases and Inspiration

| Resource | URL | Use For |
|----------|-----|---------|
| KeymapDB | https://keymapdb.com/ | Community keymap database — search by keyboard, feature, layout |
| QMK Configurator | https://config.qmk.fm/ | Online keymap builder (not ZSA-specific) |

### Emacs + Keyboard Resources

| Resource | URL | Use For |
|----------|-----|---------|
| Avoid Emacs Pinky | http://xahlee.info/emacs/emacs/emacs_pinky.html | Ergonomic strategies for emacs |
| Mugur (Emacs QMK) | https://github.com/mihaiolteanu/mugur | Generate QMK code from Emacs Lisp |
| Ergodox Emacs Keymap | https://github.com/kbtdr/ergodox-emacs | Reference emacs layout for split keyboards |

### Feature Decision Quick Reference

When an agent is helping design a keyboard layout and needs to decide which QMK feature to recommend:

| User Need | Recommended Feature | Why |
|-----------|---------------------|-----|
| "I want modifiers on the home row" | Home Row Mods + Chordal Hold + Flow Tap | Modern best practice, built-in |
| "Shift+Backspace should be Delete" | Key Overrides | Direct modifier remapping |
| "I need to type SCREAMING_SNAKE_CASE" | Caps Word | Auto-capitalizes, auto-disables |
| "I want to repeat the last key" | Repeat Key | QK_REP, with directional alternates |
| "I want Vim/emacs-style key sequences" | Leader Key | Sequential multi-key triggers |
| "I want one key to do different things on tap count" | Tap Dance | But avoid on frequently-typed keys |
| "I want to press two keys for a shortcut" | Combos | Simultaneous key chording |
| "I want sticky modifiers" | One Shot Keys | Tap mod, then tap target |
| "I want auto-correct while typing" | Autocorrect | Trie-based typo correction |
| "I want to type emoji/special chars" | Unicode Map | Requires host OS configuration |
| "I need different behavior per OS" | OS Detection | Auto-detect macOS/Windows/Linux |
| "I want mouse control from keyboard" | Mouse Keys | Already enabled on Moonlander |
| "I want a numpad I can lock on" | Layer Lock (QK_LLCK) | Lock momentary layers |
| "Lower+Upper should activate Adjust" | Tri Layer | Automatic third layer activation |
| "I want to type one-handed temporarily" | Swap Hands | Already enabled on Moonlander |
| "I want common emacs chords on one key" | Macros (SEND_STRING) | SS_LCTL sequences |
