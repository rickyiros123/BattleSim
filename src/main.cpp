#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iostream>
#include <fstream>
#include <vector>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

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
  vector<string> keywords;
  string unitName;
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

void battleSequence(Unit attacker, Unit &defender)
{
  RollResult hitResult = roll_d6(attacker.weapons[0].numberOfAttacks, 6, attacker.weapons[0].toHit, 6);
  RollResult woundResult = roll_d6(hitResult.successfulRolls, 6, attacker.weapons[0].toWound, 6);
  RollResult saveResult = roll_d6(woundResult.successfulRolls, 6, defender.save, 6);
  int wounds = woundResult.successfulRolls - saveResult.successfulRolls;
  if (defender.ward > 0)
  {
    RollResult wardSaveResult = roll_d6(wounds, 6, defender.ward, 6);
    wounds = wounds - wardSaveResult.successfulRolls;
  }
  while (wounds >= defender.healthPerModel && defender.modelCount)
  {
    defender.modelCount = defender.modelCount - (wounds / defender.healthPerModel);
    wounds = wounds - defender.healthPerModel;
  }
  defender.floatingDamage = defender.floatingDamage - wounds;
}

int main()
{
  std::srand(static_cast<unsigned int>(std::time(nullptr))); // Seed RNG

  std::ifstream factionData("data/factions/seraphon.json");
  if (!factionData)
  {
    std::cerr << "Could not open factions.json\n";
    return 1;
  }

  json jsonData;

  factionData >> jsonData;

  cout << "Unit name: " << jsonData["units"][0]["unitName"].get<std::string>() << "\n";

  // RollResult rollResult = roll_d6(20, 6, 5, 6);
  // std::cout << "20d6, 5s to hit: " << rollResult.successfulRolls << " hits and " << rollResult.crits << " crits!" << std::endl;
  return 0;
}