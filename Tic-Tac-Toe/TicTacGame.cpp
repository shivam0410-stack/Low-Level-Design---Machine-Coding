#include <bits/stdc++.h>
using namespace std;

class TicTacGame {
    vector<vector<char>> matrix;
    int size;
    char ch1, ch2;

    bool isGameWon(int row, int col, char ch) {
        int i;
        for(i=0;i<size;i++) {
            if(matrix[row][i] != ch) {
                break;
            }
        }
        if(i == size) {
            return true;
        }

        for(i=0;i<size;i++) {
            if(matrix[i][col] != ch) {
                break;
            }
        }
        if(i == size) {
            return true;
        }

        if(row == col) {
            for(i=0;i<size;i++) {
                if(matrix[i][i] != ch) {
                    break;
                }
            }
            if(i == size) {
                return true;
            }
        }

        if(row + col == size - 1) {
            for(i=0;i<size;i++) {
                if(matrix[i][size - 1 - i] != ch) {
                    break;
                }
            }
            if(i == size) {
                return true;
            }
        }

        return false;
    }

public:
    TicTacGame(int size) : size(size) {
        matrix.assign(size, vector<char> (size, '.'));
        ch1 = 'O';
        ch2 = 'X';
    }

    int doMove(int row, int col, int player) {
        // Given each move is valid therefore no player or cell checks are being done
        // No draw checks since max number of calls are only size^2 and all are valid

        char ch = (player == 1) ? ch1 : ch2;
        matrix[row][col] = ch;

        if(isGameWon(row, col, ch)) {
            return player;
        }
        return 0;
    }
};


int main() {
    return 0;
}