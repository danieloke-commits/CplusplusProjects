#ifndef LMS_H
#define LMS_H

#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <map>
#include <list>
#include <chrono>
#include <random>
#include <queue>
#include <algorithm>
#include <exception>
#include <fstream>

enum class Response
{
    success, registrationError, borrowerNameError, bookError, outOfStock, duplicateRequest,notListed,invalidInput
};
class Book
{
public:
    Book(const std::string &);
    std::string getTitle() const;
    unsigned getISBN() const;

private:
    unsigned ISBN;
    std::string title;
    std::uniform_int_distribution<unsigned> randDist;
};

class Person
{
public:
    Person(const std::string &); // used for registration of new members
    std::string getName() const;
    void printOutstandBooksList() const;
    void addBorrowedBook(const Book &);
    void removeBorrowedBook(const Book &);
    bool operator==(const Person &) const;
    bool operator<(const Person &) const;

private:
    std::list<Book> outstandingBooks;
    std::string name;
};

class Transactions;

class BookTransaction
{

public:
    BookTransaction(const std::string &); // add book, set initial copies remaining to 5, borrowerList and borrowAndDueDates to {}
    int getnumberRemainingCopies() const;
    std::list<std::string> getBorrowerNames() const;
    void removeBorrower(const Person &);
    Response BorrowBook(const Person &, std::time_t); // Registered member borrowing a book
    bool duplicateBorrowing(const Person &);
    Response ReturnBook(const Person &); // Registered member returning a book
    void decreaseCopiesRemaining();
    void increaseCopiesRemaining();

private:
    Book book;
    int copiesRemaining;
    std::map<Person, std::time_t> borrowerAndDueDates; // books are due to be returned in 5 days
};

class Transactions
{
public:
    Transactions();
    Response AddBook(const Book &);                           // adding book to list of available book -usually done by staff member
    void SearchBook(const std::string &) const;                // Search from list of books available and provide number of copies available
    Response RegisterMember(const Person &);                 // Register member as a new person.
    Response BorrowBook(const Person &, const std::string &); // Registered member borrowing a book
    Response ReturnBook(const Person &, const std::string &); // Registered member returning a book
    void ListBooks() const;                                    // list all the books the library currently has.
    void ListMembers();                                  // List all registered members

private:
    // Data structure below: string book name, book transaction info
    std::map<std::string, BookTransaction> store;
    std::list<Person> MembersList;
};

#endif