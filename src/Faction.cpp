#include "Faction.h"

#include <fstream>

void Faction::populateFaction(const json &factionData, std::vector<Unit> &units) {
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
            weapon.weaponDamage = w["weaponDamage"].get<int>();
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
        units.push_back(unitToAdd);
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

    std::ifstream factionRawData("data/factions/" + factionName + ".json");
    if (!factionRawData) {
        std::cerr << "Could not open factions.json, try again. \n" << std::endl;
        return json();
    }
    json factionData;
    factionRawData >> factionData;

    return factionData;
}