#ifndef VALIDATION_H
#define VALIDATION_H

#include <iostream>
#include <cstdlib> // For generating random numbers
#include <ctime>
#include <regex>
#include <cctype>
#include <QString>

using namespace std;
class Validation
{
public:
    Validation();
    static bool checkemail(string email);
    static bool checkpassword(string password);
    static string generateSecretKey();
    static QString TrimLeft(string S1);
    static QString TrimRight(string S1);
    static int randomNumber(int from, int to);
    static bool emptyInput(QString input);


    static string encryptPassword(string password);
    static bool comparePassword(string password,string encrypted_password);

    static bool checkUsernameExists(string username);
    static bool checkEmailExists(string email);

};

#endif // VALIDATION_H
