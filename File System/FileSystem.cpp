#include <bits/stdc++.h>
using namespace std;

class Directory {
public:
    string name;
    Directory* parent;
    map<string, Directory*> children;

    Directory(const string& n, Directory* p) : name(n), parent(p) {}
};


class FileSystem {
    const string rootDirectoryName = "/";
    Directory* root;
    Directory* cwd;

    void createDirectory(Directory* temp, const string& part) {
        Directory* newDirectory = new Directory(part, temp);
        temp->children[part] = newDirectory;
    }

    vector<string> split(const string& path) {
        int n = path.length();
        vector<string> parts;
        string part;
        for(int i=0;i<n;i++) {
            if(path[i] == '/') {
                if(!part.empty()) {
                    parts.push_back(part);
                    part.clear();
                }

                while(path[i] == '/') {
                    i++;
                }
                i--;
            }
            else {
                part += path[i];
            }
        }
        if(!part.empty()) {
            parts.push_back(part);
        }
        return parts;
    }

public:
    FileSystem() {
        root = new Directory(rootDirectoryName, nullptr);
        root->parent = root;
        cwd = root;
    }

    string pwd() {
        if(cwd == root) {
            return "/";
        }

        Directory* temp = cwd;
        vector<string> parts;
        while(temp != root) {
            parts.push_back(temp->name);
            temp = temp->parent;
        }

        string path;
        int n = parts.size();
        for(int i=n-1;i>=0;i--) {
            path += '/';
            path += parts[i];
        }
        return path;
    }

    void mkdir(const string& path) {
        if(path.empty()) {
            return ;
        }

        vector<string> parts = split(path);
        Directory* temp = path[0] == '/' ? root : cwd;

        for(auto& part : parts) {
            if(part == ".") {
                continue;
            }
            if(part == "..") {
                temp = temp->parent;
                continue;
            }
            if(temp->children.find(part) == temp->children.end()) {
                createDirectory(temp, part);
            }
            temp = temp->children[part];
        }
    }

    void cd(const string& path) {
        if(path.empty()) {
            return ;
        }

        vector<string> parts = split(path);
        Directory* temp = path[0] == '/' ? root : cwd;

        for(auto& part : parts) {
            if(part == "*") {
                if(temp->children.empty()) {
                    continue;
                }
                temp = temp->children.begin()->second;
            }
            else if(part == ".") {
                continue;
            }
            else if(part == "..") {
                temp = temp->parent;
            }
            else if(temp->children.find(part) != temp->children.end()) {
                temp = temp->children[part];
            }
            else {
                return ;
            }
        }

        cwd = temp;
    }
};


int main() {
    return 0;
}