VIA_ENABLE = no
COMBO_ENABLE = yes
CAPS_WORD_ENABLE = yes
OS_DETECTION_ENABLE = yes
REPEAT_KEY_ENABLE = yes
SEND_STRING_ENABLE = yes
ifneq ($(strip $(NO_SECRETS)), yes)
    ifneq ("$(wildcard $(USER_PATH)/secrets.c)","")
        SRC += secrets.c
    endif
endif
