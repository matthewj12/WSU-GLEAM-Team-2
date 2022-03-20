import serial, time

# replace "/dev/ttyUSB0" for the machine you're on. For example, 
# It would be "COM5" on windows (or some other COM port).
arduino = serial.Serial(port='/dev/ttyUSB0', baudrate=9600, timeout=0.2)

# allow the serial connection to be set up
time.sleep(1.2)

# TODO: Figure out how to get the serial to recieve 
# the output after the first command is entered

def write_read(cmd_input):
    global cur_angle_index

    arduino.write(bytes(cmd_input + '\n', 'utf-8'))
    time.sleep(0.1)
    data = arduino.read_until('Done. ')

    return data


def print_output(output_str)
    for i in range(len(output_str)):
        if (output_str[i:i+6] == 'Done. '):
            break

        cur_char = output_str[i]

        if not (cur_char in ['r', '\\', 'b', '\'']):
            print(chr(cur_char), end='')

        elif output_str[i:2] == '\n':
            print()


while True:
    cmd_input = input("Enter command to send to Arduino: ")
    
    cmd_output = write_read(cmd_input)

    print_output(cmd_output)
