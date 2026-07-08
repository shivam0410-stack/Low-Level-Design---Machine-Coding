20. Design Stock Trading Platform Like Zerodha, Groww
Asked in
Amazon LLD
Design a lightweight stock trading service that lets users register, browse stocks with mock prices, place buy/sell orders, and track their holdings and order history. Focus on core flows and clean, extensible code. The system is in-memory and single-process.
User and Account
Create a user with an initial cash balance.
Fetch user's current account balance.
Stocks and Prices
Keep a list of tradable stocks with a name and current price.
System should also support update of stock prices.
Orders
Two sides: BUY or SELL.
Two kinds: MARKET or LIMIT.
Validations:
BUY: user must have enough cash (including total cost at current stock price).
SELL: user must hold enough quantity.
Execution Rules
MARKET orders fill immediately at the stock's current price.
LIMIT orders fill immediately if the price is favorable:
BUY LIMIT fills when stock's current price ≤ limitPrice.
SELL LIMIT fills when stock's current price ≥ limitPrice.
No partial fills: an order is either FILLED or remains OPEN (or is REJECTED on validation failure).
Update cash and holdings after a fill; track average price per symbol.
Portfolios and History
Show current stock holdings with quantity and average buy price.
List all orders with status: FILLED, OPEN, or REJECTED.
Methods
void addUser(String userId, int openingCash)
- userId will always be a non blank string and openingCash will always be a positive integer.

int getAccountBalance(String userId)
- userId will always be a non blank string.

void addStock(String stockName, int stockPrice)
- adding an stock either creates a new stock with stockPrice or updates older price if stock already exists.
- stockName will always be a non blank string and stockPrice will always be a positive integer.

List[String] showStocks()
- show all stocks that have been added in system till now.
- return empty list if no stocks have been added.
- rows appear in the sequence stocks were first added to the system. e.g. if MSFT was added before IBM for the first time then it would come before IBM.
- each row is space separated "name price" values.
- e.g. ["AMZN 678", "ZETA 42"]

submitOrder(userId, stockName, orderKind, action, qty, limitPrice)
- orderKind will always be either MARKET or LIMIT
- action will always be either BUY or SELL
- this method will should return either of FILLED, OPEN, or REJECTED
- limitPrice will be used when orderKind == LIMIT

List[String] viewStockHoldings(userId)
- each row will be a space separated string "stockName count" , where count is the number of stocks user has.
Count must be a positive integer.
- e.g. ["AMZN 24", "ZETA 10"]
- all rows are in the sequence the user first acquired the stock successfully (FILLED) for the first time. e.g. if user bought any number of shares of MSFT before GOOG for the first time. Then MSFT will come before GOOG.
- Explanation: Stock whose order went from OPEN to FILLED will come AFTER any stocks that were ordered after the order of stock was made. e.g. if first time order for stock A was made but status was open, then first time order for stock B was made and was FILLED. Later, order for A also got filled. Here B will come before A in the list returned because it was filled before A.
- empty list is returned if user has no stocks.

List[String] viewOrders(userId)
- returns list of past orders or an empty list if no orders are available.
- each row is a space separated string "stockName action quantity orderKind status"
- list orders in the same sequence as they were placed.
- e.g. ["AMZN BUY 12 MARKET FILLED", "ACME BUY 3 LIMIT OPEN"]

Note: - Methods will never be called with null values.

Examples
Example 1 - Add stocks, show list, market & favorable limit buys, portfolios, history, and balances
TradingService trade = new TradingService();

add two stocks ACME@100 and ZETA@50
trade.addStock("ACME", 100);
trade.addStock("ZETA", 50);

show all stocks, return ["ACME 100","ZETA 50"]
trade.showStocks();

create user U1 with 10,000 cash
trade.addUser("U1", 10000);

check initial balance for U1, return 10000
trade.getAccountBalance("U1");

place MARKET BUY 20 ACME @ current 100, return FILLED
trade.submitOrder("U1","ACME","MARKET","BUY",20,0);

balance after fill (spent 2000), return 8000
trade.getAccountBalance("U1");

place LIMIT BUY 10 ZETA @ 60, since current price for ZETA is 50 (≤ 60), return FILLED
trade.submitOrder("U1","ZETA","LIMIT","BUY",10,60);

balance after second fill (spent 500), return 7500
trade.getAccountBalance("U1");

