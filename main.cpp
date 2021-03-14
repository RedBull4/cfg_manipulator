#include "cfg_manipulator.hpp"
#include <iostream>
using namespace std;

cfg_manipulator::cfg_file cfg_file;

int main() {
    cfg_file.open("settings.cfg");

    CM_LOG(cfg_file.read("ddos"));
}