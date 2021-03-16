#include "cfg_manipulator.hpp"
#include <iostream>
using namespace std;

cfg_manipulator::cfg_file file;

int main() {
    file.open("settings.cfg");

    cout << file.read("namespace", "line") << endl;
}