#include "validation.h"

#include "newsmanagementsystem.h"
Validation::Validation() {

}

string Validation::generateSecretKey(){
    string charset = "0123456789ABCDEFGHIJKLMN123456789OPQRSTUVWHYZabcdefghijklmnopqrstuvwxyz";
    int length = 8;
    string Secretcode;
    for (int i = 0; i < length; i++) {
        Secretcode.push_back(charset[rand() % charset.length()]);
    }
    return Secretcode;
}
bool Validation::checkemail(string email){
    int j = 0;
    bool email_check1 = false; //to check @
    bool email_check2 = false; // to check .
    while (email[j] != '\0') { // the test case if he writes . before @ htwk3h , elmafrod en you check on ".com" or ".eg" and make sure he wries @ before it not after the order is matter here
        if (email[j] == '@') {
            email_check1 = true;
        }
        if (email[j] == '.') {
            email_check2 = true;
        }
        j++;
    }
    if (email_check1&&email_check2)
        return true;
    else
        return false;
}

bool Validation::checkpassword(string password){
    if(password.length() < 8 ){
        return false;
    }

    bool hasUpper = false;
    bool hasLower = false;
    bool hasDigit = false;
    bool hasSpecial = false;

    for(char ch : password){
        if(isupper(ch)){
            hasUpper = true;
        }
        else if(islower(ch)){
            hasLower = true;
        }
        else if(isdigit(ch)){
            hasDigit = true;
        }
        else if(!isalnum(ch)){
            hasSpecial = true;
        }

        if(hasUpper && hasLower && hasDigit && hasSpecial){
            break;
        }
    }

    return hasUpper && hasLower && hasDigit && hasSpecial;

}


QString Validation::TrimLeft(string S1) {

    for (short i = 0; i < S1.length(); i++) {
        if (S1[i] != ' ') {
            return QString::fromStdString(S1.substr(i, S1.length() - i));
        }
    }
    return "";
}

QString Validation::TrimRight(string S1) {

    for (short i = S1.length() - 1; i >= 0; i--) {
        if (S1[i] != ' ') {
            return  QString::fromStdString(S1.substr(0, i + 1));
        }
    }

    return "";
}



int Validation::randomNumber(int from, int to) {
    int RandomNumber = rand() % (to - from + 1) + from;
    return RandomNumber;
}

bool Validation::emptyInput(QString input){
    if (input.isEmpty())
        return true;
    return false;
}


string Validation::encryptPassword(string password){
    for(int i = 0; i < password.length();i++){
        if(password[i] >= 65 && password[i] <=90){ //Capital letters (65 -> 90)
            if(password[i] == 90){
                password[i] = char(65);
                continue;
            }
            password[i] = ++password[i];
            //char ans = 'a'; ans++;

        }
        else if(password[i] >= 97 && password[i] <= 122){ // small (97 -> 122)
            if(password[i] == 122){
                password[i] = char(97);
                continue;
            }
            password[i] = ++password[i];

        }
        else if(password[i] >= 48 && password[i] <= 57){ // 0 ---> 9 , 48 ascii code of 0 , 57 ascii code of 1
            if(password[i] == 57){
                password[i] = char(48);
                continue;
            }
            password[i] = ++password[i];
            //password[i] = (int(password[i])+1)%10;
        }

    }
    return password;
}

bool Validation::comparePassword(string password,string encrypted_password){
    // 2 ways (password -> encrypted) , (encrypted becomes decrypted then compare)
    return encrypted_password == encryptPassword(password);
}


bool Validation::checkEmailExists(string email){
    bool user_check = false, admin_check = false;
    for (auto &it: NewsManagementSystem::users){
        if(it.second.get_email() == email){
            user_check = true;
            break;
        }
    }
    for (auto &it: NewsManagementSystem::admins){
        if(it.second.get_email() == email){
            admin_check = true;
            break;
        }
    }
    return user_check || admin_check;
}

bool Validation::checkUsernameExists(string username){

    auto iterator_user = NewsManagementSystem::users.find(username);
    auto iterator_admin = NewsManagementSystem::admins.find(username);

    if(iterator_user == NewsManagementSystem::users.end() && iterator_admin == NewsManagementSystem::admins.end()){
        return false;
    }
    return true;

}
