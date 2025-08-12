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
        cout << "Adding unit: " << unit["unitName"] << endl;
        cout << "  Extracting modelCount..." << endl;
        int modelCount = unit["modelCount"].get<int>();
        cout << "  Extracting healthPerModel..." << endl;
        int healthPerModel = unit["healthPerModel"].get<int>();
        cout << "  Extracting floatingDamage..." << endl;
        int floatingDamage = unit["floatingDamage"].get<int>();
        cout << "  Extracting save..." << endl;
        int save = unit["save"].get<int>();
        cout << "  Extracting ward..." << endl;
        int ward = unit["ward"].get<int>();
        vector<Weapon> weapons;
        cout << "  Extracting weapons..." << endl;
        for (const auto& w : unit["weapons"]) {
            Weapon weapon;
            cout << "    Extracting numberOfAttacks..." << endl;
            weapon.numberOfAttacks = w["numberOfAttacks"].get<int>();
            cout << "    Extracting toHit..." << endl;
            weapon.toHit = w["toHit"].get<int>();
            cout << "    Extracting toWound..." << endl;
            weapon.toWound = w["toWound"].get<int>();
            cout << "    Extracting rend..." << endl;
            weapon.rend = w["rend"].get<int>();
            cout << "    Extracting weaponDamage..." << endl;
            weapon.weaponDamage = w["weaponDamage"].get<int>();
            cout << "    Extracting range..." << endl;
            weapon.range = w["range"].get<int>();
            cout << "    Extracting weaponName..." << endl;
            weapon.weaponName = w["weaponName"].get<string>();
            weapons.push_back(weapon);
        }
        vector<string> keywords;
        cout << "  Extracting keywords..." << endl;
        for(const auto& keyword: unit["keywords"]){
            keywords.push_back(keyword);
        }
        cout << "  Extracting unitName..." << endl;
        string unitName = unit["unitName"];
        cout << "  Creating Unit object..." << endl;
        Unit unitToAdd(modelCount, healthPerModel, floatingDamage, save, ward, weapons, keywords, unitName);
        units.push_back(unitToAdd);
        cout << "  Unit added successfully.\n" << endl;
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
    std::string attackerFactionName = "Seraphon";
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
