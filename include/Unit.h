#pragma once
#include <iostream>
#include <variant>
#include <vector>

struct Weapon {
    std::variant<int, std::string> weaponDamage;
    std::variant<int, std::string> numberOfAttacks;
    int toHit, toWound, rend, range;
    std::string weaponName;

    Weapon() = default;
    ~Weapon() = default;
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

void printUnitSummary(const Unit &unit);
