#pragma once

#include "Book.hpp"
#include <string>
#include <vector>

class Search {
public:
    virtual std::vector<Book*> searchByTitle(std::string title) const = 0;
    virtual std::vector<Book*> searchByAuthor(std::string author) const = 0;
};