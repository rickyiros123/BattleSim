#pragma once
#include <chrono>
#include <random>

struct RollResult {
    int successfulRolls;
    int crits;
};

RollResult roll_dice(int numberOfDice, int numberOfSides, int desiredRoll, int critValue);