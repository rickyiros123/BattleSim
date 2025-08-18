# BattleSim: Warhammer Age of Sigmar 4th Edition Battle Simulator

A **C++ console-based battle simulator** for *Warhammer: Age of Sigmar (4th Edition)*.  
BattleSim uses JSON files to manage factions and units, with tooling to automatically convert community-maintained data into the simulator’s format.  
This project features a robust pipeline for cleaning, converting, and loading Warhammer unit and faction data, with automation and defensive coding for reliable simulation.

---

## ✨ Features

- **Dynamic data loading**  
  Load factions and units directly from JSON files.

- **Battle simulation engine**  
  - Dice-rolling mechanics
  - Wound and damage tracking
  - Modular code structure for easy expansion
  - Handles mixed data types and dice notation for weapon stats

- **JSON-first design**  
  Add or edit factions/units by simply updating JSON files — no code changes required.

- **Automated data import**  
  Python tools included to clean and convert `.cat` and `.gst` files from [BSData/age-of-sigmar-4th](https://github.com/BSData/age-of-sigmar-4th) into JSON, handling invalid characters and special cases.
  - Defensive type checking and error handling for robust conversion
  - Consistent output for fields like `range`, `rend`, and weapon stats

- **Extensible architecture**  
  Clean C++ code with room for advanced features (multiple weapon profiles, larger battle handling, UI integration, etc.).
  - C++ code loads and parses JSON, supports both integer and dice notation for weapon stats
  - Simulation logic for attacks, saves, and damage resolution
  - Well-documented code for maintainability and future development

---

## 🚀 Getting Started

### Prerequisites
- C++ compiler (GCC, Clang, MSVC, etc.)
- [nlohmann/json](https://github.com/nlohmann/json) (included in `include/`)
- Python 3 (for running conversion scripts)
- Git (for handling submodules)

### Installation

1. Clone the repository:
   ```bash
   git clone --recursive <your-repo-url>
   cd BattleSim
   ```
2. Compile the source code:
   ```bash
   make
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
  - Units and factions are instantiated from JSON, with support for complex weapon profiles and dice notation.
  - Battle sequence models attacks, saves, and damage using custom dice logic.

---

## Managing Factions and Units

- Faction and unit data is stored in JSON files under `data/factions/` (e.g., `seraphon.json`, `ossiarch_bonereapers.json`).
- To add new factions or units, create or edit these JSON files. No code changes required!
  - JSON schema supports mixed types for weapon stats and is designed for easy extension.

### Automated Unit Stat Population

This project now includes a git submodule: [BSData/age-of-sigmar-4th](https://github.com/BSData/age-of-sigmar-4th) in `tools/age-of-sigmar-4th/`.

You can use the `.cat` and `.gst` files from this submodule to help populate your unit stats and faction data. These files are maintained by the community and contain up-to-date information for all Age of Sigmar factions and units.

#### How to Use:

1. After cloning the repository, initialize and update submodules:
    ```bash
    git submodule update --init --recursive
    ```
2. Explore the `tools/age-of-sigmar-4th/` directory for faction/unit data files.
3. Use these files to help populate your own JSON files in `data/factions/`.
4. Optionally, write scripts to automate conversion from `.cat` files to your JSON schema.
   - Use the provided Python scripts to clean XML files and convert them to JSON, with robust handling for dice notation and special characters.
   - Output JSON files are consistent and ready for use in the simulator.

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
- [x] Automate cleaning and conversion of Battlescribe files to JSON
- [x] Handle mixed types and dice notation for weapon stats
- [x] Defensive coding and error handling in Python and C++
- [x] Document code for maintainability
- [ ] Add support for multiple weapon profiles per unit
- [ ] Expand menu options and user interface
- [ ] Create automated tests for core simulation functions
- [ ] Optimize simulation performance for large battles
- [ ] Document JSON schema and usage examples
- [ ] Explore desktop/mobile UI options
