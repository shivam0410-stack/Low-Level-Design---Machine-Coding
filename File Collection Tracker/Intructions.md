37. Design a File Collections Tracker
Asked in
Atlassian
Design an in-memory system that tracks files and their membership in different collections.
What is a Collection?
A collection is a logical group/label that can contain one or more files (e.g., "photos-2025", "work-docs"). A file may belong to multiple collections, and some files may belong to none.

You are given a stream/list of file records, each described as: [FileName, FileSize, [Collection]]. Collections are optional, meaning a file can have zero or more associated collections. The same file can be part of more than one collection.

Build a system that supports:

Adding or updating file metadata and collection membership.
Computing the total size of all files in the system.
Finding the top collections based on size or file-count.
Methods
addFile
void addFile(String fileName, int fileSize, List[String] collections)
fileName is a Unix-based absolute path including the file name, e.g. /a/b/file1.txt.
fileSize is in KB, always a positive integer in range [1, 100000].
collections may contain collection name or may be empty:
collection names are globally unique and contain lowercase a-z and hyphen '-'
If addFile is called again with the same fileName, update the stored file size and replace the collections list with the new one.
When a file’s size or collections are updated, all aggregate stats (total system size, per-collection sizes/counts) must reflect the change.
Example:
addFile("/a/b/file1.txt", 400, ["collection-1", "travel-collection-2"])
getTotalFileSize
int getTotalFileSize()
Return the total size (in KB) of all files currently in the system.
If a file is updated via addFile, the total size must be incremented/decremented accordingly.
If no files exist, return 0.
getTopCollections
List<String> getTopCollections(int strategy)
strategy is always either 0 or 1.
If strategy = 0: return the top 10 collections sorted by total size of files associated with each collection (descending).
If strategy = 1: return the top 10 collections sorted by total number of files associated with each collection (descending).
Ties are broken by choosing the collection name that is lexicographically smaller.
If fewer than 10 collections exist, return all of them in sorted order.
A file contributes to every collection it belongs to.
Examples
Example 1: Basic adds, total size, top collections

Router fs = new Router();   conceptual initialization

addFile("/a/b/file1.txt", 400, ["collection-1", "travel-collection-2"]);
addFile("/a/b/file2.txt", 100, ["collection-1"]);
addFile("/c/file3.txt",   200, ["travel-collection-2", "work"]);
addFile("/d/e/file4.txt", 300, []);  no collections

getTotalFileSize()  -> 1000
 total = 400 + 100 + 200 + 300

getTopCollections(0) -> ["travel-collection-2", "collection-1", "work"]
 sizes: travel-collection-2=400+200=600,
        collection-1=400+100=500,
        work=200

getTopCollections(1) -> ["collection-1", "travel-collection-2", "work"]
 counts: collection-1=2 files (file1,file2)
         travel-collection-2=2 files (file1,file3)
         work=1 file (file3)
 tie at count=2 broken lexicographically: "collection-1" < "travel-collection-2"
    
Updating a file changes totals and collection stats

addFile("/a/b/file1.txt", 50, ["work"]);   update existing file1

getTotalFileSize() -> 650
 new total = file1(50) + file2(100) + file3(200) + file4(300) = 650

getTopCollections(0) -> ["work", "travel-collection-2", "collection-1"]
 sizes now:
 work = file3(200) + file1(50) = 250
 travel-collection-2 = file3(200)
 collection-1 = file2(100)

getTopCollections(1) -> ["work", "collection-1", "travel-collection-2"]
 counts now:
 work=2 files (file1,file3)
 collection-1=1 file (file2)
 travel-collection-2=1 file (file3)
 tie at count=1 broken lexicographically: "collection-1" < "travel-collection-2"
