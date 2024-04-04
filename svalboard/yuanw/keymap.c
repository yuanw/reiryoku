/*
Copyright 2023 Morgan Venable @_claussen

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include <stdbool.h>
#include <stdint.h>
#include "features/achordion.h"
#ifdef OS_DETECTION_ENABLE
#    include "os_detection.h"
#endif
void keyboard_post_init_user(void) {
  // Customise these values if you need to debug the matrix
  //debug_enable=true;
  debug_matrix=true;
  //debug_keyboard=true;
  //debug_mouse=true;
}

// in keymap.c:
#ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
void pointing_device_init_user(void) {
    set_auto_mouse_layer(15); // only required if AUTO_MOUSE_DEFAULT_LAYER is not set to index of <mouse_layer>
    set_auto_mouse_enable(true);         // always required before the auto mouse feature will work
}
#endif

#define ESC_MED LT(LAYER_MEDIA, KC_ESC)
#define SPC_NAV LT(LAYER_NAVIGATION, KC_SPC)
#define TAB_FUN LT(LAYER_FUNCTION, KC_BSPC)
#define ENT_SYM LT(LAYER_SYMBOLS, KC_ENT)
#define E_NUM LT(LAYER_NUMERAL, KC_E)

enum my_keycodes {
   RDO = SAFE_RANGE,
                   PST,
                   CPY,
                   CUT,
                   UND,
                   ALTREP2,
                   ALTREP3,
};

enum layer {
    NORMAL,
    LAYER_NAVIGATION,
    LAYER_NUMERAL,
    LAYER_SYMBOL,
    LAYER_MEDIA,
    LAYER_FUNCTION,
    NAS,
    FUNC,
    FUNC_HOLD,
    MBO,
    NUM_LAYERS
};

const uint16_t PROGMEM keymaps[NUM_LAYERS][MATRIX_ROWS][MATRIX_COLS] = {
 [NORMAL] = LAYOUT(
             /*Center           North           East            South           West*/
        /*R1*/ RSFT_T(KC_V),    KC_J,           KC_L,           KC_K,           KC_H,
        /*R2*/ RCTL_T(KC_A),    KC_COMM,        XXXXXXX,        KC_U,           XXXXXXX,
        /*R3*/ LALT_T(KC_I),    KC_SCLN,        XXXXXXX,        KC_Y,           XXXXXXX,
        /*R4*/ RGUI_T(KC_O),    KC_DOT,         XXXXXXX,        KC_QUOT,        XXXXXXX,

        /*L1*/ LSFT_T(KC_T),    KC_P,           KC_G,           KC_D,           XXXXXXX,
        /*L2*/ LCTL_T(KC_N),    KC_M,           KC_B,           KC_F,           XXXXXXX,
        /*L3*/ LALT_T(KC_S),    KC_W,           XXXXXXX,        XXXXXXX,        KC_C,
        /*L4*/ LGUI_T(KC_R),    KC_Q,           KC_LBRC,        KC_QUESTION,    KC_DEL,

        /*Down                  Inner (pad)     Upper (Mode)    O.Upper (nail)  OL (knuckle) Pushthrough*/  
        /*RT*/ SPC_NAV,         KC_SPACE,       TO(FUNC),       KC_BSPC,        KC_LALT,     KC_R,
        /*LT*/ E_NUM,           KC_ENTER,       TO(NORMAL),     KC_TAB,         KC_LCTL,     KC_L
    ),

    [LAYER_NAVIGATION] = LAYOUT(
             /*Center           North           East            South           West*/
        /*R1*/ KC_LEFT,         XXXXXXX,        XXXXXXX,        PST,        KC_CAPS,
        /*R2*/ KC_DOWN,         KC_LPRN,        XXXXXXX,        CPY,        LCTL(KC_DOWN),
        /*R3*/ KC_UP,           KC_PIPE,        XXXXXXX,        CUT,        LCTL(KC_UP),
        /*R4*/ KC_RIGHT,        KC_RPRN,        XXXXXXX,        UND,        LCTL(KC_RIGHT),

        /*L1*/ XXXXXXX,         XXXXXXX,        XXXXXXX,        KC_BTN1,        XXXXXXX,
        /*L2*/ XXXXXXX,         XXXXXXX,        XXXXXXX,        KC_BTN3,        XXXXXXX,
        /*L3*/ XXXXXXX,         XXXXXXX,        XXXXXXX,        KC_BTN2,        XXXXXXX,
        /*L4*/ DF(NORMAL),      _______,        _______,        XXXXXXX,       _______,

        /*Down                  Inner           Upper           Outer Upper     Outer Lower  Pushthrough*/
        /*RT*/ _______,         _______,        _______,        _______,        _______, _______,
        /*LT*/ XXXXXXX,         _______,        _______,        _______,        _______, _______
    ),

    [NAS] = LAYOUT(
             /*Center           North           East            South           West*/
        /*R1*/ KC_7,            KC_AMPR,        KC_UNDS,        KC_KP_PLUS,     KC_6,
        /*R2*/ KC_8,            KC_KP_ASTERISK, KC_COLON,       KC_COMMA,       KC_CIRCUMFLEX,
        /*R3*/ KC_9,            KC_LPRN,        KC_LGUI,        KC_DOT,         KC_SEMICOLON,
        /*R4*/ KC_0,            KC_RPRN,        XXXXXXX,        KC_QUES,        KC_RBRC,

        /*L1*/ KC_4,            KC_DOLLAR,      KC_5,           KC_MINUS,       KC_SLASH,
        /*L2*/ KC_3,            KC_HASH,        KC_GT,          KC_PERCENT,     KC_LT,
        /*L3*/ KC_2,            KC_AT,          XXXXXXX,        KC_X,           KC_ESC,
        /*L4*/ KC_1,            KC_EXCLAIM,     KC_TILDE,       KC_EQUAL,       KC_DEL,

        /*Down                  Inner           Upper           Outer Upper     Outer Lower  Pushthrough*/  
        /*RT*/ MO(NAS),         KC_SPACE,       _______,       KC_BSPC,        KC_LALT, _______,
        /*LT*/ KC_LSFT,         KC_ENTER,       _______,        KC_TAB,         KC_LCTL, _______
    ),

    [FUNC] = LAYOUT(
             /*Center           North           East            South           West*/
        /*R1*/ KC_HOME,         KC_UP,          KC_RIGHT,       KC_DOWN,        KC_LEFT,
        /*R2*/ XXXXXXX,         KC_F8,          XXXXXXX,        KC_F7,          KC_END,
        /*R3*/ KC_PSCR,         KC_F10,         KC_LGUI,        KC_F9,          KC_INS,
        /*R4*/ KC_PAUSE,        KC_PGUP,        KC_F12,         KC_PGDN,        KC_F11,

        /*L1*/ KC_HOME,         KC_UP,          KC_RIGHT,       KC_DOWN,        KC_LEFT,
        /*L2*/ XXXXXXX,         KC_F6,          XXXXXXX,        KC_F5,          XXXXXXX,
        /*L3*/ XXXXXXX,         KC_F4,          XXXXXXX,        KC_F3,          KC_ESC,
        /*L4*/ XXXXXXX,         KC_F2,          XXXXXXX,        KC_F1,          KC_DEL,

             /*Down                  Inner           Upper           Outer Upper     Outer Lower  Pushthrough*/  
        /*RT*/ MO(NAS),         KC_SPACE,       _______,       KC_BSPC,      KC_LALT, _______,
        /*LT*/ KC_LSFT,       KC_ENTER,         _______, KC_TAB,         KC_LCTL,_______
    ),

    [FUNC_HOLD] = LAYOUT(
             /*Center           North           East            South           West*/
        /*R1*/ KC_LEFT,         LCTL(KC_UP),   LCTL(KC_RIGHT), LCTL(KC_DOWN), LCTL(KC_LEFT),
        /*R2*/ KC_UP,           KC_MS_U,        KC_MS_R,        KC_MS_D,        KC_MS_L,
        /*R3*/ KC_DOWN,         KC_WH_U,        KC_WH_R,        KC_WH_D,        KC_WH_L,
        /*R4*/ KC_RIGHT,        XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,

        /*L1*/ XXXXXXX,         XXXXXXX,        XXXXXXX,        XXXXXXX,     XXXXXXX,
        /*L2*/ XXXXXXX,         XXXXXXX,        XXXXXXX,        XXXXXXX,     XXXXXXX,
        /*L3*/ XXXXXXX,         XXXXXXX,        XXXXXXX,        XXXXXXX,     XXXXXXX,
        /*L4*/ _______,      _______,        _______,        _______,       _______,

             /*Down                  Inner           Upper           Outer Upper     Outer Lower  Pushthrough*/  
        /*RT*/ _______,         _______,        _______,        _______,        _______,_______,
        /*LT*/ _______,         _______,        _______,        _______,        _______, _______
    ),
    
    [MBO] = LAYOUT(
             /*Center           North           East            South           West*/
        /*R1*/ KC_TRNS,        KC_TRNS,       KC_TRNS,       KC_BTN1,       KC_TRNS,
        /*R2*/ KC_TRNS,        KC_TRNS,       KC_TRNS,       KC_BTN3,       KC_TRNS,
        /*R3*/ KC_TRNS,        KC_TRNS,       KC_TRNS,       KC_BTN2,       KC_TRNS,
        /*R4*/ KC_TRNS,        KC_TRNS,       KC_TRNS,       KC_TRNS,       KC_TRNS,
        /*L1*/ KC_TRNS,        KC_TRNS,       KC_TRNS,       KC_BTN1,        KC_TRNS,
        /*L2*/ KC_TRNS,        KC_TRNS,       KC_TRNS,       KC_BTN3,        KC_TRNS,
        /*L3*/ KC_TRNS,        KC_TRNS,       KC_TRNS,       KC_BTN2,        KC_TRNS,
        /*L4*/ KC_TRNS,        KC_TRNS,       KC_TRNS,       KC_TRNS,       KC_TRNS,
        /*RT*/ KC_TRNS,        KC_TRNS,       KC_TRNS,       KC_TRNS,       KC_TRNS,   KC_TRNS,
        /*LT*/ KC_TRNS,        KC_TRNS,       KC_TRNS,       KC_TRNS,       KC_TRNS,   KC_TRNS
        )

};


