26. Design Dictionary App to store words and their meanings
Asked in
Microsoft
Google
Build an in-memory dictionary that stores words and their meanings.

It lets users fetch meanings, supports prefix search. It also checks word existence with a “.” wildcard pattern.

See methods and examples for clarity.
Methods
1) void storeWord(String word, String meaning)
Insert or update the mapping (word → meaning).
Both word and meaning will be lowercase and non blank
'meaning' will overwrite any older 'meaning' if one already exists for 'word'.
word contains only lowercase letters [a-z], space ' ', hyphen '-'
word will never contain the dot (.) character.
2) String getMeaning(String word)
Return the meaning for word if it exists, otherwise return the empty string "".
Constraints: word is lowercase, non-blank
3) List[String] searchWords(String prefix, int n)
Return up to n words that start with prefix, sorted lexicographically (ascending).
If fewer than n matches exist, return all matches.
Constraints: prefix is non-blank, lowercase, 1 ≤ n .
4) boolean exists(String word)
Return true if at least one stored word matches the pattern in word, else false.
The pattern may include one or more dots ., where each . matches exactly one letter (a-z, space, hyphen) in 'word'.
Non-dot characters must match exactly, pattern length must equal candidate word length.
Stored words never contain ., only the query pattern may include dots.
Examples
Assume the dictionary starts empty.

Store, Overwrite, and Fetch
storeWord("apple", "a fruit")
getMeaning("apple") → "a fruit"
storeWord("apple", "sweet fruit") (overwrites)
getMeaning("apple") → "sweet fruit"
getMeaning("apples") → "" (no such word)
Prefix Search with Sorting and Limit
Insert:
storeWord("app", "short for application")
storeWord("apple", "sweet fruit")
storeWord("apply", "make a formal request")
storeWord("apt", "suitable")
storeWord("banana", "yellow fruit")
searchWords("ap", 3) → ["app", "apple", "apply"]
searchWords("app", 10) → ["app", "apple", "apply"]
searchWords("b", 2) → ["banana"]
Existence with . Wildcards
Suppose stored words: {"cat","cap","caps","map","man","many"}
exists("cat") → true
exists("c.t") → true (e.g., matches "cat")
exists("ca.") → true (matches "cat","cap")
exists("..p") → true (matches "cap","map")
exists("c..s") → true (matches "caps")
exists("....") → true (matches "caps")
exists("c..") → true (matches "cat","cap")
exists("c.") → false (no two-letter words starting with c)
exists("...y") → true (matches "many")

