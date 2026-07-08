#include <bits/stdc++.h>
using namespace std;

struct Expense {
    int id;
    vector<string> members;
    vector<int> paid;
};


class SplitBook {
    unordered_map<string, string> users;
    unordered_map<int, Expense> expenses;
    unordered_map<string, double> balances;

    bool isUserExists(const string& userId) {
        return users.find(userId) != users.end();
    }

    bool isExpenseExists(int expenseId) {
        return expenses.find(expenseId) != expenses.end();
    }


public:
    void registerUser(const string& userId, const string& displayName) {
        if(isUserExists(userId)) {
            cout<<"User already exists\n";
            return ;
        }

        users[userId] = displayName;
    }

    void recordExpense(int expenseId, const vector<string>& members, const vector<int>& paid) {
        if(isExpenseExists(expenseId)) {
            cout<<"Expense already exists\n";
            return ;
        }

        int totalMembers = members.size();
        int totalExpense = 0;
        for(int i=0;i<totalMembers;i++) {
            if(!isUserExists(members[i])) {
                cout<<"User does not exist\n";
                return ;
            }
            totalExpense += paid[i];
        }

        double expensePerMember = (double)totalExpense/totalMembers;
        for(int i=0;i<totalMembers;i++) {
            balances[members[i]] += paid[i] - expensePerMember;
        }

        expenses[expenseId] = {expenseId, members, paid};
    }

    vector<string> listBalances() {
        // Should have created a separate function for simplifying debts
        // Then return use results here for printing
        priority_queue<pair<double, string>> maxQueue;
        priority_queue<pair<double, string>, vector<pair<double, string>>, greater<pair<double, string>>> minQueue;
        for(auto& [userId, balance] : balances) {
            if(balance > 0) {
                maxQueue.push({balance, userId});
            }
            else if(balance < 0) {
                minQueue.push({balance, userId});
            }
        }

        vector<string> result;
        while(!maxQueue.empty() && !minQueue.empty()) {
            pair<double, string> debtor = minQueue.top();
            pair<double, string> creditor = maxQueue.top();
            minQueue.pop();
            maxQueue.pop();

            string s = debtor.second + " owes " + creditor.second + ": ";
            if(creditor.first > -debtor.first) {
                s += format("{:.2f}", -debtor.first);
                creditor.first += debtor.first;
                maxQueue.push(creditor);
            }
            else if(creditor.first < -debtor.first) {
                s += format("{:.2f}", creditor.first);
                debtor.first += creditor.first;
                minQueue.push(debtor);
            }
            else {
                s += format("{:.2f}", creditor.first);
            }
            result.push_back(s);
        }
        sort(result.begin(), result.end());
        return result;
    }
};


int main() {
    SplitBook* sb = new SplitBook();

    sb->registerUser("A", "Ann");
    sb->registerUser("B", "Ben");
    sb->registerUser("C", "Cam");

    // Expense #1: total = 600, each owes 200
    sb->recordExpense(10, {"A", "B", "C"}, {0, 600, 0});

    // Expense #2: total = 100, each owes 50
    sb->recordExpense(11, {"A", "C"}, {100, 0});

    // Balances after netting
    vector<string> ans = sb->listBalances();
    for (auto& res : ans) {
        cout << res << '\n';
    }

    delete sb;
    return 0;
}