#pragma once
#include <Dice.h>

#include <unordered_map>

#include "Unit.h"

struct AttackSummary {
    RollResult hitResult;
    RollResult woundResult;
    RollResult saveResult;
    RollResult wardSaveResult;
    int woundsInflicted = 0;
    int modelsLost = 0;
};

void battleSequence(Unit attacker, Unit &defender);
// Assigns combat between friendly and enemy units
// This function will assign combat based on user input
// It will take two unordered maps of units, one for friendly and one for enemy
// It will prompt the user to select which friendly unit attacks which enemy unit
// void assignCombat(std::unordered_map<int, Unit> &friendlyUnits, std::unordered_map<int, Unit> &enemyUnits);

AttackSummary resolveAttack(const Unit &attacker, Weapon attackingWeapon, Unit &defender);