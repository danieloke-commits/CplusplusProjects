#include "LMS.h"

Book::Book(const std::string &a)
{
    title = a;
    randDist = std::uniform_int_distribution<unsigned>(1111, 9999);
    static std::default_random_engine myRandEngine(time(0));
    ISBN = randDist(myRandEngine);
}

std::string Book::getTitle() const
{
    return title;
}
unsigned Book::getISBN() const
{
    return ISBN;
}

Person::Person(const std::string &nm) : name(nm), outstandingBooks({}) {}

std::string Person::getName() const
{
    return name;
}
void Person::printOutstandBooksList() const
{
    for (const auto & k : outstandingBooks)
    {
        std::cout << k.getTitle() << std::endl;
    }
}

bool Person::operator==(const Person &person) const
{
    return name == person.getName();
}
bool Person::operator<(const Person &person) const
{
    return name < person.getName();
}

void Person::addBorrowedBook(const Book &book)
{
    outstandingBooks.push_back(book);
}
void Person::removeBorrowedBook(const Book &book)
{
  auto it = std::find_if (outstandingBooks.begin(),outstandingBooks.end(), [&](const Book & b){ return b.getTitle() == book.getTitle();});
   if (it != outstandingBooks.end())
   {
    outstandingBooks.erase(it);
   }
}

// add book, set initial copies remaining, borrowerList and borrowAndDueDates to {}
BookTransaction::BookTransaction(const std::string &s) : book(s), copiesRemaining(2), borrowerAndDueDates({}) {}

int BookTransaction::getnumberRemainingCopies() const
{
    return copiesRemaining;
}
std::list<std::string> BookTransaction::getBorrowerNames() const
{
    std::list<std::string> res;

    for (const auto m : borrowerAndDueDates)
    {
        res.push_back(m.first.getName());
    }
    return res;
}

void BookTransaction::removeBorrower(const Person & person)
{


    auto it = std::find_if(borrowerAndDueDates.begin(), borrowerAndDueDates.end(), [&](std::pair<const Person  , std::time_t > & a){ return a.first == person;});
    if (it != borrowerAndDueDates.end())
    {
        borrowerAndDueDates.erase(it);
    }


}
void BookTransaction::increaseCopiesRemaining()
{
    ++copiesRemaining;
}
void BookTransaction::decreaseCopiesRemaining()
{
    --copiesRemaining;
}
// Registered member borrowing a book
Response BookTransaction::BorrowBook(const Person &person, std::time_t dueTime)
{
    if (getnumberRemainingCopies() <= 0)
    {
        return Response::outOfStock;
    }
    decreaseCopiesRemaining();
    borrowerAndDueDates.insert({person, dueTime});
    return Response::success;
}

// Registered member returning a book
Response BookTransaction::ReturnBook(const Person &person)
{
    Response val = Response::success;

    auto it = std::find_if(borrowerAndDueDates.begin(), borrowerAndDueDates.end(), [&](std::pair<const Person, std::time_t> &a)
                           { return a.first == person; });
    if (it == borrowerAndDueDates.end())
    {
        return Response::borrowerNameError;
    }
    else
    {
        borrowerAndDueDates.erase(it);
    }
    increaseCopiesRemaining();
    return val;
}
// check to see if member already borrowed the same book.
bool BookTransaction::duplicateBorrowing(const Person &person)
{
    auto it = std::find_if(borrowerAndDueDates.begin(), borrowerAndDueDates.end(), [&](std::pair<const Person, std::time_t> &a)
                           { return a.first == person; });
    if (it == borrowerAndDueDates.end())
    {
        return false;
    }
    else
    {
        return true;
    }
}

Transactions::Transactions() : store({}), MembersList({}) {}

// adding book to list of available book -usually done by staff member
Response Transactions::AddBook(const Book &book)
{

    store.insert({book.getTitle(), BookTransaction(book.getTitle())});

    return Response::success;
}
// Search from list of books available and provide number of copies available
void Transactions::SearchBook(const std::string &bookname)const
{
    bool found = false;

    std::for_each(store.begin(), store.end(), [&](const std::pair<const std::string &, BookTransaction> &a)

                  { if (a.first == bookname)
        {
            found = true;
            std::cout << "We currently have the book, "<<bookname<<" with "<<a.second.getnumberRemainingCopies() << "copies left\n";
            return;
        } });

    if (!found)
    {
        std::cout << bookname << " was not found \n";
    }
}

