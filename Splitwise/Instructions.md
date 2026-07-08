12. Design expense sharing app like Splitwise
Asked in
Salesforce
Uber
Quince
Flipkart
Licious
Tekion
Nykaa
Razorpay
Swiggy
FlexPort
Groww
Meesho
Goldman Sachs
Amazon LLD
+10 more
Design a lightweight expense-sharing system that tracks how much each person owes or is owed after group expenses are split evenly among participants. The system maintains net balances and exposes operations to add users, record expenses, and list simplified debtor-to-creditor balances.

Methods
void registerUser(String userId, String displayName)
Add a user. If userId already exists, ignore the call.
void recordExpense(int expenseId, List<String> members, List<Integer> paid)
Record an expense where each members[i] paid paid[i] (integer, in whole currency units).
The expense total is split equally among all members.
Balances are updated by comparing what each member paid vs. their fair share.
If expenseId has already been used, ignore the call.
All listed userIds must be registered; otherwise, ignore the expense.
Use two-decimal precision for all balance effects (round half up).
List<String> listBalances()
Return all non-zero net obligations after full internal netting.
Each entry must be formatted exactly as: "<debtor-id> owes <creditor-id>: <amount>"
Amounts must show exactly two decimals.
Sort primarily by debtor id, then by creditor id (lexicographic order).
Constraints
1 ≤ number of users ≤ 50,000
1 ≤ members.size() ≤ 100
members.size() == paid.size()
0 ≤ paid[i] ≤ 1,000,000,000 and at least one paid[i] > 0 per expense
All referenced userIds in an expense are already registered
expenseId values will always be unique
All outputs in listBalances() must use exactly two decimals and be sorted as specified
Examples
 Example 1
 Class name and methods follow Java naming style.
SplitBook sb = new SplitBook();
sb.registerUser("A", "Ann");
sb.registerUser("B", "Ben");

 One expense: total = 1000; each owes 500
sb.recordExpense(1, List.of("A", "B"), List.of(1000, 0));

 Balances:
sb.listBalances(); 
 ["B owes A: 500.00"]
 Example 2
SplitBook sb = new SplitBook();
sb.registerUser("A", "Ann");
sb.registerUser("B", "Ben");
sb.registerUser("C", "Cam");

 Expense #1: total=600, each owes 200
sb.recordExpense(10, List.of("A", "B", "C"), List.of(0, 600, 0));

 Expense #2: total=100, each owes 50
sb.recordExpense(11, List.of("A", "C"), List.of(100, 0));

 Balances after netting:
sb.listBalances();
 ["A owes B: 150.00", "C owes B: 250.00"]
 Example 3
SplitBook sb = new SplitBook();
sb.registerUser("A", "Ann");
sb.registerUser("B", "Ben");
sb.registerUser("C", "Cam");
sb.registerUser("D", "Dia");

 #1: [A,B], total=300, each owes 150
sb.recordExpense(21, List.of("A", "B"), List.of(0, 300));

 #2: [B,C,D], total=90, each owes 30
sb.recordExpense(22, List.of("B", "C", "D"), List.of(90, 0, 0));

 #3: [A,C], total=100, each owes 50 (both paid equally)
sb.recordExpense(23, List.of("A", "C"), List.of(50, 50));

 Balances:
sb.listBalances();
["A owes B: 150.00", "C owes B: 30.00", "D owes B: 30.00"