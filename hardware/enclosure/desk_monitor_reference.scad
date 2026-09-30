// ESP32 Mini Smart Desk Monitor - reference enclosure
// IMPORTANT: This is a generic reference model, not a scan or measurement of the physical prototype.
// Adjust dimensions to the actual OLED/ESP32 boards before fabrication.
//
// Units: mm

$fn = 48;

// Overall reference dimensions
outer_w = 110;
outer_d = 55;
outer_h = 28;
wall = 2.5;

// Front display opening
oled_w = 70;
oled_h = 38;
oled_z = 4;

// Sensor opening
sensor_d = 8;
sensor_x = 38;

// Cable opening
usb_w = 14;
usb_h = 8;
usb_z = 8;

module rounded_box(w, d, h, r) {
    minkowski() {
        cube([w-2*r, d-2*r, h-2*r], center=true);
        cylinder(r=r, h=2*r, center=true);
    }
}

difference() {
    // Outer shell
    translate([0, 0, outer_h/2])
        rounded_box(outer_w, outer_d, outer_h, 4);

    // Hollow interior
    translate([0, 0, wall + (outer_h-wall)/2])
        rounded_box(outer_w-2*wall, outer_d-2*wall, outer_h, 2.5);

    // OLED/front opening
    translate([0, -outer_d/2-0.1, oled_z + oled_h/2])
        cube([oled_w, wall+1, oled_h], center=true);

    // DHT airflow hole
    translate([sensor_x-outer_w/2, -outer_d/2-0.1, oled_z/2])
        rotate([90,0,0])
            cylinder(d=sensor_d, h=wall+2, center=true);

    // Rear/side USB cable opening
    translate([outer_w/2+0.1, 0, usb_z])
        cube([wall+1, usb_w, usb_h], center=true);
}
