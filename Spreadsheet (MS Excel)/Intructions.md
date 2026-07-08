25. Design spreadsheet like Microsoft Excel
Asked in
Microsoft
Build a simple spreadsheet data model with rows and columns. Each cell can hold text plus style (font, size, bold, italic). Support inserting rows/columns anywhere and getting/setting cell contents deterministically in a single-threaded environment.
Initialization
At the start, the sheet has 5 rows and 5 columns.
Both rows and columns are 0-based.
All cells are initially empty (no text, no style).
Methods
1) void addRow(int index)
Insert a new empty row at index.
Existing rows at index, index+1, … shift down by 1.
Validity: 0 ≤ index ≤ currentRows.
If  currentRows = 5, addRow(5) appends at the end.
addRow(0) makes the former row 0 become row 1, etc.
2) void addColumn(int index)
Insert a new empty column at index.
Existing columns at index, index+1, … shift right by 1.
Validity: 0 ≤ index ≤ currentCols.
If currentCols = 5, addColumn(5) appends at the end.
addColumn(2) makes former col 2 become col 3, etc.
3) void addEntry(int row, int column, String text, String fontName, int fontSize, boolean isBold, boolean isItalic)
Create or replace the entry at (row, column) with the given text and style.
Validity: 0 ≤ row < currentRows and 0 ≤ column < currentCols.
Input constraints:
text contains no hyphen -.
fontName will only contain lowercase characters from 'a' to 'z'
8 ≤ fontSize ≤ 72.
4) String getEntry(int row, int column)
Return the serialized content+style for the cell at (row, column).
Format: text-fontName-fontSize[-b][-i]
Include -b only if isBold = true.
Include -i only if isItalic = true.
Examples:
"some text example-tahoma-24-b"
"another text example-algerian-14-i"
"what are you waiting for-calibri-28-b-i"
"another day another dawn-pixar-21"
Empty cell: return "".
Validity: 0 ≤ row < currentRows, 0 ≤ column < currentCols.
Examples
Assume initial state is a 5×5 empty sheet (rows = 5, cols = 5).

Insert a row at the end and add an entry
addRow(5) → rows become 6 (new row index 5, all empty).
addEntry(5, 0, "hello", "tahoma", 24, true, false)
getEntry(5, 0) → "hello-tahoma-24-b"
Insert a column in the middle and verify shifting
addColumn(2) → cols become 6 (new empty column at index 2).
addEntry(0, 1, "x", "calibri", 10, false, true)
getEntry(0, 1) → "x-calibri-10-i"
After addColumn(1), the prior (0,1) shifts to (0,2):
getEntry(0, 2) → "x-calibri-10-i"
getEntry(0, 1) → ""
Replace existing entry
addEntry(5, 0, "greetings", "tahoma", 24, false, false)
getEntry(5, 0) → "greetings-tahoma-24"
Bold/Italic flags
addEntry(1, 3, "note", "algerian", 14, true, true)
getEntry(1, 3) → "note-algerian-14-b-i"
Empty cell check
getEntry(2, 2) → ""
