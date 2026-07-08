#pragma once

#include "Book.hpp"
#include "BookStatus.hpp"

class BookItem : Book {
private:
    BookStatus status_;
    std::string memberId_;
    std::string borrowedDate_;
    std::string dueDate_;
    int price_;

public:
    BookItem(BookStatus status, int price, std::string ISBN, std::string title, std::string author) :
        Book(ISBN, title, author), status_(status), price_(price) {}

    BookStatus getStatus() const {
        return status_;
    }

    std::string getMemberId() const {
        return memberId_;
    }

    void setStatus(BookStatus status) {
        status_ = status;
    }

    void setMemberId(std::string memberId) {
        memberId_ = memberId;
    }

    void setLoanDetails(std::string BorrowedDate, std::string dueDate);
};