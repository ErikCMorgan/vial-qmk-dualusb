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

#include "../keymap_support.c"
#include "keycodes.h"
#include "quantum_keycodes.h"
#include QMK_KEYBOARD_H
#include <stdbool.h>
#include <stdint.h>
#include "svalboard.h"
#include "transactions.h"
#include "split_util.h"

layer_state_t default_layer_state_set_user(layer_state_t state) {
  sval_set_active_layer(0, false);
  return state;
}

layer_state_t layer_state_set_user(layer_state_t state) {
  sval_set_active_layer(get_highest_layer(state), false);
  return state;
}

enum layer {
    NORMAL,
    NAVNAS,
    FUNC,
    BOARD_CONFIG = MH_AUTO_BUTTONS_LAYER - 1,
    MBO = MH_AUTO_BUTTONS_LAYER,
};

#if __has_include("keymap_all.h")
#include "keymap_all.h"
#else
int sval_macro_size = 0;
uint8_t sval_macros[] = {0};
const uint16_t PROGMEM keymaps[DYNAMIC_KEYMAP_LAYER_COUNT][MATRIX_ROWS][MATRIX_COLS] = {
    /* ===== NORMAL ===== .*/
    [NORMAL] = LAYOUT(
        /*     Center            North               East                South             West                Double*/
        /*R1*/ KC_J              , KC_U              , KC_QUOTE          , KC_M            , KC_H              , KC_NO           ,
        /*R2*/ KC_K              , KC_I              , KC_COLON          , KC_COMMA        , KC_Y              , KC_NO             ,
        /*R3*/ KC_L              , KC_O              , LT(BOARD_CONFIG, KC_NO)  , KC_DOT   , KC_N              , KC_NO           ,
        /*R4*/ KC_SEMICOLON      , KC_P              , KC_BSLS           , KC_SLASH        , KC_RBRC           , KC_NO          ,
        /*L1*/ KC_F              , KC_R              , KC_G              , KC_V            , LSFT(KC_QUOTE)    , KC_NO            ,
        /*L2*/ KC_D              , KC_E              , KC_T              , KC_C            , KC_GRAVE          , KC_NO         ,
        /*L3*/ KC_S              , KC_W              , KC_B              , KC_X            , KC_ESCAPE         , KC_NO           ,
        /*L4*/ KC_A              , KC_Q              , KC_LBRC           , KC_Z            , KC_DELETE         , KC_NO           ,
        
        /*     Down                 Pad                Up                  Nail            Knuckle             DoubleDown*/
        /*RT*/ MO(NAVNAS)          , KC_SPACE        , KC_NO             , KC_BSPC         , KC_LALT           , MO(FUNC)             ,
        /*LT*/ KC_LSFT       , LT(NAVNAS, KC_ENTER)  , KC_NO             , LGUI_T(KC_TAB)  , KC_LCTL           , KC_CAPS       
        ),

    [NAVNAS] = LAYOUT(
        /*     Center            North               East               South             West              Double*/
        /*R1*/ KC_7              , LSFT(KC_7)        , LSFT(KC_6)       , KC_LEFT         , KC_6            , KC_NO           ,
        /*R2*/ KC_8              , LSFT(KC_8)        , KC_NO            , KC_UP           , LSFT(KC_MINUS)  , KC_NO           ,
        /*R3*/ KC_9              , LSFT(KC_9)        , KC_NO            , KC_DOWN         , KC_INSERT       , KC_NO           ,
        /*R4*/ KC_0              , LSFT(KC_0)        , KC_NO            , KC_RIGHT        , LSFT(KC_GRAVE)  , KC_NO           ,
        /*L1*/ KC_4              , LSFT(KC_4)        , KC_5             , KC_END          , LSFT(KC_5)      , KC_NO           ,
        /*L2*/ KC_3              , LSFT(KC_3)        , KC_MINUS         , KC_PGDN       , LSFT(KC_EQUAL)  , KC_NO           ,
        /*L3*/ KC_2              , LSFT(KC_2)        , KC_DOT           , KC_PGUP         , KC_TRNS         , KC_NO           ,
        /*L4*/ KC_1              , LSFT(KC_1)        , KC_EQUAL         , KC_HOME         , KC_TRNS         , KC_NO           ,
        /*Down             Pad             Up              Nail            Knuckle         DoubleDown*/
        /*RT*/ KC_TRNS           , KC_TRNS           , KC_TRNS          , KC_TRNS         , KC_TRNS         , KC_TRNS         ,
        /*LT*/ KC_TRNS           , KC_TRNS           , KC_TRNS          , KC_TRNS         , KC_TRNS         , KC_TRNS         
        ),
    
    /* ===== FUNCTION KEYS ===== */
    [FUNC] = LAYOUT(
        /*Center           North           East            South           West            Double*/
        /*R1*/ KC_F7            , KC_NO             , KC_F16           , KC_F17         , KC_F6           , KC_NO           ,
        /*R2*/ KC_F8            , KC_NO             , KC_NO            , KC_F18         , KC_NO           , KC_NO           ,
        /*R3*/ KC_F9            , KC_NO             , KC_NO            , KC_F19         , KC_NO           , KC_NO           ,
        /*R4*/ KC_F10           , KC_NO             , KC_NO            , KC_F20         , KC_NO           , KC_NO           ,
        /*L1*/ KC_F4            , KC_F24            , KC_F5            , KC_F14         , KC_F15          , KC_NO           ,
        /*L2*/ KC_F3            , KC_F23            , KC_F10           , KC_F13         , KC_NO           , KC_NO           ,
        /*L3*/ KC_F2            , KC_F22            , KC_NO            , KC_F12         , KC_NO           , KC_NO           ,
        /*L4*/ KC_F1            , KC_F21            , KC_NO            , KC_F11         , KC_NO           , KC_NO           ,
        /*Down             Pad             Up              Nail            Knuckle         DoubleDown*/
        /*RT*/ KC_TRNS          , KC_TRNS           , KC_TRNS          , KC_TRNS        , KC_TRNS         , KC_TRNS         ,
        /*LT*/ KC_TRNS          , KC_TRNS           , KC_TRNS          , KC_TRNS        , KC_TRNS         , KC_TRNS         
        ),

    [BOARD_CONFIG] = LAYOUT(
        /*         Center              North               East                South               West                (XXX)               */
        /*R1*/     KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_NO,
        /*R2*/     KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_NO,
        /*R3*/     KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_NO,
        /*R4*/     KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_NO,
        /*L1*/     SV_OUTPUT_STATUS,   KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_NO,
        /*L2*/     KC_TRNS,            SV_RIGHT_DPI_INC,   KC_TRNS,            SV_RIGHT_DPI_DEC,   KC_TRNS,            KC_NO,
        /*L3*/     KC_TRNS,            SV_LEFT_DPI_INC,    KC_TRNS,            SV_LEFT_DPI_DEC,    KC_TRNS,            KC_NO,
        /*L4*/     KC_TRNS,            KC_TRNS, KC_TRNS,   KC_TRNS,            KC_TRNS,            KC_NO,

        /*        Down                Pad                 Up                  Nail                Knuckle             Double Down         */
        /* RT */  KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,
        /* LT */  KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS
      ),

    /* ===== MBO ===== */
    [MBO] = LAYOUT(
        /*      Center           North               East                South                West                Double*/
        /*R1*/ KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_NO             ,
        /*R2*/ KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_NO             ,
        /*R3*/ KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_NO             ,
        /*R4*/ KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_NO             ,
        /*L1*/ KC_BTN1           , KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_NO             ,
        /*L2*/ KC_BTN3           , KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_NO             ,
        /*L3*/ KC_BTN2           , KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_NO             ,
        /*L4*/ KC_TRNS           , KC_TRNS           , KC_TRNS           , SV_SNIPER_3       , KC_TRNS           , KC_NO             ,
        
        /*     Down               Pad                Up                  Nail                Knuckle             DoubleDown */
        /*RT*/ KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_TRNS           ,
        /*LT*/ KC_TRNS           , KC_BTN1           , KC_TRNS           , KC_BTN2           , KC_TRNS           , KC_TRNS          
        ),
};
#endif

