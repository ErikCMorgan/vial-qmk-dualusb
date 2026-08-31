# vial-qmk-dualusb

A fork of [svalboard/vial-qmk](https://github.com/svalboard/vial-qmk) adding the **`dualusb`** keymap: Svalboard firmware where **each half gets its own USB cable**.

The left half sends the keyboard. The right half sends all the pointer data for both trackballs, straight to the PC.

```
LEFT half  ──USB──> chording dongle / KVM ──> PC      keys
   │
   └── split cable (pointer data crosses here) ──┐
                                                 ▼
RIGHT half ──USB───────────────────────────────> PC      mouse
```

This exists because the [CharaChorder X](https://www.charachorder.com/) chording dongle can't pass a Svalboard: stock firmware sends nothing through it (NKRO routes keypresses to an endpoint the dongle never reads), and its pointer handling transposes X/Y and drops scroll entirely — with any mouse. Sending the keys down one cable and the pointer down another sidesteps both.

Also adds **sprint keys** (`SV_SPRINT_2/3/5`) — hold for 2×/3×/5× pointer speed, the mirror of the existing sniper keys.

## → [Full documentation](keyboards/svalboard/keymaps/dualusb/README.md)

Setup steps, quirks, and build instructions.

## → [Releases](../../releases)

Prebuilt `.uf2` files for Svalboard "lightly" with PMW3389 trackballs.

---

Everything else in this repository is upstream vial-qmk, unmodified. GPL v2.

Original QMK readme: [docs.qmk.fm](https://docs.qmk.fm)
