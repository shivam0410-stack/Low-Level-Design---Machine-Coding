#include <bits/stdc++.h>
using namespace std;

struct Directory {
    string name;
    int sizeMb;
    map<string, Directory*> children;

    Directory(const string& name, int sizeMb) : name(name), sizeMb(sizeMb) {}
};


class SearchStrategy {
public:
    virtual bool isSatisfied(Directory* dir) const = 0;
};

class SearchCriteria1 : public SearchStrategy {
    int minSize;

public:
    SearchCriteria1(const string& args) : minSize(stoi(args)) {}

    bool isSatisfied(Directory* dir) const override {
        return dir->sizeMb > minSize;
    }
};

class SearchCriteria2 : public SearchStrategy {
    string extension;

public:
    SearchCriteria2(const string& args) : extension(args) {}

    bool isSatisfied(Directory* dir) const override {
        string fileName = dir->name;
        if(fileName.length() < extension.length()) return false;

        return fileName.substr(fileName.length() - extension.length()) == extension;
    }
};

// Using 2-pointer approach since results are sorted
class Specification {
public:
    virtual vector<string> getResult(vector<string>& result1, vector<string>& result2) const = 0;
};

class ANDSpecification : public Specification {
public:
    vector<string> getResult(vector<string>& r1, vector<string>& r2) const override {
        vector<string> result;
        int i = 0, j = 0;
        while(i < r1.size() && j < r2.size()) {
            int comp = r1[i].compare(r2[j]);
            if(comp < 0) i++;
            else if(comp > 0) j++;
            else {
                result.push_back(r1[i]);
                i++;
                j++;
            }
        }
        return result;
    }
};

class ORSpecification : public Specification {
public:
    vector<string> getResult(vector<string>& r1, vector<string>& r2) const override {
        vector<string> result;
        int i = 0, j = 0;
        while(i < r1.size() && j < r2.size()) {
            int comp = r1[i].compare(r2[j]);
            if(comp < 0) {
                result.push_back(r1[i]);
                i++;
            }
            else if(comp > 0) {
                result.push_back(r2[j]);
                j++;
            }
            else {
                result.push_back(r1[i]);
                i++;
                j++;
            }
        }
        while(i < r1.size()) {
            result.push_back(r1[i]);
            i++;
        }
        while(j < r2.size()) {
            result.push_back(r2[j]);
            j++;
        }
        return result;
    }
};

class ANDNOTSpecification : public Specification {
public:
    vector<string> getResult(vector<string>& r1, vector<string>& r2) const override {
        vector<string> result;
        int i = 0, j = 0;
        while(i < r1.size() && j < r2.size()) {
            int comp = r1[i].compare(r2[j]);
            if(comp < 0) {
                result.push_back(r1[i]);
                i++;
            }
            else if(comp > 0) {
                j++;
            }
            else {
                i++;
                j++;
            }
        }
        while(i < r1.size()) {
            result.push_back(r1[i]);
            i++;
        }
        return result;
    }
};


class FileSearch {
    Directory* root;

    SearchStrategy* getStrategy(int searchCriteriaId, const string& args) {
        if(searchCriteriaId == 1) return new SearchCriteria1(args);
        if(searchCriteriaId == 2) return new SearchCriteria2(args);
        throw invalid_argument("Unknown search criteria");
    }

    Specification* getSpecification(const string& op) {
        if(op == "AND") return new ANDSpecification();
        if(op == "OR") return new ORSpecification();
        if(op == "AND NOT") return new ANDNOTSpecification();
        throw invalid_argument("Unknown operation");
    }

    bool isFile(const string& name) {

        return name.find('.') != string::npos;
    }

    void searchFiles(Directory* temp, string dirPath, SearchStrategy* searchStrategy, vector<string>& list) {
        // Automatically in sorted order since using maps
        for(auto& child : temp->children) {
            if(isFile(child.first) && searchStrategy->isSatisfied(child.second)) {
                list.push_back(dirPath + '/' + child.first);
            }
            else {
                searchFiles(child.second, dirPath + '/' + child.first, searchStrategy, list);
            }
        }
    }

    vector<string> splitPath(const string& path) {
        int len = path.length();
        string folder;
        vector<string> parts;
        for(int i=1;i<len;i++) {
            if(path[i] == '/') {
                parts.push_back(folder);
                folder.clear();
            }
            else {
                folder.push_back(path[i]);
            }
        }
        parts.push_back(folder);
        return parts;
    }

    Directory* cd(vector<string>& parts) {
        Directory* temp = root;
        for(auto& part : parts) {
            if(temp->children[part] == nullptr) temp->children[part] = new Directory(part, 0);
            temp = temp->children[part];
        }
        return temp;
    }

    Directory* findDirectory(vector<string>& parts) {
        Directory* temp = root;
        for(auto& part : parts) {
            if(temp->children[part] == nullptr) return nullptr;
            temp = temp->children[part];
        }
        return temp;
    }

public:
    FileSearch() {
        root = new Directory("/", 0);
    }

    void putFile(string path, int sizeMb) {
        vector<string> parts = splitPath(path);
        string file = parts.back();
        parts.pop_back();
        Directory* temp = cd(parts);

        if(temp->children[file] == nullptr) temp->children[file] = new Directory(file, sizeMb);
        else temp->children[file]->sizeMb = sizeMb;
    }

    vector<string> search(int searchCriteriaId, string dirPath, string args) {
        vector<string> parts = splitPath(dirPath);
        Directory* temp = findDirectory(parts);
        if(temp == nullptr) {
            return {};
        }

        SearchStrategy* searchStrategy = getStrategy(searchCriteriaId, args);
        vector<string> list;
        searchFiles(temp, dirPath, searchStrategy, list);

        return list;
    }

    vector<string> runQuery(vector<string>& rules, vector<string>& ops) {
        if(rules.size() == 0) {
            return {};
        }

        int first = rules[0].find(',');
        int second = rules[0].find(',', first + 1);

        int searchCriteriaId = stoi(rules[0].substr(0, first));
        string dirPath = rules[0].substr(first + 1, second - first - 1);
        string args = rules[0].substr(second + 1);

        vector<string> result = search(searchCriteriaId, dirPath, args);

        for(int i=0;i<ops.size();i++) {
            first = rules[i+1].find(',');
            second = rules[i+1].find(',', first + 1);

            searchCriteriaId = stoi(rules[i+1].substr(0, first));
            dirPath = rules[i+1].substr(first + 1, second - first - 1);
            args = rules[i+1].substr(second + 1);

            vector<string> subResult = search(searchCriteriaId, dirPath, args);
            Specification* spec = getSpecification(ops[i]);

            result = spec->getResult(result, subResult);
        }

        return result;
    }
};


int main() {

}