// Register member as a new person.
Response Transactions::RegisterMember(const Person &person)
{
    MembersList.push_back(person);
    return Response::success;
}

// Registered member borrowing a book
Response Transactions::BorrowBook(const Person &person, const std::string &bookTitle)
{

    Response val;
    // check to see if member is registered

    auto it = std::find_if(MembersList.begin(), MembersList.end(), [&](const Person &p)
                           { return p == person; });
    if (it == MembersList.end())
        return Response::registrationError;

    // See if the library carries such a book

    auto it2 = std::find_if(store.begin(), store.end(), [&](std::pair<const std::string, BookTransaction> &a)
                            { return a.first == bookTitle; });
    if (it2 == store.end())
    {
        return Response::bookError;
    }
    else if (it2->second.duplicateBorrowing(person))
    {
        return Response::duplicateRequest;
    }

    else
    {
        auto now = std::chrono::system_clock::now();
        auto dueDate = now + std::chrono::hours(24 * 5);
        std::time_t due = std::chrono::system_clock::to_time_t(dueDate);
        val = it2->second.BorrowBook(person, due);

        return val;
    }
}
// Registered member returning a book
Response Transactions::ReturnBook(const Person &person, const std::string &bookTitle)
{
    Response value;

    // check to see if member is registered
    auto it = std::find_if(MembersList.begin(), MembersList.end(), [&](const Person &p)
                           { return p == person; });
    if (it == MembersList.end())
    {
        return Response::registrationError;
    }

    // See if the library holds such a book
    auto it2 = std::find_if(store.begin(), store.end(), [&](std::pair<const std::string, BookTransaction> &a)
                            { return a.first == bookTitle; });
    if (it2 == store.end())
    {
        return Response::notListed;
    }

    value = it2->second.ReturnBook(person);

    return value;
}

// list all the books the library currently has.
void Transactions::ListBooks()const
{

    std::cout <<"\n\nOur current inventory of books as shown below:\n\n";
    std::for_each(store.begin(), store.end(), [](const std::pair<const std::string, BookTransaction> &a)
                  { std::cout << a.first << " with " << a.second.getnumberRemainingCopies() << " cop" << (a.second.getnumberRemainingCopies() > 1 ? "ies" : "y") << " left" << std::endl; });
}
void Transactions::ListMembers()
{
    for (auto k : MembersList)
        std::cout << k.getName() << std::endl;
}

std::string trim(const std::string &mystr)
{
    std::size_t beg = mystr.find_first_not_of(" \t\n\r\f\v");
    if (beg == std::string::npos)
        return "";
    std::size_t end = mystr.find_last_not_of(" \t\n\r\f\v");
    return mystr.substr(beg, end - beg + 1);
}

