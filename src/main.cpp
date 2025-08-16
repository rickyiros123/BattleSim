#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

#include "Battle.h"
#include "Dice.h"
#include "Faction.h"
#include "Unit.h"
#include "json.hpp"

using json = nlohmann::json;

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
        std::cin >> choice;
        switch (choice) {
            case 'a':
                std::cout << "Option A: Make 2 units fight eachother!\n" << std::endl;
                std::cout << "Let's get started! We'll need 2 factoins to start off." << std::endl;
                std::cout << "Choose from this list, enter digit next to faciton." << std::endl;
                std::cout << "1. Ossiarch Bonereapers" << std::endl;
                std::cout << "2. Seraphon" << std::endl;
                std::cin >> faction1;
                if (faction1 == 1) {
                    std::string faction1Name = "ossiarch_bonereapers";  // fixed typo
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
                    std::cout << "Choose a unit." << std::endl;
                    for (const auto &unit : faction1Units) {
                        std::cout << unit.unitName << std::endl;
                    }
                } else if (faction1 == 2) {
                    std::string faction1Name = "seraphon";
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
                    std::cout << "Choose a unit." << std::endl;
                    for (const auto &unit : faction1Units) {
                        std::cout << unit.unitName << std::endl;
                    }
                }
                break;
            case 'b':
                std::cout << "Choice B" << std::endl;
                break;
            case 'x':
                std::cout << "Choice x" << std::endl;
                break;
            default:
                std::cout << "Invalid choice." << std::endl;
                break;
        }
    }

    return 0;
}