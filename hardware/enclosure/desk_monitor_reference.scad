// ESP32 Mini Smart Desk Monitor — generic enclosure reference
// This is a starting point, not a measured model of any photographed device.
// Check your exact OLED module, ESP32 board, sensor, and USB connector before printing.
// Units: millimetres.

$fn = 64;

outer_w = 90;
outer_d = 55;
outer_h = 35;
wall = 2.4;
corner_r = 3;

// Approximate display opening for a small 128x64 OLED module.
display_opening_w = 28;
display_opening_h = 14;
display_center_z = 20;

// Side openings; verify against the chosen sensor/module/USB connector.
sensor_hole_d = 6;
sensor_hole_y = 12;
sensor_hole_z = 25;
usb_opening_w = 15; // along Y
usb_opening_h = 8;  // along Z
usb_opening_y = -10;
usb_opening_z = 8;

module rounded_box(w, d, h, r) {
    minkowski() {
        cube([w - 2*r, d - 2*r, h - 2*r], center=true);
        cylinder(r=r, h=2*r, center=true);
    }
}

difference() {
    // Outer shell, with a bottom at Z=0.
    translate([0, 0, outer_h/2])
        rounded_box(outer_w, outer_d, outer_h, corner_r);

    // Hollow interior; leaves the specified bottom wall and is open at the top.
    translate([0, 0, wall + (outer_h-wall)/2])
        rounded_box(outer_w-2*wall, outer_d-2*wall, outer_h-wall,
                    max(0.8, corner_r-wall));

    // OLED window on the front (negative-Y) face.
    translate([0, -outer_d/2-0.1, display_center_z])
        cube([display_opening_w, wall+1, display_opening_h], center=true);

    // Side opening for an external temperature/humidity sensor.
    translate([outer_w/2+0.1, sensor_hole_y, sensor_hole_z])
        rotate([0, 90, 0])
            cylinder(d=sensor_hole_d, h=wall+1, center=true);

    // USB access on the right-hand side.
    translate([outer_w/2+0.1, usb_opening_y, usb_opening_z])
        cube([wall+1, usb_opening_w, usb_opening_h], center=true);
}