int main()
{

    char c = 0;
    std::string temp, temp2;
    Response responses;
    Transactions library{};

    try {

    std::ifstream InitialLibaryBooks("LibraryBooks.txt");
    if (InitialLibaryBooks)
    {
        std::string temp3;
        while (std::getline(InitialLibaryBooks, temp3))
        {
            temp3 = trim(temp3);
            Book books(temp3);

            library.AddBook(books);
        }
    }
    }
    catch (std::exception e)
    {
        std::cout<<"I/O exception, unable to laod initial file list of libraries "<<e.what()<<std::endl;
    }


    while (c != 'q')
    {

        std::cout << "----------------------------\n"
                  << "| Enter 1 for member registration\n"
                  << "| Enter 2 to Borrow a book \n"
                  << "| Enter 3 to search for a book\n"
                  << "| Enter 4 to Return a book \n"
                  << "| Enter 5 to list all Books in the library \n"
                  << "| Enter 6 to list all registered members \n"
                  << "| Enter 7 to add a book to the library inventory \n"
                  << "| Enter 'q' to quit \n"
                  << "------------------------------------\n";
        std::cin >> c;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (c)
        {
        case '1':
            std::cout << "Please enter your full name\n";
            std::getline(std::cin, temp);
            temp = trim(temp);
            if (temp.empty())
            {
                responses = Response::invalidInput;
                std::cout<<"Invalid input was entered, please try again \n";
            }

            else
            {
            responses = library.RegisterMember(Person(temp));
            if (responses == Response::success)
            {
              std::cout << "You have been registered." << "Thank you " << temp << std::endl;  
            }
                else
                std::cout << "We are experiencing some technical issues and are not able to assist at the moment. Thank you \n";
              
            }

            break;

        case '2':
            std::cout << "Enter the book you would like to borrow" << std::endl;
            std::getline(std::cin, temp);
            std::cout << "Enter the your name, just to ensure you are a member of the library\n";
            std::getline(std::cin, temp2);
            temp2 = trim(temp2);
            temp = trim(temp);
            responses = library.BorrowBook(Person(temp2), temp);
            if (responses == Response::registrationError)
            {
                std::cout << "Only registered member are allowed to borrow books, please register first \n";
            }
            else if (responses == Response::bookError)
            {
                std::cout << "We do not carry the book " << temp << " in the library. Thank you \n";
            }
            else if (responses == Response::outOfStock)
            {
                std::cout << "All copies of " << temp << " have been borrowed. Please try again later. \n";
            }
            else if (responses == Response::duplicateRequest)
            {
                std::cout << "You currently have a copy of this book checked out. Please try a different book. \n";
            }
            else if (responses == Response::success)
            {
                std::cout << "The book you ordered " << temp << " is on it way to the front desk. Thank you \n";
            }
            else
                std::cout << "We are experiencing some technical issues and are not able to assist at the moment. Thank you \n";
            break;
        case '3':
            std::cout << "Please enter the name of the book you are looking for\n";
            std::getline(std::cin, temp);
            temp = trim(temp);
            library.SearchBook(temp);
            break;
        case '4':
            std::cout << "Please enter the name of the book you are returning\n";
            std::getline(std::cin, temp);
            std::cout << "Please enter your name \n";
            std::getline(std::cin, temp2);
            temp2 = trim(temp2);
            temp = trim(temp);
            responses = library.ReturnBook(Person(temp2), temp);
            if (responses == Response::registrationError)
            {
                std::cout << "Member is not registered in the library. Please enter correct name. \n";
            }
            else if (responses == Response::notListed)
            {
                std::cout << "We do have the book " << temp << " in the library. Please try correct book name. Thank you \n";
            }
            else if (responses == Response::borrowerNameError)
            {
                std::cout << "We do not have " << temp2 << " assigned to this book. Please try again using correct borrower name. Thank you \n";
            }
            else if (responses == Response::success)
            {
                std::cout << "The book " << temp << " was returned. Thank you \n";
            }
            else
                std::cout << "We are experiencing some technical issues and are not able to assist at the moment. Thank you \n";
            break;
        case '5':
            library.ListBooks();
            break;
        case '6':
            library.ListMembers();
            break;
        case '7':
            std::cout << "This is an administrative staff function, please enter password \n";
            std::getline(std::cin, temp2);
            temp2 = trim(temp2);
            responses = (temp2 == "LMS" ? Response::success : Response::registrationError);
            if (responses == Response::registrationError)
            {
                std::cout << "Password incorrect, please try again \n";
            }
            else if (responses == Response::success)
            {
                std::cout << "Please enter the name of the book \n";
                std::getline(std::cin, temp);
                temp = trim(temp);
                responses = library.AddBook(Book(temp));
                try {
                std::ofstream newEntry("LibraryBooks.txt", std::ios::app);
                newEntry <<temp<<"\n";     
                }
                catch (std::exception e)
                {
                    std::cout<<"I/O Error, unable to append changes to main file "<<e.what() <<std::endl;
                }
                std::cout << "The book titled " << temp << " was added \n";
            }
            else
            {
                std::cout << "We are experiencing some technical issues and are not able to assist at the moment. Thank you \n";
            }

            break;
        default:
            break;
        }
    }
}