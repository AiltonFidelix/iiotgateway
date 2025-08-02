import sys
from minimalmodbus import Instrument


def read(port: str, address: int = 1):
    instrument = Instrument(port, address)

    instrument.serial.baudrate = 115200

    result = instrument.read_registers(0, 2)
    print(f"Result received: {result}")

    instrument.serial.close()


if __name__ == "__main__":
    args = sys.argv

    if len(args) == 1:
        print(
            "Failed, needs to receive port and address. Example: python client.py /dev/pts/7 1"
        )
        exit(1)

    read(args[1], int(args[2]))