#if (defined MH_AUTO_BUTTONS && defined PS2_MOUSE_ENABLE && defined MOUSEKEY_ENABLE)  || defined(POINTING_DEVICE_AUTO_MOUSE_MH_ENABLE)

static uint16_t mh_auto_buttons_timer;
extern int tp_buttons; // mousekey button state set in action.c and used in ps2_mouse.c

void mouse_mode(bool);

#endif

#if defined(POINTING_DEVICE_AUTO_MOUSE_MH_ENABLE)
report_mouse_t pointing_device_task_user(report_mouse_t reportMouse) {
    print("mh_auto_buttons: called\n");
    if (reportMouse.x == 0 && reportMouse.y == 0)
        return reportMouse;

    if (mh_auto_buttons_timer) {
        mh_auto_buttons_timer = timer_read();
    } else {
        mouse_mode(true);
#if defined CONSOLE_ENABLE
        print("mh_auto_buttons: on\n");
#endif
    }
    return reportMouse;
}
#endif

// Use ALTREP2 and ALTREP3 in your layout...

bool remember_last_key_user(uint16_t keycode, keyrecord_t* record,
                            uint8_t* remembered_mods) {
    switch (keycode) {
        case ALTREP2:
        case ALTREP3:
            return false;  // Ignore ALTREP keys.
    }

    return true;  // Other keys can be repeated.
}

