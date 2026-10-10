# Submission Readiness

This index describes what is actually present in the repository. It does not certify that the hardware was built or that the firmware was tested.

## Included

- `src/esp32_smart_desk_monitor.ino` — firmware source.
- `platformio.ini` — PlatformIO project configuration.
- `hardware/BOM.csv` — intended parts list with reference notes.
- `hardware/schematic.svg` — firmware pin-connection reference.
- `hardware/enclosure/desk_monitor_reference.scad` — generic editable enclosure model.
- `hardware/enclosure/model_preview.svg` — illustration of the generic enclosure dimensions.
- `.github/workflows/platformio.yml` — automated compile check for source changes.

## Not included / not verified

- No routed PCB, KiCad project, Gerbers or drill files.
- No successful PlatformIO build/upload log in the repository yet.
- No serial log proving Wi-Fi or time synchronization.
- No recorded sensor-accuracy comparison or long-run test.
- No verified photograph of this repository's firmware running on the pictured hardware.
- No measured fit check or print test for the reference enclosure.
- No documented provenance for the project concept or any adapted material.

Only mark these items complete after obtaining real evidence. See [JOURNAL.md](JOURNAL.md) for the audit notes and [ATTRIBUTION.md](ATTRIBUTION.md) for provenance status.
