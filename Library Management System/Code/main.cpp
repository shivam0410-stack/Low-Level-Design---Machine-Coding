/**
 * Library Management System - Data Flow Demo
 *
 * This main.cpp demonstrates how data flows through the system:
 * 1. Setup: Catalog, Library (singleton), services, librarian, members
 * 2. Librarian adds books and members to Library
 * 3. Member searches catalog -> reserves book -> checks out -> returns
 * 4. Lending/Reservation services and notifications drive the flow
 */

#include "Library.hpp"
#include "Librarian.hpp"
#include "Member.hpp"
#include "Account.hpp"
#include "AccountStatus.hpp"
#include "Person.hpp"
#include "Catalog.hpp"
#include "Book.hpp"
#include "BookItem.hpp"
#include "BookStatus.hpp"
#include "BookLendingService.hpp"
#include "BookReservationService.hpp"
#include "NotificationService.hpp"
#include "EmailStrategy.hpp"

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

static void printSeparator(const std::string& title) {
    std::cout << "\n========== " << title << " ==========\n";
}

int main() {
    // -------------------------------------------------------------------------
    // PHASE 1: Setup core infrastructure (Catalog, Library singleton)
    // -------------------------------------------------------------------------
    printSeparator("PHASE 1: Setup infrastructure");

    Catalog catalog;
    Library* library = Library::getInstance();

    // Create book metadata and register in catalog (search index)
    Book* book1 = new Book("ISBN-001", "Clean Code", "Robert Martin");
    Book* book2 = new Book("ISBN-002", "Design Patterns", "Gang of Four");
    catalog.updateCatalog(book1);
    catalog.updateCatalog(book2);
    std::cout << "Catalog updated with books for search.\n";

    // -------------------------------------------------------------------------
    // PHASE 2: Create notification and lending/reservation services
    // Data flow: NotificationStrategy <- NotificationService <- BookLendingService
    // -------------------------------------------------------------------------
    printSeparator("PHASE 2: Create services");

    EmailStrategy emailStrategy;
    NotificationService notificationService(&emailStrategy);

    std::unordered_map<BookItem*, Member*> lendingMap;
    std::unordered_map<Member*, std::unordered_set<BookItem*>> memberToBooksMap;
    BookLendingService lendingService(&notificationService, lendingMap, memberToBooksMap);
    BookReservationService reservationService;
    std::cout << "BookLendingService and BookReservationService ready.\n";

    // -------------------------------------------------------------------------
    // PHASE 3: Create librarian and members (accounts + persons)
    // Data flow: Person -> Account -> Librarian / Member
    // -------------------------------------------------------------------------
    printSeparator("PHASE 3: Create librarian and members");

    Person librarianPerson("Admin User", "Library HQ");
    Account librarianAccount("lib-1", "admin123", &librarianPerson, &catalog);
    Librarian librarian(librarianAccount);

    Person personAlice("Alice", "123 Main St");
    Person personBob("Bob", "456 Oak Ave");
    Account accountAlice("mem-alice", "pass1", &personAlice, &catalog);
    Account accountBob("mem-bob", "pass2", &personBob, &catalog);

    Member alice(accountAlice, "2024-01-15", &reservationService, &lendingService);
    Member bob(accountBob, "2024-02-01", &reservationService, &lendingService);
    std::cout << "Members Alice and Bob created.\n";

    // Librarian adds members and books to Library (data flows into singleton)
    librarian.addMember(&alice);
    librarian.addMember(&bob);
    librarian.addBook(book1);
    librarian.addBook(book2);

    // Physical copies (BookItems) for lending; status starts Available
    BookItem* item1 = new BookItem(BookStatus::Available, 29, "ISBN-001", "Clean Code", "Robert Martin");
    BookItem* item2 = new BookItem(BookStatus::Available, 35, "ISBN-002", "Design Patterns", "Gang of Four");

    // -------------------------------------------------------------------------
    // PHASE 4: Member searches catalog (data flow: Member -> Account -> Catalog)
    // -------------------------------------------------------------------------
    printSeparator("PHASE 4: Search catalog");

    std::vector<Book*> resultsByTitle = alice.searchByTitle("Clean Code");
    std::cout << "Alice searched by title 'Clean Code': found " << resultsByTitle.size() << " book(s).\n";
    for(auto book : resultsByTitle)
    {
        std::cout<<book->getTitle()<<'\n';
    }

    std::vector<Book*> resultsByAuthor = bob.searchByAuthor("Gang of Four");
    std::cout << "Bob searched by author 'Gang of Four': found " << resultsByAuthor.size() << " book(s).\n";
    for(auto book : resultsByAuthor)
    {
        std::cout<<book->getTitle()<<'\n';
    }
    // -------------------------------------------------------------------------
    // PHASE 5: Reserve -> Checkout -> Return (data flow through services)
    // -------------------------------------------------------------------------
    printSeparator("PHASE 5: Reserve book");

    bool reserved = alice.reserveBookItem(item1);
    std::cout << "Alice reserve BookItem (Clean Code): " << (reserved ? "success" : "failed") << "\n";

    printSeparator("PHASE 6: Checkout book");

    bool checkedOut = alice.checkoutBookItem(item1);
    std::cout << "Alice checkout BookItem (Clean Code): " << (checkedOut ? "success" : "failed") << "\n";
    std::cout << "Alice total books checked out: " << alice.getTotalBooksCheckedOut() << "\n";

    // Fetch lending details (data flow: BookLendingService map lookup)
    Member* lender = lendingService.fetchLendingDetails(item1);
    if (lender) {
        std::cout << "BookItem is currently lent to member ID: " << lender->getId() << "\n";
    }

    printSeparator("PHASE 7: Return book");

    bool returned = alice.returnBook(item1);
    std::cout << "Alice return BookItem: " << (returned ? "success" : "failed") << "\n";
    std::cout << "Alice total books checked out: " << alice.getTotalBooksCheckedOut() << "\n";

    // Bob checks out the other book (no reservation)
    bob.checkoutBookItem(item2);
    std::cout << "Bob checked out Design Patterns.\n";

    // -------------------------------------------------------------------------
    // PHASE 8: Optional - block member (Librarian -> Account status)
    // -------------------------------------------------------------------------
    printSeparator("PHASE 8: Librarian blocks member");

    librarian.blockMember(&bob);
    std::cout << "Bob's account status: " << (bob.getStatus() == AccountStatus::Blocked ? "Blocked" : "Other") << "\n";

    printSeparator("Data flow demo complete");

    // Cleanup (in a real app you'd use smart pointers or a clear ownership model)
    delete item1;
    delete item2;
    delete book1;
    delete book2;

    return 0;
}
