#include "cfg_manipulator.hpp"
#include <string>
using namespace std;

cfg_manipulator::cfg_file settings("settings.cfg");

int main() {
    for (size_t i = 0; i <= 100; i++)
        settings.change_value("namespace", "line", to_string(i).c_str());
}