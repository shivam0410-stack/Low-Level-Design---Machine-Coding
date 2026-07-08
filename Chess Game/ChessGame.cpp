#include <bits/stdc++.h>
using namespace std;


class Piece {
public:
    virtual bool isValidMove(int startRow, int startCol, int endRow, int endCol, vector<vector<string>>& board) const = 0;
};

class King : public Piece {
public:
    bool isValidMove(int startRow, int startCol, int endRow, int endCol, vector<vector<string>>& board) const override {
        if(abs(startRow - endRow) <= 1 && abs(startCol - endCol) <= 1) {
            return true;
        }
        return false;
    }
};

class Knight : public Piece {
public:
    bool isValidMove(int startRow, int startCol, int endRow, int endCol, vector<vector<string>>& board) const override {
        if(abs(startRow - endRow) == 2 && abs(startCol - endCol) == 1) {
            return true;
        }
        if(abs(startRow - endRow) == 1 && abs(startCol - endCol) == 2) {
            return true;
        }
        return false;
    }
};

class Bishop : public Piece {
public:
    bool isValidMove(int startRow, int startCol, int endRow, int endCol, vector<vector<string>>& board) const override {
        if(abs(startRow - endRow) != abs(startCol - endCol)) {
            return false;
        }
        int drow = (startRow < endRow) ? 1 : -1;
        int dcol = (startCol < endCol) ? 1 : -1;
        while(startRow + drow != endRow) {
            startRow += drow;
            startCol += dcol;
            if(board[startRow][startCol] != "") {
                return false;
            }
        }
        return true;
    }
};

class Rook : public Piece {
public:
    bool isValidMove(int startRow, int startCol, int endRow, int endCol, vector<vector<string>>& board) const override {
        if(startRow != endRow && startCol != endCol) {
            return false;
        }

        int drow = (startRow == endRow) ? 0 : (startRow < endRow) ? 1 : -1;
        while(startRow + drow != endRow) {
            startRow += drow;
            if(board[startRow][startCol] != "") {
                return false;
            }
        }

        int dcol = (startCol == endCol) ? 0 : (startCol < endCol) ? 1 : -1;
        while(startCol + dcol != endCol) {
            startCol += dcol;
            if(board[startRow][startCol] != "") {
                return false;
            }
        }

        return true;
    }
};

class Queen : public Piece {
    Rook rook;
    Bishop bishop;

public:
    bool isValidMove(int startRow, int startCol, int endRow, int endCol, vector<vector<string>>& board) const override {
        if(startRow == endRow || startCol == endCol) {
            return rook.isValidMove(startRow, startCol, endRow, endCol, board);
        }

        if(abs(startRow - endRow) == abs(startCol - endCol)) {
            return bishop.isValidMove(startRow, startCol, endRow, endCol, board);
        }

        return false;
    }
};

class Pawn : public Piece {
public:
    bool isValidMove(int startRow, int startCol, int endRow, int endCol, vector<vector<string>>& board) const override {
        if(board[startRow][startCol][0] == 'W') {
            if(endRow != startRow + 1) {
                return false;
            }
            if(endCol == startCol && board[endRow][endCol] == "") {
                return true;
            }
            if(abs(endCol - startCol) == 1 && board[endRow][endCol][0] == 'B') {
                return true;
            }
            return false;
        }

        if(board[startRow][startCol][0] == 'B') {
            if(endRow != startRow - 1) {
                return false;
            }
            if(endCol == startCol && board[endRow][endCol] == "") {
                return true;
            }
            if(abs(endCol - startCol) == 1 && board[endRow][endCol][0] == 'W') {
                return true;
            }
            return false;
        }
        
        return false;
    }
};


class Chess {
    King king;
    Queen queen;
    Knight knight;
    Bishop bishop;
    Rook rook;
    Pawn pawn;

    vector<vector<string>> board;
    int gameStatus;
    bool currentPlayer; // 0 for white, 1 for black

    Piece* getPiece(int row, int col) {
        if(board[row][col][1] == 'K') return &king;
        if(board[row][col][1] == 'Q') return &queen;
        if(board[row][col][1] == 'H') return &knight;
        if(board[row][col][1] == 'B') return &bishop;
        if(board[row][col][1] == 'R') return &rook;
        if(board[row][col][1] == 'P') return &pawn;
        throw invalid_argument("Invalid Piece");
    }

    bool isValidDimension(int position) {
        if(position >= 0 && position < 8) {
            return true;
        }
        return false;
    }

public:
    void init(vector<vector<string>>& chessboard) {
        board = chessboard;
        gameStatus = 0;
        currentPlayer = 0; // Game will be started by white
    }

