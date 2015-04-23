union() {
    rotate(180, [1,0,0]) {

    translate([10,0,-10]) {
        cube([10,50,10]);
    }
    
    translate([15,65,-10]) {
        difference() {
            cylinder(r=20,h=10,$fn=50);
            cylinder(r=15.3,h=10,$fn=50);
        }
    }
}
    difference() {
        cube([30,13.2,10]);
        union() {
            rotate(-90, [1,0,0]) {
                translate([5,-5,0]) {
                    cylinder(d=5,h=50,$fn=20);
                }
                translate([30-5,-5,0]) {
                    cylinder(d=5,h=50,$fn=20);
                }
            }
        }
    }
}