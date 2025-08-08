#include <cstdlib>
#include <ctime>
#include <iostream>

struct RollResult {
  int successfulRolls;
  int crits;
};

RollResult roll_d6(int numberOfDice, int numberOfSides, int desiredRoll,
                   int critValue) {

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



int main() {
  std::srand(static_cast<unsigned int>(std::time(nullptr))); // Seed RNG

  RollResult rollResult = roll_d6(20, 6, 5, 6);
  std::cout << "20d6, 5s to hit: " << rollResult.successfulRolls << " hits and "
            << rollResult.crits << " crits!" << std::endl;
  return 0;
}