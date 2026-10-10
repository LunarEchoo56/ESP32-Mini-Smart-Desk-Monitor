# Engineering Notes and Validation Log

> This file records repository state and evidence, not a retroactive timesheet. No historical hour totals are asserted here. Record time in the relevant programme's official tracker only for work actually performed.

## Repository audit — 2026-10-10

### What the repository contains

- An Arduino-style ESP32 sketch targeting a 128×64 SSD1306 I²C OLED and a DHT22 sensor.
- A PlatformIO project configuration and automated compile workflow.
- A wiring schematic and a generic OpenSCAD enclosure reference.
- A BOM describing intended parts; exact purchased/installed component models are not recorded.
- No verified build photo. The only supplied reference frame reviewed shows a portrait-oriented screen, while the checked-in firmware draws a landscape display layout. It was omitted from the README rather than presented as test evidence.

### Changes made during this audit

- Replaced the earlier hour-by-hour narrative because the repository contained no contemporaneous evidence supporting its exact durations or detailed claims.
- Removed specific claims of measured sensor accuracy, a 12-hour run, reconnection tests and phone-to-clock comparison. No serial logs, test data or other reproducible evidence for those results were included.
- Removed a code screenshot that did not match the current sketch and a BOM screenshot whose OLED size/details conflicted with the documented configuration.
- Removed a decorative PCB graphic that was not a routed board design, a duplicate confusing wiring graphic, and a stale ZIP package that could diverge from the source tree.
- Reworked Wi-Fi retries and system-clock polling to avoid repeatedly blocking for network connection or valid time while trying to refresh the display.
- Replaced the oversized display aperture in the generic enclosure model and corrected the cavity height to retain the specified bottom wall.
- Removed the previous baseplate STL after review showed it was not a closed, watertight solid.
- Added explicit status, build-validation and attribution notes to the README and submission checklist.

### Checks performed in this audit

- The revised OpenSCAD source was exported to STL locally.
- The exported enclosure mesh was checked for watertightness and returned as watertight in local mesh inspection.
- Firmware compilation and upload were **not** run during this audit: PlatformIO and Arduino CLI were not installed in the audit environment.
- No physical component, sensor accuracy, Wi-Fi, SNTP or long-duration test was run during this audit.

## Current gaps

1. **Hardware verification:** add build output, serial logs and observations from the real target board.
2. **Photo/code correspondence:** provide a photo showing the exact hardware and display layout running this sketch, or update the firmware only after confirming the actual display, orientation and sensor wiring.
3. **PCB:** there is no fabrication-ready PCB or CAD project. Do not describe the schematic or old concept art as a PCB.
4. **Enclosure fit:** measure the real modules and adjust the generic OpenSCAD dimensions before printing.
5. **Attribution:** the source of the project idea and any reused code/assets must be documented in [ATTRIBUTION.md](ATTRIBUTION.md).

## Future work log template

For each new session, record facts contemporaneously:

- **Date:**
- **Time spent:** actual duration; no estimates presented as tracked time
- **Change made:** specific files/functions/features
- **Evidence:** commit, build output, serial log, measurement or photo
- **Result:** what was confirmed, what failed, and what remains untested
