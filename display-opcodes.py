import os

file_name = "pong.ch8"

def read_opcode_file(file_path, output_file_path):

    with open(file_path, 'rb') as input_file, open(output_file_path, 'w') as output_file:
        output_file.write(file_name + "\n")

        while True:
            opcode = input_file.read(2)
            if not opcode:
                break
            opcode_hex = opcode.hex()
            output_file.write(f"Opcode: {opcode_hex}\n")

# Get the directory path of the script
script_directory = os.path.dirname(os.path.abspath(__file__))

# Set the relative file path based on the script's directory
file_path = os.path.join(script_directory, file_name)
output_file_path = os.path.join(script_directory, 'output.txt')

read_opcode_file(file_path, output_file_path)
