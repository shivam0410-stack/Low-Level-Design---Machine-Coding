15. Design Unix "find" command - with boolean predicates
Asked in
Amazon LLD
Build an in-memory file lookup tool similar to the Unix find command.
Start with two Search Criteria (rules) and make it easy to add more later.

Also allow composing results from multiple criteria using Boolean Search Predicates (AND, OR, AND NOT).

Example: find all files inside /office/reports that are > 2 MB and have extension .xml.

Initial Search Criteria (rules)
Criteria 1 - Min Size (strict): Return all files strictly larger than a given size (in MB) under a specified directory (recursive).
Criteria 2 - Extension match: Return all files with a given file extension (e.g., .xml) under a specified directory (recursive).
The design must allow adding new criteria later (e.g., name-substring match).

Boolean Search Predicates (ops)
AND: Items present in both lists (intersection).
OR: Items present in either list (union).
AND NOT: Items present in the left list but not in the right list (difference).
Evaluation Order
Expression: list1 AND list2 OR list3 AND NOT list4

Step 1: list1 AND list2 → list11
Step 2: list11 OR list3 → list12
Step 3: list12 AND NOT list4 → final
Use Strategy design pattern to implement Search Criterias.
Use Specification design pattern to implement Boolean Search Predicates.
Methods
Class: FileFinder
void addFile(String path, int sizeMb)
List runQuery(List rules, List ops)
Rules are strings in the form: "ruleId,dirPath,arg1,arg2,...". For these initial rules:

ruleId 1: "1,dirPath,minSizeMb" (sizeGreaterThan)
ruleId 2: "2,dirPath,.ext" (hasExtension)
ops.size() == rules.size() - 1
Results must be unique and sorted ascending.
Examples
FileFinder f = new FileFinder();

f.addFile("/app/logs/a.log", 6);
f.addFile("/app/logs/b.log", 4);
f.addFile("/app/logs/c.log", 9);
f.addFile("/app/logs/archive/old.log.bak", 1);
f.addFile("/app/logs/archive/rare.bak", 9);
f.addFile("/app/logs/tmp/t1.log", 50);

String r1 = "1,/app/logs,5";     // > 5 MB under /app/logs
String r2 = "2,/app/logs,.log";  // .log under /app/logs
String r3 = "2,/app/logs,.bak";  // .bak under /app/logs
String r4 = "1,/app/logs/tmp,0"; // anything > 0 MB under /app/logs/tmp

runQuery([r1, r2, r3, r4], ["AND", "OR", "AND NOT"]);
Equivalent to: ((r1 AND r2) OR r3) AND NOT r4
-> ["/app/logs/a.log", "/app/logs/archive/old.log.bak", "/app/logs/archive/rare.bak", "/app/logs/c.log"]
// (jpg big AND NOT /tmp) OR (png outside /trash)
FileFinder f = new FileFinder();

f.addFile("/store/p/a.jpg", 9);
f.addFile("/store/q/b.jpg", 2);
f.addFile("/store/tmp/c.jpg", 12);
f.addFile("/repo/img/d.png", 1);
f.addFile("/repo/img/e.png", 6);
f.addFile("/repo/trash/f.png", 100);

String r1 = "2,/store,.jpg";       // .jpg in /store
String r2 = "1,/store,5";          // > 5 MB in /store
String r3 = "1,/store/tmp,0";      // anything in /store/tmp
String r4 = "2,/repo,.png";        // .png in /repo
String r5 = "1,/repo/trash,0";     // anything in /repo/trash

runQuery([r1, r2, r3, r4, r5], ["AND", "AND NOT", "OR", "AND NOT"]);
Equivalent to: (((r1 AND r2) AND NOT r3) OR r4) AND NOT r5
-> ["/repo/img/d.png", "/repo/img/e.png", "/store/p/a.jpg"]
// Narrowing with multiple AND NOT (exclude drafts and cache)
FileFinder f = new FileFinder();

f.addFile("/docs/reports/a.xml", 4);
f.addFile("/docs/reports/b.xml", 2);
f.addFile("/docs/drafts/c.xml", 10);
f.addFile("/docs/cache/d.xml", 9);
f.addFile("/docs/notes/readme.txt", 100);

String r1 = "2,/docs,.xml";      // .xml anywhere under /docs
String r2 = "1,/docs,3";         // > 3 MB under /docs
String r3 = "1,/docs/drafts,0";  // anything in /docs/drafts
String r4 = "1,/docs/cache,0";   // anything in /docs/cache

runQuery([r1, r2, r3, r4], ["AND", "AND NOT", "AND NOT"]);
Equivalent to: ((r1 AND r2) AND NOT r3) AND NOT r4
-> ["/docs/reports/a.xml"]
// OR with de-dup across overlapping sets
FileFinder f = new FileFinder();

f.addFile("/backup/x.csv", 7);
f.addFile("/backup/monthly/x.csv", 7);
f.addFile("/data/x.csv", 7);

String r1 = "2,/backup,.csv";          // .csv under /backup
String r2 = "1,/backup/monthly,5";     // > 5 MB under /backup/monthly
String r3 = "2,/data,.csv";            // .csv under /data

runQuery([r1, r2, r3], ["OR", "OR"]);
Equivalent to: (r1 OR r2) OR r3
-> ["/backup/monthly/x.csv", "/backup/x.csv", "/data/x.csv"]
Constraints
At most 1200 total calls (addFile + runQuery).
At most 2500 files stored at any time.
Absolute paths only; search under a dir is recursive.
sizeMb is in [0, 20000].
ops entries are one of: AND, OR, AND NOT.
Results must be unique and sorted ascending.
Arguments are valid (e.g., non-empty extension).