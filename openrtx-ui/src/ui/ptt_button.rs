//! PTT (Push-To-Talk) button widget

use gtk::prelude::*;
use std::cell::Cell;
use std::rc::Rc;

/// Callback for PTT toggle
type ToggleCallback = Box<dyn Fn(bool) -> () + 'static>;

pub struct PttButton {
    button: gtk::Button,
    active: Rc<Cell<bool>>,
    callbacks: Vec<ToggleCallback>,
}

impl PttButton {
    pub fn new() -> Self {
        let active = Rc::new(Cell::new(false));
        let active_clone = active.clone();
        let callbacks: Vec<ToggleCallback> = Vec::new();

        let button = gtk::Button::builder()
            .label("PTT")
            .halign(gtk::Align::Center)
            .width_request(120)
            .height_request(120)
            .css_classes(["suggested-action", "circular"])
            .build();

        button.connect_clicked(move |btn| {
            let new_state = !active_clone.get();
            active_clone.set(new_state);
            btn.set_label(if new_state { "TX" } else { "PTT" });
            btn.add_css_class(if new_state { "destructive-action" } else { "suggested-action" });
        });

        Self {
            button,
            active,
            callbacks,
        }
    }

    pub fn widget(&self) -> &gtk::Widget {
        self.button.upcast_ref()
    }

    /// Register a callback for PTT toggle events
    pub fn on_toggle<F>(&mut self, callback: F) 
    where 
        F: Fn(bool) -> () + 'static 
    {
        // For now, just store the callback
        // In a real implementation, we'd call it when PTT changes
        let _ = callback;
    }

    pub fn is_active(&self) -> bool {
        self.active.get()
    }
}