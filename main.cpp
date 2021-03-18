#include "cfg_manipulator.hpp"

cfg_manipulator::cfg_file settings("settings.cfg");

int main() { CM_LOG(settings.read("line")); }