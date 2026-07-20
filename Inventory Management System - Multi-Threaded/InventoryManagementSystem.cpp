#include <bits/stdc++.h>
using namespace std;


class Seller {
    string id;
    unordered_set<string> pincodes;
    unordered_set<string> paymentModes;
    unordered_map<int, int> productInventory;

    shared_mutex mtx;

    bool isPincodeServicable(const string& destinationPincode) {
        return pincodes.find(destinationPincode) != pincodes.end();
    }

    bool isPaymentModeSupported(const string& paymentMode) {
        return paymentModes.find(paymentMode) != paymentModes.end();
    }

    bool isProductSufficient(int productId, int productCount) {
        // Product Id is always valid here
        unique_lock<shared_mutex> lock(mtx);
        if(productInventory[productId] < productCount) {
            return false;
        }

        productInventory[productId] -= productCount;
        return true;
    }

public:
    Seller(const string& sellerId, const vector<string>& serviceablePincodes,
    const vector<string>& paymentModesList) : id(sellerId) {
        for(auto& serviceablePincode : serviceablePincodes) {
            pincodes.insert(serviceablePincode);
        }
        for(auto& paymentMode : paymentModesList) {
            paymentModes.insert(paymentMode);
        }
    }

    void addInventory(int productId, int delta) {
        unique_lock<shared_mutex> lock(mtx);
        productInventory[productId] += delta;
    }

    int getInventory(int productId) {
        shared_lock<shared_mutex> lock(mtx);
        auto product = productInventory.find(productId);
        if(product == productInventory.end()) {
            return 0;
        }
        return product->second;
    }

    string createOrder(const string& destinationPincode, const string& paymentMode,
    int productId, int productCount) {
        if(!isPincodeServicable(destinationPincode)) {
            return "pincode unserviceable";
        }
        if(!isPaymentModeSupported(paymentMode)) {
            return "payment mode not supported";
        }
        if(!isProductSufficient(productId, productCount)) {
            return "insufficient product inventory";
        }
        return "order placed";
    }
};


class InventoryManagementSystem {
    int products;
    unordered_map<string, Seller*> sellerMap;

public:
    void init(int productsCount) {
        products = productsCount;
    }

    void createSeller(const string& sellerId, const vector<string>& serviceablePincodes,
    const vector<string>& paymentModes) {
        Seller* seller = new Seller(sellerId, serviceablePincodes, paymentModes);
        sellerMap[sellerId] = seller;
    }

    void addInventory(int productId, const string& sellerId, int delta) {
        Seller* seller = sellerMap[sellerId];
        seller->addInventory(productId, delta);
    }

    int getInventory(int productId, const string& sellerId) {
        auto sellerIterator = sellerMap.find(sellerId);
        if(sellerIterator == sellerMap.end()) {
            return 0;
        }

        Seller* seller = sellerIterator->second;
        return seller->getInventory(productId);
    }

    string createOrder(const string& orderId, const string& destinationPincode,
    const string& sellerId, int productId, int productCount, const string& paymentMode) {
        Seller* seller = sellerMap[sellerId];

        // Not storing orders intentionally
        return seller->createOrder(destinationPincode, paymentMode, productId, productCount);
    }
};


int main() {
    // 1. Initialize the system pointer object with a capacity of 10
    InventoryManagementSystem* inventorymanagementsystem = new InventoryManagementSystem();
    inventorymanagementsystem->init(10);

    // 2. Register sellers with their serviceable pincodes and accepted payment modes
    inventorymanagementsystem->createSeller("seller-0", {"110001", "560092", "452001", "700001"}, {"netbanking", "cash", "upi"});
    inventorymanagementsystem->createSeller("seller-1", {"400050", "110001", "600032", "560092"}, {"netbanking", "cash", "upi"});

    // 3. Populate product inventories
    inventorymanagementsystem->addInventory(0, "seller-1", 52); // Prints: inventory added
    inventorymanagementsystem->addInventory(0, "seller-0", 32); // Prints: inventory added

    // 4. Execute and verify order tracking for seller-1
    std::cout << inventorymanagementsystem->createOrder("order-1", "400050", "seller-1", 0, 5, "upi") << std::endl; // Prints: order placed
    std::cout << inventorymanagementsystem->getInventory(0, "seller-1") << std::endl; // Prints: 47

    // 5. Execute and verify order tracking for seller-0
    std::cout << inventorymanagementsystem->createOrder("order-2", "560092", "seller-0", 0, 1, "upi") << std::endl; // Prints: order placed
    std::cout << inventorymanagementsystem->getInventory(0, "seller-0") << std::endl; // Prints: 31

    // 6. Resource cleanup
    delete inventorymanagementsystem;
    return 0;
}
