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
from server_test import add_score

logging.basicConfig(
    format="%(asctime)s - %(name)s - %(levelname)s - %(message)s", level=logging.INFO
)

log = logging.getLogger(__name__)

# TODO : Fill in the following information
# SERVER_URL = "localhost:3000"
# MAZE_FILE = "data/big_maze.csv"
MAZE_FILE = "data/big_maze.csv"
TEAM_NAME = "XiangTsaoXiaoXiang"
SERVER_URL = "http://140.112.175.18:5000"
BT_PORT = "COM11"  # ← 請填你的藍牙埠，例如 "COM11"

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
    # point = ScoreboardFake("your team name", "data/fakeUID.csv") # for local testing
    BT = BTInterface(port=bt_port)
    point = ScoreboardServer(team_name, server_url)
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
    
    # a, b, dir = 14, 2, 4 # a = node_id * 2
    # maze.vis[13], maze.vis[14] = 1, 1
    #----------- 正式賽
    a, b, dir = 47, 8, 1 # a = node_id * 2
    maze.vis[47], maze.vis[48] = 1, 1
    full_seq = ""
    u, seq, nwdir = maze.BFS(a, dir, b)
    full_seq = full_seq + ''.join(map(str, seq))
    a, b, dir = u, 18, nwdir
    u, seq, nwdir = maze.BFS(a, dir, b)
    full_seq = full_seq + ''.join(map(str, seq))
    a, b, dir = u, 43, nwdir
    # Strategy 1 : go to the farthest node first, then repeatly do bfs to the nearest unvisited node
    while True:
        #-------------- 正式賽
        u, seq, nwdir = maze.BFS(a, dir, b)
        b = -1
        print(u)
        if not u:
            full_seq = full_seq + 'b'
            break
        full_seq = full_seq + ''.join(map(str, seq))
        a, dir = u, nwdir
        #--------------
        # tmp = "rbfblb"
        # u, seq, nwdir = 1, tmp, 1 # test
        # print(f"current path : {int((a + 1) / 2)} -> {int((u + 1) / 2)}, {nwdir}")
        # print(f"sequence : {seq}")
    print(f"full sequence : {full_seq}")
    for i in full_seq:
        print(f"send : {i}")
        BT.bt.serial_write_string(i)
        while True:  
            receive = BT.get_UID()
            if receive:
                s = str(receive).strip()
                print(f"[ received : {s} ]")
                if len(s) == 8:
                    print(f"get UID : {s}")
                    score, tr = point.add_UID(s)
                    print(f"< Current score : {point.get_current_score()}, Time remaining : {tr:.1f} >")
                    continue
                else: # 收到回復，傳送下一個指令 
                    break
                
    BT.bt.serial_write_string("s")
    while True: # 正式賽 : True
        receive = BT.get_UID()
        if receive:
            s = str(receive).strip()
            print(f"[ received : {s} ]")
            if len(s) == 8:
                print(f"get UID : {s}")
                # add_score(s)
                score, tr = point.add_UID(s)
                print(f" Current score : {point.get_current_score()}, Time remaining : {tr:.1f}")
                continue
            else:
                print(f"get command : {s}")

    print("done")
    BT.bt.disconnect()

if __name__ == "__main__":
    args = parse_args()
    main(**vars(args))
