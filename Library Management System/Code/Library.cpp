#include "Library.hpp"

// ADD THIS LINE: This defines the static member
Library* Library::library_ = nullptr; 

Library* Library::getInstance() {
    if (library_ == nullptr) {
        // Fix: Compiler environment issue
        // lock_guard<mutex> lock(mtx);
        if (library_ == nullptr) {
            library_ = new Library();
        }
    }
    return library_;
}

void Library::saveMember(Member* member) {
    membersList_.push_back(member);
}

void Library::saveBook(Book* book) {
    booksList_.push_back(book);
}