# CCalculator

A sleek, modern, and lightweight calculator built using **C** and **GTK4/Libadwaita**. Designed to be fast, efficient, and native to the GNOME desktop.

## ✨ Features
*   **Native & Fast:** Built entirely in C for a minimal footprint.
*   **Modern UI:** Leverages Libadwaita for a polished, adaptive interface that follows the GNOME HIG.
*   **Adaptive Design:** Perfectly responsive across different window sizes.
*   **Clean Output:** Optimized logic to handle decimals and remove unnecessary trailing zeros.

## 🛠️ Technical Stack
*   **Language:** C
*   **Toolkit:** GTK4, Libadwaita
*   **Build System:** Meson, Ninja
*   **Packaging:** Flatpak

## 🚀 Installation & Building

### Prerequisites
Ensure you have the following installed:
- `gtk4` and `libadwaita` development libraries
- `meson`
- `ninja`

### Building from source
```bash
# Clone the repository
git clone [https://github.com/nassim/CCalculator.git](https://github.com/nassim/CCalculator.git)
cd CCalculator

# Setup the build directory
meson setup build

# Compile
meson compile -C build

# Run
./build/src/ccalculator
