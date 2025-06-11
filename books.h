#include <iostream>
//#include <ctime>  //for dates
#include <fstream>
using namespace std;

class Book{
private:
    string name;
    string iban;
    bool issued;
    string date_issued;
    string date_due;
    string user_issued;

public:
    //default constructor
    Book(){
        name = "";
        iban = "";
        issued = false;
        date_issued = "0.0.2000";
        date_due = "0.0.2000";
        user_issued= "";
    }
    //initialise
    void setBook(string pname,string piban) {
        name = pname;
        iban = piban;
        issued = false;
    }

    //issue book function to update issue status on file and on array
    void issue(string username, string issuedate, string duedate) {
        if (issued == false)
        {
            issued = true;
            user_issued = username;
            date_issued = issuedate;
            date_due =  duedate;
        }
        else
            cout << "The book is unavailable for issuance. It has already been issued to " << user_issued<< " and will be available after " << date_due << endl;
    }
    //Display info --for debugging--
    string getBookName()
    {
        return name;
    }    
    
    void setUserIssued(string user)
    {
        user_issued = user;
    }
    string getUserIssued()
    {
        return user_issued;
    }
    string getIban()
    {
        return iban;
    }
    bool isIssued()
    {
        return issued;
    }
    string getDue()
    {
        return date_due;
    }
};

int importBooks(string filename, Book books[])  //return values of books stored and outputs
{
    string line ;
    fstream bookfile;
    int i = 0;

    bookfile.open(filename,ios::in);
    if(bookfile.is_open()){
        while(!bookfile.eof()){     //could use while(getline()) for simplicity
            getline(bookfile,line);
            int separator = line.find(','); // using csv file so separator is ','
            
            string bname = line.substr(0,separator);
            string iban = line.substr(separator + 1);
            books[i].setBook(bname, iban);
            i++; //importing linearly in array
        }
        //cout << "Import successfull: "<< i << " Books imported!"<<endl;
        bookfile.close();
        return i;
    }
    else
        cout << "Error: Could not import books.";
        return 0;
}

void printBooks(int size, Book books[]) //outputs all books stored in array
{
    cout <<"__________________________________________________________\n";
    cout << "The library currently has these books: "<< endl;
    for(int i = 0; i < size; i++)
    {
        cout << books[i].getBookName() << endl;
    }
    cout <<"__________________________________________________________\n";
}

int findBook(string book, Book books[], int size) {
    for (int x = 0; x < size; ++x) {
        if (books[x].getBookName() == book) {
            return x; // Found the book
        }
    }
    return -1; // Book not found
}

void searchLib(Book books[], int size)
{
    int find;
    string bookname;
    string output;
    cout << "Enter book name to search in library: "<< endl;
    cin.ignore();
    getline(cin, bookname);
    find = findBook(bookname,books,size);
    output = (find==-1)? "Book is not in library." : (books[find].isIssued() == true)? "Book has already been issued." : "Book is available for issuance.";
    cout << output << endl;
}
