#include "../linux/cfg_manipulator.hpp"

cfg_manipulator::cfg_file settings;

int main() {
    settings.open("settings.cfg");

    CM_LOG(settings.read("line"));
}