#include <bits/stdc++.h>
using namespace std;

// Improvements - If collection size reduces to 0 then remove from collectionMap

struct File {
    string name;
    int size;
    vector<string> collections;

    File(const string& name, int size, const vector<string>& collections) :
    name(name), size(size), collections(collections) {}
};


struct Collection {
    string name;
    int size, fileCount;

    Collection(const string& name, int size) : name(name), size(size), fileCount(1) {}

    void addFile(int fileSize) {
        size += fileSize;
        fileCount++;
    }

    void removeFile(int fileSize) {
        size -= fileSize;
        fileCount--;
    }
};

struct Comparator {
    bool operator()(const pair<int, string>& p1, const pair<int, string>& p2) const {
        if(p1.first != p2.first) {
            return p1.first < p2.first;
        }
        return p1.second > p2.second;
    }
};

class Strategy {
public:
    virtual vector<string> getTopCollections(unordered_map<string, Collection*>& collectionMap) = 0;
};
class SizeStrategy : public Strategy {
public:
    vector<string> getTopCollections(unordered_map<string, Collection*>& collectionMap) {
        set<pair<int, string>, Comparator> sortedCollections;
        for(auto& [name, collection] : collectionMap) {
            if(sortedCollections.size() < 10) {
                sortedCollections.insert({collection->size, name});
            }
            else if(collection->size > sortedCollections.begin()->first ||
            (collection->size == sortedCollections.begin()->first &&
            name < sortedCollections.begin()->second))
            {
                sortedCollections.erase(sortedCollections.begin());
                sortedCollections.insert({collection->size, name});
            }
        }

        vector<string> collections;
        for(auto it = sortedCollections.rbegin(); it != sortedCollections.rend(); it++) {
            collections.push_back(it->second);
        }
        return collections;
    }
};
class FileCountStrategy : public Strategy {
public:
    vector<string> getTopCollections(unordered_map<string, Collection*>& collectionMap) {
        set<pair<int, string>, Comparator> sortedCollections;
        for(auto& [name, collection] : collectionMap) {
            if(sortedCollections.size() < 10) {
                sortedCollections.insert({collection->fileCount, name});
            }
            else if(collection->fileCount > sortedCollections.begin()->first ||
            (collection->fileCount == sortedCollections.begin()->first &&
            name < sortedCollections.begin()->second))
            {
                sortedCollections.erase(sortedCollections.begin());
                sortedCollections.insert({collection->fileCount, name});
            }
        }

        vector<string> collections;
        for(auto it = sortedCollections.rbegin(); it != sortedCollections.rend(); it++) {
            collections.push_back(it->second);
        }
        return collections;
    }
};


class CollectionManager {
    unordered_map<string, Collection*> collectionMap;
    vector<Strategy*> strategies;

public:
    CollectionManager() {
        strategies.resize(2);
        strategies[0] = new SizeStrategy();
        strategies[1] = new FileCountStrategy();
    }

    void addFileToCollections(const vector<string>& collections, int size) {
        for(auto& name : collections) {
            auto collectionIt = collectionMap.find(name);

            if(collectionIt != collectionMap.end()) {
                Collection* collection = collectionIt->second;
                collection->addFile(size);
            }
            else {
                Collection* collection = new Collection(name, size);
                collectionMap[name] = collection;
            }
        }
    }

    void removeFileFromCollections(const vector<string>& collections, int size) {
        for(auto& name : collections) {
            Collection* collection = collectionMap[name];
            collection->removeFile(size);
        }
    }

    vector<string> getTopCollections(int strategy) {
        return strategies[strategy]->getTopCollections(collectionMap);
    }
};


class FileCollectionTracker {
    unordered_map<string, File*> files;
    CollectionManager collectionManager;
    int totalFileSize;

public:
    FileCollectionTracker() : totalFileSize(0) {}

    void addFile(const string& fileName, int fileSize, const vector<string>& collections) {
        auto fileIt = files.find(fileName);

        if(fileIt != files.end()) {
            File* file = fileIt->second;
            vector<string>& previousCollections = file->collections;
            int previousFileSize = file->size;

            collectionManager.removeFileFromCollections(previousCollections, file->size);
            totalFileSize -= previousFileSize;

            file->size = fileSize;
            file->collections = collections;
        }
        else {
            files[fileName] = new File(fileName, fileSize, collections);
        }

        collectionManager.addFileToCollections(collections, fileSize);
        totalFileSize += fileSize;
    }

    int getTotalFileSize() {
        return totalFileSize;
    }

    vector<string> getTopCollections(int strategy) {
        return collectionManager.getTopCollections(strategy);
    }
};


int main() {
    // Conceptual initialization using a pointer as requested
    FileCollectionTracker* fs = new FileCollectionTracker();

    // --- Example 1: Basic adds, total size, top collections ---
    fs->addFile("/a/b/file1.txt", 400, {"collection-1", "travel-collection-2"});
    fs->addFile("/a/b/file2.txt", 100, {"collection-1"});
    fs->addFile("/c/file3.txt", 200, {"travel-collection-2", "work"});
    fs->addFile("/d/e/file4.txt", 300, {}); // no collections

    // getTotalFileSize() -> 1000
    std::cout << "Total File Size: " << fs->getTotalFileSize() << std::endl;

    // getTopCollections(0) -> ["travel-collection-2", "collection-1", "work"] (by size)
    std::vector<std::string> topBySize = fs->getTopCollections(0);
    std::cout << "Top collections by size: ";
    for (const auto& col : topBySize) std::cout << col << " ";
    std::cout << std::endl;

    // getTopCollections(1) -> ["collection-1", "travel-collection-2", "work"] (by count)
    std::vector<std::string> topByCount = fs->getTopCollections(1);
    std::cout << "Top collections by count: ";
    for (const auto& col : topByCount) std::cout << col << " ";
    std::cout << std::endl;


    // --- Updating a file changes totals and collection stats ---
    fs->addFile("/a/b/file1.txt", 50, {"work"}); // update existing file1

    // getTotalFileSize() -> 650
    std::cout << "New Total File Size: " << fs->getTotalFileSize() << std::endl;

    // getTopCollections(0) -> ["work", "travel-collection-2", "collection-1"] (by size)
    std::vector<std::string> updatedBySize = fs->getTopCollections(0);
    std::cout << "Updated top collections by size: ";
    for (const auto& col : updatedBySize) std::cout << col << " ";
    std::cout << std::endl;

    // getTopCollections(1) -> ["work", "collection-1", "travel-collection-2"] (by count)
    std::vector<std::string> updatedByCount = fs->getTopCollections(1);
    std::cout << "Updated top collections by count: ";
    for (const auto& col : updatedByCount) std::cout << col << " ";
    std::cout << std::endl;

    // Clean up memory
    delete fs;
    return 0;
}