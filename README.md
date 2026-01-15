# Kallune (Nintendo DS Port)

## 🛠 Prerequisites

To compile this project, you need the following tools installed on your system:

* **devkitPro**: The core homebrew development suite.
* **devkitARM**: The ARM compiler provided by devkitPro.
* **Homebrew Tools**: The build system uses `grit` (graphics), `mmutil` (audio), and `bin2o` (binary conversion), all included in the devkitPro installation.

For installation instructions, visit [devkitpro.org](https://devkitpro.org).

## 🚀 Building the Project

### 1. Environment Configuration
Ensure your environment variables are correctly exported in your shell (e.g., `.zshrc` or `.bashrc`). These paths allow the `Makefile` to find the necessary tools.

```bash
export DEVKITPRO=/opt/devkitpro
export DEVKITARM=$DEVKITPRO/devkitARM
```

### 2. Compilation
Navigate to the root of the repository and run the build command:

```bash
make
```

The Makefile utilizes `ds_rules` (provided by devkitARM). Upon successful compilation, an executable `.nds` file will be generated in the root directory.

### 3. Running the Game
* Emulator: I recommend using [melonDS](https://melonds.kuribo64.net/) (which was more accurate during my tests) or [No$GBA](https://problemkaputt.de/gba.htm) (which has awesome debugging features) for testing the game on your computer.
* Real hardware: You can also run the game on actual Nintendo DS by transfering the `.nds` file to a compatible flashcart.

## 📂 Code structure
* `gfx/` graphical assets.
* `audio/` audio assets.
* `source/` C++ game source code.
	* `main.cpp` main entry point.
	* `logic/` game logic.
	* `graphics/` graphics management (extensible scene system).
	* `input/` user input management (extensible state system).
	* `sounds/` audio management.
	* `utils/` miscellaneous utilities.
* `Makefile` build script.

## 📜 Credits

This port is based on the original [Kallune source code](https://github.com/guillaume-tritsch/Kallune.git).

**Original Developers:**
* [@guillaume-tritsch](https://github.com/guillaume-tritsch)
* [@BQuent1](https://github.com/BQuent1)
* [@killianguilland](https://github.com/killianguilland)

**Assets:**
* Tileset by [scrabling](https://scrabling.itch.io/pixel-isometric-tiles) (Licensed under **CC BY 4.0**).
* Most UI assets made by myself using Figma.