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
    int modelCount, healthPerModel, floatingDamage, save, ward;  
    vector<Weapon> weapons;
    vector<string> keywords;
    string unitName;
};

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
        cout << c;
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

void displayUnitNames(const json &factionData) {
    for (const auto &unit : factionData["units"]) {
        cout << unit["unitName"].get<string>() << endl;
    }
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));  // Seed RNG
    std::string yourFaction = "";
    std::string opponentFaction = "";
    json yourFactionData;
    json opponentFactionData;

    while (yourFactionData.empty()) {
        cout << "Enter your faction name: ";
        getline(std::cin, yourFaction);
        yourFactionData = loadJsonFiles(yourFaction);
        cout << endl;
    }

    while (opponentFactionData.empty()) {
        cout << "Enter your opponent's faction name: ";
        getline(std::cin, opponentFaction);
        opponentFactionData = loadJsonFiles(opponentFaction);
        cout << endl;
    }

    displayUnitNames(yourFactionData);
    displayUnitNames(opponentFactionData);

    // Unit attacker, defender;
    // Weapon osbWeapon;
    // attacker.weapons.push_back(osbWeapon);

    // attacker.weapons[0].toHit = ossiarch_bonereapersData["units"][0]["weapons"][0]["toHit"];
    // attacker.weapons[0].toWound = ossiarch_bonereapersData["units"][0]["weapons"][0]["toWound"];
    // attacker.weapons[0].weaponDamage = ossiarch_bonereapersData["units"][0]["weapons"][0]["weaponDamage"];
    // attacker.weapons[0].rend = ossiarch_bonereapersData["units"][0]["weapons"][0]["rend"];
    // attacker.weapons[0].numberOfAttacks = ossiarch_bonereapersData["units"][0]["weapons"][0]["numberOfAttacks"];
    // attacker.unitName = ossiarch_bonereapersData["units"][0]["unitName"].get<std::string>();
    // attacker.weapons[0].weaponName = ossiarch_bonereapersData["units"][0]["weapons"][0]["weaponName"].get<std::string>();

    return 0;
}