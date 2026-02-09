#pragma once

#include <Cocoa/Cocoa.h>
#include <string>
#include <vector>

std::string pick_file(
    std::string default_dir = "",
    std::vector<std::string> filters = {}
);
