# USB-C PD PSU
> [!WARNING]
> This is a WIP project. It is not yet recommended to reproduce it in its current state. 

A bidirectional buck-boost DC-DC converter with the following features:
- 240W USB PD EPR (48V 5A)*
- 5-60V input
- 3.3-60V output
- 20A maximum current (theoretically, depending on the thermal solution)
- Constant-current control
- `TI BQ25758A` DC-DC controller
- `WCH CH32X035` as the USB Type-C PD controller (hopefully*)
- Protection of CC and USB data lines up to 63V (`TI TPD4S480`)
- Rough mechanical compatibility with [FranzImsch's parkside-adapter](https://github.com/FranzImsch/parkside-adapter)
- A separate-ish "HMI"/"UI" part of the PCB containing:
  - `XT30` connectors for power in/output
  - 1.47" IPS display
  - Encoder + button controls
  - 1.27mm 2x06 expansion header with a few GPIOs / RS485 for potentially communicating with BMS's, etc.
  - A second `CH32X035`
  - Separable via rotary cutting tool (no mouse bites, because of the high current connections to the XT30 plugs and to not fall into the JLCPCB "two designs" price tier)
- 50x50mm PCB size (50x23mm without the "UI" part)

\* depending on the yet-to-be-written PD firmware (and an actually functioning DC-DC converter layout :P)

You can follow the [Mastodon thread](https://chaos.social/@LeoDJ/113874922536756547) for more up-to-date progress on this project.

<img src="doc/USB-C%20PSU%20corner.jpg" style="width: 55%">&nbsp;
<img src="doc/USB-C PSU top.jpg" style="width: 42%;">

## Hardware
The v1.0 PCB is designed in 8 layers, but 6 would probably suffice too. Thus inner layers 3 and 4 were designed as containing only more GND / power copper pours and can probably be left out, only leading to a reduction of a combined copper thickness from 175µm to 140µm.

### JLCPCB Ordering Settings
- Default, except:
- PCB Thickness: `1.0mm`
- Mark on PCB: `Remove Mark`
- Stencil:
  - (Stencil Side: `Top`)
  - Customized Size: `Yes - 200 x 200mm` (saves on shipping cost)

## References
- ESD-Paw symbol: [diminDDL](https://github.com/diminDDL) ([Source](https://github.com/Caraffa-git/PCB-Art/tree/main/SVG/ESD-paw))
- Paw-shaped Tag-Connect pawprint: [Paw-Connect](https://github.com/LeoDJ/Paw-Connect) 