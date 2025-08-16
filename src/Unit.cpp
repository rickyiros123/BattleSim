#include <Unit.h>

Unit::Unit(int modelCount, int healthPerModel, int floatingDamage, int save, int ward, std::vector<Weapon> weapons, std::vector<std::string> keywords, std::string unitName) {
    this->modelCount = modelCount;
    this->healthPerModel = healthPerModel;
    this->floatingDamage = floatingDamage;
    this->save = save;
    this->ward = ward;
    this->weapons = weapons;
    this->keywords = keywords;
    this->unitName = unitName;
}

void printUnitStats(const Unit &unit) {
    std::cout << "Unit Name: " << unit.unitName << "\n";
    std::cout << "Model Count: " << unit.modelCount << "\n";
    std::cout << "Health Per Model: " << unit.healthPerModel << "\n";
    std::cout << "Save: " << unit.save << "\n";
    std::cout << "Ward: " << unit.ward << "\n";
    std::cout << "Floating Damage: " << unit.floatingDamage << "\n";
    std::cout << "Keywords: ";
    for (const auto &k : unit.keywords) {
        std::cout << k << " ";
    }
    std::cout << "\nWeapons:" << std::endl;
    for (const auto &w : unit.weapons) {
        std::cout << "  - " << w.weaponName << ": Attacks=" << w.numberOfAttacks << ", To Hit=" << w.toHit << ", To Wound=" << w.toWound << ", Rend=" << w.rend << ", Damage=" << w.weaponDamage << ", Range=" << w.range << std::endl;
    }
    std::cout << std::endl;
}
