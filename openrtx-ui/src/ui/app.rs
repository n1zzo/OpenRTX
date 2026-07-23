//! Main application window

use gtk::prelude::*;
use libadwaita::prelude::*;

use super::vfo::VfoWidget;
use super::channel_list::ChannelList;
use super::ptt_button::PttButton;

/// Main application window
pub struct OpenRTXApp {
    window: libadwaita::ApplicationWindow,
    vfo: VfoWidget,
    channel_list: ChannelList,
    ptt: PttButton,
    status_bar: gtk::Label,
}

impl OpenRTXApp {
    /// Create a new application window
    pub fn new(app: &libadwaita::Application) -> Self {
        let window = libadwaita::ApplicationWindow::builder()
            .application(app)
            .title("OpenRTX")
            .default_size(360, 640)
            .build();

        let vfo = VfoWidget::new();
        let channel_list = ChannelList::new();
        let ptt = PttButton::new();
        let status_bar = gtk::Label::builder()
            .label("Ready")
            .halign(gtk::Align::Start)
            .build();

        // Create main layout
        let main_box = gtk::Box::builder()
            .orientation(gtk::Orientation::Vertical)
            .spacing(8)
            .margin_top(8)
            .margin_bottom(8)
            .margin_start(8)
            .margin_end(8)
            .build();

        // Header bar with title
        let header = libadwaita::HeaderBar::builder()
            .title_widget(&libadwaita::TitleWidget::new())
            .show_title()
            .build();

        // Add VFO display
        main_box.append(&vfo.widget());

        // Add channel list
        main_box.append(&channel_list.widget());

        // Add PTT button
        main_box.append(&ptt.widget());

        // Add status bar
        main_box.append(&status_bar);

        window.set_child(Some(&main_box));

        let mut app = Self {
            window,
            vfo,
            channel_list,
            ptt,
            status_bar,
        };

        app.connect_signals();
        app.refresh_state();

        app
    }

    /// Show the window
    pub fn show(&self) {
        self.window.show();
    }

    /// Connect signal handlers
    fn connect_signals(&mut self) {
        // PTT button
        let status_bar = self.status_bar.clone();
        self.ptt.on_toggle(move |active| {
            unsafe {
                ffi::libopenrtx_set_ptt(active);
            }
            status_bar.set_label(if active { "TX" } else { "RX" });
        });
    }

    /// Refresh state from radio
    fn refresh_state(&self) {
        unsafe {
            let freq = ffi::libopenrtx_get_frequency();
            let mode = ffi::libopenrtx_get_mode();
            let rssi = ffi::libopenrtx_get_rssi();

            self.vfo.set_frequency(freq);
            self.vfo.set_mode(mode);
            self.vfo.set_rssi(rssi);
        }
    }
}