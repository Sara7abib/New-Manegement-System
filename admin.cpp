#include "admin.h"
#include "news.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include "newsmanagementsystem.h"
#include "account.h"


int Admin::ADMIN_ID_COUNT =1;
string Admin::SECRET_CODE_GENERAL = "__Mahmood__";


Admin::Admin():Account(){
    this->secret_code=Validation::generateSecretKey();
}



Admin::Admin(int id,string name ,string username,string email ,string password,string secretcode)
     : Account(id,name,username,email,password){
    this->secret_code = secretcode;
}



Admin::Admin(string name ,string username,string email ,string password)
     : Account(ADMIN_ID_COUNT++,name,username,email,password){
    // HERE secret code of each admin will be initlilzed by calling a function that get random secret id
    this->secret_code = Validation::generateSecretKey();
}

void Admin::addNews(string category,News news){

    NewsManagementSystem::news[category][news.getNewsId()] = news;

}


void Admin::removeNews(string category,int id){
    auto iterator = NewsManagementSystem::news[category].find(id); //find() returns iterator to the value
    NewsManagementSystem::news[category].erase(iterator);

}



void Admin::modifyNews(string category,int id,News news_updated){

    NewsManagementSystem::news[category][id] = news_updated;
}

void Admin::addCategory(string category,string img){
    NewsManagementSystem::news[category];
    NewsManagementSystem::categories[category] = img;
}

bool Admin::loginAdmin(string a_username, string a_password, string a_secretcode){
    auto admin_itrator = NewsManagementSystem::admins.find(a_username);
    if (admin_itrator != NewsManagementSystem::admins.end()){
        if(Validation::comparePassword(a_password,admin_itrator->second.password) && admin_itrator->second.secret_code == a_secretcode){
            NewsManagementSystem::currentAdmin = &NewsManagementSystem::admins[a_username];
            return true;
        }
    }
    return false;
}

bool Admin::signupAdmin(string name ,string username,string email ,string password,string general_secret){
    if(Validation::checkEmailExists(email) || Validation::checkUsernameExists(username)){
        return false;
    }

    if(Admin::SECRET_CODE_GENERAL != general_secret){
        return false;
    }

    Admin newadmin(name,username,email,Validation::encryptPassword(password));

    NewsManagementSystem::admins[newadmin.get_user_name()] = newadmin;
    return true;

}

string Admin::getSecretKey(string username){
    return NewsManagementSystem::admins[username].secret_code;
}


// Forget Password
bool Admin::checkAdminExists(string username,string email,string secret_key){
    auto admin_itrator = NewsManagementSystem::admins.find(username);
    if (admin_itrator != NewsManagementSystem::admins.end()){
        if(admin_itrator->second.email == email && admin_itrator->second.secret_code == secret_key){
            return true;
        }
    }
    return false;

}


void Admin::resetPassword(string username,string new_password){

    NewsManagementSystem::admins[username].password = Validation::encryptPassword(new_password);
}

