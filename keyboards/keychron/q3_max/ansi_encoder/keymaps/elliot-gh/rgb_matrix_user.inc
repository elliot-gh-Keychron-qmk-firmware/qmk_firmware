/* Copyright 2025 Elliot Yoon
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

// !!! DO NOT ADD #pragma once !!! //

// Step 1.
// Declare custom effects using the RGB_MATRIX_EFFECT macro
// (note the lack of semicolon after the macro!)
RGB_MATRIX_EFFECT(esc_white_only)
RGB_MATRIX_EFFECT(all_white)
RGB_MATRIX_EFFECT(indicator_only)

// Step 2.
// Define effects inside the `RGB_MATRIX_CUSTOM_EFFECT_IMPLS` ifdef block
#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static bool esc_white_only(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    rgb_matrix_set_color_all(0, 0, 0);
    rgb_matrix_set_color(0, 255, 255, 255);
    return led_max < RGB_MATRIX_LED_COUNT;
}

static bool all_white(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    rgb_matrix_set_color_all(255, 255, 255);
    return led_max < RGB_MATRIX_LED_COUNT;
}

static bool indicator_only(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    rgb_matrix_set_color_all(0, 0, 0);
    return led_max < RGB_MATRIX_LED_COUNT;
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS

/*
0:  Esc       1:  F1      2:  F2      3:  F3      4:  F4      5:  F5      6:  F6      7:  F7      8:  F8      9:  F9      10: F10     11: F11     12: F12                    13: PrtSc   14: ScrLk   15: Pause
16: ~         17: 1       18: 2       19: 3       20: 4       21: 5       22: 6       23: 7       24: 8       25: 9       26: 0       27: -       28: =       29: Backspace  30: Ins     31: Home    32: PgUp
33: Tab       34: Q       35: W       36: E       37: R       38: T       39: Y       40: U       41: I       42: O       43: P       44: [       45: ]       46: \          47: Del     48: End     49: PgDn
50: CapsLock  51: A       52: S       53: D       54: F       55: G       56: H       57: J       58: K       59: L       60: ;       61: '                   62: Enter
63: LShift    64: Z       65: X       66: C       67: V       68: B       69: N       70: M       71: ,       72: .       73: /                               74: RShift                 75: Up
76: LCtrl     77: LWin    78: LAlt                            79: Space                                       80: RAlt    81: Fn      82: Menu    83: RCtrl                  84: Left    85: Down    86: Right
*/