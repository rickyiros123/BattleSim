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
    Unit(int modelCount, int healthPerModel, int floatingDamage, int save, int ward, vector<Weapon> weapons, vector<string> keywords, std::string unitName);                           
    int modelCount, healthPerModel, floatingDamage, save, ward;  
    vector<Weapon> weapons;
    vector<string> keywords;
    string unitName;
};

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


class Faction{
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
        for (const auto& w : unit["weapons"]) {
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
        for(const auto& keyword: unit["keywords"]){
            keywords.push_back(keyword);
        }
        string unitName = unit["unitName"];
        Unit unitToAdd(modelCount, healthPerModel, floatingDamage, save, ward, weapons, keywords, unitName);
        units.push_back(unitToAdd);
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

// Here’s a detailed review of your function’s logic, including potential issues and improvements:

// Dice Roll Sequence:
// You roll to hit, then to wound (using successful hits), then to save (using successful wounds). This is correct for most wargame systems.
// Wounds Inflicted Calculation:
// summary.woundsInflicted = summary.woundResult.successfulRolls - summary.saveResult.successfulRolls;
// This is correct: wounds that are not saved go through.
// Ward Save:
// If defender.ward > 0, you roll ward saves for woundsInflicted and subtract successful ward saves.
// This is correct, but you should check for negative woundsInflicted after this subtraction.
// Model Reduction Logic:
// The while loop and defender.modelCount -= (summary.woundsInflicted / defender.healthPerModel); is problematic:
// If woundsInflicted < healthPerModel, this subtracts zero models, and the leftover wounds are not tracked as floating damage.
// The loop and subtraction logic can result in incorrect model reduction if woundsInflicted is not a multiple of healthPerModel.
// summary.woundsInflicted -= defender.healthPerModel; inside the loop is not correct if you’re subtracting multiple models at once.
// Floating Damage:
// defender.floatingDamage -= summary.woundsInflicted; is not correct. Floating damage should accumulate leftover wounds that don’t kill a model, not subtract from a pool.
// Negative Values:
// There are no checks to prevent negative woundsInflicted, modelCount, or floatingDamage.
// No Handling for Overkill:
// If woundsInflicted is very high, modelCount could go negative.
// Summary of Issues:

// Model reduction and floating damage logic is not robust.
// Negative values are possible.
// Overkill is not handled.
// Recommendation:

// After all saves, calculate total wounds to apply: woundsInflicted + floatingDamage.
// Calculate models lost: modelsLost = totalWounds / healthPerModel.
// Update modelCount: subtract modelsLost, clamp to zero.
// Update floatingDamage: floatingDamage = totalWounds % healthPerModel.
// If modelCount == 0, set floatingDamage = 0.
// If you want, I can update the function to address all these issues for you


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
    cout << "Loading faction: " << factionName << endl;
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



// int main() {
//     std::srand(static_cast<unsigned int>(std::time(nullptr)));  // Seed RNG
//     std::string yourFaction = "";
//     std::string opponentFaction = "";
//     json attackerfaction;
//     json opponentFactionData;

//     while (attackerfaction.empty()) {
//         cout << "Enter the attckers faction name: ";
//         getline(std::cin, yourFaction);
//         attackerfaction = loadJsonFiles(yourFaction);
//         cout << endl;
//         cout << "What is the attacking unit" << endl;

//     }

//     while (opponentFactionData.empty()) {
//         cout << "Enter your opponent's faction name: ";
//         getline(std::cin, opponentFaction);
//         opponentFactionData = loadJsonFiles(opponentFaction);
//         cout << endl;
//     }

//     int input;
//     while(input != 3){
//         cout << "======Welcome to the Battle Sim======" << endl;
//         cout << "What would you like to do?" << endl;
//         cout << "1. Set up an battle." << endl;
//         cout << "2. Make two units fight" << endl;
//         cout << "3. Close program" << endl;
//         switch (input){
//             case 1: 
//                 cout << "Set up battle place holder" << endl;
//                 break;
//             case 2:
//                 cout << "Make two units fight place holder" << endl;
//                 break;
//             case 3: 
//                 cout << "Close programer placeholder" << endl;
//                 break;
//             default:
//                 cout << "Invalid optio, try again" << endl;
//                 break;
//         }
//     }


//     return 0;
// }


// Assuming all your structs, classes, and functions like Unit, Weapon, Faction,
// roll_dice, resolveAttack, battleSequence, and loadJsonFiles are already defined.

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // Seed RNG

    // Example faction names matching your JSON files (use actual names you have)
    std::string attackerFactionName = "seraphon";
    std::string defenderFactionName = "ossiarch_bonereapers";

    // Load factions from JSON
    json attackerJson = loadJsonFiles(attackerFactionName);
    json defenderJson = loadJsonFiles(defenderFactionName);

    if (attackerJson.empty() || defenderJson.empty()) {
        std::cerr << "Failed to load faction data. Make sure JSON files exist and are correct.\n";
        return 1;
    }

    // Create vectors to hold units
    std::vector<Unit> attackerUnits;
    std::vector<Unit> defenderUnits;

    // Populate units from JSON
    Faction attackerFaction;
    attackerFaction.populateFaction(attackerJson, attackerUnits);

    Faction defenderFaction;
    defenderFaction.populateFaction(defenderJson, defenderUnits);

    // Pick first unit from each faction for testing
    if (attackerUnits.empty() || defenderUnits.empty()) {
        std::cerr << "No units found in one of the factions.\n";
        return 1;
    }
    Unit attacker = attackerUnits[0];
    Unit defender = defenderUnits[0];

    std::cout << "Starting battle: " << attacker.unitName << " vs " << defender.unitName << "\n";

    // Run battle sequence
    battleSequence(attacker, defender);

    // Print results
    std::cout << "After battle:\n";
    std::cout << defender.unitName << " has " << defender.modelCount << " models remaining.\n";
    std::cout << "Floating damage: " << defender.floatingDamage << "\n";

    return 0;
}