    string move(int startRow, int startCol, int endRow, int endCol) {
        if(gameStatus) {
            return "Game already over";
        }

        if(!isValidDimension(startRow) || !isValidDimension(startCol)
            || !isValidDimension(endRow) || !isValidDimension(endCol)) {
            return "invalid";
        }

        char currentPlayerColor = currentPlayer == 0 ? 'W' : 'B';
        if(board[startRow][startCol] == "" || board[startRow][startCol][0] != currentPlayerColor) {
            return "invalid";
        }
        if(board[endRow][endCol] != "" && board[startRow][startCol][0] == board[endRow][endCol][0]) {
            return "invalid";
        }

        Piece* piece = getPiece(startRow, startCol);
        if(!piece->isValidMove(startRow, startCol, endRow, endCol, board)) {
            return "invalid";
        }

        // ChessBoard State Update
        string pieceKilled = board[endRow][endCol];
        board[endRow][endCol] = board[startRow][startCol];
        board[startRow][startCol] = "";
        currentPlayer = 1 - currentPlayer;

        if(pieceKilled == "BK") {
            gameStatus = 1;
        }
        else if(pieceKilled == "WK") {
            gameStatus = 2;
        }

        return pieceKilled;
    }

    int getGameStatus() {
        return gameStatus;
    }

    int getNextTurn() {
        if(gameStatus) return -1;
        return currentPlayer; // Next turn will be of current player
    }
};


int main() {
    vector<vector<string>> chessBoard = {
        {"WR", "WH", "WB", "WQ", "WK", "WB", "WH", "WR"},
        {"WP", "WP", "WP", "WP", "WP", "WP", "WP", "WP"},
        {"", "", "", "", "", "", "", ""},
        {"", "", "", "", "", "", "", ""},
        {"", "", "", "", "", "", "", ""},
        {"", "", "", "", "", "", "", ""},
        {"BP", "BP", "BP", "BP", "BP", "BP", "BP", "BP"},
        {"BR", "BH", "BB", "BQ", "BK", "BB", "BH", "BR"}
    };
    Chess* chess = new Chess();
    chess->init(chessBoard);

// White Pawn moves from (1, 5) to (2, 5)
std::cout << chess->move(1, 5, 2, 5) << '\n'; // returns ''
std::cout << chess->getNextTurn() << '\n';     // returns 1
std::cout << chess->getGameStatus() << '\n';   // returns 0

// Black Pawn moves from (6, 6) to (5, 6)
std::cout << chess->move(6, 6, 5, 6) << '\n'; // returns ''
std::cout << chess->getNextTurn() << '\n';     // returns 0
std::cout << chess->getGameStatus() << '\n';   // returns 0

// White Pawn moves from (2, 5) to (3, 5)
std::cout << chess->move(2, 5, 3, 5) << '\n'; // returns ''
std::cout << chess->getNextTurn() << '\n';     // returns 1
std::cout << chess->getGameStatus() << '\n';   // returns 0

// Black Pawn moves from (6, 2) to (5, 2)
std::cout << chess->move(6, 2, 5, 2) << '\n'; // returns ''
std::cout << chess->getNextTurn() << '\n';     // returns 0
std::cout << chess->getGameStatus() << '\n';   // returns 0

// White Knight moves from (0, 1) to (2, 2)
std::cout << chess->move(0, 1, 2, 2) << '\n'; // returns ''
std::cout << chess->getNextTurn() << '\n';     // returns 1
std::cout << chess->getGameStatus() << '\n';   // returns 0

// Black Pawn moves from (6, 4) to (5, 4)
std::cout << chess->move(6, 4, 5, 4) << '\n'; // returns ''
std::cout << chess->getNextTurn() << '\n';     // returns 0
std::cout << chess->getGameStatus() << '\n';   // returns 0

// White Pawn moves from (1, 7) to (2, 7)
std::cout << chess->move(1, 7, 2, 7) << '\n'; // returns ''
std::cout << chess->getNextTurn() << '\n';     // returns 1
std::cout << chess->getGameStatus() << '\n';   // returns 0

// Black Knight moves from (7, 6) to (5, 7)
std::cout << chess->move(7, 6, 5, 7) << '\n'; // returns ''
std::cout << chess->getNextTurn() << '\n';     // returns 0
std::cout << chess->getGameStatus() << '\n';   // returns 0

// White Knight moves from (2, 2) to (3, 4)
std::cout << chess->move(2, 2, 3, 4) << '\n'; // returns ''
std::cout << chess->getNextTurn() << '\n';     // returns 1
std::cout << chess->getGameStatus() << '\n';   // returns 0

// Black Pawn moves from (6, 5) to (5, 5)
std::cout << chess->move(6, 5, 5, 5) << '\n'; // returns ''
std::cout << chess->getNextTurn() << '\n';     // returns 0
std::cout << chess->getGameStatus() << '\n';   // returns 0

// White Knight moves from (3, 4) to (5, 5) and kills (captures) Black Pawn
std::cout << chess->move(3, 4, 5, 5) << '\n'; // returns 'BP'
std::cout << chess->getNextTurn() << '\n';     // returns 1
std::cout << chess->getGameStatus() << '\n';   // returns 0

// Black Pawn moves from (6, 0) to (5, 0)
std::cout << chess->move(6, 0, 5, 0) << '\n'; // returns ''
std::cout << chess->getNextTurn() << '\n';     // returns 0
std::cout << chess->getGameStatus() << '\n';   // returns 0

// White Knight moves from (5, 5) to (7, 4) and kills (captures) Black King
std::cout << chess->move(5, 5, 7, 4) << '\n'; // returns 'BK'
std::cout << chess->getNextTurn() << '\n';     // returns -1
std::cout << chess->getGameStatus() << '\n';   // returns 1, i.e. player with white pieces has WON.


    
    return 0;
}