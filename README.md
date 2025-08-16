
# BattleSim: Warhammer Age of Sigmar 4th Edition Battle Simulator

A C++ console-based battle simulator for Warhammer: Age of Sigmar, using JSON files to manage factions and units. Easily expand your armies and simulate battles between any units.

---

## Features

- Loads faction and unit data from JSON files using nlohmann/json
- Menu-driven console interface for user interaction
- Displays unit names and stats
- Simulates battles between units, including dice rolling and wound tracking
- Modular code structure for easy expansion
- Add or edit factions/units by simply updating JSON files

---

## Getting Started

### Prerequisites

- C++ compiler (GCC, MSVC, etc.)
- nlohmann/json single-header library (included in `include/`)

### Installation

1. Clone the repository
2. Compile the source code:
   ```bash
   g++ src/main.cpp -I include -o battle-sim
   ```
3. Run the program:
   ```bash
   ./battle-sim
   ```

---

## Usage

On launch, you'll see a menu:

```
=============================
   Welcome to BattleSim!
=============================
a) Make 2 units fight each other
b) Choice B
x) Exit
-----------------------------
Enter choice (a, b, x):
```

Select option 'a' to choose factions and units for battle. Unit and faction data is loaded from JSON files in `data/factions/`.

---

## Managing Factions and Units

- Faction and unit data is stored in JSON files under `data/factions/` (e.g., `seraphon.json`, `ossiarch_bonereapers.json`).
- To add new factions or units, create or edit these JSON files. No code changes required!

Example unit entry:
```json
{
  "unitName": "Saurus Warrior",
  "modelCount": 10,
  "healthPerModel": 2,
  "floatingDamage": 0,
  "save": 4,
  "ward": 0,
  "weapons": [ ... ],
  "keywords": ["Seraphon", "Warrior"]
}
```

---

## TODO

- [x] Load units and factions dynamically from JSON
- [x] Display unit names and stats in menu
- [x] Simulate basic battles between units
- [ ] Add support for multiple weapon profiles per unit
- [ ] Expand menu options and user interface
- [ ] Create automated tests for core simulation functions
- [ ] Optimize simulation performance for large battles
- [ ] Document JSON schema and usage examples
- [ ] Explore desktop/mobile UI options
