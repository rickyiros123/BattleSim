#include "Faction.h"

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "Dice.h"

Unit createUnitFromJson(json &factionJson, std::string unitName) {
    for (const auto &unit : factionJson["units"]) {
        std::string jsonUnitName = unit["unitName"].get<std::string>();
        if (jsonUnitName == unitName) {
            std::vector<Weapon> weapons = parseWeapons(unit["weapons"]);
            std::vector<std::string> keywords = unit["keywords"].get<std::vector<std::string>>();
            Unit unitToReturn(unit.value("modelCount", 1), unit.value("healthPerModel", 1), unit.value("unitFloatingDamage", 0), unit.value("save", 0), unit.value("ward", 0), weapons, keywords, unit.value("unitName", "Unknown Unit"));
            return unitToReturn;
        }
    }

    return Unit();
}

Unit createUnitFromJson(const json &factionJson, int unitId) {
    if (!factionJson.contains("units")) return Unit();
    if (unitId < 1) return Unit();

    size_t index = static_cast<size_t>(unitId - 1);
    const auto &unitsArray = factionJson["units"];
    if (index >= unitsArray.size()) return Unit();

    const auto &unit = unitsArray[index];

    std::vector<Weapon> weapons = parseWeapons(unit.value("weapons", json::array()));
    std::vector<std::string> keywords = unit.value("keywords", std::vector<std::string>());

    Unit result(unit.value("modelCount", 1), unit.value("healthPerModel", 1), unit.value("unitFloatingDamage", 0), unit.value("save", 0), unit.value("ward", 0), weapons, keywords, unit.value("unitName", std::string("Unknown Unit")));

    return result;
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

std::unordered_map<int, std::string> createFactionUnits(const json &factionJson) {
    std::unordered_map<int, std::string> unitNames;
    int id = 1;
    for (const auto &unit : factionJson["units"]) {
        std::string name = unit.value("unitName", std::string("Unknown Unit"));
        unitNames[id] = name;
        ++id;
    }
    return unitNames;
}

void printFactionUnits(const std::unordered_map<int, std::string> &units) {
    std::cout << "Faction Units:\n";
    for (int i = 1; i <= static_cast<int>(units.size()); ++i) {
        auto it = units.find(i);
        if (it != units.end()) {
            std::cout << i << ". " << it->second << "\n";
        }
    }
}

// Helper: parse weapons array from JSON into vector<Weapon>
std::vector<Weapon> parseWeapons(const json &weaponJson) {
    std::vector<Weapon> weapons;
    if (!weaponJson.is_array()) return weapons;
    for (const auto &w : weaponJson) {
        Weapon weapon;
        weapon.weaponName = w.value("weaponName", std::string("Unknown Weapon"));
        weapon.numberOfAttacks = w.value("numberOfAttacks", 1);
        weapon.toHit = w.value("toHit", 0);
        weapon.toWound = w.value("toWound", 0);
        weapon.rend = w.value("rend", 0);
        weapon.weaponDamage = w.value("weaponDamage", 0);
        weapon.range = w.value("range", 0);
        weapons.push_back(std::move(weapon));
        std::cout << "Weapon added" << std::endl;
    }
    return weapons;
}