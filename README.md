# cfg_manipulator [![Build Status](https://travis-ci.com/RedBull4/cfg_manipulator.svg?branch=main)](https://travis-ci.com/RedBull4/cfg_manipulator)
* Copy ```cfg_manipulator.cpp```, ```cfg_manipulator.hpp``` and ```cm_includes.hpp``` to your project folder.
* GCC version: >= 6.
* Star this repo if you like it 😊.
## GIF
<p align="center">
  <img src="example.gif" />
</p>

## Compilation
* ```g++ -std=c++14 -Wall -Werror *.cpp```
## Code
``` cpp
// main.cpp

#include "cfg_manipulator.hpp"

cfg_manipulator::cfg_file example;

int main() {
    // Opens the file "example.cfg"
    example.open("example.cfg");

    // Reads the line "line".
    CM_LOG(example.read("line"));

    // Reads the line "line" in the namesapce "namespace".
    CM_LOG(example.read("namespace", "line"));

    // Changes value of the line "line".
    example.change_value("line", "value");

    // Changes value of the line "line" in the namespace "namespace".
    example.change_value("namespace", "line", "value");
}
```
```
# example.cfg

line = "value"

[namespace]
line = "value"
```
## License
[MIT](https://github.com/RedBull4/cfg_manipulator/blob/main/LICENSE)