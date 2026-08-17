# CCalculator

A simple calculator app for GNOME, built with GTK4 and Libadwaita.

## About

CCalculator is a lightweight, native calculator application for the GNOME desktop. It's built with C using GTK4 and Libadwaita, following GNOME's Human Interface Guidelines for a clean and consistent look and feel with the rest of the desktop.

## Features

- Basic arithmetic operations: addition, subtraction, multiplication, and division
- Decimal number support
- Clear/reset functionality
- History display showing the previous calculation
- Clean automatic trimming of trailing zeros for readable results
- Native GNOME look with Libadwaita styling

## Built With

- **Language:** C
- **Toolkit:** GTK4
- **Design library:** Libadwaita (`AdwApplicationWindow`)
- **Build tool:** GNOME Builder / Meson

## Building from Source

### Requirements

- GTK4 development libraries
- Libadwaita development libraries
- Meson and Ninja
- A C compiler (GCC or Clang)

### Build with GNOME Builder

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/ccalculator.git
   cd ccalculator
   ```
2. Open the project folder in **GNOME Builder**.
3. Click the **Run** button (or press `Ctrl+F5`) to build and launch the app.

### Build from the command line

```bash
meson setup builddir
ninja -C builddir
./builddir/src/ccalculator
```

## Project Structure

- `ccalculator-window.c` — Main application window logic, including the calculator's input handling and arithmetic operations
- `ccalculator-window.ui` — GTK Builder UI definition for the calculator's layout (number grid, result label, history label)
- `main.css` — Custom styling applied to the application window

## How It Works

Each number pad button is connected to a single `operations` callback, which:

1. Reads the label of the clicked button.
2. If it's a digit, appends it to the current number being entered (handling both the integer and decimal parts).
3. If it's an operator (`+`, `-`, `×`, `÷`), stores the operation and prepares to read the second operand.
4. On `=`, performs the calculation and displays the result, while the previous expression appears in the history label.
5. On `C`, resets the calculator state.

## License

This project is licensed under the **GNU General Public License v3.0 or later**. See the license header in the source files for details, or the [full license text](https://www.gnu.org/licenses/gpl-3.0.html).

## Author

Nassim, 2026

## Contributing

Issues and pull requests are welcome. If you find a bug (e.g., an edge case in the arithmetic logic) or want to suggest a feature, feel free to open an issue.
