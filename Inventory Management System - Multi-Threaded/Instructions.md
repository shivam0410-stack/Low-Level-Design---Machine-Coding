4. Design an order and inventory management system - Multi-Threaded
Asked in
WayFair
Intuit
Tiktok
Licious
Apna
Amazon LLD
D.E Shaw
Meesho
Flipkart
See less
Write code for low level design of orders and inventory management system of a simple e-commerce platform.
You will need to have the capability of handling sellers, products and orders.

Inventory is number of items of a particular product in a seller's warehouse.

The way it works is, products numbered from 0 till productsCount-1 are sold on the website.
Sellers are also added along with the area pincodes that they are able to deliver goods in as well as the payment types which they support.
After that sellers add items they wish to sell.

Multiple sellers can sell the same item e.g the product-1 : bluetooth speaker boat stone 650 can be sold by multiple sellers throughout the country.

Multiple sellers can deliver goods to the same pincode as well.

For simplicity lets assume price of a specific product is same whether it is sold by seller-1 or seller-2 or any seller.

For Java solution, your code will be tested in a Multi-Threaded environment, so use thread safe data structures and handle synchronization properly.

For Python solution, your code will be tested in a Single-Threaded environment.

Your solution should implement below methods :

Method : void init(Helper04 helper, int productsCount)
- use helper for printing logs else logs will not be visible.
- Use this method for initialize your global variables and all.
- productsCount is total number of products being sold on website.


Method : void createSeller(String sellerId, List[String] serviceablePincodes, List[String] paymentModes)
- Creates a new seller. Each seller sells many products and multiple sellers can sell the product with same productId
- sellerId will always be a non null, non blank unique string
- serviceablePincodes is list of pincodes where seller can deliver products
- paymentModes will be always one of "cash", "upi", "netbanking", "debit card" and "credit card"

Method : void addInventory(int productId, String sellerId, int delta)
- seller adds multiple items of a product for selling. e.g 50 grey pure cotton shirts.
- delta: number of items seller is adding e.g. 50 . It will always be a positive integer.
- productId and sellerId will always be valid.

Method : int getInventory(int productId, String sellerId)
- returns the number of items in warehouse for a product sold by a given seller,
- if the product or seller doesn't exist then returns 0

Method : createOrder(String orderId, String destinationPincode, String sellerId, int productId, int productCount, String paymentMode)
- creates order with orderId and reduces product inventory from seller by productCount
- buyer will choose both product and seller who will deliver the product and create an order.
- For simplicity lets assume that at this time only one product (1 or more counts) is purchased in a single order.
- orderId , sellerId and productId will always be valid.
- productCount is number of items customer is ordering, it will always be a positive integer
- paymentMode will always be one of "cash", "upi", "netbanking", "debit card" and "credit card"
- returns (in that order) : "order placed" or "pincode unserviceable" or "payment mode not supported" or "insufficient product inventory"

Example : Read the below method calls to get a better understanding of how this works.

init(helper, 10) : helper is initialized.
createSeller(seller-0, [110001, 560092, 452001, 700001], [netbanking, cash, upi])
createSeller(seller-1, [400050, 110001, 600032, 560092], [netbanking, cash, upi])
addInventory(0, seller-1, 52) returned "inventory added"
addInventory(0, seller-0, 32) returned "inventory added"
createOrder(order-1, 400050, seller-1, 0, 5, upi) returned "order placed"
getInventory(0, seller-1) returned 47
createOrder(order-2, 560092, seller-0, 0, 1, upi) returned "order placed"
getInventory(0, seller-0) returned 31