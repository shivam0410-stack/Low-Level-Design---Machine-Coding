#pragma once

#include <string>
#include <vector>

enum class AccountStatus;
class Person;
class Catalog;
class Book;

class Account {
private:
    std::string id_;
    std::string password_;
    AccountStatus status_;
    Person* person_;
    Catalog* catalog_;

public:
    Account(std::string id, std::string password, Person* person, Catalog* catalog);

    void updateStatus(AccountStatus accountStatus) {
        status_ = accountStatus;
    }

    std::string getId() const {
        return id_;
    }

    AccountStatus getStatus() const {
        return status_;
    }

    bool resetPassword(std::string &newPassword);

    Person* getDetails() const {
        return person_;
    }

    std::vector<Book*> searchByTitle(std::string title);
    std::vector<Book*> searchByAuthor(std::string author);
};