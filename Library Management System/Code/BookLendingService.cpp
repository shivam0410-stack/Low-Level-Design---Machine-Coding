#include "BookLendingService.hpp"
#include "BookItem.hpp"
#include "Member.hpp"
#include "NotificationService.hpp"
#include <iostream>

Member* BookLendingService::fetchLendingDetails(BookItem* bookItem) const {
    auto it = bookToMember_.find(bookItem);
    if(it == bookToMember_.end())
    {
        return nullptr;
    }
    return it->second;
}

bool BookLendingService::loanBook(BookItem* bookItem, Member* member) {
    BookStatus status = bookItem->getStatus();
    if(status == BookStatus::Loaned)
    {
        std::cout<<"Book is already loaned\n";
        return false;
    }

    std::string memberId = member->getId();
    if(status == BookStatus::Reserved && bookItem->getMemberId() != memberId)
    {
        std::cout<<"Book is already reserved\n";
        return false;
    }

    // TODO: Set the borrowed and due date for the book item
    bookItem->setStatus(BookStatus::Loaned);
    bookItem->setMemberId(memberId);

    // TODO: If member has already 5 books then don't lend the book
    memberToBooks_[member].insert(bookItem);
    bookToMember_[bookItem] = member;

    std::string content = "Congratulations " + memberId + " on getting the book.";
    notificationService_->sendNotification(memberId, content);
    return true;
}

bool BookLendingService::returnBook(BookItem* bookItem) {
    Member* member = bookToMember_[bookItem];
    std::string memberId = member->getId();
    bookToMember_.erase(bookItem);

    memberToBooks_[member].erase(bookItem);

    // TODO: Reset borrowed and due date for the book item
    bookItem->setStatus(BookStatus::Available);
    bookItem->setMemberId("N/A");
    
    std::string content = "Thanks " + memberId + " for returning the book.";
    notificationService_->sendNotification(memberId, content);

    return true;
}