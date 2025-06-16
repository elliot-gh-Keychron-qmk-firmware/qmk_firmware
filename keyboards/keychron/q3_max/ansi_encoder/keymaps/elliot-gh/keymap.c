/* Copyright 2024 ~ 2025 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "keychron_common.h"

static bool gui_locked = false;
static const uint16_t RESET_HOLD_MS = 5000;
static uint16_t reset_timer = 0;
static uint8_t prev_mode_ind = RGB_MATRIX_CUSTOM_indicator_only;

enum layers {
    MAC_BASE,
    FN1,
    WIN_BASE
};

enum custom_keycodes {
    GUI_LOCK = NEW_SAFE_RANGE,
    RST_DLY,
    RGB_IND
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [MAC_BASE] = LAYOUT_tkl_ansi(
        KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,     KC_MPLY,    KC_PSCR,  KC_SCRL,  KC_BRK,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,     KC_BSPC,    KC_INS,   KC_HOME,  KC_PGUP,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,    KC_BSLS,    KC_DEL,   KC_END,   KC_PGDN,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,              KC_UP,
        KC_LCTL,  KC_LOPTN, KC_LCMMD,                               KC_SPC,                                 KC_RCMMD, MO(FN1),  KC_ROPTN,   KC_RCTL,    KC_LEFT,  KC_DOWN,  KC_RGHT),

    [FN1] = LAYOUT_tkl_ansi(
        RST_DLY,  KC_BRID,  KC_BRIU,  _______,  _______,  RGB_VAD,  RGB_VAI,  _______,  _______,  _______,  _______,  _______,  _______,    KC_MUTE,    _______,  _______,  _______,
        _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,    KC_MPLY,  KC_MUTE,  KC_VOLU,
        RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  _______,  _______,  NK_OFF,   _______,  _______,  _______,    _______,    KC_MPRV,  KC_MNXT,  KC_VOLD,
        RGB_IND,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,  _______,  _______,  _______,  _______,  _______,              _______,
        _______,            _______,  _______,  _______,  _______,  BAT_LVL,  NK_ON,    _______,  _______,  _______,  _______,              _______,              _______,
        _______,  GUI_LOCK, _______,                                _______,                                _______,  _______,  KC_RGUI,    _______,    _______,  _______,  _______),

    [WIN_BASE] = LAYOUT_tkl_ansi(
        KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,     KC_MPLY,    KC_PSCR,  KC_SCRL,  KC_BRK,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,     KC_BSPC,    KC_INS,   KC_HOME,  KC_PGUP,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,    KC_BSLS,    KC_DEL,   KC_END,   KC_PGDN,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,              KC_UP,
        KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                                 KC_RALT,  MO(FN1),  KC_APP,     KC_RCTL,    KC_LEFT,  KC_DOWN,  KC_RGHT),
};

// clang-format on
#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [MAC_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [FN1]  = {ENCODER_CCW_CW(KC_TRNS, KC_TRNS)},
    [WIN_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
};
#endif // ENCODER_MAP_ENABLE

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_keychron_common(keycode, record)) {
        return false;
    }

    switch (keycode) {
        case (GUI_LOCK):
            if (record->event.pressed) {
                gui_locked = !gui_locked;
            }
            return false;
        case KC_LGUI:
        case KC_RGUI:
            if (record->event.pressed) {
                return !gui_locked;
            }
            return true;
        case (RST_DLY):
            if (record->event.pressed) {
                reset_timer = timer_read();
            } else {
                if (timer_elapsed(reset_timer) >= RESET_HOLD_MS) {
                    reset_keyboard();
                }
            }
            return false;
        case RGB_IND:
            if (record->event.pressed) {
                uint8_t current_mode = rgb_matrix_get_mode();
                if (current_mode == RGB_MATRIX_CUSTOM_indicator_only) {
                    rgb_matrix_mode(prev_mode_ind);
                } else {
                    prev_mode_ind = current_mode;
                    rgb_matrix_mode(RGB_MATRIX_CUSTOM_indicator_only);
                }
            }
            return false;
    }

    return true;
}

// used in rgb_matrix_indicators_advanced_user()
void turn_on_indicators(const uint8_t index_arr[], const uint8_t size) {
    for (uint8_t index = 0; index < size; index++) {
        uint8_t led_index = index_arr[index];

        rgb_matrix_set_color(led_index, 255, 255, 255);
    }
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    static const uint8_t arr_caps_lock[] = {50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62};
    if (host_keyboard_led_state().caps_lock) {
        turn_on_indicators(arr_caps_lock, sizeof(arr_caps_lock) / sizeof(arr_caps_lock[0]));
    }

    static const uint8_t arr_scroll_lock[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    if (host_keyboard_led_state().scroll_lock) {
        turn_on_indicators(arr_scroll_lock, sizeof(arr_scroll_lock) / sizeof(arr_scroll_lock[0]));
    }

    static const uint8_t arr_gui_lock[] = {76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86};
    if (gui_locked) {
        turn_on_indicators(arr_gui_lock, sizeof(arr_gui_lock) / sizeof(arr_gui_lock[0]));
    }

    return false;
}

/*
0:  Esc       1:  F1      2:  F2      3:  F3      4:  F4      5:  F5      6:  F6      7:  F7      8:  F8      9:  F9      10: F10     11: F11     12: F12                    13: PrtSc   14: ScrLk   15: Pause
16: ~         17: 1       18: 2       19: 3       20: 4       21: 5       22: 6       23: 7       24: 8       25: 9       26: 0       27: -       28: =       29: Backspace  30: Ins     31: Home    32: PgUp
33: Tab       34: Q       35: W       36: E       37: R       38: T       39: Y       40: U       41: I       42: O       43: P       44: [       45: ]       46: \          47: Del     48: End     49: PgDn
50: CapsLock  51: A       52: S       53: D       54: F       55: G       56: H       57: J       58: K       59: L       60: ;       61: '                   62: Enter
63: LShift    64: Z       65: X       66: C       67: V       68: B       69: N       70: M       71: ,       72: .       73: /                               74: RShift                 75: Up
76: LCtrl     77: LWin    78: LAlt                            79: Space                                       80: RAlt    81: Fn      82: Menu    83: RCtrl                  84: Left    85: Down    86: Right
*/