void sval_pointer_sync_handler(uint8_t in_len, const void *in_data, uint8_t out_len, void *out_data);

void keyboard_post_init_user(void) {
  transaction_register_rpc(SVAL_POINTER_SYNC, sval_pointer_sync_handler);
  // Customise these values if you need to debug the matrix
  //debug_enable=true;
  //debug_matrix=true;
  //debug_keyboard=true;
  //debug_mouse=true;

#if __has_include("keymap_all.h")
  if (fresh_install) {
    sval_init_defaults();
  }
#endif
}

bool is_keyboard_master_impl(void);

bool is_keyboard_master_impl(void) {
#ifdef INIT_EE_HANDS_LEFT
    return true;
#else
    return false;
#endif
}


#define SVAL_F_SLAVE_SCROLL (1 << 0)
#define SVAL_F_AXIS_LOCK    (1 << 1)
#define SVAL_F_IS_MAC       (1 << 2)
#define SVAL_F_SCALE2       (1 << 3)
#define SVAL_F_SCALE3       (1 << 4)
#define SVAL_F_SCALE5       (1 << 5)
#define SVAL_F_SPRINT2      (1 << 6)
#define SVAL_F_SPRINT3      (1 << 7)
#define SVAL_F_SPRINT5      (1 << 8)

#define SVAL_F_SCALE_MASK   (SVAL_F_SCALE2 | SVAL_F_SCALE3 | SVAL_F_SCALE5 | SVAL_F_SPRINT2 | SVAL_F_SPRINT3 | SVAL_F_SPRINT5)

