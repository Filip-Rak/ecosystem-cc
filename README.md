# Requirements
- C++23 compiler.
- CMake version 3.16 or higher.
- Possibly backend libs for SFML on linux distributions:  `libx11-dev libxrandr-dev libfreetype6-dev libglew-dev libopenal-dev libudev-dev libflac-dev libvorbis-dev libgl1-mesa-dev libxcursor-dev`.

# Installation
- run `git clone https://github.com/Filip-Rak/ecosystem-cc.git` - download the source code.
- run `cd ecosystem-cc/`

either:
- from the root directory:
  - run `cmake -B build` - Configure CMake. run this from root project dir.
  - run `cmake --build build` - Compile project.

alternatively:
- add/edit in your own cmake preset to CMakePresets.json or use the clang one.
- from the root directory:
  - run `cmake --preset clang` - configure the project via preset.
  - run `cmake --build build` - compile the project.

# Usage
- run `ecosystem.exe` file. Location depended on the build system: `build\src` for Ninja, `build\src\Debug` for MSBuild.
- run executable with the following options:
  ```
  OPTIONS:
  -h,     --help              Print this help message and exit
          --preset TEXT       Simulation preset to run
          --gui               Run with GUI
  ```
- resource files, such as presets or grids are present in `resources/` directory.
- modify and create presets. Example in `resources/presets`.
- Feed the executable the preset to execute `ecosystem.exe --preset { path }`.
- Run with gui `ecosystem.exe --preset { path } --gui`.
- Executable and resource files are not bound to any other files and can be moved to any location for ease of use as long as the paths inside the preset and preset path are correct. 
