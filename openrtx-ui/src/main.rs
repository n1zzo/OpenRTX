//! OpenRTX UI - GTK4 application for OpenRTX radio firmware
//!
//! This is the main entry point for the GTK4 UI application.

mod ffi;
mod ui;

use gtk::prelude::*;
use libadwaita::prelude::*;
use std::sync::OnceLock;

use crate::ui::app::OpenRTXApp;

// Global app instance
static APP: OnceLock<OpenRTXApp> = OnceLock::new();

fn main() {
    // Initialize libopenrtx
    unsafe {
        if ffi::libopenrtx_init() != 0 {
            eprintln!("Failed to initialize libopenrtx");
            std::process::exit(1);
        }
    }

    println!("OpenRTX UI starting...");
    println!("  Version: {}", env!("CARGO_PKG_VERSION"));

    // Set up GTK application
    let app = libadwaita::Application::builder()
        .application_id("org.openrtx.ui")
        .flags(gtk::gio::ApplicationFlags::HANDLES_OPEN)
        .build();

    app.connect_activate(|app| {
        let window = OpenRTXApp::new(app);
        
        if let Err(_) = APP.set(window) {
            eprintln!("Failed to set global app instance");
        }
        
        window.show();
    });

    // Run the application
    app.run();

    // Cleanup
    unsafe {
        ffi::libopenrtx_terminate();
    }
}