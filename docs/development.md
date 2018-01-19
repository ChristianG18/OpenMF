# Development

This file should help mainly new developers get familiar with the code and development process. Please:

- See also [CONTRIBUTING.md]().
- For any question or suggestions, contact us on our discord (link in [README.md]()) or issue tracker.

## Compiling

required libraries:

- OpenSceneGraph, at least 3.4.1
- Bullet Physics, at least 2.88

We're using the standard CMake build:

1. Download/clone the repository.
2. Create a `build` subfolder in it (`mkdir build`).
3. Enter the folder (`cd build`) and run `cmake ..`.
4. On Linux, run `make`. On Windows ???.

You should now have the built binaries in `bin` subfolder of the `build` folder. These include format
utilities to inspect Mafia files and mainly the **world/model viewer**. Each program includes a help
when run from command line. You'll need to do some additional setup to be able to run the executables
on the game files.

## Running

1. Get the original game (for example [here](http://store.steampowered.com/app/40990/Mafia/)).
2. Install the game (on Linux this can be done using Wine).
3. Extract the data files into a folder (e.g. `/home/myname/mafia/`) using one of these methods:
   - At this moment, it's probably best to use [Mafia DTA extractor](http://www.moddb.com/games/mafia/downloads/mafia-data-xtractor-v11).
   - We have our own extractor (`format_utils/dta`), but it's not very well tested yet. You can try though.
4. Set the `MAFIA_INSTALL_DIR` environment variable to point to the folder with extracted files, e.g. `MAFIA_INSTALL_DIR="/home/myname/mafia/"`.
5. Now you should be able to run the world viewer. Test it for example with `./bin/viewer 00menu`.

## Programming

- currently targeted platforms: **Linux** and **Windows**, others are planned
- programming language: **C++14**
- [**single compilation unit**](https://en.wikipedia.org/wiki/Single_Compilation_Unit)
(everything except executables is in headers)
- [**CMake**](https://cmake.org/) build system
- [OpenMW **code formatting**](https://wiki.openmw.org/index.php?title=Code_Formatting_Conventions)
- when in doubt, see similar projects' solution ([OpenMW](https://github.com/OpenMW/openmw),
[OpenRW](https://github.com/rwengine/openrw), [REGoth](https://github.com/REGoth-project/REGoth), ...)
- GPLv3 license
- issue tracker: GitHub
- see also [CONTRIBUTING.md]()

## Repo structure

The folder structure is inspired by OpenMW:

- `components`: our own reusable components
  - For each Mafia file format there is a subfolder (e.g. `4ds`) containing different handlers:
    - `parser.hpp`: extracts the data from the format
    - `osg.hpp`: creates an OSG graphical representation of the format data (i.e. a 3D model)
    - `bullet.hpp` creates a Bullet Physics representation of the format data
- `apps`: executable sources, which make use of the components
  - `model_viewer`: interactive model and mission/world viewer
  - `format_utils`: sources of command line utilities for each Mafia format, mostly for dumping/extracting the content
  - `game`: will contain the final game engine
- `extern`: third party libraries we decided to include in our repository (so that we can edit them etc.)
- `tests`: automated tests
- `resources`: our own resources (e.g. icons, shaders, screenshots, ...)

## Architecture and philosophy

- We're **not** using a general [entity-component-system](https://en.wikipedia.org/wiki/Entity%E2%80%93component%E2%80%93system),
but rather a simpler approach - we have hard-coded spatial entities that unify visual and physical representation (i.e. OSG nodes and
Bullet rigid bodies) of in-game objects.
- Keep minimal dependencies, i.e. for example a format parser must not depend on Bullet Physics library etc.
- Try to make everything replaceable by first creating an independent abstract class (an interface) for a component, then subclass it with
a concrete implementation (see for example renderer).
- Make good use of design patterns.

## Useful resources

- [discord server](https://discord.gg/uXKnHEU)
- [OSG reference](http://public.vrac.iastate.edu/vancegroup/docs/OpenSceneGraphReferenceDocs-3.0/index.html) - often inaccessible and outdated, better build your own locally
- [Bullet Physics reference](http://bulletphysics.org/Bullet/BulletFull/index.html)
- [OpenMW reference](https://openmw.github.io/)
