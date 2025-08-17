#pragma once
#include <iostream>
#include <vector>

struct Weapon {
    int numberOfAttacks, toHit, toWound, rend, weaponDamage, range;
    std::string weaponName;
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
