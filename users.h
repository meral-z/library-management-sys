#include <iostream>
#include <iomanip>

using namespace std;

int hashVal(string);

class User{

private:
    string username;
    int duefee;
    int hashed_password;
    int issued;
    int issuedbooksindex[3];//each user can only issue 3 books, stores index of book issued

public:
    User()
    {
        username = "";
        hashed_password = -100;
        duefee = 0;
        issued = 0;
    }

    void issue(int issue)
    {
        if(issued < 3)
        {       
            issuedbooksindex[issued] = issue;
            issued++;
        }
        else
            cout << "This user has already been issued 3 books\n";
    }
    int hashPass()
    {
        return hashed_password;
    }
    int getBookIndex(int x)
    {
        return issuedbooksindex[x];
    }

    string getUsername()
    {
        return username;
    }

    
    void setUser(string name,int password)
    {
        username = name;
        hashed_password = password;
        duefee = 0;
    }

    void setDueFee(int fee)
    {
        duefee = fee;
    }

    int getDuefee()
    {
        return duefee;
    }

    int getIssued()
    {
        return issued;
    }
};

void printUsers(int size, User users[])
{
    for(int i=0; i<=size ; i++)
    {
        cout << users[i].getUsername() << endl;
    } 
}

int importUsers(string filename, User users[])  //return values of books stored and outputs
{
    string line ;
    fstream userfile;
    int i = 0;

    userfile.open(filename,ios::in);
    if(userfile.is_open()){
        while(!userfile.eof())
        {   //could use while(getline()) for simplicity
            getline(userfile,line);
            int separator = line.find(','); // using csv file so separator is ','
        
            string usern = line.substr(0,separator);
            int password = stoi(line.substr(separator + 1));
            users[i].setUser(usern,password);
            i++; //importing linearly in array
        }
        //cout << "Import successfull: "<< i << " users imported!"<<endl;
        userfile.close();
        //printUsers(i,users);
        return i;
    }
    else
        cout << "Error: Could not import books.";
        return 0;
}

int hashVal(string password) // Returns hash value of password //max value will be 9516 given that it is a12. so range is min 3744 given 012.
{
    int hash = 0;
    for (int i =0; i < password.length(); i++)
    {
        hash += password[i]*(i+1);
    }
    return hash;
}

int login(User arr[], int size)
{
    //get input
    string username;
    string password;

    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;
    //linear search for password corresponding to username
    for(int i=0; i<=size ; i++)
    {
        if ((arr[i].getUsername() == username)&&(arr[i].hashPass() == hashVal(password)))
        {
            cout << "\n----------|----------" << "Welcome " << username << "----------|----------\n\n";
            return i;
        }
    }
    cout << "Username or password invalid." << endl;
    return -1;
}


bool isPasswordValid(string password)
{   int numcounter=0;
    int lowercasecounter=0;
    int uppercasecounter=0;
 
    if (password.length()<12)
    {
        do 
        {
            cout << "Password is invalid. Password should have at least 12 characters." << endl;
            return false;
        }
        while (password.length()<12);
    }

    else
    {
        for (int i=0; i<password.length(); i++)
        {
            char currentChar= password[i];
            if(currentChar>='0' && currentChar<='9')
            {
                numcounter++;
            }

            else if (currentChar>='a' && currentChar<='z')
            {
                lowercasecounter++;
            }

            else if (currentChar>='A' && currentChar<='Z')
            {
                uppercasecounter++;
            }

        }
        
        if (numcounter==0)
        {
            cout << "Password is invalid. Password should have at least 1 numeric character." << endl;
            return false;
        }
        
        if (lowercasecounter==0)
        {
            cout << "Password is invalid. Password should have at least 1 lowercase letter." << endl;
            return false;
        }

        if (uppercasecounter==0)
        {
            cout << "Password is invalid. Password should have at least 1 uppercase letter." << endl;
            return false;
        }

    return true; 
    }
}

bool isUserValid(User arr[],int size,string username)
{
    // linearly search in array if username alr exists
    for(int i=0; i<=size ; i++)
    {
        if ((arr[i].getUsername() == username))
        {
            cout << "Username is already in use. Try again.\n";
            return false;
        }
    }
    return true;
}

bool signUp(User arr[], int & size, string filename)
{
    string username;
    string password;

    cout << "Username: ";
    cin >> username;

    while(isUserValid(arr,size,username)== false)
    {
        cout << "Username: ";
        cin >> username;
    }
    
    cout << "Password: ";
    cin >> password;
    while(isPasswordValid(password) == false)
    {
        cout << "Password: ";
        cin >> password;
    }
   
    arr[size].setUser(username,hashVal(password));
    size +=1;
    cout << "Sign Up Successful!" << endl;

    //appending user to file
    fstream userfile;
    userfile.open(filename,ios::app);
    string line = username + "," + to_string(hashVal(password));
    userfile << endl << line;
    userfile.close();
    return true;
}

int finduser(string username, User users[], int size)
{
    for (int x = 0; x < size; ++x) {
        if (users[x].getUsername() == username) {
            return x; // Found the book
        }
    }
    return -1; // Book not found
}