#define SVAL_SYNC_STALE_MS 100

/* Wire format. Must NOT contain report_mouse_t: its size depends on
   MOUSE_SHARED_EP, which differs between the two halves. */
typedef struct {
    int16_t  x;
    int16_t  y;
    int16_t  h;
    int16_t  v;
    uint16_t flags;
    uint8_t  buttons;
    uint8_t  turbo_scan;
    uint16_t left_dpi;
    uint16_t right_dpi;
} sval_sync_t;

_Static_assert(sizeof(sval_sync_t) == 16, "sval_sync_t must be identical on both halves");
_Static_assert(GET_SLAVE_MATRIX_CHECKSUM == 0 && PUT_SYNC_TIMER == 2 &&
                   PUT_RGBLIGHT == 3 && PUT_RPC_INFO == 4 &&
                   KEYBOARD_SYNC_A == 8 && SVAL_POINTER_SYNC == 9 &&
                   NUM_TOTAL_TRANSACTIONS == 10,
               "split transaction enum drifted - a feature flag changed on one half");
_Static_assert(sizeof(presence_rpc_t) == 1, "presence_rpc_t layout changed");
_Static_assert(RPC_M2S_BUFFER_SIZE == 32 && RPC_S2M_BUFFER_SIZE == 32, "RPC buffer sizes must match on both halves");

__attribute__((unused)) static int16_t sval_clamp(int32_t v) {
    if (v > INT16_MAX) return INT16_MAX;
    if (v < INT16_MIN) return INT16_MIN;
    return (int16_t)v;
}

#ifdef INIT_EE_HANDS_LEFT
static sval_sync_t sync_out;
#else
static sval_sync_t sync_in;
static uint8_t     slave_motion;
static uint32_t    last_sync_ms;

static int32_t sval_norm(int16_t v, uint16_t dpi) {
    int32_t x = v;
    if (x > 21474) x = 21474;
    else if (x < -21474) x = -21474;
    return (x * 100000) / dpi;
}

static void sval_apply_scale(uint16_t flags) {
    static uint16_t applied = 0xFFFF;
    if (flags == applied) return;
    applied = flags;

    uint8_t d = 1;
    if (flags & SVAL_F_SCALE2) d *= 2;
    if (flags & SVAL_F_SCALE3) d *= 3;
    if (flags & SVAL_F_SCALE5) d *= 5;

    uint8_t m = 1;
    if (flags & SVAL_F_SPRINT2) m *= 2;
    if (flags & SVAL_F_SPRINT3) m *= 3;
    if (flags & SVAL_F_SPRINT5) m *= 5;

    set_div_axis(&sniper_x, d);
    set_div_axis(&sniper_y, d);
    set_div_axis(&sniper_h, d);
    set_div_axis(&sniper_v, d);
    set_mult_axis(&sniper_x, m);
    set_mult_axis(&sniper_y, m);
    set_mult_axis(&sniper_h, m);
    set_mult_axis(&sniper_v, m);
}

