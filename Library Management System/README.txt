# 📚 Library Management System (LMS)

A simple C++ console-based **Library Management System** that allows members to register, borrow, return, and manage books.  
This project demonstrates **object-oriented design**, **file handling**, and **STL container usage** in C++.

---

## ✨ Features

- **Register Members**  
  Add new library members by name.

- **Add Books**  
  Library staff can add new books. Each book is automatically assigned an ISBN and starts with 5 copies by default.

- **Borrow Books**  
  Registered members can borrow books. Borrowed books have a **5-day due date**.

- **Return Books**  
  Members can return borrowed books, updating both their personal record and the library's inventory.

- **Search Books**  
  Look up a book by title and see how many copies are available.

- **List All Books**  
  Display all books currently in the library with available copies.

- **List All Members**  
  Show all registered members of the library.

---

## 🛠️ Technologies Used

- **C++17**  
- Standard Template Library (STL)  
  - `std::map`, `std::list`, `std::vector`  
  - `std::chrono` for due dates  
  - `std::random` for ISBN generation  
- File handling (`<fstream>`) for persistence

# 📂 Project Structure

.
├── LMS.h # Header file with class declarations
├── LMS.cpp # Implementation file (business logic)
├── main.cpp # Entry point for running the program
└── README.md # Project documentation


Example Usage
-------------------------------
1. Register Member
2. Add Book
3. Borrow Book
4. Return Book
5. Search Book
6. List Books
7. List Members
q. Quit
-------------------------------

Enter your choice: 1
Enter member name: Alice
Member registered successfully!



Compile info

$g++ -std=c++17 LMS.cpp -g -o LMS
$./LMS

Additional information
The administrator password to add a book to library is 'LMS'
Ensure file LibraryBooks.txt is located in the same folder as the cpp file
Maximum copy for each book is preset to 2.




