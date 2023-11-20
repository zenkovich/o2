import os

def is_binary(file_path):
    try:
        with open(file_path, 'tr') as check_file:  # 'tr' is text read mode
            check_file.read()
            return False
    except:  # If an error occurs then it's likely a binary file
        return True

def replace_tabs_with_spaces(file_path):
    try:
        with open(file_path, 'r', encoding='utf-8') as file:
            content = file.read()
        content = content.replace('\t', '    ')
        with open(file_path, 'w', encoding='utf-8') as file:
            file.write(content)
    except UnicodeDecodeError:
        print(f"Skipped binary file: {file_path}")

for root, dirs, files in os.walk('.'):
    for file_name in files:
        full_path = os.path.join(root, file_name)
        if not is_binary(full_path):
            replace_tabs_with_spaces(full_path)
        else:
            print(f"Skipped binary file: {full_path}")
