10348. Design Tic-Tac-Toe

Create a class named TicTacGame that simulates a Tic-tac-toe game between two players on an m x m grid.

The following rules apply:

Each move is valid and played on an empty cell.
Once a player has won, no further moves can be made.
The first player to place m of their symbols consecutively in a row, column, or diagonal wins.
Examples
Suppose m = 3, where player 1 uses "A" and player 2 uses "B":

TicTacGame game = new TicTacGame(3);

game.doMove(0, 0, 1); // Returns 0 (no winner yet)
|A| | |
| | | |    // Player 1 moves at (0, 0)
| | | |

game.doMove(0, 2, 2); // Returns 0 (no winner yet)
|A| |B|
| | | |    // Player 2 moves at (0, 2)
| | | |

game.doMove(2, 2, 1); // Returns 0 (no winner yet)
|A| |B|
| | | |    // Player 1 moves at (2, 2)
| | |A|

game.doMove(1, 1, 2); // Returns 0 (no winner yet)
|A| |B|
| |B| |    // Player 2 moves at (1, 1)
| | |A|

game.doMove(2, 0, 1); // Returns 0 (no winner yet)
|A| |B|
| |B| |    // Player 1 moves at (2, 0)
|A| |A|

game.doMove(1, 0, 2); // Returns 0 (no winner yet)
|A| |B|
|B|B| |    // Player 2 moves at (1, 0)
|A| |A|

game.doMove(2, 1, 1); // Returns 1 (player 1 wins)
|A| |B|
|B|B| |    // Player 1 moves at (2, 1)
|A|A|A|
Constraints
1 <= m <= 200
Player number is either 1 or 2
Each doMove call places a mark at an empty spot
At most m2 calls will be made to doMove