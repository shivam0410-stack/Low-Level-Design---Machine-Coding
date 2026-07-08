#pragma once

#include <unordered_map>
#include <unordered_set>
#include <string>

// Forward declarations are required to break the loop with Member.hpp
class Member;
class BookItem;
class NotificationService;

class BookLendingService {
private:
    NotificationService* notificationService_;
    std::unordered_map<BookItem*, Member*> bookToMember_;
    std::unordered_map<Member*, std::unordered_set<BookItem*>> memberToBooks_;

public:
    BookLendingService(NotificationService* notificationService, std::unordered_map<BookItem*, Member*> bookToMember, std::unordered_map<Member*, std::unordered_set<BookItem*>> memberToBooks) :
        notificationService_(notificationService), bookToMember_(bookToMember), memberToBooks_(memberToBooks) {}

    Member* fetchLendingDetails(BookItem* bookItem) const;
    bool loanBook(BookItem* bookItem, Member* member);
    bool returnBook(BookItem* bookItem);
};