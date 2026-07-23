# OpenRTX GTK UI Project

GTK4-based touch UI for OpenRTX radio firmware, running on PostmarketOS.

## Project Structure

```
openrtx-project/
├── libopenrtx/           # C library API (new)
│   ├── include/          # Public headers
│   │   ├── libopenrtx.h  # Radio control API
│   │   └── libopenrtx_audio.h # Audio/codec API
│   ├── src/
│   │   ├── init.c        # Lifecycle
│   │   ├── control.c     # Frequency, mode, PTT
│   │   ├── state.c       # RSSI, battery monitoring
│   │   ├── channel.c     # Channel memory
│   │   └── audio/        # Audio processing
│   │       └── codec.c   # CODEC2 interface
│   └── meson.build
│
├── openrtx-ui/           # Rust GTK4 application (new)
│   ├── Cargo.toml
│   ├── build.rs          # bindgen FFI generation
│   └── src/
│       ├── main.rs       # App entry point
│       ├── ffi.rs        # FFI bindings
│       └── ui/           # UI components
│           ├── app.rs    # Main window
│           ├── vfo.rs    # VFO display
│           ├── channel_list.rs
│           └── ptt_button.rs
│
├── openrtx/              # Original OpenRTX firmware (subtree)
│   └── ...               # n1zzo/OpenRTX.git
│
└── docs/                 # Documentation
```

## Building

### Prerequisites

**On macOS (editing):**
- Rust toolchain (rustup)
- VS Code with GTK extensions

**On Fedora VM (building):**
- meson, ninja, gcc
- GTK4 development libraries

**On PMOS (target):**
- musl libc
- SDL2 runtime

### Build libopenrtx

```bash
cd libopenrtx
meson setup build
ninja -C build
# Library: build/libopenrtx.a
```

### Build openrtx-ui

```bash
cd openrtx-ui
cargo build --release
# Binary: target/release/openrtx-ui
```

## API Overview

### Radio Control

```c
// Initialize
libopenrtx_init();

// Frequency (Hz)
libopenrtx_set_frequency(145500000);
uint32_t freq = libopenrtx_get_frequency();

// Mode (MODE_FM, MODE_M17, etc.)
libopenrtx_set_mode(MODE_FM);

// PTT
libopenrtx_set_ptt(true);

// State
float rssi = libopenrtx_get_rssi();
float vbat = libopenrtx_get_battery();
```

### Audio Interface

```c
// Encode PCM to wire format
int16_t pcm[160];
uint8_t encoded[16];
size_t out_len;
libopenrtx_encode(pcm, encoded, sizeof(encoded), &out_len, MODE_M17);

// Decode wire format to PCM
libopenrtx_decode(encoded, out_len, pcm, sizeof(pcm), &out_len, MODE_M17);
```

## Status

| Component | Status | Notes |
|-----------|--------|-------|
| libopenrtx C API | ✅ Complete | Full API defined |
| libopenrtx stub implementation | ✅ Complete | 520 lines C code |
| libopenrtx Makefile | ✅ Complete | Simple make build |
| libopenrtx test suite | ✅ Complete | All 16 tests pass |
| openrtx-ui Rust structure | ✅ Complete | GTK4 + libadwaita |
| openrtx-ui UI components | ✅ Complete | VFO, channels, PTT |
| Build openrtx-ui | 🔄 Todo | Needs PMOS environment |
| Deploy to PMOS | 🔄 Todo | Create APK package |