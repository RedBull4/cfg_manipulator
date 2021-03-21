# cfg_manipulator [![Build Status](https://travis-ci.com/RedBull4/cfg_manipulator.svg?branch=main)](https://travis-ci.com/RedBull4/cfg_manipulator)
* Copy ```cfg_manipulator.cpp```, ```cfg_manipulator.hpp``` and ```cm_includes.hpp``` to your project folder.
* g++ version: >= 14.
## Compilation
* ```g++ -std=c++14 -Wall -Werror *.cpp```
## Code
``` cpp
// main.cpp

#include "cfg_manipulator.hpp"

cfg_manipulator::cfg_file settings;

int main() {
    // Opens the file "settings.cfg"
    settings.open("settings.cfg");

    // Reads the line "line".
    CM_LOG(settings.read("line"));

    // Reads the line "line" in the namesapce "namespace".
    CM_LOG(settings.read("namespace", "line"));

    // Changes value of the line "line".
    settings.change_value("line", "value");

    // Changes value of the line "line" in the namespace "namespace".
    settings.change_value("namespace", "line", "value");
}
```
```
# settings.cfg

line = "value"

[namespace]
line = "value"
```
## License
[MIT](https://github.com/RedBull4/cfg_manipulator/blob/main/LICENSE)