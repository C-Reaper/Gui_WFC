## Overview
The project appears to be a Wave Function Collapse (WFC) algorithm implementation, visualized using a graphical user interface. The WFC algorithm is used for procedural content generation and can create images or mazes based on a given set of rules.

## Features
- Wave Function Collapse Algorithm
- Visualization using a graphical user interface

## Project Structure
### Prerequisites
- C/C++ Compiler and Debugger (GCC, Clang)
- Make utility
- Standard development tools
- Libraries needed for GUI: X11 for Linux, WINAPI for Windows
- Libraries needed for image handling: PNG, JPEG

## Build & Run
The project uses Makefiles to manage the build process. Here are the steps to build and run the project on different platforms:

### Linux
To build and run the project on Linux:
```sh
cd <Project>
make -f Makefile.linux all
make -f Makefile.linux exe
```

### Windows
To build and run the project on Windows:
```sh
cd <Project>
make -f Makefile.windows all
make -f Makefile.windows exe
```

### Wine (Linux cross-compilation for Windows)
To build and run the project using Wine on Linux:
```sh
cd <Project>
make -f Makefile.wine all
make -f Makefile.wine exe
```

### WebAssembly (Emscripten)
To build and run the project using Emscripten:
```sh
cd <Project>
make -f Makefile.web all
make -f Makefile.web exe
```

# Project Organization
The project structure is as follows:

- `build/`: Contains executable files produced by Main.c
- `src/`: Source code directory containing `Main.c` and other header files
  - `Main.c`
  - Other `.h` files used by Main.c
- `Makefile.linux`: Linux build configuration
- `Makefile.windows`: Windows build configuration
- `Makefile.wine`: Wine build configuration for cross-compilation
- `Makefile.web`: Emscripten build configuration

The project does not include a main README.md file or a LICENSE file in the provided information.