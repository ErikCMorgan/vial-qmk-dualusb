# dualusb — Svalboard with both halves on USB

Svalboard firmware where **each half gets its own USB cable**. The left half sends the keyboard; the right half sends all the pointer data for both trackballs.

```
LEFT half  ──USB──> chording dongle / KVM ──> PC      keys
   │
   └── split cable (pointer data crosses here) ──┐
                                                 ▼
RIGHT half ──USB───────────────────────────────> PC      mouse
```

Both cables go to the same computer. The split cable stays connected as normal — nothing about the physical build changes.

## Why

The [CharaChorder X](https://www.charachorder.com/) chording dongle sits between your keyboard and the PC, and it can't handle the Svalboard:

- **Stock firmware sends nothing through it.** QMK defaults to report protocol, so with NKRO enabled it routes keypresses to a shared endpoint the dongle never reads. Lights on, no keys.
- **Its pointer handling is broken anyway.** Through the dongle, X and Y come out transposed, negative motion reads as positive, and scroll is dropped entirely — with *any* mouse, including a plain wired office mouse.

The second problem can't be fixed from the keyboard side. So this firmware sends the keys down one cable and the pointer down another, and the dongle never touches the trackballs.

Useful for anything else in that path too — KVMs, KM switches, macro boxes — that passes keys but mangles pointer data.

## What you get

- Chording (or whatever your inline device does) on the full keyboard
- Both trackballs, scroll, and mouse buttons, clean and direct to the PC
- Sniper keys, auto-mouse layer, DPI keys, RGB layer colors, Vial — all working
- **New: sprint keys** (below)

## Sprint keys

`SV_SPRINT_2` / `SV_SPRINT_3` / `SV_SPRINT_5` — hold to move the pointer at **2× / 3× / 5× speed**. The mirror of the existing sniper keys, which slow it down.

They share the same scaling accumulators as sniper, so:
- Holding both composes — sniper-3 + sprint-2 gives 2/3 speed
- The fractional remainder carries between reports instead of truncating, so slow movement stays smooth
- They affect scroll as well as cursor movement

Assign them in Vial; they appear as **Sprint 2x / 3x / 5x**.

## Setup

**1. Flash both halves.** Grab the two `.uf2` files from [Releases](../../../../releases), or build them (below).

Double-tap reset on a half, copy the matching file to the `RPI-RP2` drive that appears, repeat for the other half.

> **Flash both halves from the same build.** They share a wire format over the split cable. Mixing builds leaves you with a keyboard that enumerates but does nothing.

**2. Plug in the split cable** between the two halves, as normal.

**3. Plug the LEFT half's `U` port** into your chording dongle (or KVM, or straight to the PC). This is the keyboard.

**4. Plug the RIGHT half's `U` port** straight into the PC. This is the mouse.

That's it. Windows will show a keyboard and a separate mouse device (`Svalboard Pointer`, PID `0x4045`).

### Power note

The split cable's VCC sits on the USB rail, so plugging in both halves ties two host ports' 5 V rails together through it. The Svalboard designer confirms this isn't a tested configuration.

In practice it has run without incident on a single host with ordinary cables. If you'd rather be careful, use a cable with the **VBUS wire cut** on the right half — the RP2040 never reads VBUS (`RP_USB_FORCE_VBUS_DETECT` is `TRUE`), so a data-only cable enumerates fine and that half draws power through the split cable as it already does. Easiest build is a cheap USB-A→C cable with the red wire cut.

## Quirks

**Vial only reaches the left half, plugged directly into the PC.** A chording dongle presents its own VID/PID and doesn't tunnel raw HID, so Vial can't see the keyboard through one. To reconfigure: unplug the left half from the dongle, plug it straight into the PC, make your changes, then put it back. The right half has no Vial interface at all.

**The right half alone is not a keyboard.** Split roles are fixed at compile time, so the right half on its own is a slave with no master — the trackball moves the cursor, but no keys work. Both halves and the split cable are required.

**Some stock features are off**, because they break the dongle or aren't needed here:

| Disabled | Reason |
|---|---|
| NKRO | The actual cause of stock firmware not working through the dongle |
| Media / consumer keys | Adds a USB interface to the half going through the dongle |
| Steno, CDC serial, console | Extra USB interfaces, unused |

Media keys are the only real loss. They can be re-enabled at the cost of retesting dongle compatibility.

**Tested on:** Windows 11, Svalboard "lightly" (RP2040), PMW3389 trackballs both halves. Not tested on macOS or Linux, or with trackpoint / Azoteq / Pimoroni pointing devices.

## Building

```sh
make svalboard/trackball/pmw3389/left:dualusb
make svalboard/trackball/pmw3389/right:dualusb
```

## How it works

QMK decides split master/slave by which half enumerates over USB first, then calls `usb_disconnect()` on the loser — which is why a normal split slave has no USB. Overriding the weak `is_keyboard_master_impl()` to pin the role by handedness skips that call, and the rest of QMK's USB stack already runs unconditionally on both halves.

`#undef SPLIT_POINTING_ENABLE` removes the master-only gate in `pointing_device_task()`, so each half polls its own sensor. The master applies sniper/sprint scaling and scroll conversion to its own ball, then pushes the result — plus the policy state the slave can't know, since only the master processes keycodes — over a custom split transaction each loop. The slave merges that with its own ball, runs the scroll accumulate/flush, and emits everything on its own USB. One byte comes back the other way so the master knows when the slave saw motion, which drives the auto-mouse layer.

### If you modify this

- **Both halves must agree on the split transaction enum.** `NUM_TOTAL_TRANSACTIONS` is XORed into the serial handshake, so if a feature flag shifts the table on one half only, *every* transaction fails — including matrix sync — and both halves go dead with no diagnostic. Enabling RGB on just one half is enough. `_Static_assert`s in `keymap.c` pin this.
- **Nothing whose size depends on a build flag may cross the wire.** `report_mouse_t` changes size with `MOUSE_SHARED_EP`, `MOUSE_EXTENDED_REPORT` and `WHEEL_EXTENDED_REPORT` — which is why the sync struct uses explicit fixed-width fields.
- Per-side config keys off `-DINIT_EE_HANDS_RIGHT` in `OPT_DEFS`, with `#error` guards that fail the build if that detection stops working.

## Credits

Fork of [svalboard/vial-qmk](https://github.com/svalboard/vial-qmk). GPL v2, same as upstream.

USB-level analysis of the CharaChorder X behaviour: [docs/charachorder-x-findings.md](docs/charachorder-x-findings.md).