//TODO do we still need this
uint16_t get_alt_repeat_key_keycode_user(uint16_t keycode, uint8_t mods) {
    /* if ((mods & MOD_MASK_CTRL)) {  // Was Ctrl held? */
    /*     switch (keycode) { */
    /*         case KC_Y: return C(KC_Z);  // Ctrl + Y reverses to Ctrl + Z. */
    /*         case KC_Z: return C(KC_Y);  // Ctrl + Z reverses to Ctrl + Y. */
    /*     } */
    /* } */
    switch (keycode) {
      case KC_LPRN: return KC_RPRN;
      case KC_RPRN: return KC_LPRN;
    }

    return KC_TRNS;  // Defer to default definitions.
}

static void process_altrep2(uint16_t keycode, uint8_t mods) {
    switch (keycode) {
        case KC_A:
        case RCTL_T(KC_A):
          SEND_STRING("tion");
          break;
        case LALT_T(KC_I): SEND_STRING("tion"); break;
        case LALT_T(KC_S): SEND_STRING("sion"); break;
        case LSFT_T(KC_T): SEND_STRING("heir"); break;
        case KC_Y: SEND_STRING("ou"); break;
        case KC_W: SEND_STRING("hich"); break;
        case KC_AT: SEND_STRING("Workiva/release-management-p"); break;
        case KC_C: SEND_STRING("ontent management"); break;
    }
}

