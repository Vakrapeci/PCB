import serial
import argparse
from generated import uart_messages_pb2


def print_control_keys():
    print("This is an overload test")
    print("It will turn on and off the LED 10000 times")
    print("")
    print("")


def process_cmd_input():
    send_command = True
    quit = False
    msg = uart_messages_pb2.Command()
    msg.value = 1

    char = input("Next command: ")
    if "1" == char:
        msg.button = uart_messages_pb2.Command.On
    elif "0" == char:
        msg.button = uart_messages_pb2.Command.Off
    elif "Q" == char:
        msg.button = uart_messages_pb2.Command.Stop
        # Stop the loop
        quit = True
    else:
        send_command = False
        print_control_keys()

    if send_command:
        return msg, quit
    else:
        return None, quit


def process_cmd_input(a):
    send_command = True
    quit = False
    msg = uart_messages_pb2.Command()
    msg.value = 1

    b = a % 2
    char = str(b) #input("Next command: ")
    if "1" == char:
        msg.button = uart_messages_pb2.Command.On
    elif "0" == char:
        msg.button = uart_messages_pb2.Command.Off
    elif "Q" == char:
        msg.button = uart_messages_pb2.Command.Stop
        # Stop the loop
        quit = True
    else:
        send_command = False
        print_control_keys()

    if send_command:
        return msg, quit
    else:
        return None, quit


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-c', '--com', default="/dev/ttyACM0", help='The desired comport to open')
    args = parser.parse_args()

    # Try to open the serial port with the default baud rate.
    with serial.Serial(args.com, 115200, timeout=1) as ser:
        print_control_keys()

        running = True

        i = 0

        while running:
            command, quit = process_cmd_input(i)
            running = not quit

            if command:
                b = bytearray()

                # Serialize the command message and send it over uart.
                command_str = command.SerializeToString()
                # First send the length of the message.
                l = len(command_str)
                b.extend(l.to_bytes(1, byteorder='little'))
                # Next send the actual data
                b.extend(command_str)
                ser.write(b)

                # Await a reply.
                # First the length of the message.
                length_bytes = ser.read(1)
                length = int.from_bytes(length_bytes, byteorder="little")
                if 0 < length:
                    # Next the actual data
                    bytes = ser.read(length)

                    # Check if we have received all bytes.
                    if length == len(bytes):
                        reply = uart_messages_pb2.Reply()
                        reply.ParseFromString(bytes)

                        ## Do something with the reply.
                        #if reply.LED_state:
                        #    print("LED is ON")
                        #else:
                        #    print("LED is OFF")
            if i == 10000:
                break
            i = i + 1
