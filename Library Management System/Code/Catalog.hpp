#pragma once

#include "Search.hpp"
#include "Book.hpp"
#include <string>
#include <vector>
#include <unordered_map>

class Catalog : public Search {
private:
    std::unordered_map<std::string, std::vector<Book*>> titleBooks_, authorBooks_;

public:
    std::vector<Book*> searchByTitle(std::string title) const override;
    std::vector<Book*> searchByAuthor(std::string author) const override;
    void updateCatalog(Book* book);
};