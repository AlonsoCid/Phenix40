// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

/* --- Enums & Typedefs --- */

// Custom keycodes for bypassing US-International dead keys
enum custom_keycodes {
    TILDE_INSTANT = SAFE_RANGE,
    DQUO_INSTANT,
};

// Tap Dance IDs
enum {
    TD_A = 0,
    TD_E,
    TD_I,
    TD_O,
    TD_U,
    TD_SLSH,
    TD_P
};

// Tap Dance States
typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_DOUBLE_TAP,
    TD_HOLD
} td_state_t;

static td_state_t cur_td_state;

/* --- Macro Logic --- */

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case TILDE_INSTANT:
                SEND_STRING(SS_LSFT("`") " ");
                return false;
            case DQUO_INSTANT:
                SEND_STRING(SS_LSFT("'") " ");
                return false;
        }
    }
    return true;
}

/* --- Tap Dance Logic --- */

// Determine the type of tap dance performed
td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
        return TD_HOLD;
    } else if (state->count == 2) {
        return TD_DOUBLE_TAP;
    }
    return TD_UNKNOWN;
}

// Handler functions for vowel accents
void a_finished(tap_dance_state_t *state, void *user_data) {
    cur_td_state = cur_dance(state);
    switch (cur_td_state) {
        case TD_SINGLE_TAP: tap_code(KC_A); break;
        case TD_DOUBLE_TAP: tap_code16(RALT(KC_A)); break; // á
        case TD_HOLD: tap_code16(RALT(KC_Q)); break;       // ä
        default: break;
    }
}

void e_finished(tap_dance_state_t *state, void *user_data) {
    cur_td_state = cur_dance(state);
    switch (cur_td_state) {
        case TD_SINGLE_TAP: tap_code(KC_E); break;
        case TD_DOUBLE_TAP: tap_code16(RALT(KC_E)); break; // é
        case TD_HOLD: SEND_STRING(SS_LSFT("'") "e"); break;// ë
        default: break;
    }
}

void i_finished(tap_dance_state_t *state, void *user_data) {
    cur_td_state = cur_dance(state);
    switch (cur_td_state) {
        case TD_SINGLE_TAP: tap_code(KC_I); break;
        case TD_DOUBLE_TAP: tap_code16(RALT(KC_I)); break; // í
        case TD_HOLD: SEND_STRING(SS_LSFT("'") "i"); break;// ï
        default: break;
    }
}

void o_finished(tap_dance_state_t *state, void *user_data) {
    cur_td_state = cur_dance(state);
    switch (cur_td_state) {
        case TD_SINGLE_TAP: tap_code(KC_O); break;
        case TD_DOUBLE_TAP: tap_code16(RALT(KC_O)); break; // ó
        case TD_HOLD: tap_code16(RALT(KC_P)); break;       // ö
        default: break;
    }
}

void u_finished(tap_dance_state_t *state, void *user_data) {
    cur_td_state = cur_dance(state);
    switch (cur_td_state) {
        case TD_SINGLE_TAP: tap_code(KC_U); break;
        case TD_DOUBLE_TAP: tap_code16(RALT(KC_U)); break; // ú
        case TD_HOLD: tap_code16(RALT(KC_Y)); break;       // ü
        default: break;
    }
}

void slsh_finished(tap_dance_state_t *state, void *user_data) {
    cur_td_state = cur_dance(state);
    switch (cur_td_state) {
        case TD_SINGLE_TAP: 
            tap_code(KC_SLSH); 
            break;
        case TD_HOLD: 
            // In US_Intl, Shift + Minus is the underscore
            tap_code16(KC_UNDS); 
            break;
        default: break;
    }
}

void p_finished(tap_dance_state_t *state, void *user_data) {
    cur_td_state = cur_dance(state);
    switch (cur_td_state) {
        case TD_SINGLE_TAP: 
            tap_code(KC_P); 
            break;
        case TD_HOLD: 
            // Sends ' followed by space to bypass US-Intl dead key
            SEND_STRING("' "); 
            break;
        default: break;
    }
}

/* --- Action Definitions --- */

tap_dance_action_t tap_dance_actions[] = {
    [TD_A] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, a_finished, NULL),
    [TD_E] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, e_finished, NULL),
    [TD_I] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, i_finished, NULL),
    [TD_O] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, o_finished, NULL),
    [TD_U] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, u_finished, NULL),
    [TD_SLSH] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, slsh_finished, NULL),
    [TD_P]    = ACTION_TAP_DANCE_FN_ADVANCED(NULL, p_finished, NULL),
};

enum layers { _BASE = 0, _MOD1, _MOD2 };

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        KC_TAB,  KC_Q,     KC_W,         TD(TD_E),     KC_R,         KC_T,      KC_Y,      TD(TD_U),     TD(TD_I),     TD(TD_O),     TD(TD_P),    KC_DEL,
        KC_ESC,  TD(TD_A), LCTL_T(KC_S), LGUI_T(KC_D), LALT_T(KC_F), KC_G,      KC_H,      RALT_T(KC_J), RGUI_T(KC_K), RCTL_T(KC_L), KC_SCLN,     KC_ENT,
        KC_LSFT, KC_Z,     KC_X,         KC_C,         KC_V,         KC_B,      KC_N,      KC_M,         KC_COMM,      KC_DOT,       TD(TD_SLSH), KC_RSFT,
                                                       KC_SPC,       MO(_MOD2), MO(_MOD1), KC_BSPC
    ),

    [_MOD1] = LAYOUT(
        RALT(LSFT(KC_SCLN)), KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_PSCR,
        _______,             KC_PPLS, KC_MINS, KC_PAST, KC_LPRN, KC_RPRN, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_PEQL, _______,
        LSFT_T(KC_F12),      KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  RSFT_T(KC_F11),
                                                        _______, KC_LGUI, _______, _______
    ),

    [_MOD2] = LAYOUT(
        TILDE_INSTANT, KC_EXLM, KC_AT,      KC_HASH,       RALT(KC_5), KC_PERC, KC_CIRC,    KC_AMPR, KC_DLR,  _______, DQUO_INSTANT, KC_PSCR,
        _______,       _______, RALT(KC_S), _______,       KC_LBRC,    KC_RBRC, KC_BRID,    KC_VOLD, KC_VOLU, KC_BRIU, KC_GRV,       _______,
        _______,       _______, _______,    RALT(KC_COMM), KC_LCBR,    KC_RCBR, RALT(KC_N), KC_MUTE, _______, KC_BSLS, KC_MINS,      _______,
                                                           _______,    _______, _______,    _______
    )
};