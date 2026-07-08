#include <bits/stdc++.h>
using namespace std;


class Style {
    string fontName;
    int fontSize;
    bool isBold;
    bool isItalic;

public:
    Style(const string& fontName, int fontSize, bool isBold,
        bool isItalic) : fontName(fontName), fontSize(fontSize),
        isBold(isBold), isItalic(isItalic) {}

    string getStyle() {
        string result = fontName + '-' + to_string(fontSize);
        if(isBold) result += "-b";
        if(isItalic) result += "-i";
        return result;
    }
};


class StyleFactory {
    unordered_map<string, Style*> styleMap;

public:
    Style* getStyle(const string& fontName, int fontSize, bool isBold,
    bool isItalic) {
        string key = fontName + '-' + to_string(fontSize);
        if(isBold) key += "-b";
        if(isItalic) key += "-i";

        if(styleMap.find(key) == styleMap.end()) {
            styleMap[key] = new Style(fontName, fontSize, isBold, isItalic);
        }
        return styleMap[key];
    }
};


class Cell {
    string content;
    Style* style;

public:
    Cell(const string& content, Style* style) : content(content), style(style) {}

    string getCell() {
        return content + '-' + style->getStyle();
    }

    void updateCell(const string& newContent, Style* newStyle) {
        content = newContent;
        style = newStyle;
    }
};


class Spreadsheet {
    StyleFactory factory;
    vector<int> rowIds, colIds;
    unordered_map<string, Cell*> cellMap;

    string getKey(int row, int column) {
        return to_string(rowIds[row]) + '-' + to_string(colIds[column]);
    }

public:
    Spreadsheet() {
        rowIds.resize(5);
        colIds.resize(5);
        for(int i=0;i<5;i++) {
            rowIds[i] = i;
            colIds[i] = i;
        }
    }

    void addRow(int index) {
        rowIds.push_back(rowIds.size());
        for(int i = rowIds.size() - 1; i > index; i--) {
            swap(rowIds[i], rowIds[i-1]);
        }
    }

    void addColumn(int index) {
        colIds.push_back(colIds.size());
        for(int i = colIds.size() - 1; i > index; i--) {
            swap(colIds[i], colIds[i-1]);
        }
    }

    string getEntry(int row, int column) {
        string key = getKey(row, column);
        if(cellMap.find(key) == cellMap.end()) {
            return "";
        }

        return cellMap[key]->getCell();
    }

    void addEntry(int row, int column, const string& text,
    const string& fontName, int fontSize, bool isBold, bool isItalic) {
        string key = getKey(row, column);
        Style* style = factory.getStyle(fontName, fontSize, isBold, isItalic);

        if(cellMap.find(key) == cellMap.end()) {
            cellMap[key] = new Cell(text, style);
            return ;
        }
        cellMap[key]->updateCell(text, style);
    }
};


int main() {
    // Pointer instantiation with Spreadsheet class
    Spreadsheet* obj = new Spreadsheet();

    // Insert a row at the end and add an entry
    obj->addRow(5);
    obj->addEntry(5, 0, "hello", "tahoma", 24, true, false);
    std::cout << obj->getEntry(5, 0) << std::endl;

    // Insert a column in the middle and verify shifting
    obj->addColumn(2);
    obj->addEntry(0, 1, "x", "calibri", 10, false, true);
    std::cout << obj->getEntry(0, 1) << std::endl;

    // After addColumn(1), the prior (0,1) shifts to (0,2)
    obj->addColumn(1);
    std::cout << obj->getEntry(0, 2) << std::endl;
    std::cout << obj->getEntry(0, 1) << std::endl;

    // Replace existing entry
    obj->addEntry(5, 0, "greetings", "tahoma", 24, false, false);
    std::cout << obj->getEntry(5, 0) << std::endl;

    // Bold/Italic flags
    obj->addEntry(1, 3, "note", "algerian", 14, true, true);
    std::cout << obj->getEntry(1, 3) << std::endl;

    // Empty cell check
    std::cout << obj->getEntry(2, 2) << std::endl;

    // Clean up memory
    delete obj;
    return 0;
}