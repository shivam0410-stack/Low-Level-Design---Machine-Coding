#include <bits/stdc++.h>
using namespace std;


class WordTrie {
    string storedWord;
    vector<WordTrie*> child;

    int getIndex(const char& ch) {
        int index;

        if(ch == ' ') index = 0;
        else if(ch == '-') index = 1;
        else index = ch - 'a' + 2;

        return index;
    }

    void storeWords(int& n, vector<string>& result) {
        if(!storedWord.empty()) {
            result.push_back(storedWord);

            if(--n == 0) {
                return ;
            }
        }

        for(int i=0;i<28;i++) {
            if(child[i] == nullptr) {
                continue;
            }
            child[i]->storeWords(n, result);

            if(n == 0) {
                return ;
            }
        }
    }

public:
    WordTrie() {
        child.resize(40, nullptr);
    }

    void insert(const string& word, int charIndex) {
        if(charIndex == word.length()) {
            storedWord = word;
            return ;
        }

        int index = getIndex(word[charIndex]);
        if(child[index] == nullptr) {
            child[index] = new WordTrie();
        }

        child[index]->insert(word, charIndex + 1);
    }

    void searchWords(const string& prefix, int n, vector<string>& result, int charIndex) {
        if(charIndex == prefix.length()) {
            storeWords(n, result);
            return ;
        }

        int index = getIndex(prefix[charIndex]);
        if(child[index] == nullptr) {
            return ;
        }

        child[index]->searchWords(prefix, n, result, charIndex + 1);
    }

    bool exists(const string& word, int charIndex) {
        if(charIndex == word.length()) {
            if(!storedWord.empty()) {
                return true;
            }
            return false;
        }

        if(word[charIndex] == '.') {
            for(int i=0;i<28;i++) {
                if(child[i] == nullptr) {
                    continue;
                }
                if(child[i]->exists(word, charIndex + 1)) {
                    return true;
                }
            }

            return false;
        }

        int index = getIndex(word[charIndex]);
        if(child[index] == nullptr) {
            return false;
        }

        return child[index]->exists(word, charIndex + 1);
    }
};


class Dictionary {
    WordTrie* wordTrie;
    unordered_map<string, string> wordMeanings;

public:
    Dictionary() {
        wordTrie = new WordTrie();
    }

    void storeWord(const string& word, const string& meaning) {
        auto wordMeaningIt = wordMeanings.find(word);

        if(wordMeaningIt != wordMeanings.end()) {
            wordMeaningIt->second = meaning;
            return ;
        }

        wordMeanings[word] = meaning;

        wordTrie->insert(word, 0);
    }

    string getMeaning(const string& word) {
        auto wordMeaningIt = wordMeanings.find(word);

        if(wordMeaningIt != wordMeanings.end()) {
            return wordMeaningIt->second;
        }

        return "";
    }

    vector<string> searchWords(const string& prefix, int n) {
        vector<string> result;
        wordTrie->searchWords(prefix, n, result, 0);
        return result;
    }

    bool exists(const string& word) {
        return wordTrie->exists(word, 0);
    }
};


int main() {
    // Dictionary object as a pointer
    Dictionary* dict = new Dictionary();

    // --- Store, Overwrite, and Fetch ---
    dict->storeWord("apple", "a fruit");
    std::cout << dict->getMeaning("apple") << std::endl;  // Prints: a fruit

    dict->storeWord("apple", "sweet fruit"); // Overwrites
    std::cout << dict->getMeaning("apple") << std::endl;  // Prints: sweet fruit
    std::cout << dict->getMeaning("apples") << std::endl; // Prints: (empty string)

    // --- Prefix Search with Sorting and Limit ---
    dict->storeWord("app", "short for application");
    dict->storeWord("apple", "sweet fruit");
    dict->storeWord("apply", "make a formal request");
    dict->storeWord("apt", "suitable");
    dict->storeWord("banana", "yellow fruit");

    // Printing prefix search results (assuming returns std::vector<std::string>)
    for (const auto& word : dict->searchWords("ap", 3)) { std::cout << word << " "; }   std::cout << std::endl;
    for (const auto& word : dict->searchWords("app", 10)) { std::cout << word << " "; } std::cout << std::endl;
    for (const auto& word : dict->searchWords("b", 2)) { std::cout << word << " "; }    std::cout << std::endl;

    // --- Existence with . Wildcards ---
    // Reset scenario with new words
    delete dict;
    dict = new Dictionary();

    dict->storeWord("cat", "...");
    dict->storeWord("cap", "...");
    dict->storeWord("caps", "...");
    dict->storeWord("map", "...");
    dict->storeWord("man", "...");
    dict->storeWord("many", "...");

    // Printing boolean results as true/false text
    std::cout << std::boolalpha;
    std::cout << dict->exists("cat") << std::endl;  // Prints: true
    std::cout << dict->exists("c.t") << std::endl;  // Prints: true
    std::cout << dict->exists("ca.") << std::endl;  // Prints: true
    std::cout << dict->exists("..p") << std::endl;  // Prints: true
    std::cout << dict->exists("c..s") << std::endl; // Prints: true
    std::cout << dict->exists("....") << std::endl; // Prints: true
    std::cout << dict->exists("c..") << std::endl;  // Prints: true
    std::cout << dict->exists("c.") << std::endl;   // Prints: false
    std::cout << dict->exists("...y") << std::endl; // Prints: true

    // Clean up memory
    delete dict;
    return 0;
}
