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

void displayMainMenu() {
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

    char choice = ' ';
    while (choice != 'x') {
        displayMainMenu();
        std::cin >> choice;
        int factionChoice = 0;
        switch (choice) {
            case 'a': {
                int factionChoice = 0;
                std::cout << "\nLet's get started! We'll need 1 faction to start off.\n";
                std::cout << "Choose from this list, enter digit next to faction.\n";
                std::cout << "1. Ossiarch Bonereapers\n";
                std::cout << "2. Seraphon\n";
                std::cout << "-----------------------------\n";
                std::cout << "Enter choice (1 or 2): ";
                std::cin >> factionChoice;

                std::string factionName;
                if (factionChoice == 1) {
                    factionName = "ossiarch_bonereapers";
                } else if (factionChoice == 2) {
                    factionName = "seraphon";
                } else {
                    std::cout << "Invalid choice. Returning to menu.\n";
                    break;
                }

                json factionJson = loadFactionData(factionName);
                if (factionJson.empty()) {
                    std::cerr << "Failed to load faction data. Make sure JSON files exist and are correct.\n";
                    break;
                }

                std::unordered_map<int, std::string> factionUnits = buildFactionUnitMap(factionJson);
                printFactionUnitList(factionUnits);

                std::vector<Unit> userArmyList;
                std::string input;

                std::cout << "\nNow let's select your units\n";
                std::cout << "Enter your unit IDs and then enter 'x' when you are finished\n";
                std::cout << "-----------------------------\n";

                while (true) {
                    printFactionUnitList(factionUnits);
                    std::cout << "-----------------------------\n";
                    std::cout << "Unit ID: ";
                    std::cin >> input;

                    if (input == "x" || input == "X") {
                        break;  // exit loop
                    }

                    try {
                        int unitId = std::stoi(input);  // convert to int
                        if (factionUnits.count(unitId)) {
                            Unit chosenUnit = makeUnitFromJson(factionJson, unitId);
                            userArmyList.push_back(chosenUnit);

                            std::cout << "Added: " << factionUnits[unitId] << "\n";
                        } else {
                            std::cout << "Invalid ID, please try again.\n";
                        }
                    } catch (std::invalid_argument &) {
                        std::cout << "Invalid input, enter a number or 'x' to exit.\n";
                    }
                }
                for (const auto &unit : userArmyList) {
                    printUnitSummary(unit);
                }

                // Later: fight logic would use userArmyList
                break;
            }
        }
    }

    return 0;
}