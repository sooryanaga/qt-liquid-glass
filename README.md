# QtLiquidGlass

A native Qt6/C++ library that brings macOS **Liquid Glass** effects (blurring the desktop behind the window) to Qt Widgets.

This library leverages private Apple APIs (`NSGlassEffectView`) to achieve the high-quality "behind-window" blur seen in modern macOS apps, with a robust fallback for future compatibility.

## Features

*   **Native Glass:** Uses private macOS APIs for the authentic "Liquid" look.
*   **Qt Integration:** Works seamlessly with `QWidget`.
*   **Smart Injection:** Automatically handles **Frameless Windows** (`Qt::FramelessWindowHint`) by intelligently swapping the Cocoa content view hierarchy to keep the glass behind your widgets.
*   **Fallback:** Gracefully degrades to standard `NSVisualEffectView` if the private API is unavailable.

## Installation

Requires **Qt 6** and **macOS**.

1.  Add this project as a subdirectory in CMake:
    ```cmake
    add_subdirectory(QtLiquidGlass)
    target_link_libraries(YourApp PRIVATE QtLiquidGlass)
    ```

2.  Use in C++:
    ```cpp
    #include <QtLiquidGlass/QtLiquidGlass.h>

    // In your QMainWindow constructor:
    QtLiquidGlass::addGlassEffect(this, QtLiquidGlass::Material::Sidebar);
    ```

## Acknowledgments

This library is built upon the research by the [electron-liquid-glass](https://github.com/Meridius-Labs/electron-liquid-glass) project. We gratefully acknowledge their discovery and reverse-engineering of Apple's private `NSGlassEffectView` API documentation and the mapping of its variant properties, which provided the foundation for this native Qt port.

## License

MIT

