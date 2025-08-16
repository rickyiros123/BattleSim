#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

#include "json.hpp"

using namespace std;
using json = nlohmann::json;

struct RollResult {
    int successfulRolls = 0;
    int crits = 0;
};

struct AttackSummary {
    RollResult hitResult;
    RollResult woundResult;
    RollResult saveResult;
    RollResult wardSaveResult;
    int woundsInflicted = 0;
    int modelsLost = 0;
};

struct Weapon {
    int numberOfAttacks, toHit, toWound, rend, weaponDamage, range;
    string weaponName;
};

class Unit {
   public:
    Unit(int modelCount, int healthPerModel, int floatingDamage, int save, int ward, vector<Weapon> weapons, vector<string> keywords, std::string unitName);
    int modelCount, healthPerModel, floatingDamage, save, ward;
    vector<Weapon> weapons;
    vector<string> keywords;
    string unitName;
};
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
Unit::Unit(int modelCount, int healthPerModel, int floatingDamage, int save, int ward, vector<Weapon> weapons, vector<string> keywords, std::string unitName) {
    this->modelCount = modelCount;
    this->healthPerModel = healthPerModel;
    this->floatingDamage = floatingDamage;
    this->save = save;
    this->ward = ward;
    this->weapons = weapons;
    this->keywords = keywords;
    this->unitName = unitName;
}

class Faction {
   public:
    void populateFaction(const json &factionData, vector<Unit> &units);
    vector<Unit> units;
};

void Faction::populateFaction(const json &factionData, vector<Unit> &units) {
    for (const auto &unit : factionData["units"]) {
        int modelCount = unit["modelCount"].get<int>();
        int healthPerModel = unit["healthPerModel"].get<int>();
        int floatingDamage = unit["floatingDamage"].get<int>();
        int save = unit["save"].get<int>();
        int ward = unit["ward"].get<int>();
        vector<Weapon> weapons;
        for (const auto &w : unit["weapons"]) {
            Weapon weapon;
            weapon.numberOfAttacks = w["numberOfAttacks"].get<int>();
            weapon.toHit = w["toHit"].get<int>();
            weapon.toWound = w["toWound"].get<int>();
            weapon.rend = w["rend"].get<int>();
            weapon.weaponDamage = w["weaponDamage"].get<int>();
            weapon.range = w["range"].get<int>();
            weapon.weaponName = w["weaponName"].get<string>();
            weapons.push_back(weapon);
        }
        vector<string> keywords;
        for (const auto &keyword : unit["keywords"]) {
            keywords.push_back(keyword);
        }
        string unitName = unit["unitName"];
        Unit unitToAdd(modelCount, healthPerModel, floatingDamage, save, ward, weapons, keywords, unitName);
        units.push_back(unitToAdd);
    }
}

RollResult roll_dice(int numberOfDice, int numberOfSides, int desiredRoll, int critValue) {
    RollResult rollResult{0, 0};

    static std::mt19937 rng(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));

    std::uniform_int_distribution<int> dist(1, numberOfSides);
    for (int i = 0; i < numberOfDice; i++) {
        int roll = dist(rng);
        if (roll >= desiredRoll) {
            rollResult.successfulRolls++;
        }
        if (roll == critValue) {
            rollResult.crits++;
        }
    }
    return rollResult;
}

AttackSummary resolveAttack(const Unit &attacker, Weapon attackingWeapon, Unit &defender) {
    AttackSummary summary;

    summary.hitResult = roll_dice(attackingWeapon.numberOfAttacks * attacker.modelCount, 6, attackingWeapon.toHit, 6);
    summary.woundResult = roll_dice(summary.hitResult.successfulRolls, 6, attackingWeapon.toWound, 6);
    summary.saveResult = roll_dice(summary.woundResult.successfulRolls, 6, defender.save, 6);

    summary.woundsInflicted = summary.woundResult.successfulRolls - summary.saveResult.successfulRolls;

    if (defender.ward > 0) {
        summary.wardSaveResult = roll_dice(summary.woundsInflicted, 6, defender.ward, 6);
        summary.woundsInflicted -= summary.wardSaveResult.successfulRolls;
    }

    defender.floatingDamage = defender.floatingDamage + summary.woundsInflicted;
    while (defender.floatingDamage >= defender.healthPerModel && defender.modelCount > 0) {
        defender.modelCount--;
        defender.floatingDamage -= defender.healthPerModel;
    }

    return summary;
}

void battleSequence(Unit attacker, Unit &defender) {
    for (const auto &weapon : attacker.weapons) {
        AttackSummary attackResult = resolveAttack(attacker, weapon, defender);
        // Update defender with attackResult
        if (defender.modelCount == 0) {
            // Defender is defeated
            defender.floatingDamage = 0;
            break;
        }
    }
}

