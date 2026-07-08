#include <bits/stdc++.h>
using namespace std;


class CharacterStyle {
    string fontName;
    int fontSize;
    bool isBold;
    bool isItalic;

public:
    CharacterStyle(const string& fontName, int fontSize,
        bool isBold, bool isItalic) : fontName(fontName),
        fontSize(fontSize), isBold(isBold), isItalic(isItalic) {}

    string getStyle() {
        string result = fontName + '-' + to_string(fontSize);
        if(isBold) result += "-b";
        if(isItalic) result += "-i";
        return result;
    }
};


class CharacterStyleFactory {
    unordered_map<string, CharacterStyle*> charStyleMap;

public:
    CharacterStyle* getStyle(const string& fontName, int fontSize,
    bool isBold, bool isItalic) {
        string key = fontName + '-' + to_string(fontSize);
        if(isBold) key += "-b";
        if(isItalic) key += "-i";

        if(charStyleMap.find(key) == charStyleMap.end()) {
            charStyleMap[key] = new CharacterStyle(fontName, fontSize, isBold, isItalic);
        }
        return charStyleMap[key];
    }
};


class TextEditor {
    vector<vector<pair<char, CharacterStyle*>>> document;
    CharacterStyleFactory factory;

public:
    void addCharacter(int row, int column, char ch,
    const string& fontName, int fontSize,
    bool isBold, bool isItalic) {
        CharacterStyle* style = factory.getStyle(
            fontName, fontSize, isBold, isItalic);

        if(document.size() <= row) {
            document.resize(row+1);
        }

        document[row].push_back({ch, style});
        for(int i = document[row].size() - 1; i > column; i--) {
            swap(document[row][i], document[row][i-1]);
        }
    }

    string getStyle(int row, int col) {
        if(document.size() <= row || document[row].size() <= col) {
            return "";
        }

        return document[row][col].first + string("-") + document[row][col].second->getStyle();
    }

    string readLine(int row) {
        if(document.size() <= row) {
            return "";
        }

        string result;
        for(auto& [ch, style] : document[row]) {
            result += ch;
        }
        return result;
    }

    bool deleteCharacter(int row, int col) {
        if(document.size() <= row || document[row].size() <= col) {
            return false;
        }

        int length = document[row].size();
        for(int i = col + 1; i < length; i++) {
            document[row][i - 1] = document[row][i]; 
        }
        document[row].pop_back();
        return true;
    }
};


int main() {
    // Pointer instantiation
    TextEditor* obj = new TextEditor();
    
    // Method calls using pointer formatting
    // obj->init(helper);
    obj->addCharacter(0, 0, 'g', "Cambria", 17, true, true);
    obj->addCharacter(1, 0, 'y', "Century Gothic", 14, true, true);
    obj->addCharacter(1, 1, 'h', "Courier New", 22, false, false);
    obj->addCharacter(1, 2, 'y', "Georgia", 14, false, false);

    // Outputs using std::cout
    std::cout << obj->getStyle(0, 0) << std::endl;
    std::cout << obj->readLine(0) << std::endl;
    
    obj->addCharacter(0, 0, 'q', "Arial", 21, false, true);
    std::cout << obj->readLine(0) << std::endl;

    std::cout << obj->readLine(1) << std::endl;
    std::cout << std::boolalpha << obj->deleteCharacter(1, 1) << std::endl;
    std::cout << obj->readLine(1) << std::endl;
    std::cout << std::boolalpha << obj->deleteCharacter(1, 4) << std::endl;

    // Clean up memory
    delete obj;
    return 0;
}