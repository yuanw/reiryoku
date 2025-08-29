#include QMK_KEYBOARD_H

#if (__has_include("secrets.h") && !defined(NO_SECRETS))
#    include "secrets.h"
#else
const char secret_0[] PROGMEM = "test1";
#endif

static const char * const secrets[] PROGMEM = {
    secret_0,
};

bool process_record_secrets(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case KC_SECRET_1: // Secrets!  Externally defined strings, not stored in repo
      if (record->event.pressed) {
        clear_mods();
        clear_oneshot_mods();
        send_string_with_delay_P(secrets[keycode - KC_SECRET_1], MACRO_TIMER);
      }
      return false;
      break;
  }
  return true;
}
