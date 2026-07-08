#include "Account.hpp"
#include "AccountStatus.hpp"
#include "Person.hpp"
#include "Catalog.hpp"
#include <iostream>

Account::Account(std::string id, std::string password, Person* person, Catalog* catalog) :
    id_(id), password_(password), status_(AccountStatus::Active), person_(person), catalog_(catalog) {}

bool Account::resetPassword(std::string &newPassword) {
    password_ = newPassword;
    std::cout<<"Password Reset Successful\n";
    return true;
}

std::vector<Book*> Account::searchByTitle(std::string title) {
    return catalog_->searchByTitle(title);
}

std::vector<Book*> Account::searchByAuthor(std::string author) {
    return catalog_->searchByAuthor(author);
}