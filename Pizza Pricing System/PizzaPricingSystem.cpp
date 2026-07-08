#include <bits/stdc++.h>
using namespace std;


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
    int firstServingCost;
    int costPerServing;
    int count;

public:
    Chesseburst(Pizza* pizza) : PizzaDecorator(pizza),
        firstServingCost(100), costPerServing(70) {}

    void addTopping(int servingsCount) override {
        count = servingsCount;
    }

    int getPrice() const override {
        return decoratedPizza->getPrice() + firstServingCost;
            + costPerServing*(count - 1);
    }
};

class Corn : public PizzaDecorator {
    int costPerServing;
    int count;

public:
    Corn(Pizza* pizza) : PizzaDecorator(pizza), costPerServing(50) {}

    void addTopping(int servingsCount) override {
        count = servingsCount;
    }

    int getPrice() const override {
        return decoratedPizza->getPrice() + costPerServing*count;
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
    unordered_set<string> toppings;

    bool tryApplyTopping(Pizza* &pizza, const string& topping) {
        if(topping == "cheeseburst") {
            if(toppings.find("mushroom") != toppings.end()) {
                return false;
            }
            pizza = new Chesseburst(pizza);
        }
        if(topping == "corn") {
            pizza = new Corn(pizza);
        }
        if(topping == "onion") {
            pizza = new Onion(pizza);
        }
        if(topping == "capsicum") {
            pizza = new Capsicum(pizza);
        }
        if(topping == "pineapple") {
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
        if(tryApplyTopping(pizza, topping) == false) {
            return false;
        }
        toppings.insert(topping);
        pizza->addTopping(servingsCount);
        return true;
    }

    int getFinalPrice() {
        int price = pizza->getPrice();
        double finalTax = baseTax * ((toppings.find("cheeseburst") != toppings.end()) ? 1.3 : 1);
        return price + round(finalTax*price/100);
    }
};


int main() {

    // Create pizza
    PizzaPricing *p = new PizzaPricing(200, 10, "small");

    // Add toppings
    p->addTopping("cheeseburst", 1);   // true, tax 10 -> 13 (one-time uplift)
    p->addTopping("onion", 2);         // true

    // Compute final price
    int price = p->getFinalPrice();    // 407

    // Calculation verification output
    std::cout << "Final Price: " << price << std::endl; 
    // Output: 407

    // Create pizza using a smart pointer (instead of Java's 'new')
    auto q = std::make_unique<PizzaPricing>(350, 8, "medium");

    // Add toppings using the arrow -> operator for pointers
    q->addTopping("mushroom", 2);      // true (2×40=80)
    q->addTopping("cheeseburst", 1);   // false (blocked by mutual exclusion)

    // Compute final price
    int price2 = q->getFinalPrice();    // 464

    // Calculation verification output
    // Calculation: base=350; toppings=80; subtotal=430;
    // tax=8% of 430=34.4; final=464.4 -> round half up => 464
    std::cout << "Final Price: " << price2 << std::endl; 
    // Output: 464

    return 0; // Memory is automatically cleaned up here

    return 0;
}