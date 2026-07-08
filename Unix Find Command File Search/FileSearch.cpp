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


class FileSearch {
    Directory* root;

    SearchStrategy* getStrategy(int searchCriteriaId, const string& args) {
        if(searchCriteriaId == 1) return new SearchCriteria1(args);
        return new SearchCriteria2(args);
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
        Directory* temp = cd(parts);

        SearchStrategy* searchStrategy = getStrategy(searchCriteriaId, args);
        vector<string> list;
        searchFiles(temp, dirPath, searchStrategy, list);

        return list;
    }
};


int main() {
//     FileSearch* s = new FileSearch();

// s->putFile("/data/pics/photoA.jpg", 4);
// s->putFile("/data/pics/movie.mp4", 12);
// s->putFile("/work/docs/readme.md", 1);
// s->putFile("/work/docs/report.xml", 7);

// //   Update (overwrite) size
// s->putFile("/data/pics/photoA.jpg", 9);

// //   Search Criteria 1: files > 8 MB inside /data
// vector<string> files = s->search(1, "/data", "8");
// for(auto& file : files) cout<<file<<" ";
// cout<<'\n';
// // Returns: ["/data/pics/photoA.jpg", "/data/pics/movie.mp4"]

// //   Search Criteria 2: files with ".xml" inside /work
// files = s->search(2, "/work", ".xml");
// for(auto& file : files) cout<<file<<" ";
// cout<<'\n';
// // Returns: ["/work/docs/report.xml"]
//     return 0;

FileSearch* s = new FileSearch();

s->putFile("/media/images/aa.jpg", 6);
s->putFile("/media/images/ab.jpg", 7);
s->putFile("/media/images/ac.xml", 2);
s->putFile("/office/reports/r1.xml", 9);
s->putFile("/office/reports/r2.xml", 4);
s->putFile("/office/notes.txt", 3);

//   Search Criteria 1 under a subdirectory
vector<string> files = s->search(1, "/media/images", "5");
for(auto& file : files) cout<<file<<" ";
cout<<'\n';
// Returns: ["/media/images/aa.jpg", "/media/images/ab.jpg"]

//   Search Criteria 2 under a parent directory (recursive)
files = s->search(2, "/office", ".xml");
for(auto& file : files) cout<<file<<" ";
cout<<'\n';
// Returns: ["/office/reports/r1.xml", "/office/reports/r2.xml"]
}