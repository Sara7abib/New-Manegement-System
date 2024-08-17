#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
using namespace std;
class Account{
public:
    int id;
    string name;
    string user_name;
    string password;
    string email;

public:

    Account();
    Account(int,string,string,string,string);

    int get_id();
    void set_id(int id);
    string get_name();
    string get_user_name();
    string get_email();
    string get_password();
};

#endif // ACCOUNT_H
