106. Longest Path and Increasing Paths in a Grid

You are given a row x column integer grid. Find the length of the longest strictly increasing path in the grid.

From any cell, you may move only to its immediate left, right, up, or down neighbor. Diagonal movement is not allowed, and you cannot go outside the grid.

In addition to the maximum length, also return one longest strictly increasing path, and return all strictly increasing paths.

The grid is provided as List<String> gridLines, where each string represents one row and the numbers in that row are separated by commas.

A path is written in the following format:
(rowIndex,colIndex)=value -> (rowIndex,colIndex)=value -> ...

A path is strictly increasing if every next value in the path is greater than the value before it.

Method Signatures
int getLongestPathLength(List<String> gridLines)
Returns the number of cells in the longest strictly increasing path.
String getOneLongestPath(List<String> gridLines)
Returns one longest strictly increasing path.
If multiple longest paths exist, return the lexicographically smallest path string.
List<String> getAllStrictlyIncreasingPaths(List<String> gridLines)
Returns all strictly increasing paths.
Each returned path must contain at least one cell.
Return the paths in lexicographically increasing order of the full path strings.
Input Format
gridLines.size() == row
Each string in gridLines represents one grid row, such as "8,8,3"
All row strings must have the same number of comma-separated integers
If each row has column integers, then the grid size is row x column
Rules
From cell (x, y), the only valid neighboring cells are (x - 1, y), (x + 1, y), (x, y - 1), and (x, y + 1), if they are inside the grid.
You may move to a neighboring cell only if its value is strictly greater than the current cell value.
Diagonal moves are not allowed.
Wrap-around moves are not allowed.
A path consisting of exactly one cell is also a valid strictly increasing path.
Constraints
1 ≤ row ≤ 200
1 ≤ column ≤ 200
0 ≤ grid[i][j] ≤ 2^20
Each row string contains only non-negative integers separated by commas
No row string is empty
For test cases that call getAllStrictlyIncreasingPaths, the total number of valid strictly increasing paths will be small enough to fit in memory and output limits
Examples
Example 1
Input:
gridLines = ["8,8,3", "5,5,7", "2,1,0"]

Method Calls and Outputs:
getLongestPathLength(gridLines = ["8,8,3", "5,5,7", "2,1,0"])
5

getOneLongestPath(gridLines = ["8,8,3", "5,5,7", "2,1,0"])
"(2,2)=0 -> (2,1)=1 -> (2,0)=2 -> (1,0)=5 -> (0,0)=8"

Explanation:
The longest strictly increasing path has length 5. One such path is [0, 1, 2, 5, 8].

Example 2
Input:
gridLines = ["4,5,6", "4,3,7", "3,3,2"]

Method Calls and Outputs:
getLongestPathLength(gridLines = ["4,5,6", "4,3,7", "3,3,2"])
4

getOneLongestPath(gridLines = ["4,5,6", "4,3,7", "3,3,2"])
"(0,0)=4 -> (0,1)=5 -> (0,2)=6 -> (1,2)=7"

Explanation:
The longest strictly increasing path has length 4. One such path is [4, 5, 6, 7]. Diagonal movement is not allowed.

Example 3
Input:
gridLines = ["2"]

Method Calls and Outputs:
getLongestPathLength(gridLines = ["2"])
1

getOneLongestPath(gridLines = ["2"])
"(0,0)=2"

getAllStrictlyIncreasingPaths(gridLines = ["2"])
["(0,0)=2"]

Explanation:
The only strictly increasing path is the single cell itself.

Example 4
Input:
gridLines = ["2,3", "4,5"]

Method Calls and Outputs:
getLongestPathLength(gridLines = ["2,3", "4,5"])
3

getOneLongestPath(gridLines = ["2,3", "4,5"])
"(0,0)=2 -> (0,1)=3 -> (1,1)=5"

getAllStrictlyIncreasingPaths(gridLines = ["2,3", "4,5"])
[ "(0,0)=2", "(0,0)=2 -> (0,1)=3", "(0,0)=2 -> (0,1)=3 -> (1,1)=5", "(0,0)=2 -> (1,0)=4", "(0,0)=2 -> (1,0)=4 -> (1,1)=5", "(0,1)=3", "(0,1)=3 -> (1,1)=5", "(1,0)=4", "(1,0)=4 -> (1,1)=5", "(1,1)=5" ]

Explanation:
All valid strictly increasing paths are returned in lexicographically increasing order.