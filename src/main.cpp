#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

#include "Battle.h"
#include "Dice.h"
#include "Faction.h"
#include "Unit.h"
#include "json.hpp"
#include "utils.h"

using json = nlohmann::json;

void displayMainMenu() {
    clearScreen();
    std::cout << "=============================\n";
    std::cout << "   Welcome to BattleSim!\n";
    std::cout << "=============================\n";
    std::cout << "a) Set up a battle\n";
    std::cout << "b) Choice B\n";
    std::cout << "x) Exit\n";
    std::cout << "-----------------------------\n";
    std::cout << "Enter choice (a, b, x): ";
}

int main() {
    std::srand(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));  // Seed RNG

    char choice = ' ';
    while (choice != 'x') {
        displayMainMenu();
        {
            std::string line;
            if (!readLine(line)) return 0;
            if (line.empty()) continue;
            choice = line[0];
        }
        int factionChoice = 0;
        int factionChoice2 = 0;
        switch (choice) {
            case 'a': {
                clearScreen();
                std::cout << "=============================\n";
                std::cout << "   Battle Sim Setup\n";
                std::cout << "=============================\n";
                std::cout << "\nLet's get started! We'll need your faction.\n";
                std::cout << "Choose from this list, enter digit next to faction.\n";
                std::cout << "1. Ossiarch Bonereapers\n";
                std::cout << "2. Seraphon\n";
                std::cout << "-----------------------------\n";
                std::cout << "Enter choice (1 or 2): ";
                {
                    std::string line;
                    if (!readLine(line)) return 0;
                    try {
                        factionChoice = std::stoi(line);
                    } catch (...) {
                        clearScreen();
                        std::cout << "Invalid choice. Returning to menu." << std::endl;
                        break;
                    }
                }

                std::string userFactionName;
                std::string opponentFaction;
                if (factionChoice == 1) {
                    userFactionName = "ossiarch_bonereapers";
                } else if (factionChoice == 2) {
                    userFactionName = "seraphon";
                } else {
                    clearScreen();
                    std::cout << "Invalid choice. Returning to menu.\n";
                    break;
                }

                json factionJson = loadFactionData(userFactionName);
                if (factionJson.empty()) {
                    clearScreen();
                    std::cerr << "Failed to load faction data. Make sure JSON files exist and are correct.\n";
                    break;
                }

                std::unordered_map<int, std::string> factionUnits = buildFactionUnitMap(factionJson);
                std::unordered_map<int, Unit> userArmyList;

                clearScreen();
                std::cout << "Now let's select your units\n";
                std::cout << "Enter your unit IDs and then enter 'x' when you are finished\n";
                std::cout << "-----------------------------\n";

                std::string input;
                buildUserArmyList(factionUnits, userArmyList, factionJson);
                std::cout << "\n=============================\n";
                std::cout << "   Battle Sim Setup\n";
                std::cout << "=============================\n";
                std::cout << "Choose from this list, enter digit next to faction.\n";
                std::cout << "Select your opponent units." << std::endl;
                std::cout << "1. Ossiarch Bonereapers\n";
                std::cout << "2. Seraphon\n";
                std::cout << "-----------------------------\n";
                std::cout << "Enter choice (1 or 2): ";
                {
                    std::string line;
                    if (!readLine(line)) return 0;
                    try {
                        factionChoice2 = std::stoi(line);
                    } catch (...) {
                        clearScreen();
                        std::cout << "Invalid choice. Returning to menu." << std::endl;
                        break;
                    }
                }

                if (factionChoice2 == 1) {
                    opponentFaction = "ossiarch_bonereapers";
                } else if (factionChoice2 == 2) {
                    opponentFaction = "seraphon";
                } else {
                    std::cout << "Invalid choice. Returning to menu.\n";
                    break;
                }

                json factionJson2 = loadFactionData(opponentFaction);
                if (factionJson2.empty()) {
                    clearScreen();
                    std::cerr << "Failed to load faction data. Make sure JSON files exist and are correct.\n";
                    break;
                }

                std::unordered_map<int, std::string> opponentFactionUnits = buildFactionUnitMap(factionJson2);

                std::unordered_map<int, Unit> opponentArmyList;
                std::string input2;
                clearScreen();
                std::cout << "Now let's select your opponene units\n";
                std::cout << "Enter their unit IDs and then enter 'x' when you are finished\n";
                std::cout << "-----------------------------\n";

                buildUserArmyList(opponentFactionUnits, opponentArmyList, factionJson2);
                break;
            }
        }
    }

    return 0;
}