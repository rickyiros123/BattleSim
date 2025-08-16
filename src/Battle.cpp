#include "Battle.h"

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