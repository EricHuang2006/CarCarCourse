import argparse
import logging
import os
import sys
import time

import numpy as np
import pandas
from BTinterface import BTInterface
from maze import Action, Maze
from score import ScoreboardServer, ScoreboardFake

logging.basicConfig(
    format="%(asctime)s - %(name)s - %(levelname)s - %(message)s", level=logging.INFO
)

log = logging.getLogger(__name__)

# TODO : Fill in the following information
TEAM_NAME = "TEST"
#SERVER_URL = "http://140.112.175.18:5000/"
SERVER_URL = "localhost:3000"
MAZE_FILE = "data/maze5.csv"
BT_PORT = "COM11"


def parse_args():
    print("parsing...")
    parser = argparse.ArgumentParser()
    parser.add_argument("--mode", default="0", help="0: treasure-hunting, 1: self-testing", type=str)
    parser.add_argument("--maze-file", default=MAZE_FILE, help="Maze file", type=str)
    parser.add_argument("--bt-port", default=BT_PORT, help="Bluetooth port", type=str)
    parser.add_argument(
        "--team-name", default=TEAM_NAME, help="Your team name", type=str
    )
    parser.add_argument("--server-url", default=SERVER_URL, help="Server URL", type=str)
    return parser.parse_args()


def main(mode: int, bt_port: str, team_name: str, server_url: str, maze_file: str):
    maze = Maze(maze_file)
    #point = ScoreboardServer(team_name, server_url)
    point = ScoreboardFake("your team name", "data/fakeUID.csv") # for local testing
    #BT = BTInterface(port=bt_port)
    # TODO : Initialize necessary variables

    if mode == "0":
        log.info("Mode 0: For treasure-hunting")
        # TODO : for treasure-hunting, which encourages you to hunt as many scores as possible
    elif mode == "1":
        log.info("Mode 1: Self-testing mode.")
        # TODO: You can write your code to test specific function.
    else:
        log.error("Invalid mode")
        sys.exit(1)
    
    a, b, dir = list(map(int, input("Enter s, t, dir :").split()))
    u, seq, dir = maze.BFS(a, dir, b)
    #u, seq, dir = maze.BFS(4, 4, 3)
    seq.insert(0, 'f')
    print("seq :", seq)
    s = ""
    for i in seq:
        s += i
    print(s)
    return
    for i in seq:
        print("sent :", i)
        BT.bt.serial_write_string(i)
        c = 0
        while c <= 5:
            u = BT.get_UID()
            if u:
                ss = str(u)
                #print(ss)
                #print("get :", res)
                if len(ss) == 9:
                    print("get UID :", ss)
                    point.add_UID(ss[:-1])
                    continue
                else:
                    break
    BT.bt.serial_write_string("s")
    print("done")
    BT.bt.disconnect()

if __name__ == "__main__":
    args = parse_args()
    main(**vars(args))
