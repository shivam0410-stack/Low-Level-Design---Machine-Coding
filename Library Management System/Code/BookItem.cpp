#include "BookItem.hpp"

void BookItem::setLoanDetails(std::string borrowedDate, std::string dueDate) {
    borrowedDate_ = borrowedDate;
    dueDate_ = dueDate;
}
