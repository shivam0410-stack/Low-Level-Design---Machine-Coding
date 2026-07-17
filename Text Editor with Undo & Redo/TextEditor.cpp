#include <bits/stdc++.h>
using namespace std;


enum class OPERATIONTYPE {
    ADD,
    DELETE,
    UNDO,
    REDO
};


struct Operation {
    OPERATIONTYPE operationType;
    int row, column;
    int length;
    string text;
};


class TextEditor {
    vector<string> document;
    stack<Operation> undoStack, redoStack;

    void fixStack(OPERATIONTYPE operationType, Operation reverseOperation) {
        if(operationType == OPERATIONTYPE::REDO) {
            undoStack.push(reverseOperation);
            redoStack.pop();
            return ;
        }

        if(operationType == OPERATIONTYPE::UNDO) {
            redoStack.push(reverseOperation);
            undoStack.pop();
            return ;
        }

        undoStack.push(reverseOperation);
        while(!redoStack.empty()) {
            redoStack.pop();
        }
    }

    void addText(int row, int column, const string& text, OPERATIONTYPE operationType) {
        if(row == document.size()) {
            document.push_back("");
        }

        string& line = document[row];
        line = line.substr(0, column) + text + line.substr(column);

        Operation reverseOperation = {OPERATIONTYPE::DELETE, row, column, (int)text.length(), ""};

        fixStack(operationType, reverseOperation);
    }

    void deleteText(int row, int startColumn, int length, OPERATIONTYPE operationType) {
        string& line = document[row];
        string deletedText = line.substr(startColumn, length);

        line = line.substr(0, startColumn) + line.substr(startColumn + length);

        Operation reverseOperation = {OPERATIONTYPE::ADD, row, startColumn, 0, deletedText};
        
        fixStack(operationType, reverseOperation);
    }

public:
    void addText(int row, int column, const string& text) {
        addText(row, column, text, OPERATIONTYPE::ADD);
    }

    void deleteText(int row, int startColumn, int length) {
        deleteText(row, startColumn, length, OPERATIONTYPE::DELETE);
    }

    void undo() {
        if(undoStack.empty()) {
            return ;
        }

        Operation undoOperation = undoStack.top();

        if(undoOperation.operationType == OPERATIONTYPE::ADD) {
            addText(undoOperation.row, undoOperation.column, undoOperation.text, OPERATIONTYPE::UNDO);
            return ;
        }

        deleteText(undoOperation.row, undoOperation.column, undoOperation.length, OPERATIONTYPE::UNDO);
    }

    void redo() {
        if(redoStack.empty()) {
            return ;
        }

        Operation redoOperation = redoStack.top();

        if(redoOperation.operationType == OPERATIONTYPE::ADD) {
            addText(redoOperation.row, redoOperation.column, redoOperation.text, OPERATIONTYPE::REDO);
            return ;
        }

        deleteText(redoOperation.row, redoOperation.column, redoOperation.length, OPERATIONTYPE::REDO);
    }

    string readLine(int row) {
        return document[row];
    }
};


#include <iostream>

int main() {
    // Assume TextEditor is a class with member functions matching the examples.
    // Creating the main object as a pointer named TextEditor.
    TextEditor* TextEditor = new ::TextEditor();

    std::cout << "--- Example 1 ---" << std::endl;
    TextEditor->addText(0, 0, "hello");
    std::cout << "readLine(0): " << TextEditor->readLine(0) << std::endl;
    TextEditor->addText(1, 0, "world");
    std::cout << "readLine(1): " << TextEditor->readLine(1) << std::endl;

    std::cout << "\n--- Example 2 ---" << std::endl;
    TextEditor->addText(0, 5, "-there");
    std::cout << "readLine(0): " << TextEditor->readLine(0) << std::endl;
    TextEditor->deleteText(0, 5, 6);
    std::cout << "readLine(0): " << TextEditor->readLine(0) << std::endl;
    TextEditor->undo();
    std::cout << "readLine(0) [after undo]: " << TextEditor->readLine(0) << std::endl;
    TextEditor->redo();
    std::cout << "readLine(0) [after redo]: " << TextEditor->readLine(0) << std::endl;

    std::cout << "\n--- Example 3 ---" << std::endl;
    TextEditor->addText(1, 5, "-wide web");
    std::cout << "readLine(1): " << TextEditor->readLine(1) << std::endl;
    TextEditor->deleteText(1, 5, 5);
    std::cout << "readLine(1): " << TextEditor->readLine(1) << std::endl;
    TextEditor->undo();
    std::cout << "readLine(1) [after undo]: " << TextEditor->readLine(1) << std::endl;

    std::cout << "\n--- Example 4 ---" << std::endl;
    TextEditor->addText(0, 5, "!");
    TextEditor->addText(0, 6, "!");
    TextEditor->undo();
    TextEditor->undo();
    TextEditor->redo();
    TextEditor->addText(0, 6, "?");
    TextEditor->redo();
    std::cout << "readLine(0): " << TextEditor->readLine(0) << std::endl;

    std::cout << "\n--- Example 5 ---" << std::endl;
    TextEditor->addText(0, 0, "aa bb-cc");
    TextEditor->deleteText(0, 0, 8);
    std::cout << "readLine(0): " << TextEditor->readLine(0) << std::endl;
    TextEditor->undo();
    std::cout << "readLine(0) [after undo]: " << TextEditor->readLine(0) << std::endl;

    std::cout << "\n--- Example 6 ---" << std::endl;
    TextEditor->addText(1, 0, "world");
    TextEditor->addText(1, 0, "hello-");
    std::cout << "readLine(1): " << TextEditor->readLine(1) << std::endl;

    delete TextEditor;
    return 0;
}
