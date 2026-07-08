#pragma once

#include <vector>
// #include <mutex>

class Member;
class Book;

class Library {
private:
    std::vector<Member*> membersList_;
    std::vector<Book*> booksList_;
    static Library* library_;

    // Fix: Compiler environment issue
    // static std::mutex mtx_;

    // Private Constructor
    Library() {};

public:
    Library(const Library&) = delete;
    Library& operator=(const Library&) = delete;

    // Static method to get the Singleton instance
    static Library* getInstance();

    void saveMember(Member* member);
    void saveBook(Book* book);
};