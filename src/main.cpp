#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>

#include "json.hpp"

using namespace std;
using json = nlohmann::json;

struct RollResult {
    int successfulRolls;
    int crits;
};

struct AttackSummary {
    RollResult hitResult;
    RollResult woundResult;
    RollResult saveResult;
    RollResult wardSaveResult;
    int woundsInflicted;
    int modelsLost;
};

struct Weapon {
    int numberOfAttacks, toHit, toWound, rend, weaponDamage, range;
    string weaponName;
};

class Unit {                                                     
   public:                            
    Unit();                           
    int modelCount, healthPerModel, floatingDamage, save, ward;  
    vector<Weapon> weapons;
    vector<string> keywords;
    string unitName;
};

Unit::Unit(json faction){

}

class Faction{
    public:
    vector<Unit> Units;
};

void displayUnitNames(const json &factionData) {
    for (const auto &unit : factionData["units"]) {
        cout << unit["unitName"].get<string>() << endl;
    }
}

RollResult roll_dice(int numberOfDice, int numberOfSides, int desiredRoll, int critValue) {
    RollResult rollResult{0, 0};
    for (int i = 0; i < numberOfDice; i++) {
        int roll = 0;
        roll = (std::rand() % numberOfSides) + 1;
        if (roll >= desiredRoll) {
            rollResult.successfulRolls++;
        }
        if (roll == critValue) {
            rollResult.crits++;
        }
    }
    return rollResult;
}

AttackSummary resolveAttack(Weapon attackingWeapon, Unit &defender) {
    AttackSummary summary;

    summary.hitResult = roll_dice(attackingWeapon.numberOfAttacks, 6, attackingWeapon.toHit, 6);
    summary.woundResult = roll_dice(summary.hitResult.successfulRolls, 6, attackingWeapon.toWound, 6);
    summary.saveResult = roll_dice(summary.woundResult.successfulRolls, 6, defender.save, 6);

    summary.woundsInflicted = summary.woundResult.successfulRolls - summary.saveResult.successfulRolls;

    if (defender.ward > 0) {
        summary.wardSaveResult = roll_dice(summary.woundsInflicted, 6, defender.ward, 6);
        summary.woundsInflicted -= summary.wardSaveResult.successfulRolls;
    }

    // Apply woundsInflicted to defender's health and model count
    while (summary.woundsInflicted >= defender.healthPerModel && defender.modelCount) {
        defender.modelCount -= (summary.woundsInflicted / defender.healthPerModel);
        summary.woundsInflicted -= defender.healthPerModel;
    }
    defender.floatingDamage -= summary.woundsInflicted;

    return summary;
}

void battleSequence(Unit attacker, Unit &defender) {
    for (const auto &weapon : attacker.weapons) {
        AttackSummary attackResult = resolveAttack(weapon, defender);
        // Update defender with attackResult
        if (defender.modelCount == 0) {
            // Defender is defeated
            defender.floatingDamage = 0;
            break;
        }
    }
}

json loadJsonFiles(std::string &factionName) {
    for(char &c : factionName){
        c = tolower(c);
        
        if(c == ' '){
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



int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));  // Seed RNG
    std::string yourFaction = "";
    std::string opponentFaction = "";
    json attackerfaction;
    json opponentFactionData;

    while (attackerfaction.empty()) {
        cout << "Enter the attckers faction name: ";
        getline(std::cin, yourFaction);
        attackerfaction = loadJsonFiles(yourFaction);
        cout << endl;
        cout << "What is the attacking unit" << endl;

    }

    while (opponentFactionData.empty()) {
        cout << "Enter your opponent's faction name: ";
        getline(std::cin, opponentFaction);
        opponentFactionData = loadJsonFiles(opponentFaction);
        cout << endl;
    }

    int input;
    while(input != 3){
        cout << "======Welcome to the Battle Sim======" << endl;
        cout << "What would you like to do?" << endl;
        cout << "1. Set up an battle." << endl;
        cout << "2. Make two units fight" << endl;
        cout << "3. Close program" << endl;
        switch (input){
            case 1: 
                cout << "Set up battle place holder" << endl;
                break;
            case 2:
                cout << "Make two units fight place holder" << endl;
                break;
            case 3: 
                cout << "Close programer placeholder" << endl;
                break;
            default:
                cout << "Invalid optio, try again" << endl;
                break;
        }
    }


    return 0;
}