#include "Battle.h"

#include <string>
#include <unordered_map>
#include <iostream>
#include <variant>
#include <chrono>
#include "Dice.h"
#include "Faction.h"
#include "json.hpp"
using json = nlohmann::json;

#include "Unit.h"

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

AttackSummary resolveAttack(const Unit &attacker, Weapon attackingWeapon, Unit &defender) {
    AttackSummary summary;
    // numberOfAttacks is a variant<int,string> -> extract int safely
    auto get_int_from_variant = [](const std::variant<int, std::string> &v, int def) {
        if (std::holds_alternative<int>(v)) return std::get<int>(v);
        try {
            return std::stoi(std::get<std::string>(v));
        } catch (...) {
            return def;
        }
    };

    int attacks = get_int_from_variant(attackingWeapon.numberOfAttacks, 1) * attacker.modelCount;
    summary.hitResult = roll_dice(attacks, 6, attackingWeapon.toHit, 6);
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


// Still working on this function
// This function will assign combat between friendly and enemy units based on user input
void assignCombat(std::unordered_map<int, Unit> &friendlyUnits, std::unordered_map<int, Unit> &enemyUnits){
    std::unordered_map<Unit, Unit> combatAssignments;
    for(auto &friendlyUnit : friendlyUnits) {
        std::cout << "Who will " << friendlyUnit.second.unitName << " attack?" << std::endl;
        for(const auto &enemyUnit : enemyUnits) {
            std::cout << enemyUnit.first << ": " << enemyUnit.second.unitName << std::endl;
        }
        int enemyId = 0;

    }
}
