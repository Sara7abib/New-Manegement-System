#include "account.h"

Account::Account(){
    this->id = 0;
    this->name = "";
    this->user_name = "";
    this->email = "";
    this->password = "";
}

Account::Account(int id,string name,string username,string email,string password){
    this->id = id;
    this->name = name;
    this->user_name = username;
    this->email = email;
    this->password = password;
}


int Account::get_id(){
    return id;
}
string Account::get_name(){
    return name;
}

string Account::get_user_name(){
    return user_name;
}

string Account::get_email(){
    return email;
}

string Account::get_password(){
    return password;
}

void Account::set_id(int id){
    this->id=id;
}

