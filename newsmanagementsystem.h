#ifndef NEWSMANAGEMENTSYSTEM_H
#define NEWSMANAGEMENTSYSTEM_H

#include <unordered_map>
#include "user.h"
#include <string>
//#include <iostream>
#include "news.h"
#include "admin.h"
#include <vector>
#include<QFile>
#include<sstream>
#include<QTextStream>
using namespace std;
class NewsManagementSystem{
public:
    static unordered_map<string,unordered_map<int,News>> news;
    static unordered_map<string,string> categories; // name of category to image url
    static Admin *currentAdmin;
    static User *currentUser;

    NewsManagementSystem();

    static Admin* getCurrentAdmin();
    static vector<string> allCategories();

    static int noNews();
    static int noCategories();
    static int noUsers();
    static int noComments();

    static vector<pair<string,int>> MapToVector();


    static unordered_map<string,User> users;
    static unordered_map<string,Admin> admins;
    static void readCategoryFromFile(QString);
    static void writeCategoriesInFiles(QString);
    static void  writeNewsDataInFiles(QString NewsData);
    static void readNewsDataFromFiles(QString );
    static void readNewsReviewFromFiles(QString);
    static void writeNewsReviewInFiles(QString);
    static void writeUserInfoInFils(QString);
    static void readUserFromFile(QString );
    static void readAdminFromFile(string );
    static void writeAdminToFile();

};

#endif // NEWSMANAGEMENTSYSTEM_H
