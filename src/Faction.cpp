#include "Faction.h"

#include <fstream>

#include "Dice.h"

void Faction::populateFaction(const json &factionData, std::unordered_map<int, Unit> &units) {
    int unitId = 0;  // Initialize unit ID for unique identification
    for (const auto &unit : factionData["units"]) {
        int modelCount = unit["modelCount"].get<int>();
        int healthPerModel = unit["healthPerModel"].get<int>();
        int floatingDamage = unit["floatingDamage"].get<int>();
        int save = unit["save"].get<int>();
        int ward = unit["ward"].get<int>();
        std::vector<Weapon> weapons;
        for (const auto &w : unit["weapons"]) {
            Weapon weapon;
            weapon.numberOfAttacks = w["numberOfAttacks"].get<int>();
            weapon.toHit = w["toHit"].get<int>();
            weapon.toWound = w["toWound"].get<int>();
            weapon.rend = w["rend"].get<int>();
            // Handle weaponDamage as int or dice notation string
            if (w["weaponDamage"].is_number_integer()) {
                weapon.weaponDamage = w["weaponDamage"].get<int>();
            } else if (w["weaponDamage"].is_string()) {
                std::string dmgStr = w["weaponDamage"].get<std::string>();
                // Example: handle "D3", "D6", "2D6" etc.
                if (dmgStr == "D3") {
                    weapon.weaponDamage = roll_dice(1, 3, 1, 3).successfulRolls;  // or your dice roll logic
                } else if (dmgStr == "D6") {
                    weapon.weaponDamage = roll_dice(1, 6, 1, 6).successfulRolls;
                } else if (dmgStr == "2D6") {
                    weapon.weaponDamage = roll_dice(2, 6, 1, 6).successfulRolls;
                } else {
                    weapon.weaponDamage = 0;  // fallback for unknown notation
                }
            } else {
                weapon.weaponDamage = 0;
            }
            weapon.range = w["range"].get<int>();
            weapon.weaponName = w["weaponName"].get<std::string>();
            weapons.push_back(weapon);
        }
        std::vector<std::string> keywords;
        for (const auto &keyword : unit["keywords"]) {
            keywords.push_back(keyword);
        }
        std::string unitName = unit["unitName"];
        Unit unitToAdd(modelCount, healthPerModel, floatingDamage, save, ward, weapons, keywords, unitName);
        units[unitId] = unitToAdd;
        unitId++;
    }
}

json loadJsonFiles(std::string &factionName) {
    std::cout << "Loading faction: " << factionName << std::endl;
    for (char &c : factionName) {
        c = tolower(c);

        if (c == ' ') {
            c = '_';
        }
    }

    std::ifstream factionRawData("data/factions/" + factionName + "_-_library" + ".json");
    if (!factionRawData) {
        std::cerr << "Could not open factions.json, try again. \n" << std::endl;
        return json();
    }
    json factionData;

    factionRawData >> factionData;

    return factionData;
}