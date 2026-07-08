#pragma once

#include "Account.hpp"

class BookItem;
class BookLendingService;
class BookReservationService;


class Member : public Account {
private:
    std::string dateOfMembership_;
    int totalBooksCheckedOut_ = 0;
    BookReservationService* bookReservation_;
    BookLendingService* bookLending_;

public:
    Member(const Account& account, std::string dateOfMembership, BookReservationService* bookReservation, BookLendingService* bookLending) :
        Account(account), dateOfMembership_(dateOfMembership), bookReservation_(bookReservation), bookLending_(bookLending) {}

    int getTotalBooksCheckedOut() const {
        return totalBooksCheckedOut_;
    }

    bool reserveBookItem(BookItem* bookItem);
    bool checkoutBookItem(BookItem* bookItem);
    bool returnBook(BookItem* bookItem);
};


// #include <iostream>
// #include <chrono>

// int main() {
//     using namespace std::chrono;

//     // 1. Get current date from system_clock
//     auto now = system_clock::now();
//     year_month_day today{floor<days>(now)};
//     std::cout << "Today's date: " << today << "\n"; // e.g., 2024-05-20

//     // 2. Create a specific future date using "cute syntax"
//     auto next_birthday = 2024y / December / 25d; //

//     // 3. Date Arithmetic: Adding months/years
//     auto next_year = today + years{1}; //
//     std::cout << "One year from today: " << next_year << "\n";

//     // 4. Calculating difference (requires conversion to sys_days)
//     // sys_days is needed because year_month_day doesn't support direct day subtraction
//     days days_until = sys_days{next_birthday} - sys_days{today}; 
//     std::cout << "Days until Christmas: " << days_until.count() << " days\n"; //

//     return 0;
// }