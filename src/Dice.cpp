#include <Dice.h>

RollResult roll_dice(int numberOfDice, int numberOfSides, int desiredRoll, int critValue) {
    RollResult rollResult{0, 0};

    static std::mt19937 rng(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));

    std::uniform_int_distribution<int> dist(1, numberOfSides);
    for (int i = 0; i < numberOfDice; i++) {
        int roll = dist(rng);
        if (roll >= desiredRoll) {
            rollResult.successfulRolls++;
        }
        if (roll == critValue) {
            rollResult.crits++;
        }
    }
    return rollResult;
}