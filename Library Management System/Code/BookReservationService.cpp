#include "BookReservationService.hpp"
#include "BookItem.hpp"
#include "Member.hpp"
#include "NotificationService.hpp"
#include <iostream>

Member* BookReservationService::fetchReservationDetails(BookItem* bookItem) const {
    auto it = bookToMember_.find(bookItem);
    if(it == bookToMember_.end())
    {
        return nullptr;
    }
    return it->second;
}

bool BookReservationService::reserveBook(BookItem* bookItem, Member* member) {
    BookStatus status = bookItem->getStatus();
    if(status == BookStatus::Loaned)
    {
        std::cout<<"Book is already loaned\n";
        return false;
    }
    else if(status == BookStatus::Reserved && bookItem->getMemberId() != member->getId())
    {
        std::cout<<"Book is already reserved\n";
        return false;
    }

    // TODO: Set the borrowed and due date for the book item
    bookItem->setStatus(BookStatus::Reserved);
    bookItem->setMemberId(member->getId());

    // TODO: If member has already 5 books then don't lend the book
    memberToBook_[member] = bookItem;
    bookToMember_[bookItem] = member;

    return true;
}