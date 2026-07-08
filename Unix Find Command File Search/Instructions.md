14. Design Unix "find" command for file search
Asked in
Amazon LLD
Build an in-memory file lookup tool similar to a Unix "find" command. The system should support two starter Search Criterias and be easy to extend with new Search Criterias later.

Initial Search Criterias

Search Criteria 1: Return all files strictly larger than a given size (in MB) under a specified directory (recursive).
Search Criteria 2: Return all files with a given file extension (for example, ".xml") under a specified directory (recursive).
Use Strategy design pattern to implement Search Criterias.
API shape (use Java-style naming):

void putFile(String path, int sizeMb)
- adds or replaces a file entry at the given absolute path.
- "path" will always be a valid file path.
- sizeMb will always be >=0
List<String> search(int searchCriteriaId, String dirPath, String args) - Finds files by Search Criteria. Results must be sorted in ascending lexicographical order.
- The args string contains the parameter(s) needed by the Search Criteria.
- arguments will never be invalid or empty. e.g. for search criteria 2, "extension" will never be empty.
Search Criteria 1: args = "minSizeMb" (e.g., "5")
Search Criteria 2: args = ".ext" (e.g., ".xml")
The design should allow adding new Search Criterias later (for example, a name-substring match).

Examples
FileSearch s = new FileSearch();

s.putFile("/data/pics/photoA.jpg", 4);
s.putFile("/data/pics/movie.mp4", 12);
s.putFile("/work/docs/readme.md", 1);
s.putFile("/work/docs/report.xml", 7);

  Update (overwrite) size
s.putFile("/data/pics/photoA.jpg", 9);

  Search Criteria 1: files > 8 MB inside /data
s.search(1, "/data", "8")
Returns: ["/data/pics/photoA.jpg", "/data/pics/movie.mp4"]

  Search Criteria 2: files with ".xml" inside /work
s.search(2, "/work", ".xml")
Returns: ["/work/docs/report.xml"]
FileSearch s = new FileSearch();

s.putFile("/media/images/aa.jpg", 6);
s.putFile("/media/images/ab.jpg", 7);
s.putFile("/media/images/ac.xml", 2);
s.putFile("/office/reports/r1.xml", 9);
s.putFile("/office/reports/r2.xml", 4);
s.putFile("/office/notes.txt", 3);

  Search Criteria 1 under a subdirectory
s.search(1, "/media/images", "5")
Returns: ["/media/images/aa.jpg", "/media/images/ab.jpg"]

  Search Criteria 2 under a parent directory (recursive)
s.search(2, "/office", ".xml")
Returns: ["/office/reports/r1.xml", "/office/reports/r2.xml"]
Constraints
At most 1000 total calls to putFile and search combined.
Up to 2000 files may be stored at once.
All paths are absolute (start with "/") and use "/" as the separator.
sizeMb is an integer in the range [0, 10000].
search must traverse all nested subdirectories under dirPath.
Return file paths in strict ascending lexicographical order with no duplicates.