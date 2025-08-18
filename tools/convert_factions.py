# Import required modules
import os
import json
import xml.etree.ElementTree as ET
import re

# Input directory containing Battlescribe .cat files
INPUT_DIR = "tools/age-of-sigmar-4th/"
# Output directory for generated JSON faction files
OUTPUT_DIR = "data/factions/"

def parse_cat_file(filepath):
    # Print the file being parsed for easier error tracking
    print(f"Parsing file: {filepath}")
    # Parse a single .cat XML file and extract faction/unit data
    tree = ET.parse(filepath)
    root = tree.getroot()
    ns = {'bs': 'http://www.battlescribe.net/schema/catalogueSchema'}

    # Get the faction name from the root element
    faction_name = root.attrib.get("name", "Unknown Faction")
    units = []

    # Find all unit entries by looking for type="unit"...
    for entry in root.findall('.//bs:selectionEntry[@type="unit"]', ns):  
        unit = {
            "unitName": "Unknown Unit",
            "modelCount": 1,  # Default for now
            "healthPerModel": 1,
            "floatingDamage": 0,
            "save": 0,
            "ward": 0,
            "weapons": [],
            "keywords": [faction_name]
        }

        # After finding each unit entry
        unit_id = entry.attrib.get("id")
        unit_name = entry.attrib.get("name")
        print(f"Found unit: {unit_name}")
        unit["unitName"] = unit_name

        # Find the profile for this unit
        for profile in root.findall('.//bs:profile[@typeName="Unit"]', ns):
            if profile.attrib.get("name") == unit_name:
                for char in profile.findall('.//bs:characteristic', ns):
                    name = char.attrib.get('name')
                    value = char.text
                    if name == "Health":
                        try:
                            unit["healthPerModel"] = int(value)
                        except:
                            pass
                    elif name == "Save":
                        try:
                            unit["save"] = int(value)
                        except:
                            pass
                    # Add more stats as needed

        model_count = 1  # Default
        for model_entry in entry.findall('.//bs:selectionEntry[@type="model"]', ns):
            default_amount = model_entry.attrib.get("defaultAmount")
            if default_amount:
                # If it's a comma-separated list, take the highest value
                try:
                    model_count = max([int(x) for x in default_amount.split(",")])
                except:
                    pass
        unit["modelCount"] = model_count

        ward_value = 0
        for catlink in entry.findall('.//bs:categoryLink', ns):
            name = catlink.attrib.get('name', '')
            match = re.match(r'WARD \((\d+)\+\)', name)
            if match:
                ward_value = int(match.group(1))
        unit["ward"] = ward_value

        weapons = []
        for upgrade_entry in entry.findall('.//bs:selectionEntry[@type="upgrade"]', ns):
            for profile in upgrade_entry.findall('.//bs:profile', ns):
                if profile.attrib.get("typeName") in ["Melee Weapon", "Ranged Weapon", "Missile Weapon"]:
                    weapon = {
                        "weaponName": profile.attrib.get("name", ""),
                        "numberOfAttacks": 0,
                        "toHit": 0,
                        "toWound": 0,
                        "rend": 0,
                        "weaponDamage": 0,
                        "range": 0
                    }
                    for char in profile.findall('.//bs:characteristic', ns):
                        name = char.attrib.get('name')
                        value = char.text
                        # Use safe_int for all stats, store dice notation as string
                        if name == "Atk":
                            weapon["numberOfAttacks"] = safe_int(value, value)
                        elif name == "Hit":
                            weapon["toHit"] = safe_int(value.replace("+", ""), value)
                        elif name == "Wnd":
                            weapon["toWound"] = safe_int(value.replace("+", ""), value)
                        elif name == "Rnd":
                            # Always output rend as int, '-' becomes 0
                            if value == "-":
                                weapon["rend"] = 0  # '-' means no rend, so set to 0
                            else:
                                weapon["rend"] = safe_int(value, 0)  # Convert rend to int
                        elif name == "Dmg":
                            weapon["weaponDamage"] = safe_int(value, value)
                        elif name == "Rng":
                            # Always output range as int
                            weapon["range"] = safe_int(value, 0)  # Convert range to int
                    weapons.append(weapon)
        unit["weapons"] = weapons

        units.append(unit)

    # Return the faction name and all units found
    return faction_name, units

def convert_all():
    # Create the output directory if it doesn't exist
    os.makedirs(OUTPUT_DIR, exist_ok=True)
    # Loop through all .cat files in the input directory
    for filename in os.listdir(INPUT_DIR):
        if filename.endswith(".cat"):
            # Parse each .cat file to extract faction and unit data
            faction, units = parse_cat_file(os.path.join(INPUT_DIR, filename))
            # Build the output path for the JSON file
            outpath = os.path.join(OUTPUT_DIR, f"{faction.lower().replace(' ', '_')}.json")
            # Write the extracted data to a JSON file
            with open(outpath, "w", encoding="utf-8") as f:
                json.dump({"faction": faction, "units": units}, f, indent=4)
            print(f"Converted {faction} -> {outpath}")

def safe_int(value, default=0):
    # Extract integer from value, or handle dice notation and special cases
    if isinstance(value, int):
        return value  # Already an integer
    if isinstance(value, str):
        # If value is dice notation, return default (original string)
        if re.match(r'^(D\d+|\d+D\d+|D3\+\d+|D6|D3|D6\+\d+)$', value):
            return default  # Dice notation, not a simple int
        match = re.search(r'\d+', value)  # Find first integer in string
        if match:
            return int(match.group(0))  # Return extracted integer
        return default  # No integer found, return default
    try:
        return int(value)  # Try to convert to int
    except (ValueError, TypeError):
        return default  # On error, return default

if __name__ == "__main__":
    # Run the conversion for all .cat files when the script is executed
    convert_all()
