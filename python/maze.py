import csv
import logging
import math
from enum import IntEnum
from typing import List
from collections import deque

import numpy as np
import pandas

from node import Direction, Node

log = logging.getLogger(__name__)


class Action(IntEnum):
    ADVANCE = 1
    U_TURN = 2
    TURN_RIGHT = 3
    TURN_LEFT = 4
    HALT = 5

def get(a, b):
    if a == b:
        return 'f'
    if abs(a - b) == 1 and a + b != 5:
        return 'b'
    if (abs(a - b) == 2 and a < b) or (abs(a - b) != 2 and a > b):
        return 'l'
    return 'r'

class Maze:
    def __init__(self, filepath: str):
        # TODO : read file and implement a data structure you like
        # For example, when parsing raw_data, you may create several Node objects.
        # Then you can store these objects into self.nodes.
        # Finally, add to nd_dict by {key(index): value(corresponding node)}
        self.g = pandas.read_csv(filepath)
        self.g.fillna(-1, inplace = True)
        self.g = self.g.values.astype(int)
        self.n = len(self.g)
        self.nodes = []
        print(self.g)
        self.vis = [0] * (2 * self.n + 1)
        for i in range(self.n):
            a, b = Node(self.g[i][0] * 2 - 1), Node(self.g[i][0] * 2) # a : north-south, b : west-east
            a.pb(self.g[i][0] * 2, 0), b.pb(self.g[i][0] * 2 - 1, 0)
            for j in range(1, 5):
                if self.g[i][j] == -1: 
                    continue
                if j <= 2:
                    a.pb(self.g[i][j] * 2 - 1, j)
                else:
                    b.pb(self.g[i][j] * 2, j)
            self.nodes.append(a), self.nodes.append(b)    
        self.mp = {d.index : i for i, d in enumerate(self.nodes)} #{node index : index in list}
        # self.node_dict = dict()  # key: index, value: the correspond node
    '''
    def get_start_point(self):
        if len(self.node_dict) < 2:
            log.error("Error: the start point is not included.")
            return 0
        return self.node_dict[1]
    
    def get_node_dict(self):
        return self.node_dict
    '''
    def BFS(self, node: Node, pdir, t = -1): # (start node, direction, target)
        dist = [10000] * (self.n * 2 + 1)
        pre = [(0, 0)] * (self.n * 2 + 1)
        dist[node] = 0
        q = deque()
        q.append(node)
        while q:
            x = q[0]
            q.popleft()
            for (i, dir, d) in self.nodes[self.mp[x]].adj:
                if dist[i] > dist[x] + d:
                    dist[i] = dist[x] + d
                    pre[i] = (x, dir)
                    q.append(i)
        tar = 0
        for i in range(1, len(dist)):
            if self.vis[i] == 0 and dist[i] < dist[tar]:
                tar = i
        if tar == 0:
            return (0, 0, 0)
        if t != -1:
            tar = t * 2
        seq = []
        tmp = tar
        while tmp != node:
            (i, dir) = pre[tmp]
            seq.append(dir)
            tmp = i
        seq = [x for x in seq if x]
        seq.append(pdir)
        seq.reverse()
        print("raw seq: ", seq)
        move = []
        for i in range(1, len(seq), 1):
            move.append(get(seq[i - 1], seq[i]))

        self.vis[tar] = 1, self.vis[tar + 3 * (tar % 2) - 2]
        return (tar / 2, move, seq[-1])   

    def BFS_2(self, node_from: Node, node_to: Node):
        # TODO : similar to BFS but with fixed start point and end point
        # Tips : return a sequence of nodes of the shortest path
        return None

    def getAction(self, car_dir, node_from: Node, node_to: Node):
        # TODO : get the car action
        # Tips : return an action and the next direction of the car if the node_to is the Successor of node_to
        # If not, print error message and return 0
        return None

    def getActions(self, nodes: List[Node]):
        # TODO : given a sequence of nodes, return the corresponding action sequence
        # Tips : iterate through the nodes and use getAction() in each iteration
        return None

    def actions_to_str(self, actions):
        # cmds should be a string sequence like "fbrl....", use it as the input of BFS checklist #1
        cmd = "fbrls"
        cmds = ""
        for action in actions:
            cmds += cmd[action - 1]
        log.info(cmds)
        return cmds

    def strategy(self, node: Node):
        return self.BFS(node)
        dist = np.array()

    def strategy_2(self, node_from: Node, node_to: Node):
        return self.BFS_2(node_from, node_to)

if __name__ == "__main__":
    maze = Maze("data/maze4.csv")
    seq = maze.BFS(1, 2, 12)
    print("seq: ", seq)