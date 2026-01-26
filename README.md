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

path from northwest corner to southwest corner:
```
+---+---+---+---+---+---+---+---+               +---+---+---+---+---+---+---+---+
| 0   1   2   3   4   5   6   7 |               | 0   1   2   3   4             |
+---+   +   +   +   +---+---+   +               +---+   +   +   +   +---+---+   +
| 3   2 | 3 | 4 | 5   6 | 9   8 |               |       |   |   | 5     |       |
+   +---+---+---+   +   +---+---+               +   +---+---+---+   +   +---+---+
| 4   5   6 | 7   6 | 7   8   9 |               |           |     6 |           |
+   +---+   +---+   +---+   +---+               +   +---+   +---+   +---+   +---+
| 5 | 8   7 | 8   7   8 | 9   a |               |   |       | 8   7     |       |
+   +---+---+   +   +   +   +---+               +   +---+---+   +   +   +   +---+
| 6 | b   a   9 | 8 | 9 | a   b |               |   | b   a   9 |   |   |       |
+---+   +---+   +   +   +   +---+               +---+   +---+   +   +   +   +---+
| d   c | b   a | 9 | a | b   c |               | d   c |       |   |   |       |
+   +   +   +   +   +   +---+   +               +   +   +   +   +   +   +---+   +
| e | d | c | b | a | b | e   d |               | e |   |   |   |   |   |       |
+   +---+---+   +---+   +   +   +               +   +---+---+   +---+   +   +   +
| f   g   h | c   d | c | f | e |               | f         |       |   |   |   |
+---+---+---+---+---+---+---+---+               +---+---+---+---+---+---+---+---+
```

#### Longest Path

Using Dijkstra algorithm we can now find one of the longest paths in our maze.
For that we run the algorithm once from an arbitrary starting point to find either of the farthest cells.
Then we run the algorithm once again from the farthest cell and find the farthest cell from it.
This will give us one of the longest paths in the maze.

choosing first arbitrary cell as the north-west corner:
```
+---+---+---+---+---+---+---+---+               +---+---+---+---+---+---+---+---+
| 0   1   2   3   4   5   6   7 |               |                 e   d         |
+   +   +---+---+   +   +---+---+               +   +   +---+---+   +   +---+---+
| 1 | 2 | 7   6   5 | 6   7   8 |               |   |   | h   g   f | c         |
+   +---+   +---+   +   +---+   +               +   +---+   +---+   +   +---+   +
| 2 | 9   8   9 | 6 | 7 | a   9 |               |   |     i     |   | b |       |
+---+   +   +---+---+   +---+---+               +---+   +   +---+---+   +---+---+
| b   a | 9   a   b | 8   9   a |               |       | j         | a   9     |
+---+---+   +---+---+---+   +---+               +---+---+   +---+---+---+   +---+
| c   b   a   b | c   b   a   b |               | m   l   k     | 6   7   8     |
+   +   +   +---+   +---+---+---+               +   +   +   +---+   +---+---+---+
| d | c | b   c | d   e   f   g |               | n |   |       | 5   4   3   2 |
+   +---+   +---+   +   +   +   +               +   +---+   +---+   +   +   +   +
| e | d   c | f   e | f | g | h |               | o |       |       |   |   | 1 |
+   +---+   +   +   +---+---+   +               +   +---+   +   +   +---+---+   +
| f | e   d | g | f   g   h | i |               | p |       |   |           | 0 |
+---+---+---+---+---+---+---+---+               +---+---+---+---+---+---+---+---+
```
