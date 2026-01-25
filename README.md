# Sketchbook

Creative coding experiments using [OpenFrameworks](https://openframeworks.cc).

---

### [Mazes](openFrameworks/apps/myApps/mazes)

Generating various mazes based on [Mazes for Programmers](https://pragprog.com/titles/jbmaze/mazes-for-programmers/) book.

Using ASCII and OpenFramewroks to display resulting mazes.

#### Binary Tree

North-east would be the "root" of the tree, since there's nowhere else to go.
And it starts branching with children to the south and west from it.
It has a north-east diagonal direction bias.

```
+---+---+---+---+---+---+---+---+
|                               |
+   +---+   +   +   +   +---+   +
|   |       |   |   |   |       |
+---+---+---+   +---+   +---+   +
|               |       |       |
+---+---+---+---+---+---+   +   +
|                           |   |
+---+---+---+   +---+   +   +   +
|               |       |   |   |
+   +   +---+---+---+---+---+   +
|   |   |                       |
+---+---+   +---+   +---+   +   +
|           |       |       |   |
+---+   +   +   +   +   +   +   +
|       |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+
```

#### Sidewinder

Sidewinder is similar to Binary Tree, however it winds from side to side (hehe)
by grouping neighboring cells before carving a passage north from them.

```
+---+---+---+---+---+---+---+---+
|                               |
+   +---+---+---+   +---+   +   +
|           |           |   |   |
+   +   +   +---+---+---+   +   +
|   |   |   |               |   |
+---+---+---+   +---+---+---+   +
|                           |   |
+---+   +   +   +---+   +   +   +
|       |   |   |       |   |   |
+---+   +---+---+   +   +   +   +
|               |   |   |   |   |
+   +---+---+---+   +---+   +---+
|               |       |       |
+---+   +---+   +---+---+   +---+
|           |   |               |
+---+---+---+---+---+---+---+---+
```

#### Dijkstra

Simplified Dijkstra algorithm to display distances (base-36) from the root cell to each cell in the maze.
Here root cell is (0, 0) in the top-left corner.

It will help us observe biases in mazes and do fun visualizations.

```
+---+---+---+---+---+---+---+---+
| 0   1   2   3   4   5   6   7 |
+   +   +   +---+---+---+---+---+
| 1 | 2 | 3   4   5   6   7   8 |
+   +---+   +---+---+   +---+---+
| 2 | 5   4   5   6 | 7   8   9 |
+---+   +   +---+---+   +   +---+
| 7   6 | 5   6 | 9   8 | 9   a |
+   +---+   +---+   +   +---+   +
| 8   9 | 6   7 | a | 9   a | b |
+---+   +   +---+   +---+---+---+
| b   a | 7 | c   b   c   d   e |
+---+---+   +---+   +---+---+   +
| a   9   8 | d   c   d   e | f |
+---+   +---+   +   +---+---+   +
| b   a | f   e | d   e | h   g |
+---+---+---+---+---+---+---+---+
```
