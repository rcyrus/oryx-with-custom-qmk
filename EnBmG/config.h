#define USB_SUSPEND_WAKEUP_DELAY 0
#define NO_AUTO_SHIFT_ALPHA
#define SERIAL_NUMBER "EnBmG/qmQ003"
#define LAYER_STATE_8BIT

#define RGB_MATRIX_STARTUP_SPD 60


// ═══════════════════════════════════════════════════════════════════
// Custom QMK Configuration — append only, do not edit above
// ═══════════════════════════════════════════════════════════════════

// Tap-Hold System
#define TAPPING_TERM 200
#define QUICK_TAP_TERM 0
#define TAPPING_TERM_PER_KEY
#define CHORDAL_HOLD
#define FLOW_TAP_TERM 150

// Combos
#define COMBO_TERM 40
#define COMBO_MUST_HOLD_MODS
#define EXTRA_SHORT_COMBOS

// Caps Word
#define CAPS_WORD_IDLE_TIMEOUT 5000
#define BOTH_SHIFTS_TURNS_ON_CAPS_WORD

// Leader Key
#define LEADER_TIMEOUT 400
#define LEADER_PER_KEY_TIMING
#define LEADER_NO_TIMEOUT

// Tri Layer (Nav + Sym = Func)
#define TRI_LAYER_LOWER_LAYER 1
#define TRI_LAYER_UPPER_LAYER 2
#define TRI_LAYER_ADJUST_LAYER 3

// Key Overrides
#define KEY_OVERRIDE_REPEAT_DELAY 500
