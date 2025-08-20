#pragma once
#include <iostream>
#include <variant>
#include <vector>

struct Weapon {
    int numberOfAttacks;
    int toHit, toWound, rend, range;
    int weaponDamage;
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
