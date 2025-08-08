# Warhammer: Age of Sigmar 4th Edition Battle Simulator

A battle simulator designed to model combat between units using JSON-based army data. It can simulate individual combats or run large batches of simulations to estimate statistical averages and win rates.

---

## Features

- Dice rolling mechanics modeled after Age of Sigmar rules
- Structs and classes to represent units and their weapons
- Planned combat simulation between two units for in-game use
- Ability to run thousands of simulations to generate statistical insights
- JSON-based unit and faction data for easy customization and expansion

---

## Getting Started

### Prerequisites

- A C++ compiler (e.g., GCC or MSVC)
- Basic knowledge of command line to compile and run the program

### Installation

1. Clone the repository
2. Compile the source code:
   ```bash
   g++ src/main.cpp -o battle-sim
   ```
