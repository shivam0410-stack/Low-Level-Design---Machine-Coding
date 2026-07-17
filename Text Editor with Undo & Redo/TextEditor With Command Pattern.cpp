#include <bits/stdc++.h>
using namespace std;

// ---- Command interface ----
class ICommand {
public:
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual ~ICommand() = default;
};

// ---- Document (the "receiver") ----
class Document {
    vector<string> rows;
public:
    void ensureRow(int row) {
        if (row == (int)rows.size()) rows.push_back("");
    }
    void insert(int row, int column, const string& text) {
        ensureRow(row);
        string& line = rows[row];
        line = line.substr(0, column) + text + line.substr(column);
    }
    string remove(int row, int column, int length) {
        string& line = rows[row];
        string removed = line.substr(column, length);
        line = line.substr(0, column) + line.substr(column + length);
        return removed;
    }
    string readLine(int row) const { return rows[row]; }
};

// ---- Concrete commands ----
class AddCommand : public ICommand {
    Document& doc;
    int row, column;
    string text;
public:
    AddCommand(Document& d, int r, int c, string t)
        : doc(d), row(r), column(c), text(move(t)) {}

    void execute() override { doc.insert(row, column, text); }
    void undo() override { doc.remove(row, column, (int)text.length()); }
};

class DeleteCommand : public ICommand {
    Document& doc;
    int row, column, length;
    string removedText; // captured on execute, needed for undo
public:
    DeleteCommand(Document& d, int r, int c, int len)
        : doc(d), row(r), column(c), length(len) {}

    void execute() override { removedText = doc.remove(row, column, length); }
    void undo() override { doc.insert(row, column, removedText); }
};

// ---- Invoker / Editor ----
class TextEditor {
    Document document;
    stack<unique_ptr<ICommand>> undoStack, redoStack;

    void clearRedo() {
        while (!redoStack.empty()) redoStack.pop();
    }

public:
    void addText(int row, int column, const string& text) {
        auto cmd = make_unique<AddCommand>(document, row, column, text);
        cmd->execute();
        undoStack.push(move(cmd));
        clearRedo();
    }

    void deleteText(int row, int startColumn, int length) {
        auto cmd = make_unique<DeleteCommand>(document, row, startColumn, length);
        cmd->execute();
        undoStack.push(move(cmd));
        clearRedo();
    }

    void undo() {
        if (undoStack.empty()) return;
        auto cmd = move(undoStack.top());
        undoStack.pop();
        cmd->undo();
        redoStack.push(move(cmd));
    }

    void redo() {
        if (redoStack.empty()) return;
        auto cmd = move(redoStack.top());
        redoStack.pop();
        cmd->execute();
        undoStack.push(move(cmd));
    }

    string readLine(int row) { return document.readLine(row); }
};


int main() {
    return 0;
}