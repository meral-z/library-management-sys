#include <iostream>
#include <iomanip>
#include <ctime>
#include "admin.h"

//#include "books.h" // class declaration + import book function + print imported books
//#include "users.h" // include hash + password validation + users class declaration

using namespace std;

string currentDate()
{
    // Get the current time as a time_t object
    time_t currentTime = std::time(nullptr);

    // Convert time_t to local time as a tm structure
    tm* localTime = std::localtime(&currentTime);

    // Extract the date, month, and year
    int year = 1900 + localTime->tm_year; // tm_year is years since 2000
    int month = 1 + localTime->tm_mon;   // tm_mon is months since January (0-11)
    int day = localTime->tm_mday;        // Day of the month

    // Output the current date
    string current = to_string(day) + "." + to_string(month) + "." + to_string(year - 2000);
    cout << "Current Date: " << current << endl;
    return current;
}


void adminDash(Book books[], int book_count, User users[], int user_count, string today) ////need to work on dunction
{
    int input = 0;
    cout << "--------------Welcome Admin---------------" << endl;
    while(input!=4)
    {
        cout << "1. Issue book to user\n2. View All issued Books\n3. View overdue Books\n4. Exit\n";
        cin >> input;
        switch (input)
        {
        case 1:
            issueBook("issuedbk.txt", books, book_count, users, user_count);
            break;
        case 2:
            cout << setfill('_') << setw(22) << "" << endl;
            cout << "ISSUED BOOKS:" << endl;
            printAllIssued(books,book_count);
            cout << setfill('_') << setw(22) << "" << endl;
            break;
        case 3:
            cout << setfill('_') << setw(22) << "" << endl;
            cout << "OVERDUE BOOKS:" << endl;
            printAlloverdue(books,book_count,today);
            cout << setfill('_') << setw(22) << "" << endl;
            break;
        default:
            break;
        }
    }

    cout << "EXITING ADMIN DASHBOARD" << endl << endl;
}


int main()
{
    string today = currentDate();
    string const ADMIN = "admin";
    string admin_pass = "admin123";

    //A library by defination contains atleast 1000 books
    Book books[1000]; 
    User users[200];

    int input;
    string const bookfile = "books.txt";
    string const userfile = "users.txt";
    string const issuefile = "issuedbk.txt";

    //use function import books to import from file
    int book_count = importBooks(bookfile, books);
    int user_count = importUsers(userfile, users); 
    int issued_books = importIssuedBooks(issuefile, books,users);
    int userindex = -1;

    string bookname;
    int find = -1;

    while(input != 100)
    {
        cout<< "1. Show all Books available in Library"<< endl;
        cout<< "2. Login as User" << endl;
        cout<< "3. Login as Admin" << endl;
        cout<< "4. Sign Up" << endl;
        cout<< "5. Search book in library" << endl;
        cout<< "100. Exit library"<< endl;
        cin >> input;

        switch (input)
        {
        case (1):
            printBooks(book_count, books);
            break;
        case (2):
            userindex = login(users,user_count);

            if (userindex!=-1)
            {
                User user = users[userindex];
                cout << setfill('_') << setw(22) << "" << endl;
                cout <<"You have currently been issued the following Books:\n";
                printBooksIssued(user.getIssued(), books,user);
                cout << setfill('_') << setw(22) << "" << endl;

                cout << endl;

                printBooksDue(user.getIssued(),books,user,today);
                cout << setfill('_') << setw(22) << "" << endl;
                
            }
            break;
        case(3):
            if(loginAsAdmin(ADMIN,admin_pass) == true)
                adminDash(books, book_count, users, user_count, today);
            break;
        case (4):
            //password validation unaiza
            signUp(users,user_count,userfile);
            break;
        case 5:
            searchLib(books, book_count);
            break;
        case (100):
            cout << "EXITING PROGRAM\n";
            break;
        default:
            cout << "Invalid Input\n";
            break;
        }
    }
    return 0;
}

