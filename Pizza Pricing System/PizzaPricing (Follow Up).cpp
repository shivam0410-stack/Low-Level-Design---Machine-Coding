#include <bits/stdc++.h>
using namespace std;

// try Apply Topping has a lot of logic
// Redesign, every topping should manage its own rules


class Pizza {
    int basePrice;

public:
    Pizza() {}

    Pizza(int basePrice) : basePrice(basePrice) {}

    virtual void addTopping(int servingsCount) {};

    virtual int getPrice() const {
        return basePrice;
    }
};


class PizzaDecorator : public Pizza {
protected:
    Pizza* decoratedPizza;

public:
    PizzaDecorator(Pizza *pizza) : decoratedPizza(pizza) {}
};

class Chesseburst : public PizzaDecorator {
    int costPerServing;
    int count;

public:
    Chesseburst(Pizza* pizza) : PizzaDecorator(pizza), costPerServing(100) {}

    void addTopping(int servingsCount) override {
        count = servingsCount;
    }

    int getPrice() const override {
        return decoratedPizza->getPrice() + costPerServing*count;
    }
};

class Corn : public PizzaDecorator {
    int firstServingCost;
    int costPerServing;
    int count;

public:
    Corn(Pizza* pizza, int costPerServing = 50, int firstServingCost = 50)
        : PizzaDecorator(pizza), costPerServing(costPerServing),
            firstServingCost(firstServingCost)  {}

    void addTopping(int servingsCount) override {
        count = servingsCount;
    }

    int getPrice() const override {
        return decoratedPizza->getPrice() + firstServingCost
            + costPerServing*(count - 1);
    }
};

class Onion : public PizzaDecorator {
    int costPerServing;
    int count;

public:
    Onion(Pizza* pizza) : PizzaDecorator(pizza), costPerServing(30) {}

    void addTopping(int servingsCount) override {
        count = servingsCount;
    }

    int getPrice() const override {
        return decoratedPizza->getPrice() + costPerServing*count;
    }
};

class Capsicum : public PizzaDecorator {
    int costPerServing;
    int count;

public:
    Capsicum(Pizza* pizza) : PizzaDecorator(pizza), costPerServing(50) {}

    void addTopping(int servingsCount) override {
        count = servingsCount;
    }

    int getPrice() const override {
        return decoratedPizza->getPrice() + costPerServing*count;
    }
};

class Pineapple : public PizzaDecorator {
    int costPerServing;
    int count;

public:
    Pineapple(Pizza* pizza) : PizzaDecorator(pizza), costPerServing(60) {}

    void addTopping(int servingsCount) override {
        count = servingsCount;
    }

    int getPrice() const override {
        return decoratedPizza->getPrice() + costPerServing*count;
    }
};

class Mushroom : public PizzaDecorator {
    int costPerServing;
    int count;

public:
    Mushroom(Pizza* pizza) : PizzaDecorator(pizza), costPerServing(40) {}

    void addTopping(int servingsCount) override {
        count = servingsCount;
    }

    int getPrice() const override {
        return decoratedPizza->getPrice() + costPerServing*count;
    }
};


class PizzaPricing {
    Pizza* pizza;
    int baseTax;
    string size;
    unordered_map<string, int> toppings;

    bool tryApplyTopping(Pizza* &pizza, const string& topping, int servingsCount) {
        if(topping == "cheeseburst") {
            if(toppings.find("mushroom") != toppings.end()) {
                return false;
            }
            if(size == "small" && toppings["cheeseburst"] + servingsCount > 1) {
                return false;
            }
            if(size == "medium" && toppings["cheeseburst"] + servingsCount > 2) {
                return false; 
            }
            pizza = new Chesseburst(pizza);
        }
        if(topping == "corn") {
            if(size == "large") {
                pizza = new Corn(pizza, 20, 20);
            }
            else if(size == "medium") {
                pizza = new Corn(pizza, 40);
            }
            else {
                pizza = new Corn(pizza);
            }
        }
        if(topping == "onion") {
            pizza = new Onion(pizza);
        }
        if(topping == "capsicum") {
            pizza = new Capsicum(pizza);
        }
        if(topping == "pineapple") {
            if(size != "medium" && size != "large") {
                return false;
            }
            pizza = new Pineapple(pizza);
        }
        if(topping == "mushroom") {
            if(toppings.find("cheeseburst") != toppings.end()) {
                return false;
            }
            pizza = new Mushroom(pizza);
        }
        return true;
    }

public:
    PizzaPricing(int basePrice, int taxPercentage, const string& size)
        : baseTax(taxPercentage), size(size)
    {
        pizza = new Pizza(basePrice);
    }

    bool addTopping(const string& topping, int servingsCount) {
        if(tryApplyTopping(pizza, topping, servingsCount) == false) {
            return false;
        }
        toppings[topping] += servingsCount;
        pizza->addTopping(servingsCount);
        return true;
    }

    int getFinalPrice() {
        int price = pizza->getPrice();
        double finalTax = baseTax * ((toppings.find("cheeseburst") != toppings.end()) ? 1.3 : 1);
        finalTax *= (toppings.find("mushroom") != toppings.end()) ? 0.9 : 1;
        return price + round(finalTax*price/100);
    }
};


int main() {
    // Create pizza using pointers
    PizzaPricing* p = new PizzaPricing(200, 10, "small");

    // Add toppings using pointer arrow operator
    std::cout << std::boolalpha;
    std::cout << "Add cheeseburst 1: " << p->addTopping("cheeseburst", 1) << std::endl;   // true
    std::cout << "Add cheeseburst 1: " << p->addTopping("cheeseburst", 1) << std::endl;   // false
    std::cout << "Add pineapple 1:   " << p->addTopping("pineapple", 1) << std::endl;     // false
    std::cout << "Add onion 2:       " << p->addTopping("onion", 2) << std::endl;         // true

    // Compute final price
    int price = p->getFinalPrice();
    std::cout << "Final Price:       " << price << std::endl;                             // 407
    cout<<'\n';

    // Free allocated memory
    delete p;
    p = nullptr;



    // Create pizza using pointers
    PizzaPricing* q = new PizzaPricing(350, 8, "medium");

    // Add toppings using pointer arrow operator
    std::cout << std::boolalpha;
    std::cout << "Add mushroom 1:    " << q->addTopping("mushroom", 1) << std::endl;    // true
    std::cout << "Add corn 3:        " << q->addTopping("corn", 3) << std::endl;        // true
    std::cout << "Add cheeseburst 1: " << q->addTopping("cheeseburst", 1) << std::endl; // false

    // Compute final price
    int price2 = q->getFinalPrice();
    std::cout << "Final Price:       " << price2 << std::endl;                              // 557
    cout<<'\n';

    // Free allocated memory
    delete q;
    q = nullptr;



    // Create pizza using pointers
    PizzaPricing* r = new PizzaPricing(900, 15, "large");

    // Add toppings using pointer arrow operator
    std::cout << std::boolalpha;
    std::cout << "Add pineapple 2: " << r->addTopping("pineapple", 2) << std::endl; // true
    std::cout << "Add corn 4:      " << r->addTopping("corn", 4) << std::endl;      // true
    std::cout << "Add onion 1:     " << r->addTopping("onion", 1) << std::endl;     // true

    // Compute final price
    int price3 = r->getFinalPrice();
    std::cout << "Final Price:     " << price3 << std::endl;                         // 1300
    cout<<'\n';

    // Free allocated memory
    delete r;
    r = nullptr;

    return 0;
}