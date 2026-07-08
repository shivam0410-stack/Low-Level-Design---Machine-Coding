#include "Librarian.hpp"
#include "Library.hpp"
#include "Member.hpp"
#include "AccountStatus.hpp"
#include <iostream>

Librarian::Librarian(const Account& account) :
    Account(account), library_(Library::getInstance()) {}

void Librarian::addMember(Member* member) {
    library_->saveMember(member);
    std::cout<<"Member saved\n";
}

void Librarian::blockMember(Member* member) {
    member->updateStatus(AccountStatus::Blocked);
    std::cout<<"Member blocked\n";
}

void Librarian::addBook(Book* book) {
    library_->saveBook(book);
    std::cout<<"Book saved\n";
}