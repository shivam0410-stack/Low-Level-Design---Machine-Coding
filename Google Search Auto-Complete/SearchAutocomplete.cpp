#include <bits/stdc++.h>
using namespace std;


struct Comparator {
    bool operator()(const pair<string, int>& sentence1, const pair<string, int>& sentence2) {
        if(sentence1.second != sentence2.second) {
            return sentence1.second > sentence2.second;
        }
        return sentence1.first < sentence2.first;
    }
};

struct Node {
    Node* child[27];
    set<pair<string, int>, Comparator> sentences;

    Node() {
        for(int i=0;i<27;i++) {
            child[i] = nullptr;
        }
    }

    void insert(const string& phrase, int frequency) {
        auto sentenceIt = sentences.find({phrase, frequency - 1});
        if(sentenceIt != sentences.end()) {
            sentences.erase(sentenceIt);
        }
        sentences.insert({phrase, frequency});
        if(sentences.size() > 3) {
            sentences.erase(--sentences.end());
        }
    }


    vector<string> top3Phrases() {
        vector<string> list;
        for(auto& sentence : sentences) {
            list.push_back(sentence.first);
            if(list.size() >= 3) {
                break;
            }
        }
        return list;
    }
};


class SearchAutocomplete {
    Node* head;
    Node* currentNode;
    string currentPhrase;
    unordered_map<string, int> frequency;

    void insert(const string& phrase, int count) {
        Node* tempHead = head;
        int length = phrase.length();
        for(int i=0;i<length;i++) {
            int index = phrase[i] == ' ' ? 26 : phrase[i] - 'a';
            if(tempHead->child[index] == nullptr) {
                tempHead->child[index] = new Node();
            }
            tempHead = tempHead->child[index];
            tempHead->insert(phrase, count);
        }

        frequency[phrase] = count;
    }

public:
    SearchAutocomplete(const vector<string>& phrases, const vector<int>& counts) {
        head = new Node();
        currentNode = head;

        int totalPhrases = phrases.size();
        for(int i=0;i<totalPhrases;i++) {
            insert(phrases[i], counts[i]);
        }
    }

    vector<string> getSuggestions(char newChar) {
        if(newChar == '#') {
            if(frequency.find(currentPhrase) == frequency.end()) {
                insert(currentPhrase, 1);
            }
            else {
                insert(currentPhrase, frequency[currentPhrase] + 1);
            }
            currentPhrase.clear();
            currentNode = head;
            return {};
        }

        currentPhrase += newChar;
        int index = newChar == ' ' ? 26 : newChar - 'a';
        if(currentNode->child[index] == nullptr) {
            currentNode->child[index] = new Node();
        }
        currentNode = currentNode->child[index];
        return currentNode->top3Phrases();
    }
};


int main() {
    return 0;
}