static void sval_apply_cpi(uint16_t cpi) {
    static uint16_t applied = 0;
    if (cpi == 0 || cpi == applied) return;
    applied = cpi;
    pointing_device_set_cpi(cpi);
}
#endif

void sval_pointer_sync_handler(uint8_t in_len, const void *in_data, uint8_t out_len, void *out_data) {
#ifdef INIT_EE_HANDS_LEFT
    (void)in_len;
    (void)in_data;
    (void)out_len;
    (void)out_data;
#else
    if (in_len == sizeof(sval_sync_t)) {
        const sval_sync_t *n = (const sval_sync_t *)in_data;

        sync_in.x = sval_clamp((int32_t)sync_in.x + n->x);
        sync_in.y = sval_clamp((int32_t)sync_in.y + n->y);
        sync_in.h = sval_clamp((int32_t)sync_in.h + n->h);
        sync_in.v = sval_clamp((int32_t)sync_in.v + n->v);

        sync_in.buttons    = n->buttons;
        sync_in.flags      = n->flags;
        sync_in.turbo_scan = n->turbo_scan;
        sync_in.left_dpi   = n->left_dpi;
        sync_in.right_dpi  = n->right_dpi;

        last_sync_ms = timer_read32();
    }
    if (out_data && out_len >= sizeof(uint8_t)) {
        *(uint8_t *)out_data = slave_motion;
        slave_motion         = 0;
    }
#endif
}

