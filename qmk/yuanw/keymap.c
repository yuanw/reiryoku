/**
 * Copyright 2021 Charly Delay <charly@codesink.dev> (@0xcharly)
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
#include "keycodes.h"
#ifdef OS_DETECTION_ENABLE
#include "os_detection.h"
#endif
#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
#    include "timer.h"
#endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
#include "repeat.h"

// Automatically enable sniping-mode on the pointer layer.
#define CHARYBDIS_AUTO_SNIPING_ON_LAYER LAYER_POINTER

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
static uint16_t auto_pointer_layer_timer = 0;

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS 1000
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD 8
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#endif     // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

#ifndef POINTING_DEVICE_ENABLE
#    define DRGSCRL KC_NO
#    define DPI_MOD KC_NO
#    define S_D_MOD KC_NO
#    define SNIPING KC_NO
#endif // !POINTING_DEVICE_ENABLE
       //
#define U_RDO SCMD(KC_Z)
#define U_PST LCMD(KC_V)
#define U_CPY LCMD(KC_C)
#define U_CUT LCMD(KC_X)
#define U_UND LCMD(KC_Z)
// clang-format off
/** \brief QWERTY layout (3 rows, 10 columns). */
#define LAYOUT_LAYER_BASE                                                                     \
       REPEAT,  KC_W,    KC_M,    KC_P,    KC_Q,    KC_Z,    KC_K,    KC_COMM, KC_DOT,  KC_SCLN, \
       KC_R,    KC_S,    KC_N,    KC_T,    KC_G,    KC_V,    KC_H,    KC_A,    KC_I,    KC_O,      \
       REV_REP, KC_C,    KC_F,    KC_D,    KC_B,    KC_J,    KC_L,    KC_U,    KC_Y,     KC_QUOT, \
                         ESC_MED, SPC_NAV, TAB_FUN, ENT_SYM, BSP_NUM

/** Convenience row shorthands. */
#define _______________DEAD_HALF_ROW_______________ XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
#define ______________HOME_ROW_GACS_L______________ KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX
#define ______________HOME_ROW_GACS_R______________ XXXXXXX, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI

/*
 * Layers used on the Charybdis Nano.
 *
 * These layers started off heavily inspired by the Miryoku layout, but trimmed
 * down and tailored for a stock experience that is meant to be fundation for
 * further personalization.
 *
 * See https://github.com/manna-harbour/miryoku for the original layout.
 */

/**
 * \brief Function layer.
 *
 * Secondary right-hand layer has function keys mirroring the numerals on the
 * primary layer with extras on the pinkie column, plus system keys on the inner
 * column. App is on the tertiary thumb key and other thumb keys are duplicated
 * from the base layer to enable auto-repeat.
 */
#define LAYOUT_LAYER_FUNCTION                                                                 \
    _______________DEAD_HALF_ROW_______________, XXXXXXX,   XXXXXXX,  KC_LCBR,   KC_RCBR,  KC_BSLS, \
    ______________HOME_ROW_GACS_L______________, KC_SCRL,   LAG(KC_1),       LAG(KC_2),       LAG(KC_3),       LAG(KC_4), \
    _______________DEAD_HALF_ROW_______________, KC_PAUS,   LSG(KC_1),       LSG(KC_2),       LSG(KC_3),       LSG(KC_4), \
                      XXXXXXX, XXXXXXX, _______, XXXXXXX, KC_TAB
/**
 * \brief Media layer.
 *
 * Tertiary left- and right-hand layer is media and RGB control.  This layer is
 * symmetrical to accomodate the left- and right-hand trackball.
 */
#define LAYOUT_LAYER_MEDIA                                                                    \
    XXXXXXX,RGB_RMOD, RGB_TOG, RGB_MOD, XXXXXXX, XXXXXXX, KC_V, KC_LBRC, KC_RBRC, KC_SLSH, \
    KC_MPRV, KC_VOLD, KC_MUTE, KC_VOLU, KC_MNXT, XXXXXXX, KC_H, KC_J, KC_K, KC_L, \
    XXXXXXX, XXXXXXX, XXXXXXX,  EE_CLR, QK_BOOT, QK_BOOT, KC_G, XXXXXXX, XXXXXXX, XXXXXXX, \
                      _______, XXXXXXX, XXXXXXX, XXXXXXX, KC_TAB

