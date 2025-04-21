import logging
from time import sleep

from serial import Serial
from serial.serialutil import SerialException

log = logging.getLogger(__name__)

# these codes are for bluetooth
# hint: please check the function "sleep". how does it work?


class Bluetooth:
    """
    The Bluetooth class is used to connect to the Arduino via Bluetooth.
    """

    def __init__(self):
        self.serial = Serial()

    def do_connect(self, port: str):
        self.serial.close()
        log.info(f"Connecting to {port}...")
        try:
            self.serial = Serial(port, 9600, timeout=2)
            log.info("Success\n")
        except SerialException:
            log.warning("Fail to connect\n")
            return False
        return True

    def disconnect(self):
        self.serial.close()

    def serial_write_string(self, data: str):
        # send = data.encode("utf-8")
        send = data.encode("ascii")
        self.serial.write(send)

    def serial_write_bytes(self, data: bytes):
        self.serial.write(data)

    def serial_read_string(self):
        waiting = self.serial.in_waiting
        if waiting > 0:
            u = self.serial.readline()
            print(f"raw info : {u}")
            try:
                rv = u.decode("utf-8")[:-1]
                self.serial.reset_input_buffer()
                return rv
            except UnicodeDecodeError:
                print("Decode failed, data was invalid")
                return 'x'
            self.serial.reset_input_buffer()
        return ""

    def serial_read_byte(self):
        sleep(0.05)
        waiting = self.serial.in_waiting
        rv = self.serial.read(waiting)
        if rv:
            uid = hex(int.from_bytes(rv, byteorder="big", signed=False))
            self.serial.reset_input_buffer()
            return uid
        else:
            return 0

#rv = b'\xB5\xC9\x1B\x52'
#uid = hex(int.from_bytes(rv, byteorder="big", signed=False))
#print(rv)
#print(uid)  # Output: '0xb5c91b52'
