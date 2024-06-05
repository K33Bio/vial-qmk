/* K33B - ergonomic keyboards - https://k33b.com
 * maintainer: Matija Golub (PartWizard) <contact@k33b.com>
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

enum layers {
	_QWERTY,
	_RAISE,
	_LOWER,
	_CUSTOM,
	_CONFIG,
};

#define RAISE MO(_RAISE)
#define LOWER MO(_LOWER)

// COMBO implementation (Vial doesn't allow preconfigured COMBOS)
#if defined(TRIPPLE_TAP_CONF_ENABLE) || defined(DOUBLE_TAP_LAYER_ENABLE)

#define KEY_COMBO_1 RAISE
#define KEY_COMBO_2 LOWER

#ifdef TRIPPLE_TAP_CONF_ENABLE

#define TIMEOUT_BETWEEN_KEYS 50
#define TIMEOUT_BETWEEN_COMBO 200

static uint16_t timer_between_keys = 0;
static uint16_t timer_between_combo = 0;
static bool released_combo = true;
static uint8_t count_combo = 0;
static bool key_combo_1_pressed = false;
static bool key_combo_2_pressed = false;

#endif

// TAP DANCE vars
#ifdef DOUBLE_TAP_LAYER_ENABLE
static uint16_t timer_double_tap_1 = 0;
static uint8_t double_tap_key_1 = 0;
static bool held_down_key_1 = false;
static bool layer_1_on = false;

static uint16_t timer_double_tap_2 = 0;
static uint8_t double_tap_key_2 = 0;
static bool held_down_key_2 = false;
static bool layer_2_on = false;
#endif

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
	#ifdef TRIPPLE_TAP_CONF_ENABLE

	if ( keycode == KEY_COMBO_1) {
		key_combo_1_pressed = record->event.pressed;
	}
	if ( keycode == KEY_COMBO_2 ) {
		key_combo_2_pressed = record->event.pressed;
	}

	if (key_combo_1_pressed ^ key_combo_2_pressed) {
		timer_between_keys = record->event.time + TIMEOUT_BETWEEN_KEYS;
	}

	if (key_combo_1_pressed && key_combo_2_pressed && !timer_expired(timer_read(), timer_between_keys) && released_combo) {
		if (timer_expired(timer_read(), timer_between_combo)){
			count_combo = 0;
		}
		released_combo = false;
		count_combo++;
		timer_between_combo = timer_read() + TIMEOUT_BETWEEN_COMBO;
		
		if (count_combo == 3) {
			count_combo = 0;
			layer_invert(_CONFIG);
			return false;
		}
	}

	if (!key_combo_1_pressed && !key_combo_2_pressed) {
		released_combo = true;
	}

	#endif

	#ifdef DOUBLE_TAP_LAYER_ENABLE
	// TAP DANCE code
	if ( keycode == KEY_COMBO_1 && ((keycode == KEY_COMBO_1) ^ (keycode != KEY_COMBO_1)) && record->event.pressed ) {
		held_down_key_1 = true;
		double_tap_key_1++;
		if(double_tap_key_1 == 1) {
			timer_double_tap_1 = record->event.time + 200;
		}
		if(double_tap_key_1 == 2){
			layer_1_on = !layer_1_on;
			double_tap_key_1 = 0;
		}
	}

	if ( keycode == KEY_COMBO_2 && ((keycode == KEY_COMBO_2) ^ (keycode != KEY_COMBO_2)) && record->event.pressed ) {
		held_down_key_2 = true;
		double_tap_key_2++;
		if(double_tap_key_2 == 1) {
			timer_double_tap_2 = record->event.time + 200;
		}
		if(double_tap_key_2 == 2){
			layer_2_on = !layer_2_on;
			double_tap_key_2 = 0;
		}
	}

	#endif
	
	return true;
}

#ifdef DOUBLE_TAP_LAYER_ENABLE

void matrix_scan_user(void) {
  if(timer_expired(timer_read(), timer_double_tap_1) && (double_tap_key_1 > 0)){
		double_tap_key_1 = 0;
	}

	if(held_down_key_1){
		if(layer_state_is(_RAISE) && !layer_1_on){
		} else {
			held_down_key_1 = false;
		}
	}

	if(layer_1_on && !held_down_key_1 && !layer_state_is(_RAISE)){
		layer_on(_RAISE);
	}
	if(!layer_1_on && !held_down_key_1 && layer_state_is(_RAISE)) {
		layer_off(_RAISE);
	}

	if(timer_expired(timer_read(), timer_double_tap_2) && (double_tap_key_2 > 0)){
		double_tap_key_2 = 0;
	}

	if(held_down_key_2){
		if(layer_state_is(_LOWER) && !layer_2_on){
		} else {
			held_down_key_2 = false;
		}
	}

	if(layer_2_on && !held_down_key_2 && !layer_state_is(_LOWER)){
		layer_on(_LOWER);
	}
	if(!layer_2_on && !held_down_key_2 && layer_state_is(_LOWER)) {
		layer_off(_LOWER);
	}

}

#endif

#endif
// END TAP DANCE code


// END COMBO implementation

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[_QWERTY] = LAYOUT(
		QK_GESC,KC_1,		KC_2,		KC_3,		KC_4,		KC_5,																					KC_6,		KC_7,		KC_8,		KC_9,		KC_0,		KC_BSPC,
		KC_TAB,	KC_Q,		KC_W,		KC_E,		KC_R,		KC_T,																					KC_Y,		KC_U,		KC_I,		KC_O,		KC_P,		KC_MINS,
		KC_LSFT,KC_A,		KC_S,		KC_D,		KC_F,		KC_G,																					KC_H,		KC_J,		KC_K,		KC_L,		KC_SCLN,KC_QUOT,
		KC_LCTL,KC_Z,		KC_X,		KC_C,		KC_V,		KC_B,																					KC_N,		KC_M,		KC_COMM,KC_DOT,	KC_SLSH,KC_BSLS,
										KC_LBRC,KC_RBRC,				RAISE,	KC_SPC,	KC_DEL,				KC_BSPC,KC_ENT,	LOWER,					KC_RGUI,KC_EQL,
																										KC_LGUI,KC_HOME,			KC_END,	KC_LALT,
	),

	[_RAISE] = LAYOUT(
		KC_F1,	KC_F2,	KC_F3,	KC_F4,	KC_F5,	KC_F6,																				KC_F7,	KC_F8,	KC_F9,	KC_F10,	KC_F11,	KC_F12,
		_______,_______,_______,KC_UP,	_______,KC_LBRC,																			KC_RBRC,KC_BTN1,KC_MS_U,KC_BTN2,_______,_______,
		_______,_______,KC_LEFT,KC_DOWN,KC_RGHT,KC_LPRN,																			KC_RPRN,KC_MS_L,KC_MS_D,KC_MS_R,_______,_______,
		_______,_______,_______,_______,_______,KC_LABK,																			KC_RABK,KC_WH_L,KC_WH_U,KC_WH_R,_______,_______,
										_______,_______,				_______,_______,_______,			_______,_______,_______,				KC_WH_D,_______,					
																										_______,_______,			_______,_______
	),

	[_LOWER] = LAYOUT(
		_______,_______,KC_PSCR,KC_SCRL,KC_PAUS,_______,																			_______,KC_NUM,	KC_PSLS,KC_PAST,KC_PMNS,_______,
		_______,KC_MUTE,KC_INS,	KC_HOME,KC_PGDN,KC_LBRC,																			KC_RBRC,KC_P7,	KC_P8,	KC_P9,	KC_PPLS,_______,
		_______,KC_VOLU,KC_DEL,	KC_END,	KC_PGUP,KC_LPRN,																			KC_RPRN,KC_P4,	KC_P5,	KC_P6,	KC_PEQL,_______,
		_______,KC_VOLD,KC_MPRV,KC_MPLY,KC_MNXT,KC_LABK,																			KC_RABK,KC_P1,	KC_P2,	KC_P3,	KC_PENT,_______,
										KC_MRWD,KC_MFFD,				_______,_______,_______,			_______,_______,_______,				KC_P0,	KC_PDOT,
																										_______,_______,			_______,_______
	),

	[_CUSTOM] = LAYOUT(
		_______,_______,_______,_______,_______,_______,																			_______,_______,_______,_______,_______,_______,
		_______,_______,_______,_______,_______,_______,																			_______,_______,_______,_______,_______,_______,
		_______,_______,_______,_______,_______,_______,																			_______,_______,_______,_______,_______,_______,
		_______,_______,_______,_______,_______,_______,																			_______,_______,_______,_______,_______,_______,
										_______,_______,				_______,_______,_______,			_______,_______,_______,				_______,_______,
																										_______,_______,			_______,_______,
	),

	[_CONFIG] = LAYOUT(
		QK_BOOT,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,																			XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,
		XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,																			XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,
		XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,																			XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,
		XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,																			CW_TOGG,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,
										XXXXXXX,XXXXXXX,				_______,AU_TOGG,AU_NEXT,			CK_UP,	CK_TOGG,_______,				XXXXXXX,XXXXXXX,
																										MU_TOGG,MU_NEXT,			CK_DOWN,CK_RST
	),
};

const uint8_t music_map[MATRIX_ROWS][MATRIX_COLS] = LAYOUT(
		37,	38,	39,	40,	41,	42,						43,	44,	45,	46,	47,	48,
		25,	26,	27,	28,	29,	30,						31,	32,	33,	34,	35,	36,
		13,	14,	15,	16,	17,	18,						19,	20,	21,	22,	23,	24,
		0,	1,	2,	3,	4,	5,						6,	7,	9,	10,	11,	12,
						0,	0,														0,  0,
										0,	0,						0,  0,
												0,	0,		0,  0,
												0,	0,		0,  0
);