/** \brief Mouse emulation and pointer functions. */
#define LAYOUT_LAYER_POINTER                                                                  \
    XXXXXXX, XXXXXXX, XXXXXXX, DPI_MOD, S_D_MOD, S_D_MOD, DPI_MOD, XXXXXXX, XXXXXXX, XXXXXXX, \
    ______________HOME_ROW_GACS_L______________, ______________HOME_ROW_GACS_R______________, \
    _______, DRGSCRL, SNIPING,  EE_CLR, QK_BOOT, QK_BOOT,  EE_CLR, SNIPING, DRGSCRL, _______, \
                      KC_BTN2, KC_BTN1, KC_BTN3, KC_BTN3, KC_BTN1

/**
 * \brief Navigation layer.
 *
 * Primary right-hand layer (left home thumb) is navigation and editing. Cursor
 * keys are on the home position, line and page movement below, clipboard above,
 * caps lock and insert on the inner column. Thumb keys are duplicated from the
 * base layer to avoid having to layer change mid edit and to enable auto-repeat.
 */
#define LAYOUT_LAYER_NAVIGATION                                                               \
    _______________DEAD_HALF_ROW_______________, XXXXXXX, XXXXXXX, KC_LPRN,  KC_RPRN,  KC_PIPE,\
    ______________HOME_ROW_GACS_L______________, KC_CAPS, KC_LEFT, KC_DOWN,   KC_UP, KC_RGHT, \
    _______________DEAD_HALF_ROW_______________, RDO,   PST,   CPY,   CUT,  UND, \
                      XXXXXXX, _______, XXXXXXX,  XXXXXXX, KC_TAB

/**
 * \brief Numeral layout.
 *
 * Primary left-hand layer (right home thumb) is numerals and symbols. Numerals
 * are in the standard numpad locations with symbols in the remaining positions.
 * `KC_DOT` is duplicated from the base layer.
 */
#define LAYOUT_LAYER_NUMERAL                                                                  \
    KC_LBRC,    KC_7,    KC_8,    KC_9, KC_RBRC, _______________DEAD_HALF_ROW_______________, \
    KC_SCLN,    KC_4,    KC_5,    KC_6,  KC_EQL, ______________HOME_ROW_GACS_R______________, \
     KC_GRV,    KC_1,    KC_2,    KC_3, KC_BSLS, _______________DEAD_HALF_ROW_______________, \
                       KC_DOT,    KC_0, KC_MINS, XXXXXXX, _______

/**
 * \brief Symbols layer.
 *
 * Secondary left-hand layer has shifted symbols in the same locations to reduce
 * chording when using mods with shifted symbols. `KC_LPRN` is duplicated next to
 * `KC_RPRN`.
 */
#define LAYOUT_LAYER_SYMBOLS                                                                  \
    KC_LCBR, KC_AMPR, KC_ASTR, KC_LPRN, KC_RCBR, _______________DEAD_HALF_ROW_______________, \
    KC_COLN,  KC_DLR, KC_PERC, KC_CIRC, KC_PLUS, ______________HOME_ROW_GACS_R______________, \
    KC_TILD, KC_EXLM,   KC_AT, KC_HASH, KC_PIPE, _______________DEAD_HALF_ROW_______________, \
                      KC_LPRN, KC_RPRN, KC_UNDS, _______, XXXXXXX

/**
 * \brief Add Home Row mod to a layout.
 *
 * Expects a 10-key per row layout.  Adds support for GACS (Gui, Alt, Ctl, Shift)
 * home row.  The layout passed in parameter must contain at least 20 keycodes.
 *
 * This is meant to be used with `LAYER_ALPHAS_QWERTY` defined above, eg.:
 *
 *     HOME_ROW_MOD_GACS(LAYER_ALPHAS_QWERTY)
 */
#define _HOME_ROW_MOD_GACS(                                            \
    L00, L01, L02, L03, L04, R05, R06, R07, R08, R09,                  \
    L10, L11, L12, L13, L14, R15, R16, R17, R18, R19,                  \
    ...)                                                               \
             L00,         L01,         L02,         L03,         L04,  \
             R05,         R06,         R07,         R08,         R09,  \
      LGUI_T(L10), LALT_T(L11), LCTL_T(L12), LSFT_T(L13),        L14,  \
             R15,  RSFT_T(R16), RCTL_T(R17), LALT_T(R18), RGUI_T(R19), \
      __VA_ARGS__
