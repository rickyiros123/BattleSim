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

std::unordered_map<int, std::string> buildFactionUnitMap(const json &factionJson);
void buildUserArmyList(const std::unordered_map<int, std::string> &factionUnits, std::unordered_map<int, Unit> &userArmyList, json factionJson);
void printFactionUnitList(const std::unordered_map<int, std::string> &units);

json loadFactionData(const std::string &factionName);
Unit makeUnitFromJson(const json &factionJson, int unitId);
std::vector<Weapon> parseWeapons(const json &weaponJson);
