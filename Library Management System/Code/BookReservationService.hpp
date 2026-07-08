#pragma once

#include <unordered_map>
#include <string>

// Forward declarations are required to break the loop with Member.hpp
class Member;
class BookItem;
class NotificationService;

class BookReservationService {
private:
    std::unordered_map<BookItem*, Member*> bookToMember_;
    std::unordered_map<Member*, BookItem*> memberToBook_;

public:
    Member* fetchReservationDetails(BookItem* bookItem) const;
    bool reserveBook(BookItem* bookItem, Member* member);

    // TODO: Unreserve book by the member or automatically after a certain threshold
};