#define HOME_ROW_MOD_GACS(...) _HOME_ROW_MOD_GACS(__VA_ARGS__)

/**
 * \brief Add pointer layer keys to a layout.
 *
 * Expects a 10-key per row layout.  The layout passed in parameter must contain
 * at least 30 keycodes.
 *
 * This is meant to be used with `LAYER_ALPHAS_QWERTY` defined above, eg.:
 *
 *     POINTER_MOD(LAYER_ALPHAS_QWERTY)
 */
#define _POINTER_MOD(                                                  \
    L00, L01, L02, L03, L04, R05, R06, R07, R08, R09,                  \
    L10, L11, L12, L13, L14, R15, R16, R17, R18, R19,                  \
    L20, L21, L22, L23, L24, R25, R26, R27, R28, R29,                  \
    ...)                                                               \
             L00,         L01,         L02,         L03,         L04,  \
             R05,         R06,         R07,         R08,         R09,  \
             L10,         L11,         L12,         L13,         L14,  \
             R15,         R16,         R17,         R18,         R19,  \
      _L_PTR(L20),        L21,         L22,         L23,         L24,  \
             R25,         R26,         R27,         R28,  _L_PTR(R29), \
      __VA_ARGS__
#define POINTER_MOD(...) _POINTER_MOD(__VA_ARGS__)

#define LAYOUT_wrapper(...) LAYOUT(__VA_ARGS__)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT_wrapper(
    POINTER_MOD(HOME_ROW_MOD_GACS(LAYOUT_LAYER_BASE))
  ),
  [LAYER_FUNCTION] = LAYOUT_wrapper(LAYOUT_LAYER_FUNCTION),
  [LAYER_NAVIGATION] = LAYOUT_wrapper(LAYOUT_LAYER_NAVIGATION),
  [LAYER_MEDIA] = LAYOUT_wrapper(LAYOUT_LAYER_MEDIA),
  [LAYER_NUMERAL] = LAYOUT_wrapper(LAYOUT_LAYER_NUMERAL),
  [LAYER_POINTER] = LAYOUT_wrapper(LAYOUT_LAYER_POINTER),
  [LAYER_SYMBOLS] = LAYOUT_wrapper(LAYOUT_LAYER_SYMBOLS),
};
// clang-format on

#ifdef POINTING_DEVICE_ENABLE
#    ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (abs(mouse_report.x) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD || abs(mouse_report.y) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD) {
        if (auto_pointer_layer_timer == 0) {
            layer_on(LAYER_POINTER);
#        ifdef RGB_MATRIX_ENABLE
            rgb_matrix_mode_noeeprom(RGB_MATRIX_NONE);
            rgb_matrix_sethsv_noeeprom(HSV_GREEN);
#        endif // RGB_MATRIX_ENABLE
        }
        auto_pointer_layer_timer = timer_read();
    }
    return mouse_report;
}

void matrix_scan_user(void) {
    if (auto_pointer_layer_timer != 0 && TIMER_DIFF_16(timer_read(), auto_pointer_layer_timer) >= CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS) {
        auto_pointer_layer_timer = 0;
        layer_off(LAYER_POINTER);
#        ifdef RGB_MATRIX_ENABLE
        rgb_matrix_mode_noeeprom(RGB_MATRIX_DEFAULT_MODE);
#        endif // RGB_MATRIX_ENABLE
    }
}
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

#    ifdef CHARYBDIS_AUTO_SNIPING_ON_LAYER
layer_state_t layer_state_set_user(layer_state_t state) {
    charybdis_set_pointer_sniping_enabled(layer_state_cmp(state, CHARYBDIS_AUTO_SNIPING_ON_LAYER));
    return state;
}
#    endif // CHARYBDIS_AUTO_SNIPING_ON_LAYER
#endif     // POINTING_DEVICE_ENABLE

#ifdef RGB_MATRIX_ENABLE
// Forward-declare this helper function since it is defined in
// rgb_matrix.c.
void rgb_matrix_update_pwm_buffers(void);
#endif

