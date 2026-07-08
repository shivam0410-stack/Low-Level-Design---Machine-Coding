#include <bits/stdc++.h>
using namespace std;


struct PairHash {
    size_t operator()(const pair<int, int>& position) const {
        return position.first*100 + position.second;
    }
};

class SnakeGame {
    queue<pair<int, int>> food, snake; // Snake Positions from tail to head
    unordered_set<pair<int, int>, PairHash> snakePositions;
    int rows, cols, score;
    pair<int, int> head;

    pair<int, int> delimit(const string& position) {
        int delimitIndex = position.find(',');
        return {stoi(position.substr(0, delimitIndex)), stoi(position.substr(delimitIndex + 1))};
    }

    bool isGameEnded() {
        if(head.first < 0 || head.first >= rows || head.second < 0 || head.second >= cols) {
            return true;
        }

        if(snakePositions.find(head) != snakePositions.end() && head != snake.front()) {
            return true;
        }

        return false;
    }

public:
    SnakeGame(int rows, int cols, const vector<string>& foodPositions) : rows(rows), cols(cols), score(0) {
        for(auto& foodPosition : foodPositions) {
            food.push(delimit(foodPosition));
        }
        snake.push({0, 0});
        snakePositions.insert({0, 0});
    }

    int move(const string& direction) {
        if(direction == "U") head.first--;
        else if(direction == "D") head.first++;
        else if(direction == "L") head.second--;
        else if(direction == "R") head.second++;

        if(isGameEnded()) {
            return score = -1;
        }

        // Food Available;
        if(food.front() == head) {
            food.pop();
            score++;
        }
        else {
            snakePositions.erase(snake.front());
            snake.pop();
        }

        snakePositions.insert(head);
        snake.push(head);
        return score;
    }
};


int main() {
    return 0;
}