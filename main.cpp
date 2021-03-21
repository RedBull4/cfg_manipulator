#include "cfg_manipulator.hpp"

cfg_manipulator::cfg_file settings("settings.cfg");

int main() {
    // Reads line "line".
    CM_LOG(settings.read("line"));
    // Reads line "line" in namesapce "namespace"
    CM_LOG(settings.read("namespace", "line"));

    // Changes value of the line "line"
    settings.change_value("line", "value");
    // Changes value of the line "line" in the namespace "namespace"
    settings.change_value("namespace", "line", "value");
}