void shutdown_user(void) {
#ifdef RGBLIGHT_ENABLE
    rgblight_enable_noeeprom();
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
    rgblight_setrgb(RGB_RED);
#endif // RGBLIGHT_ENABLE
#ifdef RGB_MATRIX_ENABLE
    rgb_matrix_set_color_all(RGB_RED);
    rgb_matrix_update_pwm_buffers();
#endif // RGB_MATRIX_ENABLE
}

// Thumb Combos
/* #define EXTRA_SHORT_COMBOS */

enum combos {
    FD_B,
    LU_J,
    MP_Q,
    KCOMM_Z,
    LEFT_QUESTION,
    CF_X,
    COMBO_LENGTH
};

uint16_t COMBO_LEN = COMBO_LENGTH; // remove the COMBO_COUNT define and use this instead!

const uint16_t PROGMEM b_combo[]    = {KC_F, KC_D, COMBO_END};
const uint16_t PROGMEM j_combo[]    = {KC_L, KC_U, COMBO_END};
const uint16_t PROGMEM q_combo[]    = {KC_M, KC_P, COMBO_END};
const uint16_t PROGMEM z_combo[]    = {KC_K, KC_COMM, COMBO_END};
const uint16_t PROGMEM left_combo[] = {KC_W, KC_M, COMBO_END};
const uint16_t PROGMEM x_combo[]    = {KC_C, KC_F, COMBO_END};

combo_t key_combos[] = {
    [FD_B] = COMBO(b_combo, KC_B), [LU_J] = COMBO(j_combo, KC_J), [MP_Q] = COMBO(q_combo, KC_Q), [KCOMM_Z] = COMBO(z_combo, KC_Z),
    [LEFT_QUESTION] = COMBO(left_combo, KC_QUESTION),
    [CF_X] = COMBO(x_combo, KC_X),
};
/*   #define U_RDO KC_AGIN */
/*   #define U_PST KC_PSTE */
/*   #define U_CPY KC_COPY */
/*   #define U_CUT KC_CUT */
/*   #define U_UND KC_UNDO */
/* #elif defined (MIRYOKU_CLIPBOARD_MAC) */
/*   #define U_RDO SCMD(KC_Z) */
/*   #define U_PST LCMD(KC_V) */
/*   #define U_CPY LCMD(KC_C) */
/*   #define U_CUT LCMD(KC_X) */
/*   #define U_UND LCMD(KC_Z) */
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if ( keycode != REPEAT && keycode != REV_REP)  {
       if ( record->event.pressed  ) {
        register_key_to_repeat(keycode);
       }
    }
    switch (keycode) {
        case REPEAT:
          update_repeat_key(record);
          return false;
        case REV_REP:
          update_reverse_repeat_key(record);
          return false;
        case CPY:
            if (record->event.pressed) {
                switch (detected_host_os()) {
                    case OS_MACOS: // On Mac, set default layer to BASE_MAC.
                    case OS_IOS:
                        tap_code16(LCMD(KC_C));
                        return false;
                    default:
                        tap_code16(KC_COPY);
                        return false;
                }
            }
        case PST:
            if (record->event.pressed) {
                switch (detected_host_os()) {
                    case OS_MACOS: // On Mac, set default layer to BASE_MAC.
                    case OS_IOS:
                        tap_code16(LCMD(KC_V));
                        return false;
                    default:
                        tap_code16(KC_PSTE);
                        return false;
                }
            }
        case CUT:
            if (record->event.pressed) {
                switch (detected_host_os()) {
                    case OS_MACOS: // On Mac, set default layer to BASE_MAC.
                    case OS_IOS:
                        tap_code16(LCMD(KC_X));
                        return false;
                    default:
                        tap_code16(KC_CUT);
                        return false;
                }
            }
        case UND:
            if (record->event.pressed) {
                switch (detected_host_os()) {
                    case OS_MACOS: // On Mac, set default layer to BASE_MAC.
                    case OS_IOS:
                        tap_code16(LCMD(KC_Z));
                        return false;
                    default:
                        tap_code16(KC_UNDO);
                        return false;
                }
            }
        case RDO:
            if (record->event.pressed) {
                switch (detected_host_os()) {
                    case OS_MACOS: // On Mac, set default layer to BASE_MAC.
                    case OS_IOS:
                        tap_code16(LCMD(KC_Z));
                        return false;
                    default:
                        tap_code16(KC_AGIN);
                        return false;
                }
            }
        default:
            return true; // Process all other keycodes normally
    }
}