static void process_altrep3(uint16_t keycode, uint8_t mods) {
    switch (keycode) {
        case RCTL_T(KC_A): SEND_STRING("bout"); break;
        case LALT_T(KC_I): SEND_STRING("ng"); break;
        case LALT_T(KC_S): SEND_STRING("tate"); break;
        case LSFT_T(KC_T): SEND_STRING("here"); break;
        case KC_W: SEND_STRING("ould"); break;
        case KC_AT: SEND_STRING("rmconsole-wf"); break;
        case E_NUM: SEND_STRING("specially");break;
        case KC_C: SEND_STRING("ontent-management-service"); break;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

      // If console is enabled, it will print the matrix position and status of each key pressed
#ifdef CONSOLE_ENABLE
    uprintf("KL: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
#endif
     if (!process_achordion(keycode, record)) { return false; }

#if (defined MH_AUTO_BUTTONS && defined PS2_MOUSE_ENABLE && defined MOUSEKEY_ENABLE) || defined(POINTING_DEVICE_AUTO_MOUSE_MH_ENABLE)
    if (mh_auto_buttons_timer) {
      switch (keycode) {
      case KC_BTN1:
      case KC_BTN2:
      case KC_BTN3:
      case KC_BTN4:
      case KC_BTN5:
      case KC_WH_U:
      case KC_WH_D:
      case KC_WH_R:
      case KC_WH_L:
	break;
      default:
	mouse_mode(false);
      }
    }
#endif

  switch (keycode) {
        case ALTREP2:
            if (record->event.pressed) {
                process_altrep2(get_last_keycode(), get_last_mods());
            }
            return false;
        case ALTREP3:
            if (record->event.pressed) {
                process_altrep3(get_last_keycode(), get_last_mods());
            }
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
                        break;
                    default:
                        tap_code16(KC_PSTE);
                        break;
                }
            }
            return false;
        case CUT:
            if (record->event.pressed) {
                switch (detected_host_os()) {
                    case OS_MACOS: // On Mac, set default layer to BASE_MAC.
                    case OS_IOS:
                        tap_code16(LCMD(KC_X));
                        break;
                    default:
                        tap_code16(KC_CUT);
                        break;
                }
            }
            return false;
        case UND:
            if (record->event.pressed) {
                switch (detected_host_os()) {
                    case OS_MACOS: // On Mac, set default layer to BASE_MAC.
                    case OS_IOS:
                        tap_code16(LCMD(KC_Z));
                        break;
                    default:
                        tap_code16(KC_UNDO);
                        break;
                }
            }
            return false;
        case RDO:
            if (record->event.pressed) {
                switch (detected_host_os()) {
                    case OS_MACOS: // On Mac, set default layer to BASE_MAC.
                    case OS_IOS:
                        tap_code16(LCMD(KC_Z));
                        break;
                    default:
                        tap_code16(KC_AGIN);
                        break;
                }
            }
            return false;
        default:
            return true;
        }

};

#if defined MH_AUTO_BUTTONS && defined PS2_MOUSE_ENABLE && defined MOUSEKEY_ENABLE
void ps2_mouse_moved_user(report_mouse_t *mouse_report) {
  if (mh_auto_buttons_timer) {
    mh_auto_buttons_timer = timer_read();
  } else {
    if (!tp_buttons) {
      mouse_mode(true);
  #if defined CONSOLE_ENABLE
      print("mh_auto_buttons: on\n");
  #endif
    }
  }
}
#endif



#if (defined MH_AUTO_BUTTONS && defined PS2_MOUSE_ENABLE && defined MOUSEKEY_ENABLE) || defined(POINTING_DEVICE_AUTO_MOUSE_MH_ENABLE)
void matrix_scan_user(void) {
    achordion_task();
  if (mh_auto_buttons_timer && (timer_elapsed(mh_auto_buttons_timer) > MH_AUTO_BUTTONS_TIMEOUT)) {
    if (!tp_buttons) {
      mouse_mode(false);
  #if defined CONSOLE_ENABLE
      print("mh_auto_buttons: off\n");
  #endif
    }
  }
}

void mouse_mode(bool on) {
  if (on) {
    layer_on(MH_AUTO_BUTTONS_LAYER);
    mh_auto_buttons_timer = timer_read();
  } else {
    layer_off(MH_AUTO_BUTTONS_LAYER);
    mh_auto_buttons_timer = 0;
  }
}

#endif // defined MH_AUTO_BUTTONS && defined PS2_MOUSE_ENABLE && #defined MOUSEKEY_ENABLE