report_mouse_t pointing_device_task_user(report_mouse_t reportMouse) {
#ifdef INIT_EE_HANDS_LEFT
    if (enable_scale_2 || enable_scale_3 || enable_scale_5 || enable_sprint_2 || enable_sprint_3 || enable_sprint_5) {
        reportMouse.x = add_to_axis(&sniper_x, reportMouse.x);
        reportMouse.y = add_to_axis(&sniper_y, reportMouse.y);
        reportMouse.h = add_to_axis(&sniper_h, reportMouse.h);
        reportMouse.v = add_to_axis(&sniper_v, reportMouse.v);
    }

    if ((global_saved_values.left_scroll != scroll_hold) != scroll_toggle) {
        reportMouse.h = add_to_axis(&l_x, reportMouse.x);
        reportMouse.v = add_to_axis(&l_y, -reportMouse.y);
        reportMouse.x = 0;
        reportMouse.y = 0;
    }

    if (reportMouse.x || reportMouse.y || reportMouse.h || reportMouse.v) mouse_mode(true);

    if (!is_transport_connected()) return reportMouse;

    sync_out.x     = reportMouse.x;
    sync_out.y     = reportMouse.y;
    sync_out.h     = reportMouse.h;
    sync_out.v     = reportMouse.v;
    sync_out.flags = 0;
    if ((global_saved_values.right_scroll != scroll_hold) != scroll_toggle) sync_out.flags |= SVAL_F_SLAVE_SCROLL;
    if (global_saved_values.axis_scroll_lock) sync_out.flags |= SVAL_F_AXIS_LOCK;
    if (is_mac) sync_out.flags |= SVAL_F_IS_MAC;
    if (enable_scale_2) sync_out.flags |= SVAL_F_SCALE2;
    if (enable_scale_3) sync_out.flags |= SVAL_F_SCALE3;
    if (enable_scale_5) sync_out.flags |= SVAL_F_SCALE5;
    if (enable_sprint_2) sync_out.flags |= SVAL_F_SPRINT2;
    if (enable_sprint_3) sync_out.flags |= SVAL_F_SPRINT3;
    if (enable_sprint_5) sync_out.flags |= SVAL_F_SPRINT5;
    sync_out.turbo_scan = global_saved_values.turbo_scan;
    sync_out.left_dpi   = (uint16_t)get_left_dpi();
    sync_out.right_dpi  = (uint16_t)get_right_dpi();

    memset(&reportMouse, 0, sizeof(reportMouse));
    return reportMouse;
#else
    sval_sync_t in;
    bool        stale;

    chSysLock();
    memcpy(&in, &sync_in, sizeof(in));
    sync_in.x = sync_in.y = sync_in.h = sync_in.v = 0;
    stale = (last_sync_ms == 0) || (timer_elapsed32(last_sync_ms) > SVAL_SYNC_STALE_MS);
    if (stale) memset(&sync_in, 0, sizeof(sync_in));
    chSysUnlock();

    if (stale) {
        memset(&in, 0, sizeof(in));
    } else {
        global_saved_values.turbo_scan = (in.turbo_scan < 7) ? in.turbo_scan : 0;
        sval_apply_cpi(in.right_dpi);
        sval_apply_scale(in.flags & SVAL_F_SCALE_MASK);
    }

    if (in.left_dpi == 0) in.left_dpi = 1;
    if (in.right_dpi == 0) in.right_dpi = 1;

    if (in.flags & SVAL_F_SCALE_MASK) {
        reportMouse.x = add_to_axis(&sniper_x, reportMouse.x);
        reportMouse.y = add_to_axis(&sniper_y, reportMouse.y);
        reportMouse.h = add_to_axis(&sniper_h, reportMouse.h);
        reportMouse.v = add_to_axis(&sniper_v, reportMouse.v);
    }

    if (reportMouse.x || reportMouse.y) {
        chSysLock();
        slave_motion = 1;
        chSysUnlock();
    }

    if (in.flags & SVAL_F_SLAVE_SCROLL) {
        reportMouse.h = add_to_axis(&r_x, reportMouse.x);
        reportMouse.v = add_to_axis(&r_y, -reportMouse.y);
        reportMouse.x = 0;
        reportMouse.y = 0;
    }

    if ((reportMouse.h != 0 || reportMouse.v != 0 || in.h != 0 || in.v != 0) && !scroll_timer_running) {
        scroll_timer_running = true;
        scroll_timer         = timer_read();
    }

    if (scroll_timer_running) {
        m_scroll_accumulator_h += sval_norm(in.h, in.left_dpi);
        m_scroll_accumulator_v += sval_norm(in.v, in.left_dpi);
        m_scroll_accumulator_h += sval_norm(reportMouse.h, in.right_dpi);
        m_scroll_accumulator_v += sval_norm(reportMouse.v, in.right_dpi);

        scroll_accumulator_h += reportMouse.h + in.h;
        scroll_accumulator_v += reportMouse.v + in.v;
        reportMouse.h = in.h = 0;
        reportMouse.v = in.v = 0;
    }

    if (scroll_timer_running && timer_elapsed(scroll_timer) > SCROLL_FREQUENCY_MS) {
        if ((in.flags & SVAL_F_AXIS_LOCK) && !(in.flags & SVAL_F_IS_MAC)) {
            update_axis_scroll_mode(m_scroll_accumulator_h, m_scroll_accumulator_v);
            if (axis_scroll_mode == SV_AXIS_LOCKED_V) {
                reportMouse.v = scroll_accumulator_v;
                reportMouse.h = 0;
            } else {
                reportMouse.h = scroll_accumulator_h;
                reportMouse.v = 0;
            }
        } else {
            reportMouse.h = scroll_accumulator_h;
            reportMouse.v = scroll_accumulator_v;
        }

        scroll_timer_running   = false;
        scroll_accumulator_h   = 0;
        scroll_accumulator_v   = 0;
        m_scroll_accumulator_h = 0;
        m_scroll_accumulator_v = 0;
    }

    reportMouse.x       = sval_clamp((int32_t)reportMouse.x + in.x);
    reportMouse.y       = sval_clamp((int32_t)reportMouse.y + in.y);
    reportMouse.h       = sval_clamp((int32_t)reportMouse.h + in.h);
    reportMouse.v       = sval_clamp((int32_t)reportMouse.v + in.v);
    reportMouse.buttons = in.buttons;

    return reportMouse;
#endif
}

void housekeeping_task_user(void) {
#ifdef INIT_EE_HANDS_LEFT
    if (!is_transport_connected()) return;
#    ifdef MOUSEKEY_ENABLE
    sync_out.buttons = mousekey_get_report().buttons;
#    endif
    uint8_t motion = 0;
    if (transaction_rpc_exec(SVAL_POINTER_SYNC, sizeof(sync_out), &sync_out, sizeof(motion), &motion)) {
        if (motion) mouse_mode(true);
        sync_out.x = sync_out.y = sync_out.h = sync_out.v = 0;
    }
#endif
}
