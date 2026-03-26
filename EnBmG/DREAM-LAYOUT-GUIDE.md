# Dream Layout Guide — Moonlander Custom QMK Firmware

Rajeev's comprehensive reference for the "Dream Layout" on ZSA Moonlander.
Firmware: Oryx layout ID `EnBmG`, QMK with custom features.

Stack: macOS (Darwin) / Alacritty 0.18.0-dev / Zellij 0.44.0 / Emacs 30.2

---

## Table of Contents

1.  Quick Start
2.  Understanding the Layer System
3.  Home Row Mods Explained
4.  Thumb Cluster Explained
5.  Combos Explained
6.  Feature Reference (CapsWord, Repeat Key, Leader Key, Key Overrides)
7.  Gaming Mode
8.  Integration with Alacritty / Zellij / Emacs
9.  LED Color Guide
10. Tuning Guide
11. Oryx Compatibility

---

## 1. Quick Start

### Flashing the firmware

1. Open https://configure.zsa.io/moonlander/layouts/EnBmG in a browser.
2. Click "Compile" (or use the local QMK build if you have custom_features.c changes).
3. Enter bootloader: hold the leftmost or rightmost inner-top key while plugging
   in USB, or hold `Sys` (`MO(5)`) and press the top-inner-left/right key
   marked `BOOT`.
