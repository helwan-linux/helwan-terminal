# Helwan Terminal

**A simple, beautiful, and powerful terminal emulator for your daily tasks.**

---

### Why Helwan Terminal?
Helwan Terminal is built to make using the command line easier and more enjoyable. Whether you are a student, a developer, or just a home user, we have designed it to be fast, reliable, and distraction-free.

### Key Features
*   **Easy Tabs:** Open multiple terminals in one window and switch between them effortlessly.
*   **Everything at Your Fingertips:** Right-click to copy and paste text quickly—just like you do in your favorite text editor.
*   **Customizable Look:** Want larger text? A different window size? Or perhaps a semi-transparent background? You can adjust everything in the settings.
*   **Smart Shortcuts:** Speed up your work with simple keyboard shortcuts for zooming and editing.
*   **Always Ready:** Helwan Terminal remembers your settings, so it looks exactly how you left it every time you open it.

### How to Use
*   **Copy/Paste:** Use `Ctrl + Shift + C` to copy and `Ctrl + Shift + V` to paste.
*   **Zoom In/Out:** Use `Ctrl + +` to make text bigger, `Ctrl + -` to make it smaller, or `Ctrl + 0` to reset.
*   **Settings:** Click the settings icon in the top bar to customize your experience.
*   **New Tab:** Click the plus icon to start a new session.

### Built for You
Helwan Terminal is proudly developed at **Helwan Linux**, focusing on the "Keep It Simple" philosophy. We believe your tools should get out of your way and let you get your work done.

---# Hel-Terminal

A minimalist, lightweight terminal emulator built for the Helwan Linux distribution. Developed with the KISS (Keep It Simple, Stupid) philosophy, it focuses on efficiency, performance, and seamless integration with the Helwan desktop environments[cite: 2, 5].

## Technical Overview
- **Language:** Written in C for optimal performance.
- **Dependencies:** Built using GTK3 modules for a lightweight graphical interface[cite: 5].
- **Build System:** Utilizes the Meson build system for streamlined compilation[cite: 5].
- **Customization:** Supports dynamic command loading via JSON, allowing users to define custom "intents" and aliases easily[cite: 5, 7].

## Installation
1. Ensure `meson` and `gtk3` development headers are installed.
2. Clone the repository.
3. Build the project:
   ```bash
   meson setup build
   ninja -C build
