import csv
import numpy as np
import pandas
from collections import deque
from enum import IntEnum

filepath = "./python/data/small_maze.csv"
class Direction(IntEnum):
    SAME = 0
    NORTH = 1
    SOUTH = 2
    WEST = 3
    EAST = 4

# Construct class Node and its member functions
# You may add more member functions to meet your needs
class Node:
    def __init__(self, index: int = 0):
        self.index = index
        # store successor as (Node, direction to node, distance)
        self.adj = []

    def pb(self, successor, direction, length=1):
        self.adj.append((successor, Direction(direction), int(length)))
        print(f"{self.index} -> {successor}")
        # print(f"For Node {self.index}, a successor {self.adj[-1]} is set.")
        return

    def get_direction(self, node):
        # TODO : if node is adjacent to the present node, return the direction of node from the present node
        # For example, if the direction of node from the present node is EAST, then return Direction.EAST = 4
        # However, if node is not adjacent to the present node, print error message and return 0
        return

    def is_successor(self, node):
        for succ in self.successors:
            if succ[0] == node:
                return True
        return False

g = pandas.read_csv(filepath)
g.fillna(-1, inplace = True)
g = g.values.astype(int)
print(g)
n = len(g)
nodes = []
for i in range(len(g)):
    a, b = Node(g[i][0] * 2 - 1), Node(g[i][0] * 2) # a : north-south, b : west-east
    a.pb(g[i][0] * 2, 0), b.pb(g[i][0] * 2 - 1, 0)
    for j in range(1, 5):
        if g[i][j] == -1: 
            continue
        if j <= 2:
            a.pb(g[i][j] * 2 - 1, j)
        else:
            b.pb(g[i][j] * 2, j)
    nodes.append(a), nodes.append(b)    
mp = {d.index : i for i, d in enumerate(nodes)}
print(mp)

vis = [0] * (n * 2 + 1)

def get(a, b):
    if a == b:
        return 'f'
    if abs(a - b) == 1 and a + b != 5:
        return 'b'
    if (abs(a - b) == 2 and a < b) or (abs(a - b) != 2 and a > b):
        return 'l'
    return 'r'

def BFS(node: int, pdir):
    dist = [10000] * (n * 2 + 1)
    pre = [(0, 0)] * (n * 2 + 1)
    dist[node] = 0
    q = deque()
    q.append(node)
    while q:
        x = q[0]
        q.popleft()
        for (i, dir, d) in nodes[mp[x]].adj:
            if dist[i] > dist[x] + d:
                dist[i] = dist[x] + d
                pre[i] = (x, dir)
                q.append(i)
    tar = 0
    for i in range(1, len(dist)):
        if vis[i] == 0 and dist[i] < dist[tar]:
            tar = i
    if tar == 0:
        return (0, 0, 0)
    seq = []
    tmp = tar
    while tmp != node:
        (i, dir) = pre[tmp]
        seq.append(dir)
        tmp = i
    seq = [x for x in seq if x]
    seq.append(pdir)
    seq.reverse()
    move = []
    for i in range(1, len(seq)):
        move.append(get(seq[i - 1], seq[i]))
        if move[-1] != 'f':
            move.append('f')
    print("seq :", seq)
    print("move :", move)
    # print("next tar : ", tar, int((tar + 1) / 2))
    return (tar, move, seq[-1])

vis[1], vis[2] = 1, 1
u, dir = 1, 2

while True:
    u, seq, dir = BFS(u, dir)
    if not u:
        break
    print("tar : ", int((u + 1) / 2))
    vis[u] = 1
    vis[((u + 1) ^ 1) - 1] = 1