json loadJsonFiles(std::string &factionName) {
    cout << "Loading faction: " << factionName << endl;
    for (char &c : factionName) {
        c = tolower(c);

        if (c == ' ') {
            c = '_';
        }
    }

    std::ifstream factionRawData("data/factions/" + factionName + ".json");
    if (!factionRawData) {
        std::cerr << "Could not open factions.json, try again. \n" << endl;
        return json();
    }
    json factionData;
    factionRawData >> factionData;

    return factionData;
}

void displayOpeningMenu() {
    std::cout << "=============================\n";
    std::cout << "   Welcome to BattleSim!\n";
    std::cout << "=============================\n";
    std::cout << "a) Make 2 units fight each other\n";
    std::cout << "b) Choice B\n";
    std::cout << "x) Exit\n";
    std::cout << "-----------------------------\n";
    std::cout << "Enter choice (a, b, x): ";
}

int main() {
    std::srand(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
    // Seed RNG

    // // Example faction names matching your JSON files (use actual names you
    // // have)
    // std::string attackerFactionName = "seraphon";
    // std::string defenderFactionName = "ossiarch_bonereapers";

    // // Load factions from JSON
    // json attackerJson = loadJsonFiles(attackerFactionName);
    // json defenderJson = loadJsonFiles(defenderFactionName);

    // if (attackerJson.empty() || defenderJson.empty()) {
    //     std::cerr << "Failed to load faction data. Make sure JSON files exist "
    //                  "and are correct.\n";
    //     return 1;
    // }

    // // Create vectors to hold units
    // std::vector<Unit> attackerUnits;
    // std::vector<Unit> defenderUnits;

    // // Populate units from JSON
    // Faction attackerFaction;
    // attackerFaction.populateFaction(attackerJson, attackerUnits);

    // Faction defenderFaction;
    // defenderFaction.populateFaction(defenderJson, defenderUnits);

    // // Pick first unit from each faction for testing
    // if (attackerUnits.empty() || defenderUnits.empty()) {
    //     std::cerr << "No units found in one of the factions.\n";
    //     return 1;
    // }
    // Unit attacker = attackerUnits[0];
    // Unit defender = defenderUnits[0];

    // std::cout << "Starting battle: " << attacker.unitName << " vs " << defender.unitName << "\n";

    // // Run battle sequence
    // battleSequence(attacker, defender);
    // battleSequence(defender, attacker);

    // // Print results
    // std::cout << "After battle:\n";
    // std::cout << defender.unitName << " has " << defender.modelCount << " models remaining.\n";
    // std::cout << "Floating damage: " << defender.floatingDamage << "\n\n";

    // std::cout << attacker.unitName << " has " << attacker.modelCount << " models remaining.\n";
    // std::cout << "Floating damage: " << attacker.floatingDamage << endl;

    int faction1;
    char choice = ' ';
    while (choice != 'x') {
        displayOpeningMenu();
        cin >> choice;
        switch (choice) {
            case 'a':
                cout << "Option A: Make 2 units fight eachother!\n" << endl;
                cout << "Let's get started! We'll need 2 factoins to start off." << endl;
                cout << "Choose from this list, enter digit next to faciton." << endl;
                cout << "1. Ossiarch Bonereapers" << endl;
                cout << "2. Seraphon" << endl;
                cin >> faction1;
                if (faction1 == 1) {
                    string faction1Name = "ossiarch_bonereapers";  // fixed typo
                    json faction1Json = loadJsonFiles(faction1Name);
                    if (faction1Json.empty()) {
                        std::cerr << "Failed to load faction data. Make sure JSON files exist "
                                     "and are correct.\n";
                        return 1;
                    }
                    std::vector<Unit> faction1Units;
                    Faction faction1;
                    faction1.populateFaction(faction1Json, faction1Units);
                    if (faction1Units.empty()) {
                        std::cerr << "No units found in faction.\n";
                        return 1;
                    }
                    cout << "Choose a unit." << endl;
                    for (const auto &unit : faction1Units) {
                        std::cout << unit.unitName << std::endl;
                    }
                } else if (faction1 == 2) {
                    string faction1Name = "seraphon";
                    json faction1Json = loadJsonFiles(faction1Name);
                    if (faction1Json.empty()) {
                        std::cerr << "Failed to load faction data. Make sure JSON files exist "
                                     "and are correct.\n";
                        return 1;
                    }
                    std::vector<Unit> faction1Units;
                    Faction faction1;
                    faction1.populateFaction(faction1Json, faction1Units);
                    if (faction1Units.empty()) {
                        std::cerr << "No units found in faction.\n";
                        return 1;
                    }
                    cout << "Choose a unit." << endl;
                    for (const auto &unit : faction1Units) {
                        std::cout << unit.unitName << std::endl;
                    }
                }
                break;
            case 'b':
                cout << "Choice B" << endl;
                break;
            case 'x':
                cout << "Choice x" << endl;
                break;
            default:
                cout << "Invalid choice." << endl;
                break;
        }
    }

    return 0;
}