view holdings for U1, return ["ACME 20","ZETA 10"]
trade.viewStockHoldings("U1");

view orders for U1, return 
["ACME BUY 20 MARKET FILLED",
"ZETA BUY 10 LIMIT FILLED"]
trade.viewOrders("U1");
    
Example 2 - Validations and OPEN: insufficient cash, sell without holdings, unfavorable buy limit
TradingService trade = new TradingService();

add AMZN@678
trade.addStock("AMZN", 678);

create user U2 with 1,500 cash
trade.addUser("U2", 1500);

check initial balance for U2, return 1500
trade.getAccountBalance("U2");

place MARKET BUY 3 AMZN needs 3×678=2034 (>1500), return REJECTED
trade.submitOrder("U2","AMZN","MARKET","BUY",3,0);

balance unchanged after REJECTED, return 1500
trade.getAccountBalance("U2");

place LIMIT BUY 2 AMZN @ 600; current price 678 ＞ 600 so not favorable, cash sufficient, return OPEN
trade.submitOrder("U2","AMZN","LIMIT","BUY",2,600);

balance unchanged for OPEN (no fill), return 1500
trade.getAccountBalance("U2");

place MARKET SELL 1 AMZN with zero holdings, return REJECTED
trade.submitOrder("U2","AMZN","MARKET","SELL",1,0);

balance unchanged after REJECTED, return 1500
trade.getAccountBalance("U2");

view holdings for U2, return []
trade.viewStockHoldings("U2");

view orders for U2, return 
["AMZN BUY 3 MARKET REJECTED",
"AMZN BUY 2 LIMIT OPEN",
"AMZN SELL 1 MARKET REJECTED"]
trade.viewOrders("U2");
    
Example 3 - Price updates, selling flows (FILLED/OPEN/REJECTED), final portfolio & history (with balances) - with auto-fill after price rises to 160
TradingService trade = new TradingService();

add ACME@90
trade.addStock("ACME", 90);

create user U3 with 10,000 cash
trade.addUser("U3", 10000);

check initial balance for U3, return 10000
trade.getAccountBalance("U3");

place MARKET BUY 50 ACME @ 90, cost 4500, return FILLED
trade.submitOrder("U3","ACME","MARKET","BUY",50,0);

balance after buy, return 5500
trade.getAccountBalance("U3");

update ACME price to 120
trade.addStock("ACME", 120);

show all stocks, return ["ACME 120"]
trade.showStocks();

place LIMIT SELL 20 ACME @ 110 when current is 120 (≥ 110), return FILLED at 120
trade.submitOrder("U3","ACME","LIMIT","SELL",20,110);

balance after selling 20 (added 2400), return 7900
trade.getAccountBalance("U3");

attempt LIMIT SELL 40 ACME @ 150 with only 30 remaining, return REJECTED
trade.submitOrder("U3","ACME","LIMIT","SELL",40,150);

balance unchanged after REJECTED, return 7900
trade.getAccountBalance("U3");

place LIMIT SELL 10 ACME @ 150; since current price is 120 ＜ 150 and holdings sufficient, return OPEN
trade.submitOrder("U3","ACME","LIMIT","SELL",10,150);

balance unchanged for OPEN, return 7900
trade.getAccountBalance("U3");

place MARKET SELL 5 ACME @ current 120, return FILLED
trade.submitOrder("U3","ACME","MARKET","SELL",5,0);

balance after market sell (added 600), return 8500
trade.getAccountBalance("U3");

raise ACME price to 160; previously OPEN LIMIT SELL 10 @ 150 becomes FILLED at 160
trade.addStock("ACME", 160);

balance after auto-fill of open order (added 1600), return 10100
trade.getAccountBalance("U3");

show all stocks, return ["ACME 160"]
trade.showStocks();

view holdings for U3, return ["ACME 15"]
trade.viewStockHoldings("U3");

view orders for U3, return 
["ACME BUY 50 MARKET FILLED",
"ACME SELL 20 LIMIT FILLED",
"ACME SELL 40 LIMIT REJECTED",
"ACME SELL 10 LIMIT FILLED",
"ACME SELL 5 MARKET FILLED"]
trade.viewOrders("U3");
  
Constraints
Single currency; all amounts are non-negative, prices > 0, quantities are positive integers.
All data is in-memory; persistence is not required.
Assume a single-process execution model; concurrency control is not required.