#pragma once

#include "Account.hpp"

class Library;
class Member;
class Book;

class Librarian : public Account {
private:
    Library* library_;

public:
    Librarian(const Account& account);

    void addMember(Member* member);
    void blockMember(Member* member);
    void addBook(Book* book);
};