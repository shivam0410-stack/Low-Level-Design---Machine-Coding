#include <bits/stdc++.h>
using namespace std;

class LongestPathAndIncreasingPaths {
    int rows, cols;
    char delimiter;
    vector<vector<int>> grid, longestPath;

    void delimit(const vector<string>& gridLines) {
        rows = gridLines.size();
        delimiter = ',';
        grid.resize(rows);

        for(int i=0;i<rows;i++) {
            stringstream ss(gridLines[i]);
            string num;
            while(getline(ss, num, delimiter)) {
                grid[i].push_back(stoi(num));
            }
        }

        cols = grid[0].size();
        longestPath.assign(rows, vector<int> (cols, 0));
    }

    string stringify(int row, int col) {
        return '(' + to_string(row) + ',' + to_string(col) + ")=" + to_string(grid[row][col]);
    }
    
    bool isValid(int row, int col) {
        if(row < 0 || row >= rows || col < 0 || col >= cols) {
            return false;
        }
        return true;
    }

    int dfs(int row, int col) {
        if(longestPath[row][col] != 0) {
            return longestPath[row][col];
        }
        int dx[] = {-1, 0, 1, 0, -1};
        for(int i=0;i<4;i++) {
            int newRow = row + dx[i];
            int newCol = col + dx[i+1];
            if(isValid(newRow, newCol) && grid[newRow][newCol] > grid[row][col]) {
                longestPath[row][col] = max(longestPath[row][col], 1 + dfs(newRow, newCol));
            }
        }
        return longestPath[row][col];
    }

public:
    int getLongestPathLength(const vector<string>& gridLines) {
        delimit(gridLines);
        int longestPathLength = 0;
        for(int i=0;i<rows;i++) {
            for(int j=0;j<cols;j++) {
                longestPathLength = max(longestPathLength, dfs(i, j));
            }
        }
        return longestPathLength;
    }

    string getOneLongestPath(const vector<string>& gridLines) {
        int longestPathLength = getLongestPathLength(gridLines);
        int row = -1, col = -1;
        string path;

        for(int i=0;i<rows;i++) {
            for(int j=0;j<cols;j++) {
                if(longestPath[i][j] == longestPathLength) {
                    if(path.empty()) {
                        path = stringify(i, j);
                        row = i;
                        col = j;
                    }
                    else {
                        string newPath = stringify(i, j);
                        if(newPath < path) {
                            path = newPath;
                            row = i;
                            col = j;
                        }
                    }
                }
            }
        }

        while(--longestPathLength) {
            int dx[] = {-1, 0, 1, 0, -1};
            string next;
            int nextRow, nextCol;
            for(int i=0;i<4;i++) {
                int newRow = row + dx[i];
                int newCol = col + dx[i+1];
                if(isValid(newRow, newCol) && grid[newRow][newCol] > grid[row][col] && longestPath[row][col] == 1 + longestPath[newRow][newCol]) {
                    if(next.empty()) {
                        next = stringify(newRow, newCol);
                        nextRow = newRow;
                        nextCol = newCol;
                    }
                    else {
                        string newNext = stringify(newRow, newCol);
                        if(newNext < next) {
                            next = newNext;
                            nextRow = newRow;
                            nextCol = newCol;
                        }
                    }
                }
            }
            path += "->";
            path += next;
            row = nextRow;
            col = nextCol;
        }

        return path;
    }

    vector<string> getAllStrictlyIncreasingPaths(const vector<string>& gridLines) {

    }
};

int main() {
    return 0;
}