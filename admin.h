#ifndef ADMIN_H
#define ADMIN_H

#include <string>
#include <iostream>
#include <vector>
#include "news.h"
#include "account.h"
//#include "adminwindow.h"
//#include "userwindow.h"
#include "validation.h"

using namespace std;

class Admin : public Account{
public:
    static int ADMIN_ID_COUNT;
    static string SECRET_CODE_GENERAL;
    string secret_code;

public:
    void addNews(string category,News news);
    void modifyNews(string category,int id,News news_updated);
    void removeNews(string category,int id);
    void addCategory(string category,string img);


    Admin();
    Admin(int,string,string,string,string,string secret_key);
    Admin(string,string,string,string);
public:
    void signupAdmin(string name ,string username,string email ,string password);

public:
    static bool signupAdmin(string name ,string username,string email ,string password,string);
    static bool loginAdmin(string a_username, string a_password, string a_secretcode);
    static bool checkAdminExists(string username,string email,string secret_key);
    static void resetPassword(string username,string new_password);
    static string getSecretKey(string username);



};

#endif // ADMIN_H
