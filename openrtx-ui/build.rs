use std::env;
use std::path::PathBuf;

fn main() {
    // Find libopenrtx headers
    let libopenrtx_include = env::var("LIBOPENRTX_INCLUDE")
        .unwrap_or_else(|_| {
            // Try relative to this file
            let mut path = PathBuf::from(env!("CARGO_MANIFEST_DIR"));
            path.push("..");
            path.push("libopenrtx");
            path.push("include");
            path.to_string_lossy().to_string()
        });

    println!("cargo:rerun-if-changed={}", libopenrtx_include);

    // Generate bindings
    let bindings = bindgen::Builder::default()
        .header(format!("{}/libopenrtx.h", libopenrtx_include))
        .header(format!("{}/libopenrtx_audio.h", libopenrtx_include))
        .generate()
        .expect("Unable to generate bindings");

    // Write bindings to src/
    let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());
    bindings
        .write_to_file(out_path.join("libopenrtx_bindings.rs"))
        .expect("Couldn't write bindings");

    println!("cargo:rustc-link-search=native=../libopenrtx/build");
    println!("cargo:rerun-if-changed=src/lib.rs");
}