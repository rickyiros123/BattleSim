#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iostream>
#include <vector>

using namespace std;

struct RollResult
{
  int successfulRolls;
  int crits;
};

struct Weapon
{
  int numberOfAttacks, toHit, toWound, rend, weaponDamage, range;
  string weaponName;
};

class Unit
{                                                             // The class
public:                                                       // Access specifier
  int modelCount, healthPerModel, floatingDamage, save, ward; // Attribute (int variable)
  vector<Weapon> weapons;
  string unitName;
  vector<string> keywords;
};

RollResult roll_d6(int numberOfDice, int numberOfSides, int desiredRoll, int critValue)
{
  RollResult rollResult{0, 0};
  for (int i = 0; i < numberOfDice; i++)
  {
    int roll = 0;
    roll = (std::rand() % numberOfSides) + 1;
    if (roll >= desiredRoll)
    {
      rollResult.successfulRolls++;
    }
    if (roll == critValue)
    {
      rollResult.crits++;
    }
  }
  return rollResult;
}

// void combatSim(Unit unitA, Unit &unitB)
// {
//   int attacks = unitA.models * unitA.attacks;
//   RollResult attackerHitResult = roll_d6(attacks, 6, unitA.toHit, 6);
//   RollResult attackerWoundResult = roll_d6(attackerHitResult.successfulRolls, 6, unitA.toWound, 6);
//   RollResult defenderSavesMade = roll_d6(attackerWoundResult.successfulRolls, 6, unitB.save, 6);
//   int damage = attackerWoundResult.successfulRolls - defenderSavesMade.successfulRolls;
//   unitB.totalWounds = unitB.totalWounds - damage;
//   unitB.models = unitB.totalWounds / unitB.healthPerModel;
// }

int main()
{
  std::srand(static_cast<unsigned int>(std::time(nullptr))); // Seed RNG

  RollResult rollResult = roll_d6(20, 6, 5, 6);
  std::cout << "20d6, 5s to hit: " << rollResult.successfulRolls << " hits and " << rollResult.crits << " crits!" << std::endl;
  return 0;
}