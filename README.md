# dxx (dota++)

Dota 2 internal modification framework with AngelScript scripting support. Injects into the game process as a DLL and provides a scriptable overlay system with custom GUI, rendering primitives, and access to game engine internals.

> **Status:** In active development. Expect breaking changes, incomplete features, and missing documentation.

## Features

- **AngelScript scripting** -- load/unload `.as` scripts at runtime from the `%USERPROFILE%/dota++` directory with hot-reload support
- **DirectX 11 overlay** -- render custom overlays on top of the game via hooked swap chain
- **Dear ImGui GUI** -- built-in menu system (toggle with `Insert`) with a custom widget framework
- **World-to-screen projection** -- transform 3D game coordinates to 2D screen positions from scripts
- **Valve VTEX parser** -- load game textures (DXT1, DXT5, RGBA8888, embedded PNG, LZ4-compressed mipmaps, YCoCg)
- **Valve VFS integration** -- read files from the game's virtual filesystem
- **JSON configuration** -- persistent settings saved to `%USERPROFILE%/dota++/config.json`
- **Input binding system** -- keyboard/mouse bindings with US layout remapping

## Architecture

The project uses a service-oriented architecture with dependency injection:

```
DllMain -> C_Application -> bootstrap::Build()
                              |
              +---------------+----------------+
              |               |                |
         Hook System    SDK Layer      Rendering System
        (MinHook)     (Source2 VMT)    (DX11 + ImGui)
              |               |                |
              +-------+-------+--------+-------+
                      |                |
               AngelScript        GUI System
              (Runtime scripts)  (Widget tree)
```

**Key components:**

| Module | Description |
|--------|-------------|
| `src/bootstrap/` | Composition root -- wires all services in order |
| `src/hook/` | MinHook wrappers + typed pub-sub event dispatcher |
| `src/sdk/` | Source2 engine interfaces, pattern scanning, VTEX parser |
| `src/renderer/` | DX11/ImGui rendering pipeline, texture loading |
| `src/angelscript/` | Script engine, manager, bindings (ImGui, renderer, hooks) |
| `src/gui/` | Widget framework (windows, buttons, checkboxes, sliders, etc.) |
| `src/menu/` | Menu forms (navbar, settings with script list) |
| `src/service/` | IoC container with compile-time FNV-1a type hashing |
| `src/config/` | JSON config persistence |
| `src/input/` | Keyboard/mouse input binding |
| `src/memory/` | Pattern scanner, VMT call helper, offset macro |

## Script API

Scripts are `.as` files placed in `%USERPROFILE%/dota++/`. Entry points:

```angelscript
void main()       // called on script load
void onDispose()  // called on script unload
```

Available bindings:

- **Drawing** -- `render::C_Frame` with lines, rectangles, circles, triangles, text, images
- **Textures** -- `render::loadVTEX()`, `render::loadPNG()`, `render::loadFont()`
- **Math** -- `render::worldToScreen()`
- **Hooks** -- `hook::setOnRender()` to subscribe to render events
- **ImGui types** -- `imgui::ImVec2`, `imgui::ImVec4`, `imgui::ImFont`, `imgui::color32()`
- **Utilities** -- `render::getTickCount()`

## Building

### Requirements

- **CMake** >= 4.0
- **GCC / MinGW-w64** with C++20 modules support
- **Windows x64** (Win32 API, DirectX 11)

### Steps

```bash
git clone --recurse-submodules https://github.com/<owner>/dxx.git
cd dxx
cmake -B build -G "MinGW Makefiles"
cmake --build build
```

The output is `dxx_dlc.dll`.

### Compile flags

| Flag | Purpose |
|------|---------|
| `-mcx16` | 128-bit compare-and-swap (CMPXCHG16B) |
| `-mavx2` | AVX2 SIMD instructions |
| `-mfma` | Fused multiply-add |
| `-Wa,-mbig-obj` | Large object file support (needed for C++20 modules) |

## Dependencies

All dependencies are included as git submodules or vendored in `external/`:

| Library | Purpose |
|---------|---------|
| [Dear ImGui](https://github.com/ocornut/imgui) | GUI rendering (DX11 + Win32 backends) |
| [AngelScript](https://www.angelcode.com/angelscript/) | Embedded scripting engine |
| [asbind20](https://github.com/HenryAWE/asbind20) | C++20 bindings for AngelScript |
| [MinHook](https://github.com/TsudaKageworoshi/minhook) | x86/x64 API hooking |
| [LightningScanner](https://github.com/localcc/LightningScanner) | Fast byte pattern scanning |
| [nlohmann/json](https://github.com/nlohmann/json) | JSON serialization |
| [cs_libguarded](https://github.com/copperspice/cs_libguarded) | Thread-safe container wrappers |
| [xxHash](https://github.com/Cyan4973/xxHash) | Fast hashing |
| [LZ4](https://github.com/lz4/lz4) | Compression (VTEX textures) |
| [s3tc-dxt-decompression](https://github.com/Benjamin-Dobell/s3tc-dxt-decompression) | DXT1/DXT5 texture decompression |
| [stb](https://github.com/nothings/stb) | Image loading |
| [DirectXMath](https://github.com/microsoft/DirectXMath) | SIMD math |
| [DirectXTK](https://github.com/microsoft/DirectXTK) | SimpleMath wrapper |

System libraries: `d3d11`, `d3dcompiler`, `dwmapi`, `ws2_32`

## Controls

| Key | Action |
|-----|--------|
| `Insert` | Toggle menu |
| `End` | Unload / shutdown |

## Project Structure

```
dxx/
  main.cpp              # DLL entry point (DllMain)
  CMakeLists.txt        # Build configuration
  external/             # Third-party libraries (submodules + vendored)
  src/
    application.ixx     # Main application class
    bootstrap/          # Initialization and service wiring
    hook/               # Hook system (MinHook + event dispatcher)
    sdk/                # Source2 engine SDK (interfaces, math, VTEX)
    renderer/           # DX11 rendering pipeline
    angelscript/        # Script engine and bindings
    gui/                # Widget framework
    menu/               # Menu forms
    service/            # IoC container and service locator
    config/             # JSON config
    input/              # Input bindings
    memory/             # Pattern scanning, VMT helpers
    queue/              # Thread-safe worker queue
    hash/               # xxHash wrapper
    home/               # User directory management
    debug/              # Debug output logging
    directx/            # DX swap chain discovery
```

## License

[GNU General Public License v3.0](LICENSE)
