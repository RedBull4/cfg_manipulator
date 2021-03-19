#include "cfg_manipulator.hpp"

cfg_manipulator::cfg_file settings("settings.cfg");

int main() { settings.change_value("lines", " sawd"); }