#pragma once
#include <vector>

#include "Unit.h"
#include "json.hpp"
using json = nlohmann::json;

class Faction {
   public:
    void populateFaction(const json &factionData, std::vector<Unit> &units);
    std::vector<Unit> units;
};

json loadJsonFiles(std::string &factionName);