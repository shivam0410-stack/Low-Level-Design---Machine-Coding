#pragma once

#include <string>

class Person {
private:
    std::string name_;
    std::string address_;

public:
    Person(std::string name, std::string address) {
        name_ = name;
        address_ = address;
    }

    std::string getName() const {
        return name_;
    }
};