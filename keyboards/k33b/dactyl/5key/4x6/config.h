#pragma once

// Left half of keyboard defined with a pin
#define SPLIT_HAND_PIN GP27

// Serial protocol to use, specific to RP2040
#define SERIAL_PIO_USE_PIO1

// MIDI support
#define MIDI_ADVANCED

// Number of layers
#define DYNAMIC_KEYMAP_LAYER_COUNT 5

// Triple tap RAISE and LOWER key to access _CONF layer
#define TRIPPLE_TAP_CONF_ENABLE
// Double tap RAISE or LOWER to enable _RAISE or _LOWER layer
//#define DOUBLE_TAP_LAYER_ENABLE

// Mouse keys settings
#define MOUSEKEY_INTERVAL 16
#define MOUSEKEY_MOVE_DELTA 5
#define MOUSEKEY_TIME_TO_MAX 80

// Audio configuration
#ifdef AUDIO_ENABLE

#define AUDIO_PIN GP16
#define AUDIO_PWM_DRIVER PWMD0
#define AUDIO_PWM_CHANNEL RP2040_PWM_CHANNEL_A

#define AUDIO_INIT_DELAY

#define AUDIO_CLICKY
#define AUDIO_CLICKY_FREQ_MAX 14000.0f
#define AUDIO_CLICKY_FREQ_RANDOMNESS 0.1f

#define AUDIO_VOICES

#define MUSIC_MAP
#define MUSIC_MASK keycode != QK_MUSIC_TOGGLE

#endif // AUDIO_ENABLE
