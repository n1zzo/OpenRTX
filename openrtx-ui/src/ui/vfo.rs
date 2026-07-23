//! VFO (Variable Frequency Oscillator) display widget

use gtk::prelude::*;
use libadwaita::prelude::*;
use crate::ffi;

pub struct VfoWidget {
    container: gtk::Box,
    frequency_label: gtk::Label,
    mode_label: gtk::Label,
    rssi_label: gtk::Label,
    battery_label: gtk::Label,
}

impl VfoWidget {
    pub fn new() -> Self {
        let container = gtk::Box::builder()
            .orientation(gtk::Orientation::Vertical)
            .halign(gtk::Align::Center)
            .spacing(4)
            .build();

        // Frequency display
        let frequency_label = gtk::Label::builder()
            .label("145.500 MHz")
            .css_classes(["title-1"])
            .build();

        // Mode indicator
        let mode_label = gtk::Label::builder()
            .label("FM")
            .css_classes(["accent"])
            .build();

        // RSSI meter
        let rssi_label = gtk::Label::builder()
            .label("RSSI: -87 dBm")
            .css_classes(["dim-label"])
            .build();

        // Battery indicator
        let battery_label = gtk::Label::builder()
            .label("Battery: 100%")
            .css_classes(["dim-label"])
            .build();

        // Frequency control buttons
        let freq_controls = gtk::Box::builder()
            .orientation(gtk::Orientation::Horizontal)
            .spacing(4)
            .halign(gtk::Align::Center)
            .build();

        let btn_up = gtk::Button::with_label("▲");
        let btn_down = gtk::Button::with_label("▼");
        
        // Temporary callback - would increment/decrement frequency
        btn_up.connect_clicked(|_| {
            unsafe {
                let current = ffi::libopenrtx_get_frequency();
                ffi::libopenrtx_set_frequency(current + 125000); // +125kHz
            }
        });
        
        btn_down.connect_clicked(|_| {
            unsafe {
                let current = ffi::libopenrtx_get_frequency();
                if current >= 125000 {
                    ffi::libopenrtx_set_frequency(current - 125000);
                }
            }
        });

        freq_controls.append(&btn_down);
        freq_controls.append(&btn_up);

        // Assemble the widget
        container.append(&frequency_label);
        container.append(&mode_label);
        container.append(&freq_controls);
        container.append(&rssi_label);
        container.append(&battery_label);

        Self {
            container,
            frequency_label,
            mode_label,
            rssi_label,
            battery_label,
        }
    }

    /// Get the GTK widget
    pub fn widget(&self) -> &gtk::Widget {
        self.container.upcast_ref()
    }

    /// Update frequency display
    pub fn set_frequency(&self, hz: u32) {
        self.frequency_label.set_label(&ffi::format_frequency(hz));
    }

    /// Update mode display
    pub fn set_mode(&self, mode: i32) {
        self.mode_label.set_label(ffi::mode_to_string(mode));
    }

    /// Update RSSI display
    pub fn set_rssi(&self, rssi: f32) {
        self.rssi_label.set_label(&format!("RSSI: {:.0} dBm", rssi));
    }

    /// Update battery display
    pub fn set_battery(&self, voltage: f32) {
        let percent = ((voltage - 6.0) / 2.4 * 100.0).clamp(0.0, 100.0) as i32;
        self.battery_label.set_label(&format!("Battery: {}%", percent));
    }
}