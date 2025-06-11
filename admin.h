#include <iostream>
#include "books.h"
#include "users.h"
using namespace std;


void printBooksIssued(int issued, Book books[], User user)
{
    for(int x= 0; x < issued;x++)
    {
        int index = user.getBookIndex(x);
        cout << books[index].getBookName() << endl;
    }
}

void printAllIssued(Book books[], int book_count)
{
    for(int x= 0; x < book_count;x++)
    {
        if(books[x].isIssued())
            cout << books[x].getBookName() << endl;
    }
}


void parseDate(string date, int &day, int &month, int &year)
{
    int firstDot = date.find('.');
    int secondDot = date.find('.', firstDot + 1);

    // Extract day, month, and year using substr
    day = stoi(date.substr(0, firstDot));
    month = stoi(date.substr(firstDot + 1, secondDot - firstDot - 1));
    year = stoi(date.substr(secondDot + 1));
}

void printAlloverdue(Book books[], int book_count,string today)
{
    int tday,tmonth,tyear; // for today
    int dday,dmonth,dyear; //for due

    parseDate(today,tday,tmonth,tyear);
    for(int x= 0; x < book_count;x++)
    {
        parseDate(books[x].getDue(),dday,dmonth,dyear);
        if((dyear < tyear) || (dyear == tyear && dmonth < tmonth) || (dyear == tyear && dmonth == tmonth && dday < tday))
        {
            cout << books[x].getBookName() << endl;
        }
    }
}

void printBooksDue(int issued, Book books[], User user, string today)
{
    cout << "Books Due recently:" << endl;
    int tday,tmonth,tyear; // for today
    int dday,dmonth,dyear; //for due

    parseDate(today,tday,tmonth,tyear);
    for(int x= 0; x < issued;x++)
    {
        int index = user.getBookIndex(x);
        parseDate(books[index].getDue(),dday,dmonth,dyear);
        if((dyear < tyear) || (dyear == tyear && dmonth < tmonth) || (dyear == tyear && dmonth == tmonth && dday < tday))
        {
            cout << "Due Date has passed for: ";
            cout << books[index].getBookName() << endl;
        }
        else if(((dday <= (tday+3)) && (dmonth == tmonth)&&(dyear ==tyear)))
        {
            if (dday == tday)
                cout << books[index].getBookName() << "IS DUE TO BE RETURNED TODAY!";
            else
                cout << "REMINDER " << books[index].getBookName() << " IS DUE TO BE RETURNED IN " << dday - tday << " DAY(S)!\n";
        }
    }
}

int importIssuedBooks(string filename, Book books[], User users[])  //return values of books stored and outputs
{
    string line ;
    fstream issuefile;
    int i = 0;

    issuefile.open(filename,ios::in);
    if(issuefile.is_open()){
        while(getline(issuefile,line)){    
            int separator = line.find(','); // using csv file so separator is ','
            
            string iban = line.substr(0,separator);

            int prevsep = separator;
            separator = line.find(',', prevsep +1 ); //checking for next , updating the index we start looking from

            string username = line.substr(prevsep + 1, separator -(prevsep + 1));

            prevsep = separator;
            separator = line.find(',' , prevsep + 1);

            string issueddate = line.substr(prevsep + 1, separator - (prevsep + 1));;

            string duedate = line.substr(separator + 1) ;

            int j = 0;
            while(books[j].getIban()!=iban && books[j].getIban() != "")
            {
                j++;
            }
            books[j].issue(username, issueddate, duedate);
            int x = 0;
            while(users[x].getUsername()!=username && users[x].getUsername() != "")
            {
                x++;
            }
            users[x].issue(j);
            
            i++; //importing linearly in array
        }
        //cout << "Import successfull: "<< i << " Books imported!"<<endl;
        issuefile.close();
        return i;
    }
    else
        cout << "Error: Could not import books.\n";
    return 0;
}

/*void changePass()
{
    string old, newpass, confirm;
    cout << "Enter current password: ";
    cin >> old;
    if(old == admin_pass)
    {
        cout << "Enter new password: ";
        cin >> newpass;
        cout << "To confirm Re-enter new password: ";
        cin >> confirm;
        if(newpass == confirm)
        {
            admin_pass = confirm;
            cout << "Password has been changed succesfully!\n";
        }
        else
        {
            cout << "Passwords do not match!\n";
        }
    }
    else
    {
        cout << "Incorrect current password!\n";
    }
}*/

void setDates(string &start, string &end, int day, int month, int year)
{
    start = to_string(day) + "." + to_string(month) + "." + to_string(year);
    day +=14; 
    if(day >= 30) //assuming each month is of 30 days
    {
        day -= 30;
        if(++month == 13)
        {
            month = 1;
            year++;
        }
    }
    end = to_string(day) + "." + to_string(month) + "." + to_string(year);
}

void issueBook(string filename, Book books[], int book_count, User users[], int user_count)
{
    int bookindex = -1;
    string bookname = "";
    int userindex = -1;
    string username = "";
    while(bookindex < 0)
    {
        cout << "Enter Book name: ";
        cin.ignore();
        getline(cin,bookname);
        bookindex = findBook(bookname, books, book_count);
    }
    if(books[bookindex].isIssued() == true)  
    {
        cout << "Book has already been issued\n\n";
        return;
    }
    while(userindex < 0)
    {
        cout << "Enter Issuers Username: ";
        cin >> username;
        userindex = finduser(username, users, user_count);
    }
    string issuedate, duedate;
    int date =0, month = 0, year = 0;
    while(date <= 0 || date > 30)
    { 
        cout << "Enter date (0-30): ";
        cin >> date;
    }
    while(month > 12 || month <= 0)
    {
        cout << "Enter month (0-12): ";
        cin >> month;
    }
    while(year < 24 || year > 100 )
    {
        cout << "Enter year (XX): ";
        cin >> year;
    }
    setDates(issuedate, duedate, date, month, year);
    if (books[bookindex].isIssued() == false && users[userindex].getIssued() < 3)
    {
        books[bookindex].issue(username, issuedate, duedate);
        users[userindex].issue(bookindex);
        cout << bookname << " issued to: " << username << endl;


        fstream file;
        file.open(filename,ios::app);
        string line = books[bookindex].getIban() + "," + username +"," + issuedate + "," + duedate ;
        file << endl << line;
        file.close();
    }
    else
        users[userindex].issue(bookindex);
}


bool loginAsAdmin(string ADMIN, string admin_pass)
{
    string username,password;
    cout << "Admin Username: ";
    cin >> username;
    cout << "Admin Password: ";
    cin >> password;
    if((ADMIN == username)&&(admin_pass == password))
    {
        return true;
    }
    else
        cout << "Unable to login as admin!\n";
    return false;
}


