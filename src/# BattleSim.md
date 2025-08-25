# BattleSim

BattleSim is a C++ project aiming to simulate tabletop-style battles between customizable units and factions. The goal is to provide a flexible engine where users can define units, weapons, and factions using JSON files, then run automated or interactive battles to see how different strategies and army compositions perform.

## Vision

BattleSim will become a cross-platform app (desktop and mobile) that lets players:
- Create and edit factions, units, and weapons.
- Simulate battles with detailed combat resolution (hit, wound, save, ward, etc.).
- Assign which units fight which enemy units, either manually or automatically.
- View summaries of attacks, casualties, and outcomes.
- Experiment with rules, dice mechanics, and unit stats for game design or fun.

## Project Structure

```
BattleSim/
├── include/      # Header files (.h)
│   ├── Battle.h
│   ├── Unit.h
│   └── ...
├── src/          # Source files (.cpp)
│   ├── Battle.cpp
│   ├── Faction.cpp
│   ├── Unit.cpp
│   ├── main.cpp
│   └── ...
├── build/        # Compiled object files (.o) [created by Makefile]
├── data/         # Game data (e.g., factions, units) in JSON
│   └── factions/
│       └── <faction>_-_library.json
├── Makefile      # Build instructions
└── README.md     # Project documentation
```

## Building

To build the project, make sure you have `g++` and `make` installed.

**On Windows (MSYS2/MinGW):**
```sh
make
```

**On Linux/macOS:**
```sh
make
```

This will produce the executable `battle-sim` in the project root.

To clean up build artifacts:
```sh
make clean
```

## Running

Run the simulation with:
```sh
./battle-sim
```
or on Windows:
```sh
battle-sim.exe
```

## Adding Factions and Units

- Faction/unit data is stored in JSON files under `data/factions/`.
- Each faction should have a file named `<faction>_-_library.json`.
- Example structure for a unit in JSON:
  ```json
  {
    "units": [
      {
        "unitName": "Warrior",
        "modelCount": 10,
        "healthPerModel": 1,
        "floatingDamage": 0,
        "save": 5,
        "ward": 6,
        "weapons": [
          {
            "numberOfAttacks": 2,
            "toHit": 4,
            "toWound": 4,
            "rend": -1,
            "weaponDamage": "D3",
            "range": 1,
            "weaponName": "Sword"
          }
        ],
        "keywords": ["Infantry", "Melee"]
      }
    ]
  }
  ```

## Features

- **Unit and Weapon Customization:** Define units and weapons with flexible stats and dice mechanics.
- **Faction Management:** Organize units into factions for team-based battles.
- **Combat Assignment:** Choose which units fight which enemies, or let the engine auto-assign.
- **Detailed Battle Resolution:** Simulate attacks, saves, wounds, and casualties with output summaries.
- **Extensible Rules:** Easily modify or add rules for new game mechanics.

## Code Overview

- **Unit.h / Unit.cpp**: Defines `Unit` and `Weapon` structs/classes.
- **Battle.h / Battle.cpp**: Handles combat logic, attack resolution, and user assignment of combat pairs.
- **Faction.cpp**: Loads faction/unit data from JSON and populates units.
- **main.cpp**: Entry point for the simulation.

## Assigning Combat

The simulation allows the user to assign which friendly units fight which enemy units. This is handled by the `assignCombat` function in `Battle.cpp`.

## Building Tips

- The Makefile automatically finds all `.cpp` files in `src/` and compiles them.
- Object files are placed in `build/`.
- If you add new source files, just put them in `src/` and run `make`.

## Requirements

- C++17 or newer
- g++ compiler
- make utility
- [nlohmann/json](https://github.com/nlohmann/json) library (header-only, should be in your `include/` directory)

## Troubleshooting

- If you get linker errors about multiple definitions, ensure constructors are only defined in `.cpp` files or marked `inline` in headers.
- On Windows, use MSYS2/MinGW for best compatibility with the Makefile.
- For memory leak detection, use AddressSanitizer (Linux/WSL) or Dr. Memory (Windows).

---

Feel free to expand this README with more gameplay instructions, contributor info, or screenshots as