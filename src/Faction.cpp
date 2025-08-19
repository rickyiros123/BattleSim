#include "Faction.h"

#include <fstream>

#include "Dice.h"

void Faction::populateFaction(const json &factionData, std::unordered_map<int, Unit> &units) {
    std::cout << "Populating faction with data from JSON." << std::endl;
    int unitId = 0;  // Initialize unit ID for unique identification
    for (const auto &unit : factionData["units"]) {
        std::cout << "Adding: " << unit["unitName"] << std::endl;
        int modelCount = unit["modelCount"].get<int>();
        int healthPerModel = unit["healthPerModel"].get<int>();
        int floatingDamage = unit["floatingDamage"].get<int>();
        int save = unit["save"].get<int>();
        int ward = unit["ward"].get<int>();
        std::vector<Weapon> weapons;
        for (const auto &w : unit["weapons"]) {
            Weapon weapon;
            if( w["numberOfAttacks"].is_number_integer()) {
                weapon.numberOfAttacks = w["numberOfAttacks"].get<int>();
            } else if (w["numberOfAttacks"].is_string()) {
                weapon.numberOfAttacks = w["numberOfAttacks"].get<std::string>();
            }
            weapon.toHit = w["toHit"].get<int>();
            weapon.toWound = w["toWound"].get<int>();
            weapon.rend = w["rend"].get<int>();
            // Handle weaponDamage as int or dice notation string
            if (w["weaponDamage"].is_number_integer()) {
                weapon.weaponDamage = w["weaponDamage"].get<int>();
            }
            else if (w["weaponDamage"].is_string()) {
                weapon.weaponDamage = w["weaponDamage"].get<std::string>();
            }
            weapon.range = w["range"].get<int>();
            weapon.weaponName = w["weaponName"].get<std::string>();
            std::cout << "Weapon added" << std::endl;   
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