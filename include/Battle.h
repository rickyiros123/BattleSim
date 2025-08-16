#pragma once
#include <Dice.h>

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

AttackSummary resolveAttack(const Unit &attacker, Weapon attackingWeapon, Unit &defender);