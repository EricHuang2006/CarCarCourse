import logging
from typing import Optional

from BT import Bluetooth

log = logging.getLogger(__name__)

# hint: You may design additional functions to execute the input command,
# which will be helpful when debugging :)

port = "COM11"

class BTInterface:
    def __init__(self, port: Optional[str] = None):
        log.info("Arduino Bluetooth Connect Program.")
        self.bt = Bluetooth()
        if port is None:
            print("Connect : COM11")
            port = "COM11"
           #port = input("PC bluetooth port name: ")
        while not self.bt.do_connect(port):
            if port == "quit":
                self.bt.disconnect()
                quit()
            port = input("PC bluetooth port name: ")

    def start(self):
        print("start.")
        #self.bt.serial_write_string("s")

    def get_UID(self):
        return self.bt.serial_read_string()

    def send_action(self, dirc):
        # TODO : send the action to car
        return

    def end_process(self):
        #self.bt.serial_write_string("e")
        self.bt.disconnect()


if __name__ == "__main__":
    test = BTInterface()
    test.start()
    #test.bt.serial_write_string("s")
    c = 0
    while c <= 10:
        u = test.get_UID()
        #res = bytes.fromhex(u).decode("utf-8")
        if u:
            print("u : ", u)
            ss = str(u)
            print("ss :", ss)
            res = bytearray.fromhex(ss[2:]).decode()
            print("get :", res)
            print(c)
            test.bt.serial_write_string("s")
            break
    test.end_process()
