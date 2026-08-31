Svalboard (Vial-QMK, dual trackballs) works fine plugged direct. Through the CCX it powers up but nothing enumerates: LEDs on, no keys, no Vial.
CharaChorder X S2, CCOS 3.0.0 / v2.8.0 · Svalboard "lightly" (RP2040, vial-qmk) · Win11

**1. Composite HID keyboards fail**

Stock descriptor: `bDeviceClass 0xEF` (IAD), 6 interfaces, 9 endpoints, 500mA
```
0   boot keyboard (subclass/protocol 01)
1   raw HID (Vial, IN+OUT)
2   shared HID 223B (mouse+consumer+NKRO)
3   console HID
4/5 CDC ACM
```
Rebuilt to one boot-keyboard iface (1 EP, 100mA, `bDeviceClass 0x00`) → **works, chording included!**
Re-added Vial raw HID + boot-mouse iface (3 ifaces, 4 EPs, NKRO/CDC/console/shared EP off) → **still works** — extra HID interfaces aren't the problem.

Mechanism: QMK defaults to `USB_PROTOCOL_REPORT`. Without `SET_PROTOCOL(Boot)` it sends keys as NKRO reports on the *shared* endpoint while the boot-keyboard endpoint stays silent — the host sees a powered device sending nothing.

**Fix:** issue `SET_PROTOCOL(Boot)` on `bInterfaceSubClass 0x01` interfaces — likely fixes most QMK/Vial boards as-is. Also skip unknown functions (CDC) rather than failing.

*Caveat: several changes at once — haven't isolated which.*

**2. Mouse passthrough broken for every mouse tried**

Office mouse (3-byte boot mouse), a higher-end mouse (no input at all), and both Svalboard trackballs on a boot-mouse iface — all fail.

Cursor mapping is deterministic:
```
down  -> right
right -> down
left  -> right
up    -> down
```
X/Y are transposed and negative deltas produce positive motion — consistent with a wrong byte offset and/or signed int8 read as unsigned.

Scroll never works. Confirmed by swapping both trackballs' modes: symptoms follow the mode, not the side. Boot mouse protocol is 3 bytes (buttons, X, Y) with **no wheel** — if the CCX binds in boot protocol, scroll cannot work by design.

**Fix:** parse the report descriptor instead of assuming a fixed boot-mouse layout.
