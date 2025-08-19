#pragma once
#include <iostream>
#include <vector>
#include <variant>

struct Weapon {
    std::variant<int, std::string> numberOfAttacks;
    int toHit, toWound, rend, range;
    std::variant<int, std::string> weaponDamage;
    std::string weaponName;

    // Default constructor
};

class Unit {
   public:
    Unit(int modelCount, int healthPerModel, int floatingDamage, int save, int ward, std::vector<Weapon> weapons, std::vector<std::string> keywords, std::string unitName);
    Unit();
    int modelCount, healthPerModel, floatingDamage, save, ward;
    std::vector<Weapon> weapons;
    std::vector<std::string> keywords;
    std::string unitName;
};

void printUnitStats(const Unit &unit);
