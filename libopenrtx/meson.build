project('libopenrtx', 'c',
    version: '0.1.0',
    default_options: [
        'warning_level=2',
        'optimization=2',
    ]
)

# Get OpenRTX source directories
openrtx_src = join_paths(meson.source_root(), 'openrtx')
openrtx_platform = join_paths(openrtx_src, 'platform')

# Compiler flags
cargs = [
    '-DPLATFORM_LINUX',
    '-DVP_USE_FILESYSTEM',
    '-DGIT_VERSION="libopenrtx-@0@"'.format(meson.project_version()),
]

# Include directories
inc = include_directories('include')

# OpenRTX includes
openrtx_inc = include_directories(
    join_paths(openrtx_src, 'openrtx/include'),
    join_paths(openrtx_src, 'platform'),
    join_paths(openrtx_src, 'platform/targets/linux'),
    join_paths(openrtx_src, 'platform/targets/linux/emulator'),
)

# Core source files from OpenRTX
core_sources = files(
    'src/init.c',
    'src/control.c',
    'src/state.c',
    'src/channel.c',
)

# Audio sources
audio_sources = files(
    'src/audio/codec.c',
)

# Create static library from OpenRTX core sources
libopenrtx_core = static_library(
    'openrtx-core',
    core_sources + audio_sources,
    include_directories: [inc, openrtx_inc],
    c_args: cargs,
    install: false,
)

# Create shared library that wraps the static library
libopenrtx = shared_library(
    'openrtx',
    [],
    include_directories: inc,
    link_with: libopenrtx_core,
    install: true,
    version: meson.project_version(),
    soversion: 0,
)

# Development package
libopenrtx_devel = declare_dependency(
    include_directories: inc,
    dependencies: [],
    version: meson.project_version(),
)

# Install headers
install_headers(
    'include/libopenrtx.h',
    'include/libopenrtx_audio.h',
    subdir: 'libopenrtx',
)

# Install library
install_lib = get_option('layout') == 'libstatic' ? libopenrtx_core : libopenrtx
install_lib.install = true

# Generate pkg-config file
pkgconfig = import('pkgconfig')
pkgconfig.generate(
    install_lib,
    name: 'libopenrtx',
    description: 'OpenRTX Core Library',
    version: meson.project_version(),
)

# Tests
test('libopenrtx-basic', executable('test_basic', 'tests/test_basic.c', link_with: libopenrtx_core))