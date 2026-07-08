#include "Member.hpp"
#include "BookItem.hpp"
#include "BookLendingService.hpp"
#include "BookReservationService.hpp"

bool Member::reserveBookItem(BookItem* bookItem) {
    return bookReservation_->reserveBook(bookItem, this);
}

bool Member::checkoutBookItem(BookItem* bookItem) {
    bool isLoaned = bookLending_->loanBook(bookItem, this);
    if(isLoaned) {
        totalBooksCheckedOut_++;
    }
    return isLoaned;
}

bool Member::returnBook(BookItem* bookItem) {
    totalBooksCheckedOut_--;
    return bookLending_->returnBook(bookItem);
}