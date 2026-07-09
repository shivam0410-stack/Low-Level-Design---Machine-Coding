16. Design Locker Management System for Warehouse Packages
Asked in
Amazon LLD
Build a simple Parcel Locker Service that manages lockers of fixed sizes: S, M, L, XL, XXL. Each package must go into a locker of the same size, and one locker can hold at most one package at any time. The system must support adding lockers on the fly, finding a free locker of a requested size, marking a locker as occupied, and freeing it later.

Provide the following operations:

addSlot(size: String): void Add a new empty locker of the given size, if size is invalid then do nothing.
- generate locker id by appending size with next integer id for that size. e.g. M1, M2 .... Mn , S1, S2, S3 ..... and so on .
findSlot(size: String): Locker | "" Return an available locker of that size, or empty string "" if none exist.
- If multiple lockers are available then return lexicographically smallest locker.
occupySlot(lockerId: String): boolean Mark the locker as in use; returns true if successful.
freeSlot(lockerId: String): boolean Mark the locker as empty again; returns true if successful.
Expected behavior:

Each locker has a unique ID and a fixed size.
Once occupied, a locker cannot be reassigned until it is freed.
Examples
// Example 1
LockerManager sys = new LockerManager();
sys.addSlot("M"); // creates "M1"
sys.addSlot("L");  // creates "L1"

Locker m1 = sys.findSlot("M");     // returns a free medium locker ( id "M1")
sys.occupySlot(m1.getId());        // true
sys.freeSlot(m1.getId());          // true
  
// Example 2
LockerManager sys = new LockerManager();
sys.addSlot("S"); // S1
sys.addSlot("S"); // S2

Locker sA = sys.findSlot("S");     // get one small locker S1
sys.occupySlot(sA.getId());        // true
sys.occupySlot(sA.getId());        // false (already taken)

Locker sB = sys.findSlot("S");     // returns a different free small locker, S2
  
Constraints
Size must be one of: "S", "M", "L", "XL", "XXL" (uppercase).
Locker IDs are unique, non-empty strings (up to 32 characters).
Only one package per locker at any time.
Total lockers managed by the system will not exceed 200,000.
