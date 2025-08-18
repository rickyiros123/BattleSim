import os

def clean_file(filepath):
    with open(filepath, 'rb') as f:
        data = f.read()
    # Remove non-UTF8 and control characters except for standard whitespace
    cleaned = bytearray([b for b in data if b == 9 or b == 10 or b == 13 or (32 <= b <= 126) or (b >= 128)])
    # Convert to string for text replacement
    text = cleaned.decode('utf-8', errors='ignore')
    # Replace curly apostrophes (U+2019) with straight apostrophes
    text = text.replace('’', "'")
    # Write back as UTF-8
    with open(filepath, 'w', encoding='utf-8') as f:
        f.write(text)
    print(f"Cleaned: {filepath}")

# Change this to your .cat files directory
cat_dir = "tools/age-of-sigmar-4th/"
for filename in os.listdir(cat_dir):
    if filename.endswith(".cat"):
        clean_file(os.path.join(cat_dir, filename))