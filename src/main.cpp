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

struct Weapon {
    int numberOfAttacks, toHit, toWound, rend, weaponDamage, range;
    string weaponName;
};

class Unit {                                                     // The class
   public:                                                       // Access specifier
    int modelCount, healthPerModel, floatingDamage, save, ward;  // Attribute (int variable)
    vector<Weapon> weapons;
    vector<string> keywords;
    string unitName;
};

RollResult roll_d6(int numberOfDice, int numberOfSides, int desiredRoll, int critValue) {
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

void battleSequence(Unit attacker, Unit &defender) {
    RollResult hitResult = roll_d6(attacker.weapons[0].numberOfAttacks, 6, attacker.weapons[0].toHit, 6);
    RollResult woundResult = roll_d6(hitResult.successfulRolls, 6, attacker.weapons[0].toWound, 6);
    RollResult saveResult = roll_d6(woundResult.successfulRolls, 6, defender.save, 6);
    int wounds = woundResult.successfulRolls - saveResult.successfulRolls;
    if (defender.ward > 0) {
        RollResult wardSaveResult = roll_d6(wounds, 6, defender.ward, 6);
        wounds = wounds - wardSaveResult.successfulRolls;
    }
    while (wounds >= defender.healthPerModel && defender.modelCount) {
        defender.modelCount = defender.modelCount - (wounds / defender.healthPerModel);
        wounds = wounds - defender.healthPerModel;
    }
    defender.floatingDamage = defender.floatingDamage - wounds;

    // pseudocode for resolve attack function:
    //     function resolveAttack(attackingWeapon, defender):
    //     hits = roll_dice(attackingWeapon.numberOfAttacks, attackingWeapon.toHit)
    //     wounds = roll_dice(hits.successes, attackingWeapon.toWound)
    //     saves = roll_dice(wounds.successes, defender.save)

    //     wounds_inflicted = wounds.successes - saves.successes

    //     if defender has ward save:
    //         ward_saves = roll_dice(wounds_inflicted, defender.ward)
    //         wounds_inflicted = wounds_inflicted - ward_saves.successes

    //     apply wounds_inflicted to defender's health and model count

    //     return summary of hits, wounds, saves, wounds_inflicted, updated defender status

    // function battleSequence(attacker, defender):
    //     for each weapon in attacker.weapons:
    //         attack_result = resolveAttack(weapon, defender)
    //         update defender with attack_result
    //         if defender is defeated:
    //             break
}

json loadJsonFiles(std::string factionName) {
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
        cin >> yourFaction;
        yourFactionData = loadJsonFiles(yourFaction);
    }

    while (opponentFactionData.empty()) {
        cout << "Enter your opponent's faction name: ";
        cin >> opponentFaction;
        opponentFactionData = loadJsonFiles(opponentFaction);
    }

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