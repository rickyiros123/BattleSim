import os
import json
import xml.etree.ElementTree as ET

INPUT_DIR = "tools/age-of-sigmar-4th/"
OUTPUT_DIR = "data/factions/"

def parse_cat_file(filepath):
    tree = ET.parse(filepath)
    root = tree.getroot()

    faction_name = root.attrib.get("name", "Unknown Faction")
    units = []

    # Example: parse profiles
    for profile in root.findall(".//profile"):
        if profile.attrib.get("typeName") == "Unit":
            unit = {
                "unitName": profile.attrib.get("name", "Unknown Unit"),
                "modelCount": 1,  # Default for now
                "healthPerModel": 1,
                "floatingDamage": 0,
                "save": 0,
                "ward": 0,
                "weapons": [],
                "keywords": [faction_name]
            }

            # Look at characteristics inside <characteristics>
            for char in profile.findall(".//characteristic"):
                name = char.attrib.get("name")
                value = char.text

                if name == "Wounds":
                    unit["healthPerModel"] = int(value)
                elif name == "Save":
                    unit["save"] = int(value.replace("+",""))
                elif name == "Ward":
                    unit["ward"] = int(value.replace("+",""))

            units.append(unit)

    return faction_name, units

def convert_all():
    os.makedirs(OUTPUT_DIR, exist_ok=True)
    for filename in os.listdir(INPUT_DIR):
        if filename.endswith(".cat"):
            faction, units = parse_cat_file(os.path.join(INPUT_DIR, filename))
            outpath = os.path.join(OUTPUT_DIR, f"{faction.lower().replace(' ', '_')}.json")
            with open(outpath, "w", encoding="utf-8") as f:
                json.dump({"faction": faction, "units": units}, f, indent=4)
            print(f"Converted {faction} -> {outpath}")

if __name__ == "__main__":
    convert_all()
