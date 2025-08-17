#pragma once
#include <vector>

#include "Unit.h"
#include "json.hpp"
using json = nlohmann::json;

class Faction {
   public:
void populateFaction(const json &factionData, std::unordered_map<int, Unit> &units);    
std::vector<Unit> units;
};

json loadJsonFiles(std::string &factionName);