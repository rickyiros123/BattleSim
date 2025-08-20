#pragma once
#include <string>
#include <unordered_map>
#include <vector>

#include "Unit.h"
#include "json.hpp"
using json = nlohmann::json;

class Faction {
   public:
    std::vector<Unit> units;
};

std::unordered_map<int, std::string> createFactionUnits(const json &factionJson);
void printFactionUnits(const std::unordered_map<int, std::string> &units);

json loadJsonFiles(std::string &factionName);
Unit createUnitFromJson(const json &factionJson, int unitId);
std::vector<Weapon> parseWeapons(const json &weaponJson);