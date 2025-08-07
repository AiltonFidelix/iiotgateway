import os
from pty import openpty
from serial import Serial
from threading import Thread
from select import select


def create_virtual_serial():
    master_fd, slave_fd = openpty()
    slave_name = os.ttyname(slave_fd)
    print(f"[INFO] Virtual serial port created: {slave_name}")
    return master_fd, slave_name


def modbus_serial_server(slave_name, master_fd):

    ser = Serial(slave_name, 115200, timeout=1, exclusive=False)

    print("[INFO] Fake modbus device is listening...")

    try:
        while True:
            readable, _, _ = select([master_fd], [], [], 1)

            if readable:
                data = os.read(master_fd, 1024)
                print(f"[FAKE DEVICE] Received: {data.hex()}")

                if data.startswith(b"\x01\x03"):
                    response = b"\x01\x03\x04\x00\x2d\x00\x2b" + crc16(
                        b"\x01\x03\x04\x00\x2d\x00\x2b"
                    )
                    print(f"[FAKE DEVICE] Sending reply: {response.hex()}")
                    os.write(master_fd, response)

    except KeyboardInterrupt:
        ser.close()
        print("Closing...")


def crc16(data):
    crc = 0xFFFF
    for b in data:
        crc ^= b
        for _ in range(8):
            if crc & 1:
                crc = (crc >> 1) ^ 0xA001
            else:
                crc >>= 1
    return crc.to_bytes(2, byteorder="little")


if __name__ == "__main__":
    master_fd, slave_name = create_virtual_serial()

    server_thread = Thread(
        target=modbus_serial_server, args=(slave_name, master_fd), daemon=True
    )
    server_thread.start()

    print(f"\n[USE] Connect to the port: {slave_name}")

    input("Press ENTER to close...\n")
