//! Channel list widget

use gtk::prelude::*;
use libadwaita::prelude::*;
use crate::ffi::{self, libopenrtx_channel_t};

pub struct ChannelList {
    model: gtk::StringList,
    view: gtk::ListView,
}

impl ChannelList {
    pub fn new() -> Self {
        let model = gtk::StringList::new(&[]);
        
        // Load channels from libopenrtx
        unsafe {
            let count = ffi::libopenrtx_channel_count();
            for i in 0..count {
                let mut channel: libopenrtx_channel_t = std::mem::zeroed();
                if ffi::libopenrtx_channel_get(i, &mut channel) == 0 {
                    // Format channel entry
                    let name = std::ffi::CStr::from_ptr(channel.name.as_ptr() as *const i8)
                        .to_string_lossy();
                    let freq = channel.rx_frequency;
                    let mode = ffi::mode_to_string(channel.mode as i32);
                    
                    model.append(&format!("{}  {:.3} MHz  {}", 
                        name, freq as f64 / 1_000_000.0, mode));
                }
            }
        }

        let factory = gtk::SignalListItemFactory::new();
        factory.connect_setup(|_, item| {
            let label = gtk::Label::builder()
                .halign(gtk::Align::Start)
                .margin_start(12)
                .margin_end(12)
                .margin_top(8)
                .margin_bottom(8)
                .build();
            item.downcast_ref::<gtk::ListItem>()
                .unwrap()
                .set_child(Some(&label));
        });
        factory.connect_bind(|_, item| {
            let label = item.downcast_ref::<gtk::ListItem>()
                .unwrap()
                .child()
                .unwrap()
                .downcast_ref::<gtk::Label>()
                .unwrap();
            let string_item = item.downcast_ref::<gtk::StringListItem>().unwrap();
            label.set_label(string_item.string().as_str());
        });

        let view = gtk::ListView::builder()
            .model(&gtk::NoSelection::new(Some(&model.clone())))
            .factory(&factory)
            .single_activate(true)
            .build();

        let scrolled = gtk::ScrolledWindow::builder()
            .child(&view)
            .min_content_height(150)
            .max_content_height(200)
            .build();

        // Store view for later use
        Self {
            model,
            view,
        }
    }

    pub fn widget(&self) -> &gtk::Widget {
        self.view.upcast_ref()
    }

    pub fn select_channel(&self, index: u32) {
        unsafe {
            ffi::libopenrtx_channel_select(index as i32);
        }
    }
}