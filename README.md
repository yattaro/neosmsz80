# neosmsz80
An attempt at a Sega Master System emulator in C++ with a Qt frontend.

Currently non-functional WIP loosely following the guide at [codeslinger.co.uk](http://www.codeslinger.co.uk/pages/projects/mastersystem.html)

---
## TODO
- [ ] VDP
- [ ] Interrupts
- [ ] Controls
- [ ] Sound
- [ ] Ports to other platforms?
---
## Requirements
- [libz80 library](https://github.com/ggambetta/libz80)
- C++11 or above
- Qt 5 or above development environment
- qmake for building
  - (I use Qt Creator in my development environment; help converting this to something more common like CMake so it plays nicely with other IDEs would be greatly appreciated)
- make

## Build instructions without Qt Creator (for Linux)
1. Create build directory and navigate to it in your terminal
2. Run `qmake [path to neosmsz80.pro] && make qmake_all && make`

The resulting executable file will be created in the build directory as `neosmsz80-qt`; the emulation backend and GUI frontend are built as libraries and are added to the libs subdirectory.  They must be added to your LD_LIBRARY_PATH to run the executable.
