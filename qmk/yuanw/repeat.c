#include "repeat.h"
#include "keycodes.h"

uint16_t last_keycode = KC_NO;

bool tap_undead_key(bool key_down, uint16_t code);

void register_key_to_repeat(uint16_t keycode) {

    // fix me
    // Get the base keycode of a mod or layer tap key
    /* switch (keycode) { */
    /*     case QK_MOD_TAP ... QK_MOD_TAP_MAX: */
    /*     case QK_LAYER_TAP ... QK_LAYER_TAP_MAX: */
    /*         keycode = keycode & 0xFF; */
    /*         break; */
    /* } */

    // Merge current mod state with keycode, for easy comparison when
    // we want to do special key reverse repeats.
    /* uint8_t mods = get_mods(); */
    /* if (mods & MOD_MASK_CTRL) keycode |= QK_LCTL; */
    /* if (mods & MOD_MASK_SHIFT) keycode |= QK_LSFT; */
    /* if (mods & MOD_MASK_GUI) keycode |= QK_LGUI; */
    /* if (mods & MOD_BIT(KC_LALT)) keycode |= QK_LALT; */
    /* if (mods & MOD_BIT(KC_RALT)) keycode |= QK_RALT; */

    last_keycode = keycode;
}

void update_key(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        register_code16(keycode);
    } else {
        unregister_code16(keycode);
    }
}

void update_repeat_key(keyrecord_t *record) {
  update_key(last_keycode, record);
}
