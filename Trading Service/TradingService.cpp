Update the code to user vector instead of linked lists then evaluate

#include <bits/stdc++.h>
using namespace std;

// Not using enums due to time constraint and input is always given valid.
// enum class OrderStatus {
//     FILLED,
//     OPEN,
//     REJECTED
// };

// enum class OrderKind {
//     MARKET,
//     LIMIT
// };

// enum class Action {
//     BUY,
//     SELL
// };


class Stock {
    String name;
    int price;

public:
    Stock(const string& name, int price) : name(name), price(price) {}

    string getName() { return name; }

    int getPrice() { return price; }

    void updatePrice(int newPrice) { price = newPrice; }
};




class Order {
    string userId, stockName, orderKind, action, status;
    int quantity, limitPrice;
};


class User {
    string id;
    int balance;
    StockList *stockHead, *stockTail;
    unordered_map<string, StockList*> holdings;

public:
    User(const string& id, int balance) : id(id), balance(balance) {}

    int getBalance() { return balance; }

    void updateBalance(int amount) { balance += amount; }

    vector<string> getStockHoldings() {
        vector<string> result;
        StockList* temp = stockList;
        while(temp != null) {
            result.push_back(temp->name + " " + temp->quantity);
            temp = temp->next;
        }
        return result;
    }

    void updateHolding(const string& stockName, int qty, int amount) {
        if(holdings.find(stockName) == holdings.end()) {
            StockList* newStock = new StockList(stockName, qty, (double)amount/qty);
            if(stockTail != nullptr) {
                stockTail->next = newStock;
                newStock->prev = stockTail;
            }
            else {
                stockHead = stockTail = newStock;
            }
            holdings[stockName] = newStock;
            return ;
        }

        StockList* stock = holdings[stockName];
        if(stock->quantity + qty == 0) {
            if(stock->prev) {
                stock->prev->next = stock->next;
            }
            else {
                stockHead = stock->next;

            }
            ifstock->next->prev = stock->prev;
        }
    }
};


// Orchestrator Service but handles order logic
class TradingService {
    unordered_map<string, User*> users;
    vector<Stock*> stockList;
    unordered_map<string, Stock*> stocks;
    unordered_map<string, Order*> openOrders;

    string executeBuyOrder(const string& userId, const string& stockName,
    int qty) {
        int requiredAmount = stocks[stockName]->getPrice()*qty;
        int currentBalance = users[userId]->getBalance();

        if(requiredAmount > currentBalance) {
            return "REJECTED";
        }

        users[userId]->updateBalance(-requiredAmount);
        users[userId]->updateHolding(stockName, qty, requiredAmount);
        return "FILLED";
    }

    string executeSellOrder(const string& userId, const string& stockName,
    int qty) {
        int currentQuantity = users[userId]->getQuantity(stockName);
        if(currentQuantity < qty) {
            return "REJECTED";
        }

        int sellAmount = stocks[stockName]->getPrice()*qty;
        users[userId]->updateBalance(sellAmount);
        users[userId]->updateHolding(stockName, -qty, requiredAmount);
        return "FILLED";
    }

    string submitMarketOrder(const string& userId, const string& stockName,
    const string& action, int qty) {
        if(action == "BUY") {
            return executeBuyOrder(userId, stockName, qty);
        }

        return executeSellOrder(userId, stockName, qty);
    }

    string submitLimitOrder(const string& userId, const string& stockName,
    const string& action, int qty, int limitPrice) {
        if(action == "BUY") {
            if(stocks[stockName]->getPrice() > limitPrice) {
                openOrders
                return "OPEN";
            }
            return executeBuyOrder(userId, stockName, qty);
        }

        if(stocks[stockName]->getPrice() < limitPrice) {
            openOrders
            return "OPEN";
        }
        return executeSellOrder(userId, stockName, qty);
    }

public:
    void addUser(const string& userId, int openingCash) {
        if(users.find(userId) != users.end()) {
            cout<<"User already exists\n";
            return ;
        }

        users[userId] = new User(userId, openingCash);
    }

    int getAccountBalance(const string& userId) {
        if(users.find(userId) == users.end()) {
            cout<<"User does not exist\n";
            return ;
        }

        return users[userId]->getBalance();
    }

    void addStock(const string& stockName, int stockPrice) {
        if(stocks.find(stockName) != stocks.end()) {
            stocks[stockName]->updatePrice(stockPrice);
            executeOrders();
            return ;
        }

        Stock* stock = new Stock(stockName, stockPrice);
        stocks[stockName] = stock;
        stockList.push_back(stock);
    }

    vector<string> showStocks() {
        vector<string> result;
        for(auto& stock : stockList) {
            result.push_back(stock->getName() + " " + to_string(stock->getPrice()));
        }
        return result;
    }

    string submitOrder(const string& userId, const string& stockName,
    const string& orderKind, const string& action, int qty, int limitPrice) {
        if(orderKind == "MARKET") {
            return submitMarketOrder(userId, stockName, action, qty);
        }

        return submitLimitOrder(userId, stockName, action, qty, limitPrice);
    }

    vector<string> viewStockHoldings(const string& userId) {
        return users[userId]->getStockHoldings();
    }

    vector<string> viewOrders(const string& userId) {
        return users[userId]->getOrders();
    }
};


int main() {
    return 0;
}