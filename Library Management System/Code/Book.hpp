#pragma once

#include <string>

class Book {
private:
    std::string ISBN_;
    std::string title_;
    std::string author_;

public:
    Book(std::string ISBN, std::string title, std::string author) :
        ISBN_(ISBN), title_(title), author_(author) {}

    std::string getAuthor() const {
        return author_;
    }

    std::string getTitle() const {
        return title_;
    }
};