4. Use Keymapp (ZSA's flashing tool) to flash the compiled .bin file.
5. After flashing, the keyboard reboots. LEDs should show the Tokyo Night
   blue/cyan/magenta gradient — you are on Layer 0 (Base).

### First steps after flashing

- Type normally. The layout is QWERTY. Everything in the letter area works
  as expected for regular typing.
- If a key "sticks" as a modifier instead of typing a letter, you are holding
  it too long. Read section 3 (Home Row Mods) and section 10 (Tuning).
- Test the layer-tap thumb keys: tap Space on the left lower thumb row and
  Enter on the right lower thumb row. Hold them briefly to confirm Nav and
  Sym layers activate (watch the LEDs change to green or amber).

### If something goes wrong

- Gaming mode stuck? Press the `Game` key (`TG(6)`) again.
- Caps stuck on? Press both Shift keys, or tap Space/Enter to deactivate.
- Need to reflash? The bootloader keys are on the System layer (section 2),
  or hold the inner-top-left key while plugging in.

---

## 2. Understanding the Layer System

Seven layers, activated by different mechanisms:

```
Layer  Name     Activation              Color Theme
-----  ------   ----------------------  ----------------
  0    Base     Always on               Tokyo Night (blue)
  1    Nav      Hold Space              Forest Green
  2    Sym      Hold Enter              Amber / Gold
  3    Func     Hold Space AND Enter    Amethyst / Purple
  4    Mouse    Combo Z+X (hold both)   Coral / Pink
  5    System   Hold Sys / MO(5)        Ember / Red
  6    Gaming   Toggle Game / TG(6)     Neon Lime
```

### How layers combine

Transparent keys (marked `___` below) fall through to the layer beneath.
Most of Nav, Sym, Func, Mouse, and System are transparent — only the keys
that differ from Base are defined. This means you still have access to the
number row, brackets, etc. even when on a higher layer.

### Tri Layer

The Func layer (3) activates automatically when BOTH Space and Enter are
held simultaneously. You do not need a dedicated key for it. This is QMK's
Tri Layer feature: Nav (1) + Sym (2) = Func (3).

### Diagram legend

- `Cmd` = macOS Command. QMK calls this modifier `GUI`, so `LGUI` / `RGUI`
  in the code mean left/right Command.
- `A/Cmd` means "tap A, hold Command". The part before the slash is the tap
  behavior; the part after the slash is the hold behavior.
- `Sys` = System layer key. In QMK terms this is `MO(5)`, where `MO` means
  "momentary" and `5` is the layer number.
- `Game` = Gaming toggle key. In QMK terms this is `TG(6)`, where `TG` means
  "toggle" and `6` is the layer number.
- `CW` = CapsWord toggle. `Rep` = Repeat Key. `Meh` = Ctrl+Alt+Shift.
  `Hypr` = Ctrl+Alt+Shift+Cmd.
- `___` = transparent on that layer; the key falls through to the layer below.
- `>>>>> / HELD` = the key you keep pressed to stay on that layer.

### Layer 0: Base (QWERTY + Home Row Mods)

```
,------+------+------+------+------+------+------.     ,------+------+------+------+------+------+------.
|  =   |  1   |  2   |  3   |  4   |  5   | Esc  |     | Del  |  6   |  7   |  8   |  9   |  0   |  -   |
|------+------+------+------+------+------+------|     |------+------+------+------+------+------+------|
| Tab  |  Q   |  W   |  E   |  R   |  T   |  CW  |     |Leader|  Y   |  U   |  I   |  O   |  P   |  \   |
|------+------+------+------+------+------+------|     |------+------+------+------+------+------+------|
| Meh  | A/Cmd| S/Alt|D/Shift|F/Ctrl|  G  | Game |     | Rep  |  H   |J/Ctrl|K/Shift|L/Alt| ;/Cmd|  '   |
|------+------+------+------+------+------+------|     |------+------+------+------+------+------+------|
| LSft |  Z   |  X   |  C   |  V   |  B   |             |  N   |  M   |  ,   |  .   |  /   | RSft |
|------+------+------+------+------+------+             +------+------+------+------+------+------|
|LCtrl | LAlt |  `   | Left |Right |                           |  Up  | Down |  [   |  ]   | RAlt |
`------+------+------+------+------+------.             .------+------+------+------+------+------'
                                    |LCtrl |            |  Sys |
                             ,------+------+------.     ,------+------+------.
                             |Space/| Bspc | LCmd |     | Hypr | Tab  |Enter/|
                             | Nav  |      |      |     |      |      | Sym  |
                             `------+------+------'     `------+------+------'
```

Notes:

- The left thumb pod is now swapped: the upper thumb-pod key is `LCtrl`,
  and the inner thumb-row key is `LCmd`.
- Left/right arrows are back on the left-hand cluster. `[` and `]` now live
  on the right-hand cluster next to Up/Down.
- `A/Cmd` means A on tap, Command on hold. See section 3.
- `Space/Nav` means Space on tap, activates Nav layer on hold.
- `Sys` is the System layer key (`MO(5)` in QMK).
- `Game` is the Gaming toggle (`TG(6)` in QMK).

### Layer 1: Nav (Hold Space)

```
,------+------+------+------+------+------+------.     ,------+------+------+------+------+------+------.
| ___  | ___  | ___  | ___  | ___  | ___  | ___  |     | ___  | ___  | ___  | ___  | ___  | ___  | ___  |
|------+------+------+------+------+------+------|     |------+------+------+------+------+------+------|
| ___  | ___  | ___  | ___  | ___  | ___  | ___  |     | ___  | ___  | Home | PgUp | PgDn | End  | ___  |
|------+------+------+------+------+------+------|     |------+------+------+------+------+------+------|
| ___  | Cmd  | Alt  |Shift | Ctrl | ___  | ___  |     | ___  | ___  | Left | Down |  Up  |Right | ___  |
|------+------+------+------+------+------+------|     |------+------+------+------+------+------+------|
| ___  |Cmd+Z |Cmd+X |Cmd+C |Cmd+V | ___  |             | ___  | ___  | ___  | ___  | ___  | ___  |
|------+------+------+------+------+------+             +------+------+------+------+------+------|
| ___  | ___  | ___  | ___  | ___  |                           | ___  | ___  | ___  | ___  | ___  | ___  |
`------+------+------+------+------+------.             .------+------+------+------+------+------'
                                    | ___  |             |      |
                             ,------+------+------.     ,------+------+------.
                             |>>>>> | ___  | ___  |     | ___  | ___  | ___  |
                             |HELD  |      |      |     |      |      |      |
                             `------+------+------'     `------+------+------'
```

Right hand: Vim-style HJKL navigation. UIOP for Home/PgUp/PgDn/End.
Left home row: plain modifiers (no tap function) for combining with arrows.

Usage examples:

- Hold Space + J = Left arrow
- Hold Space + Shift(D) + K = Shift+Down (select text downward)
- Hold Space + Ctrl(F) + L = Ctrl+Up (Emacs: scroll up one line)
- Z/X/C/V on this layer send Cmd+Z/X/C/V (macOS undo/cut/copy/paste)

### Layer 2: Sym (Hold Enter)

```
,------+------+------+------+------+------+------.     ,------+------+------+------+------+------+------.
| ___  |  !   |  @   |  #   |  $   |  %   | ___  |     | ___  |  =   |  7   |  8   |  9   |  +   | ___  |
|------+------+------+------+------+------+------|     |------+------+------+------+------+------+------|
| ___  |  ^   |  &   |  {   |  }   |  |   | ___  |     | ___  |  \   |  4   |  5   |  6   |  0   | ___  |
|------+------+------+------+------+------+------|     |------+------+------+------+------+------+------|
| ___  | Cmd  | Alt  |Shift | Ctrl |  ~   | ___  |     | ___  |  *   |  1   |  2   |  3   |  -   | ___  |
|------+------+------+------+------+------+------|     |------+------+------+------+------+------+------|
| ___  |  `   |  _   |  (   |  )   | ___  |             |  ,   |  .   | ___  | ___  | ___  | ___  |
|------+------+------+------+------+------+             +------+------+------+------+------+------|
| ___  | ___  | ___  |  <   |  >   |                           | ___  | ___  | ___  | ___  | ___  | ___  |
`------+------+------+------+------+------.             .------+------+------+------+------+------'
                                    | ___  |             |      |
                             ,------+------+------.     ,------+------+------.
                             | ___  | ___  | ___  |     | ___  | ___  |>>>>> |
                             |      |      |      |     |      |      |HELD  |
                             `------+------+------'     `------+------+------'
```

Right hand: numpad layout (7-8-9 on top, 1-2-3 on home, 0 on the right).
Left hand: symbols on the upper rows, brackets grouped logically:

- E/R = { } (curly braces)
- C/V = ( ) (parentheses)
- [/] on bottom row = < > (angle brackets)
  Left home row: plain modifiers for combining with numbers/symbols.

### Layer 3: Func (Hold Space AND Enter)

```
,------+------+------+------+------+------+------.     ,------+------+------+------+------+------+------.
| ___  |  F1  |  F2  |  F3  |  F4  |  F5  | ___  |     | ___  |  F6  |  F7  |  F8  |  F9  | F10  | F11  |
|------+------+------+------+------+------+------|     |------+------+------+------+------+------+------|
| ___  | ___  | ___  | ___  | ___  | ___  | ___  |     | ___  | ___  | ___  | Bri+ | ___  | ___  | F12  |
|------+------+------+------+------+------+------|     |------+------+------+------+------+------+------|
| ___  | Cmd  | Alt  |Shift | Ctrl | ___  | ___  |     | ___  | ___  | Prev |Vol-  |Vol+  | Next | ___  |
|------+------+------+------+------+------+------|     |------+------+------+------+------+------+------|
| ___  | ___  | ___  | ___  | ___  | ___  |             | ___  | ___  | ___  | ___  | Play | ___  |
|------+------+------+------+------+------+             +------+------+------+------+------+------|
| ___  | ___  | ___  | ___  | ___  |                           | ___  | ___  | ___  | ___  | ___  | ___  |
`------+------+------+------+------+------.             .------+------+------+------+------+------'
                                    | ___  |             |      |
                             ,------+------+------.     ,------+------+------.
                             |>>>>> | ___  | ___  |     | ___  | ___  |>>>>> |
                             |HELD  |      |      |     |      |      |HELD  |
                             `------+------+------'     `------+------+------'
```

F-keys across the top row. Media controls on the right home row.
Brightness up on I position. Play/Pause on right bottom row (/ position).

Usage: Hold Space with left thumb, hold Enter with right thumb, then tap
the desired F-key or media control. Both thumbs are occupied, so everything
you need is on the finger rows.

### Layer 4: Mouse (Hold Z+X)

```
,------+------+------+------+------+------+------.     ,------+------+------+------+------+------+------.
| ___  | ___  | ___  | ___  | ___  | ___  | ___  |     | ___  | ___  | ___  | ___  | ___  | ___  | ___  |
|------+------+------+------+------+------+------|     |------+------+------+------+------+------+------|
| ___  | ___  | ___  | ___  | ___  | ___  | ___  |     | ___  | ___  |Scr Up|Mse Up|Scr Dn| ___  | ___  |
|------+------+------+------+------+------+------|     |------+------+------+------+------+------+------|
| ___  | Cmd  | Alt  |Shift | Ctrl | ___  | ___  |     | ___  | ___  |Mse L |Mse Dn|Mse R | ___  | ___  |
|------+------+------+------+------+------+------|     |------+------+------+------+------+------+------|
| ___  | ___  | ___  | ___  | ___  | ___  |             | ___  | Btn1 | Btn3 | Btn2 | ___  | ___  |
|------+------+------+------+------+------+             +------+------+------+------+------+------|
| ___  | ___  | ___  | ___  | ___  |                           | ___  |Accel0|Accel1|Accel2| ___  | ___  |
`------+------+------+------+------+------.             .------+------+------+------+------+------'
                                    | ___  |             |      |
                             ,------+------+------.     ,------+------+------.
                             | Btn1 | Btn2 | ___  |     | ___  | ___  | ___  |
                             |      |      |      |     |      |      |      |
                             `------+------+------'     `------+------+------'
```

Hold Z+X simultaneously with the left pinky and ring finger to enter Mouse
mode. Release Z or X to leave. Right hand HJKL = mouse movement.
Scroll wheel on U/O positions. Buttons on bottom row and left thumb cluster.
Acceleration keys on the bottom right: Accel0 (slow), Accel1 (medium),
Accel2 (fast).

### Layer 5: System (`Sys` / hold `MO(5)` — right bottom inner key)

```
,------+------+------+------+------+------+------.     ,------+------+------+------+------+------+------.
|Audio | ___  | ___  | ___  | ___  | ___  | BOOT |     | BOOT | ___  | ___  | ___  | ___  | ___  | ___  |
|------+------+------+------+------+------+------|     |------+------+------+------+------+------+------|
| ___  | ___  | ___  | ___  | ___  | ___  | ___  |     | ___  | ___  | ___  | ___  | ___  | ___  | ___  |
|------+------+------+------+------+------+------|     |------+------+------+------+------+------+------|
| ___  | ___  | ___  | ___  | ___  | ___  | ___  |     | ___  |RGBTog|RGB H+|RGB S+|RGB V+|RGB P+| ___  |
|------+------+------+------+------+------+------|     |------+------+------+------+------+------+------|
| ___  | ___  | ___  | ___  | ___  | ___  |             |RGBMod|RGB H-|RGB S-|RGB V-|RGB P-| ___  |
|------+------+------+------+------+------+             +------+------+------+------+------+------|
| ___  | ___  | ___  |DT Dn |DT Up |                           | HELD | ___  | ___  | ___  | ___  | ___  |
`------+------+------+------+------+------.             .------+------+------+------+------+------'
                                    |DT Prn|             |      |
                             ,------+------+------.     ,------+------+------.
                             | ___  | ___  | ___  |     | ___  | ___  | ___  |
                             |      |      |      |     |      |      |      |
                             `------+------+------'     `------+------+------'
```

**BOOT keys enter bootloader mode. The keyboard will disconnect. Only press
these when you intend to flash firmware.** They are on the inner-top corners
of each half.

RGB controls are on the right home/bottom rows. DT (Dynamic Tapping Term)
controls are on the left bottom row — see section 10.

### Layer 6: Gaming (toggle `Game` / `TG(6)`)

```
,------+------+------+------+------+------+------.     ,------+------+------+------+------+------+------.
| Esc  |  1   |  2   |  3   |  4   |  5   |  F1  |     |  F3  |  6   |  7   |  8   |  9   |  0   |  -   |
|------+------+------+------+------+------+------|     |------+------+------+------+------+------+------|
| Tab  |  Q   |  W   |  E   |  R   |  T   |  F2  |     |  F4  |  Y   |  U   |  I   |  O   |  P   |  \   |
|------+------+------+------+------+------+------|     |------+------+------+------+------+------+------|
| Caps |  A   |  S   |  D   |  F   |  G   | Game |     |  F5  |  H   |  J   |  K   |  L   |  ;   |  '   |
|------+------+------+------+------+------+------|     |------+------+------+------+------+------+------|
| LSft |  Z   |  X   |  C   |  V   |  B   |             |  N   |  M   |  ,   |  .   |  /   | RSft |
|------+------+------+------+------+------+             +------+------+------+------+------+------|
|LCtrl | LAlt | LCmd | Left |Right |                           |  F9  |  Up  | Down |  [   |  ]   | RAlt |
`------+------+------+------+------+------.             .------+------+------+------+------+------'
                                    |  F5  |             |      |
                             ,------+------+------.     ,------+------+------.
                             |Space | Bspc |LCtrl |     |RCtrl | Tab  |Enter |
                             |      |      |      |     |      |      |      |
                             `------+------+------'     `------+------+------'
```

Standard keyboard: no home row mods, no layer-taps, no combos.
WASD works without any timing considerations. F-keys on inner columns.
Toggle back to Base by pressing `Game` (`TG(6)`) again (same position, left
inner row 3).

---

## 3. Home Row Mods Explained

### What are home row mods?

Each home row key has two functions: tap it for the letter, hold it for a
modifier. The layout uses GASC order (from pinky to index):

```
Left hand:                    Right hand:
A = Cmd (QMK GUI)             J = Ctrl
S = Alt (Option)              K = Shift
D = Shift                     L = Alt (Option)
F = Ctrl                      ; = Cmd (QMK GUI)
```

This means your modifiers are always under your fingers. No reaching for
corner keys.

### Why GASC and not GACS or CAGS?

- Ctrl on the index finger (F/J): the strongest, fastest finger gets the
  most-used modifier for Emacs. Ctrl+X, Ctrl+C, Ctrl+S — all with the
  index finger rolling into its neighbor.
- Shift on the middle finger (D/K): capitalizing letters is a high-frequency
  action, and the middle finger is the second-strongest.
- Alt on the ring finger (S/L): Alt is used less often than Ctrl or Shift.
  Note: L uses LALT (not RALT) to avoid triggering AltGr behavior on macOS.
- Command on the pinky (A/;): QMK calls this modifier `GUI`, but on macOS it
  is the Command key. Cmd is used for shortcuts like Cmd+C and Cmd+V, which
  tend to be deliberate, not speed-critical. The pinky gets extra tapping
  term (+40ms) to compensate for its slower movement.

### How to use them

**To type a letter:** Tap the key quickly and release. If you type at
normal speed, Flow Tap ensures the key always registers as a tap.

**To use the modifier:** Press and hold the key, then tap another key.
For example:

- Hold F, tap S = Ctrl+S (save in Emacs)
- Hold D, tap Q = Shift+Q (capital Q)
- Hold A, tap C = Cmd+C (copy on macOS)

**Combining modifiers:** Hold multiple home row keys simultaneously.

- Hold D + F, tap A = Ctrl+Shift+A
- Hold A + D, tap Z = Cmd+Shift+Z (redo on macOS)

### The learning curve

The first week will feel slow. Common issues:

1. **Accidental modifiers when typing fast:** Flow Tap (150ms window) handles
   this. If you type a home row key within 150ms of the previous keypress,
   it always registers as a tap. The hold function only activates after a
   pause.

2. **Same-hand modifier+key fails:** Chordal Hold requires the second key to
   be on the opposite hand by default. Exceptions are configured for common
   same-hand shortcuts:
   - Cmd (A) + Z/X/C/V/S/W/Q/E/R/T = macOS shortcuts
   - Ctrl (F) + Z/X/C/V/A/S/D/Q/W/E/R/T/G/B = Emacs C-key shortcuts
   - Shift (D) + all left-hand letters = capitalize
   - Shift (K) + all right-hand letters = capitalize
   - Ctrl (J) + all right-hand letters = Emacs C-key shortcuts

3. **Pinkies feel too slow:** The pinky keys (A, ;) have +40ms added to
   their tapping term (240ms total). Ring fingers (S, L) get +20ms (220ms).
   This means you can hold them slightly longer before they register as
   modifiers. See section 10 for adjusting these values.

### Practice tips

- Start by using home row mods only for Emacs. Keep using the physical
  modifiers (`LCtrl`, `LAlt`, `LCmd`, etc.) for everything else until
  comfortable.
- Practice Ctrl+X sequences: hold F, tap X, release both. Then the second
  key. This is a common source of frustration early on.
- If you frequently misfire, temporarily increase the tapping term via the
  System layer DT controls (section 10).

---

## 4. Thumb Cluster Explained

The Moonlander thumb area is better thought of as an upper thumb pod plus a
three-key lower thumb row on each side. On the Base layer, the left upper pod
key is assigned and the right upper pod key is unused.

```
LEFT THUMB AREA                          RIGHT THUMB AREA
                ,------.                                ,------.
                |LCtrl |                                | ___  |
                `------'                                `------'
         ,------+------+------.                 ,------+------+------.
         |Space/| Bspc | LCmd |                 | Hypr | Tab  |Enter/|
         | Nav  |      |      |                 |      |      | Sym  |
         `------+------+------'                 `------+------+------'
```

### Left thumb area

| Position      | Tap       | Hold                | Notes                                                                                |
| ------------- | --------- | ------------------- | ------------------------------------------------------------------------------------ |
| Upper pod key | LCtrl     | LCtrl               | Dedicated physical Ctrl. This is the key that used to be Command and is now swapped. |
| Lower outer   | Space     | Nav layer           | Main layer-tap thumb key. Tapping term -20ms (180ms) for fast layer access.          |
| Lower middle  | Backspace | Backspace (no hold) | Plain key. Shift+Bspc = Delete (key override).                                       |
| Lower inner   | LCmd      | LCmd                | Dedicated physical Command key (`LGUI` in QMK naming).                               |

### Right thumb area

| Position      | Tap            | Hold           | Notes                                                                                             |
| ------------- | -------------- | -------------- | ------------------------------------------------------------------------------------------------- |
| Upper pod key | Unused on Base | Unused on Base | Transparent on Base; used on some other layers.                                                   |
| Lower outer   | Hyper          | Hyper          | Ctrl+Alt+Shift+Cmd. Useful for application-level hotkeys that rarely conflict with anything else. |
| Lower middle  | Tab            | Tab (no hold)  | Plain key.                                                                                        |
| Lower inner   | Enter          | Sym layer      | Main layer-tap thumb key. Tapping term -20ms (180ms).                                             |

### Tri Layer interaction

When both layer-tap thumb keys are held, Space (Nav) + Enter (Sym) = Func
layer. This gives you F-keys and media controls without any additional key.
The technique:

1. Press and hold Space with left thumb.
2. Press and hold Enter with right thumb.
3. Tap F1-F12 on the number row, or media keys on the right home row.
4. Release both thumbs to return to Base.

### Why Space and Enter are layer-taps

Space and Enter are the most frequently tapped keys, so they belong on the
strongest thumb positions. Making them double as layer activators means your
fingers never leave the home row to access navigation or symbols.

---

## 5. Combos Explained

Combos are two keys pressed simultaneously that produce a different output.
Four combos are defined:

| Keys  | Output                  | Mnemonic                                                       |
| ----- | ----------------------- | -------------------------------------------------------------- |
| J + K | Escape                  | Right-hand home row, quick press. Vim users will feel at home. |
| D + F | Tab                     | Left-hand home row. Replaces reaching for the Tab key.         |
| K + L | Backspace               | Right-hand home row. Deletes without moving to thumb.          |
| Z + X | Mouse layer (momentary) | Bottom-left. Hold both to stay in Mouse mode.                  |

### Timing

Combo term is 40ms. Both keys must be pressed within 40ms of each other.
At normal typing speed, consecutive keypresses are usually >50ms apart,
so accidental combos are rare.

### How to execute a combo

Press both keys at the same time. Not one-then-the-other — truly
simultaneously. Practice: place two fingers on J and K, press down together.
You should get Escape, not J or K.

### Combos and modifiers

The `COMBO_MUST_HOLD_MODS` option means that if a combo result is a modifier,
you must hold the combo keys to keep the modifier active. This matters for the
Z+X Mouse layer: hold both Z and X to stay in Mouse mode. Releasing either
key exits Mouse mode.

### Combos and Gaming mode

All combos are disabled on the Gaming layer. Pressing J+K in a game types
J and K. This is handled by `combo_should_trigger()` in the firmware.

---

## 6. Feature Reference

### CapsWord

**Activation:** Press both Shift keys simultaneously (left Shift + right
Shift), or tap the CW toggle key (left inner column, row 2).

**Behavior while active:**

- Letters are automatically shifted (typed as capitals).
- The `-` key types `_` instead (useful for SCREAMING_SNAKE_CASE).
- Numbers, Backspace, Delete, and `_` continue CapsWord without deactivating.
- Space, Enter, or any other key deactivates CapsWord.

**Visual indicator:** Status LEDs 1 and 4 (leftmost and rightmost) light up
while CapsWord is active.

**Timeout:** CapsWord deactivates automatically after 5 seconds of inactivity.

**Use case:** Type `MY_CONSTANT_NAME` by activating CapsWord, then typing
`my-constant-name`. The letters auto-capitalize and dashes become underscores.

### Repeat Key

**Location:** Right inner column, row 3 (between the right letter block and
the thumb cluster).

**Behavior:** Pressing the Repeat Key sends the last key you pressed, with
the same modifiers. Press it multiple times for rapid repetition.

**Alt-Repeat (reverse direction):** Hold Shift (or use home row Shift) and
press Repeat to get the "opposite" key:

| Last pressed | Alt-Repeat sends |
| ------------ | ---------------- |
| Left arrow   | Right arrow      |
| Right arrow  | Left arrow       |
| Up arrow     | Down arrow       |
| Down arrow   | Up arrow         |
| Page Up      | Page Down        |
| Page Down    | Page Up          |
| Home         | End              |
| End          | Home             |

**Use case in navigation:** Press Left a few times to move the cursor, then
alt-repeat to go back. Useful in Emacs buffer navigation and Zellij pane
switching.

### Leader Key

**Location:** Right inner column, row 2 (above Repeat Key).

**How it works:** Tap the Leader key, then type a short sequence of keys.
There is no timeout — you can take as long as you need between keys
(`LEADER_NO_TIMEOUT` is enabled). The sequence executes when the final key
is recognized.

**Defined sequences:**

| Sequence     | Sends   | Emacs command             |
| ------------ | ------- | ------------------------- |
| Leader, x, s | C-x C-s | save-buffer               |
| Leader, x, f | C-x C-f | find-file                 |
| Leader, x, b | C-x b   | switch-to-buffer          |
| Leader, x, k | C-x k   | kill-buffer               |
| Leader, x, 0 | C-x 0   | delete-window             |
| Leader, x, 1 | C-x 1   | delete-other-windows      |
| Leader, x, 2 | C-x 2   | split-window-below        |
| Leader, x, 3 | C-x 3   | split-window-right        |
| Leader, g    | C-x g   | magit-status              |
| Leader, c, c | C-c C-c | context-dependent confirm |

**Why use Leader instead of pressing Ctrl directly?** The Leader key converts
multi-chord Emacs sequences into simple letter sequences. Instead of
"hold Ctrl, tap X, release Ctrl, hold Ctrl, tap S, release Ctrl" you do
"tap Leader, tap X, tap S." No modifier timing to worry about.

**Tip:** The Leader key is especially valuable during the home-row-mod
learning period. If you are struggling with Ctrl+X sequences via home row
mods, use Leader sequences instead.

### Key Overrides

One key override is defined:

| Input             | Output | Purpose                                                                       |
| ----------------- | ------ | ----------------------------------------------------------------------------- |
| Shift + Backspace | Delete | Forward-delete without reaching. Works with any Shift (home row or physical). |

---

## 7. Gaming Mode

### Entering Gaming mode

Press `Game` (`TG(6)`) — the left inner column key on row 3 (between the
CapsWord toggle and the left half's inner column). The LEDs change to neon
lime.

### What changes

| Feature          | Base layer                  | Gaming layer                                           |
| ---------------- | --------------------------- | ------------------------------------------------------ |
| Home row mods    | Active (A=Cmd, S=Alt, etc.) | Disabled — all plain letters                           |
| Layer-tap thumbs | Space=Nav, Enter=Sym        | Plain Space, plain Enter                               |
| Combos           | J+K=Esc, D+F=Tab, etc.      | All disabled                                           |
| CapsWord         | Available                   | Caps Lock instead (traditional)                        |
| Meh key          | Ctrl+Alt+Shift              | Caps Lock                                              |
| F-keys           | Via Func layer              | Directly on inner columns (F1-F5 left, F3-F5+F9 right) |

### Why a separate gaming layer?

Home row mods and layer-taps add small timing delays to every keypress. In
games where milliseconds matter, you want every key to fire instantly on
press-down. Gaming mode gives you a standard keyboard with zero timing
overhead.

### Returning to Base

Press `Game` (`TG(6)`) again — it is in the same position (left inner, row 3).
The LEDs return to Tokyo Night colors.

---

## 8. Integration with Alacritty / Zellij / Emacs

This section explains how the keyboard layout interacts with each piece of
your terminal stack. Understanding these interactions prevents confusion when
a keypress does not do what you expect.

### The input pipeline

```
Keypress -> Moonlander (QMK) -> USB HID -> macOS -> Alacritty -> Zellij -> Emacs
```

Each layer can intercept or modify the keypress:

- QMK resolves home row mods, combos, leader sequences, and layers.
- macOS handles Cmd shortcuts (Cmd+C = copy) before they reach the terminal.
- Alacritty interprets terminal escape sequences and key bindings.
- Zellij intercepts its own keybindings (when unlocked).
- Emacs receives whatever makes it through.

### Zellij: LOCKED mode and Alt+Z

Zellij starts in LOCKED mode. In this mode, Zellij passes all keys through
to Emacs. This is critical — without LOCKED mode, Zellij would intercept
Ctrl+P, Ctrl+N, Ctrl+B, and other keys that Emacs needs.

**Alt+Z** toggles Zellij between LOCKED and NORMAL mode. On your keyboard:

- Home row: hold S (Alt), tap Z. This uses the chordal hold exception for
  the left Alt key (S) which is configured for opposite-hand default but
  S is Alt, not Ctrl or Cmd, so it follows the standard chordal hold rule.
  You may find it more reliable to use the physical LAlt key on the bottom
  row + Z.
- Physical key: hold the bottom-row LAlt, tap Z.

**When Zellij is UNLOCKED (NORMAL mode),** common Zellij keys include:

- Alt+N = new pane
- Alt+H/J/K/L = move between panes (mirrors Vim/Nav layer directions)
- Alt+[ / Alt+] = switch tabs
- Ctrl+P, then ... = Zellij command palette

**When Zellij is LOCKED,** all keys go to the application (Emacs). You should
stay in LOCKED mode while editing and only unlock briefly for pane/tab
management.

### Alacritty key handling

Alacritty 0.18.0-dev uses the `keyboard.bindings` configuration. Your
Moonlander sends standard keycodes, so Alacritty treats it like any
keyboard. Key points:

- **Hyper key** (right thumb inner): Alacritty can bind Hyper+<key>
  combinations. Since Hyper = Ctrl+Alt+Shift+Cmd, this creates unique
  bindings that nothing else intercepts. Useful for Alacritty-level actions
  like font size changes or opening new windows.
- **Ctrl sequences**: Alacritty translates Ctrl+<letter> into terminal
  control codes (e.g., Ctrl+C = 0x03). Home row Ctrl (F/J) sends identical
  codes to physical Ctrl.
- **Alt/Meta**: Alacritty can be configured to send Alt as Meta (ESC prefix)
  for Emacs. Ensure `window.option_as_alt` is set appropriately if using
  home row Alt (S/L) for Emacs Meta commands.

### Emacs integration

#### Ctrl sequences via home row mods

The most important integration. In Emacs, you use Ctrl constantly:

| Keyboard action   | QMK sends  | Emacs receives             |
| ----------------- | ---------- | -------------------------- |
| Hold F, tap X     | Ctrl+X     | C-x prefix                 |
| Hold F, tap S     | Ctrl+S     | isearch / save (after C-x) |
| Hold F, tap G     | Ctrl+G     | keyboard-quit              |
| Hold F, tap Space | Ctrl+Space | set-mark                   |

These work because home row Ctrl sends the same USB HID codes as physical
Ctrl. Emacs cannot tell the difference.

#### Meta/Alt sequences via home row mods

| Keyboard action | QMK sends | Emacs receives                 |
| --------------- | --------- | ------------------------------ |
| Hold S, tap X   | Alt+X     | M-x (execute-extended-command) |
| Hold S, tap F   | Alt+F     | M-f (forward-word)             |
| Hold S, tap B   | Alt+B     | M-b (backward-word)            |

Requires Alacritty to send Alt as ESC prefix (Meta).

#### Leader key for C-x sequences

The Leader key is designed specifically for Emacs C-x prefix commands. These
are two-step sequences that are awkward with home row mods because you need
to hold Ctrl, tap X, release, then hold Ctrl again (or not) for the second
key. The Leader key eliminates this:

| Old way (home row)                    | New way (Leader)         | Result                |
| ------------------------------------- | ------------------------ | --------------------- |
| Hold F, tap X, release, hold F, tap S | Tap Leader, tap X, tap S | C-x C-s (save)        |
| Hold F, tap X, release, tap B         | Tap Leader, tap X, tap B | C-x b (switch buffer) |
| Hold F, tap X, release, tap G         | Tap Leader, tap G        | C-x g (magit)         |

#### Vertico, Consult, Corfu

These completion frameworks use standard Emacs keybindings:

- **Vertico**: C-n / C-p to navigate candidates (hold F, tap N/P).
- **Consult**: M-x to invoke, then type. Standard minibuffer interaction.
- **Corfu**: Completion popup uses Tab (combo D+F or physical Tab) to confirm.
  C-n / C-p to cycle candidates.
- **Magit**: Leader → G opens magit-status. Within Magit, all keys are
  standard (s to stage, c c to commit, etc.).

#### Nav layer for Emacs movement

The Nav layer gives you arrow keys on HJKL, plus Home/End/PgUp/PgDn. These
work in Emacs as cursor movement. Combine with left-hand modifiers:

| Action               | Keys (Nav layer)                | Emacs effect                  |
| -------------------- | ------------------------------- | ----------------------------- |
| Move by line         | Hold Space, tap J/K             | C-n / C-p equivalent          |
| Move by char         | Hold Space, tap H/L             | C-b / C-f equivalent          |
| Select text          | Hold Space + D(Shift), tap HJKL | Shift-select                  |
| Top/bottom of buffer | Hold Space, tap I/O (Home/End)  | Beginning/end in some configs |
| Scroll               | Hold Space, tap U/P (PgUp/PgDn) | scroll-down / scroll-up       |

**Note:** Emacs purists may prefer C-n/C-p/C-b/C-f over arrow keys. Both
approaches work. The Nav layer is there when you want it.

---

## 9. LED Color Guide

### Color vocabulary (consistent across all layers)

| Brightness                              | Meaning                                          | Example                         |
| --------------------------------------- | ------------------------------------------------ | ------------------------------- |
| Bright (full saturation, full value)    | Primary function — this is what the layer is for | Arrow keys on Nav               |
| Medium (full saturation, reduced value) | Supporting function — secondary purpose          | Home/End on Nav                 |
| Pastel (reduced saturation, full value) | Modifier — always available, not layer-specific  | Cmd/Alt/Shift/Ctrl on any layer |
| Off / dark (value = 0)                  | Nothing here — key is transparent or unused      | Empty spots                     |
| White (no hue, full value)              | Mode toggle                                      | `Game` / `TG(6)` toggle key     |
| Bright Red (H=0, full saturation)       | DANGER — destructive action                      | Bootloader keys                 |

### Per-layer color themes

**Layer 0 — Base: Tokyo Night**
Blue, cyan, magenta gradient. The default resting state. Each column subtly
shifts hue from left to right, matching the Tokyo Night color palette.

**Layer 1 — Nav: Forest Green (H=85)**

- Bright green: Arrow keys (HJKL)
- Medium green: Home, End, Page Up, Page Down
- Pastel green: Left-hand modifiers (Cmd, Alt, Shift, Ctrl)
- Teal accent (H=106): macOS shortcuts (Cmd+Z/X/C/V)

**Layer 2 — Sym: Amber/Gold (H=32)**

- Bright amber: Number keys (0-9)
- Orange (H=21): Bracket pairs and symbols
- Gold (H=43): Operators (+, -, \*, =)
- Desaturated amber: Lesser symbols and modifiers

**Layer 3 — Func: Amethyst/Purple (H=191)**

- Bright purple: F-keys (F1-F12)
- Violet (H=213): Media controls (Prev, Next)
- Rose (H=234): Volume controls

**Layer 4 — Mouse: Coral/Pink (H=234)**

- Bright coral: Mouse movement (HJKL positions)
- Pink: Mouse buttons (Btn1, Btn2, Btn3)
- Rose: Scroll wheel keys
- Desaturated coral: Left-hand modifiers and thumb buttons

**Layer 5 — System: Ember/Red (H=0)**

- BRIGHT RED: Bootloader keys (top inner corners) — DANGER
- Orange (H=21): RGB controls
- Warm red (desaturated): Audio toggle, DT tuning keys

**Layer 6 — Gaming: Neon Lime (H=64)**

- Bright lime: WASD and main letter keys
- Green accent (H=85): Number row, F-keys
- Pastel lime: Modifier keys (Shift, Ctrl, Alt)
- Dim lime: Right-hand auxiliary keys
- White: `Game` / `TG(6)` toggle key (so you can find it to exit)

### CapsWord indicator

When CapsWord is active, status LEDs 1 and 4 (the small LEDs on the outer
edges of each half) light up regardless of which layer you are on.

---

## 10. Tuning Guide

### Understanding tapping term

The tapping term is the time window (in milliseconds) that determines
whether a keypress is a "tap" or a "hold":

- **Press and release within the tapping term** = tap (sends the letter)
- **Press and hold beyond the tapping term** = hold (activates modifier/layer)

Default: 200ms. Per-key adjustments:

| Key                       | Tapping term    | Why                                                 |
| ------------------------- | --------------- | --------------------------------------------------- |
| A, ; (pinkies)            | 240ms (+40)     | Pinkies are slower — need more time to release      |
| S, L (ring fingers)       | 220ms (+20)     | Ring fingers are slightly slower than middle/index  |
| D, K, F, J (middle/index) | 200ms (default) | Fastest fingers, default timing                     |
| Space (LT Nav)            | 180ms (-20)     | Thumb is fast and deliberate — quicker layer access |
| Enter (LT Sym)            | 180ms (-20)     | Same as Space                                       |

### Flow Tap: the anti-misfire system

Flow Tap (150ms) is the single most important anti-misfire feature. It works
as follows:

- If you press a home row key within 150ms of the previous keypress, it
  ALWAYS registers as a tap, regardless of how long you hold it.
- The hold function only activates if there was a pause of at least 150ms
  before the keypress.

This means: during normal typing flow, home row mods never accidentally
trigger. You have to deliberately pause before a keypress to activate a hold.

**Exception:** Flow Tap is disabled when Cmd, Ctrl, or Left Alt are already
held. This ensures that once you activate a modifier, additional modifier
presses work correctly even at speed.

### Chordal Hold: the opposite-hand rule

Chordal Hold adds another layer of misfire prevention:

- A hold function only activates when the second key is on the **opposite
  hand**. Pressing A (left) then J (right) = hold A triggers. Pressing
  A (left) then S (left) = no hold, both are taps.

**Configured exceptions** (same-hand holds that DO work):

- Cmd (A) + left-hand keys: for Cmd+Z, Cmd+X, Cmd+C, Cmd+V, Cmd+S, Cmd+W,
  Cmd+Q, Cmd+T, etc.
- Ctrl (F) + left-hand keys: for C-x, C-c, C-a, C-s, C-z, C-g, C-b, etc.
- Shift (D) + left-hand keys: for capitalizing left-hand letters.
- Ctrl (J) + right-hand keys: for Emacs chords with right-hand letters.
- Shift (K) + right-hand keys: for capitalizing right-hand letters.

### Dynamic Tapping Term (runtime tuning)

On the System layer (hold `Sys` / `MO(5)` on the right bottom inner key), the left
bottom row has three DT keys:

| Key position                            | Function | What it does                              |
| --------------------------------------- | -------- | ----------------------------------------- |
| Left bottom, col 4 (where [ is on Base) | DT_DOWN  | Decrease tapping term by 5ms              |
| Left bottom, col 5 (where ] is on Base) | DT_UP    | Increase tapping term by 5ms              |
| Left upper thumb-pod position           | DT_PRNT  | Print current tapping term to the console |

**How to use DT_PRNT:** Enable QMK console (hid_listen or qmk console) to
see the current value. Each press of DT_UP or DT_DOWN adjusts by 5ms.
Changes persist until the keyboard is unplugged.

### Diagnosing misfires

**Symptom: Letters appear as modifiers (e.g., pressing A opens Spotlight)**

- Cause: You are holding A too long.
- Fix: Increase tapping term for pinkies (DT_UP while on System layer), or
  practice tapping more crisply.

**Symptom: Modifiers do not activate when intended**

- Cause: You are pressing the second key too quickly (within Flow Tap window).
- Fix: Pause briefly before the hold key, then press the second key.
- Alternative: Decrease FLOW_TAP_TERM in config.h (requires reflashing).

**Symptom: Home row mod works with opposite hand but not same hand**

- Cause: Chordal Hold is blocking same-hand activation.
- Fix: Check if the combination is in the exception list. If not, add it
  to `get_chordal_hold()` in custom_features.c.

**Symptom: Combos fire accidentally during normal typing**

- Cause: COMBO_TERM (40ms) may be too generous for your typing speed.
- Fix: Reduce COMBO_TERM in config.h (requires reflashing). Try 30ms or 25ms.

**Symptom: Space/Enter hold activates layer too easily**

- Cause: Tapping term for thumbs may be too long.
- Fix: These are already set to 180ms. If still too sensitive, you can
  increase them, but this slows layer access.

### Recommended tuning process

1. Use the keyboard for a full day without changing anything.
2. Note which misfires occur and how often.
3. Adjust DT (Dynamic Tapping Term) up or down in 5ms increments.
4. If DT alone is not enough, edit config.h values and reflash:
   - `TAPPING_TERM` (base value, currently 200)
   - `FLOW_TAP_TERM` (currently 150)
   - `COMBO_TERM` (currently 40)
   - Per-key offsets in `get_tapping_term()` in custom_features.c

---

## 11. Oryx Compatibility

### How this layout relates to Oryx

This layout was originally created in Oryx (https://configure.zsa.io) with
layout ID `EnBmG`. The keymap.c file merges Oryx-generated scaffolding with
custom QMK features that Oryx does not support natively.

### What Oryx CAN manage

- The keymaps[] array: key positions and layer assignments
- LED color maps (the ledmap[] array)
- Oryx-specific keycodes (RGB_SLD, HSV presets)
- Basic firmware settings (rules.mk top section, config.h top section)

### What Oryx CANNOT manage

These features are in custom_features.c/h and the bottom sections of
config.h and rules.mk. Oryx does not know about them:

- Chordal Hold configuration
- Flow Tap
- Per-key tapping term
- Combos (J+K, D+F, K+L, Z+X)
- Leader Key sequences
- Key Overrides (Shift+Bspc = Del)
- Repeat Key and Alt-Repeat definitions
- CapsWord customization (continuation keys, LED indicators)
- Tri Layer configuration
- Dynamic Tapping Term keys

### Workflow for making changes

**If you only need to change key positions or LED colors:**

1. Edit in Oryx at https://configure.zsa.io/moonlander/layouts/EnBmG
2. Export the updated keymap.c
3. The exported file will NOT contain your custom features.
4. Merge manually: keep the Oryx keymaps[] array, but preserve the
   `#include "custom_features.c"` at the bottom and all custom sections.
5. Compile and flash.

**If you need to change custom features:**

1. Edit custom_features.c / custom_features.h / config.h / rules.mk directly.
2. Do NOT re-export from Oryx — it will overwrite your changes.
3. Compile locally: `qmk compile -kb zsa/moonlander/reva -km EnBmG`
4. Flash with Keymapp.

**If you need to change both:**

1. Make the Oryx changes and export.
2. Use git diff to identify what Oryx changed in keymap.c.
3. Apply only the Oryx keymaps[] changes, preserving custom sections.
4. Make your custom feature changes.
5. Compile and flash.

### File structure

```
EnBmG/
  config.h              Top: Oryx-generated. Bottom: custom #defines.
  rules.mk              Top: Oryx-generated. Bottom: custom feature flags.
  keymap.c              Oryx scaffolding + custom keymaps[] + #include custom.
  keymap.json           Oryx module declarations (minimal).
  custom_features.h     Layer enums, key aliases, combo IDs, LED palette.
  custom_features.c     All custom QMK implementations.
```

The boundary between Oryx-managed and custom code is marked with:

```
// ═══════════════════════════════════════════════════════════════════
// Custom QMK Configuration — append only, do not edit above
// ═══════════════════════════════════════════════════════════════════
```

Everything above that line in config.h and rules.mk is Oryx territory.
Everything below is yours. When Oryx re-exports, it only touches the
top sections. Your custom additions below the separator are safe as long
as you do not let Oryx overwrite the entire file.

### Serial number

The firmware serial is `EnBmG/qmQ003`. This identifies the layout+revision
when connected via USB. Oryx uses this to match the physical keyboard to
the web configurator.

---

## Appendix: Quick Reference Card

### Thumb keys

```
Left:  [LCtrl pod]  [Space/Nav]  [Bspc]  [LCmd]     Right:  [Hypr]  [Tab]  [Enter/Sym]
```

### Home row mods (GASC)

```
Left:   A=Cmd  S=Alt  D=Shift  F=Ctrl
Right:  J=Ctrl K=Shift L=Alt   ;=Cmd
```

### Combos

```
J+K = Esc    D+F = Tab    K+L = Bspc    Z+X = Mouse
```

### Leader sequences (tap Leader, then...)

```
x,s = save    x,f = find-file    x,b = switch-buf    x,k = kill-buf
x,0 = del-win x,1 = only-win     x,2 = split-h       x,3 = split-v
g   = magit   c,c = C-c C-c
```

### Layer access

```
Hold Space        = Nav (green)      Release = back to Base
Hold Enter        = Sym (amber)      Release = back to Base
Hold Space+Enter  = Func (purple)    Release = back to Base
Hold Z+X          = Mouse (pink)     Release = back to Base
Hold Sys / MO(5)  = System (red)     Release = back to Base
Press Game / TG(6) = Gaming (lime)   Press again = back to Base
```

### Key override

```
Shift + Backspace = Delete
```

### CapsWord

```
Both Shifts = toggle on     Space/Enter = deactivate
LEDs 1+4 light up when active
```
