#include "Catalog.hpp"

std::vector<Book*> Catalog::searchByTitle(std::string title) const {
    if(titleBooks_.find(title) == titleBooks_.end()) {
        return {};
    }
    return titleBooks_.at(title);
}

std::vector<Book*> Catalog::searchByAuthor(std::string author) const {
    if(authorBooks_.find(author) == authorBooks_.end()) {
        return {};
    }
    return authorBooks_.at(author);
}

void Catalog::updateCatalog(Book* book) {
    titleBooks_[book->getTitle()].push_back(book);
    authorBooks_[book->getAuthor()].push_back(book);
}