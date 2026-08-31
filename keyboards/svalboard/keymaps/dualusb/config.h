/* SPDX-License-Identifier: GPL-2.0-or-later */
#pragma once

#define STENO_COMBINEDMAP

#define VIAL_KEYBOARD_UID {0x1B, 0x18, 0x7D, 0xF2, 0x21, 0xF6, 0x29, 0x48}

// Vial security combos, depending on which unit this is...
#ifdef INIT_EE_HANDS_RIGHT
// right thumb lock
#define VIAL_UNLOCK_COMBO_ROWS { 5, 5 }
#define VIAL_UNLOCK_COMBO_COLS { 0, 1 }
#elif INIT_EE_HANDS_LEFT
// left thumb lock
#define VIAL_UNLOCK_COMBO_ROWS { 0, 0 }
#define VIAL_UNLOCK_COMBO_COLS { 0, 1 }
#else
// both thumb locks
#define VIAL_UNLOCK_COMBO_ROWS { 0, 0, 5, 5 }
#define VIAL_UNLOCK_COMBO_COLS { 2, 5, 2, 5 }
#endif

// Shorten the unlock timeout (needs mod in `quantum/vial.c`; without
// it the override doesn't work)
#define VIAL_UNLOCK_COUNTER_MAX 12

#undef OS_DETECTION_KEYBOARD_RESET



#undef SPLIT_POINTING_ENABLE
#undef POINTING_DEVICE_COMBINED

#undef SPLIT_WATCHDOG_ENABLE

#define SVAL_DUALUSB

#undef SPLIT_TRANSACTION_IDS_KB
#define SPLIT_TRANSACTION_IDS_KB KEYBOARD_SYNC_A, SVAL_POINTER_SYNC

#ifdef INIT_EE_HANDS_RIGHT
#    undef PRODUCT_ID
#    define PRODUCT_ID 0x4045
#    undef PRODUCT
#    define PRODUCT "Svalboard Pointer"
#endif

#if !defined(INIT_EE_HANDS_LEFT) && !defined(INIT_EE_HANDS_RIGHT)
#    error "dualusb must be built from a .../left or .../right target"
#endif
#if defined(INIT_EE_HANDS_LEFT) && defined(INIT_EE_HANDS_RIGHT)
#    error "both handedness defines set"
#endif
#ifdef SPLIT_POINTING_ENABLE
#    error "SPLIT_POINTING_ENABLE puts report_mouse_t on the wire; its size differs with MOUSE_SHARED_EP"
#endif
#ifdef SPLIT_TRANSACTION_IDS_USER
#    error "user transaction ids would shift NUM_TOTAL_TRANSACTIONS"
#endif

#if defined(INIT_EE_HANDS_RIGHT) && !defined(MOUSE_SHARED_EP)
#    error "rules.mk handedness detection failed: right half must build MOUSE_SHARED_EP=yes"
#endif
#if defined(INIT_EE_HANDS_RIGHT) && (defined(VIA_ENABLE) || defined(VIAL_ENABLE) || defined(RAW_ENABLE))
#    error "rules.mk handedness detection failed: right half must not build VIA/VIAL/RAW"
#endif
#if defined(INIT_EE_HANDS_LEFT) && defined(MOUSE_SHARED_EP)
#    error "left half must build MOUSE_SHARED_EP=no"
#endif
