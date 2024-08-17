#ifndef USER_H
#define USER_H
#include<string>
#include<unordered_map>
#include<vector>
#include<QString>
#include<QDate>
#include"news.h"
#include<stack>
#include "account.h"
#include "validation.h"

using namespace std;

class User : public Account {
public:
    list<pair<string,int>>favouriteList;

public:
    static int USER_ID_COUNT;
    list<pair<string,int>> getFvourite();
    User();
    User(int id,string name ,string username,string email ,string password);
    User(string,string,string,string);
    stack<pair<string, int>>displayNewsByCategory(string);
    vector<pair<string, int>> searchByTitle(QString);
    vector<pair<string, int>> searchByKeyWord(QString);
    vector<pair<string, int>> searchByDate(QDate );
    void pushrate(int);
    void trendingNews(unordered_map<QString,vector<News>>);
    stack<pair<string,int>>showLatestNews();
    vector<pair<string,int>>rateSort(vector<pair<string,int>>);
    void  bookMarkingNews(int ,string ,bool);
    vector<pair<string,int>>sortByDate(vector<pair<string,int>>);
    void reportAsSpam(News &);
    stack<pair<string,int>>displayTrendingNews();

    unordered_map<string,pair<bool,int>> subscribedCategories; // to keep track subscribed categories and last news he clicked on it
    void compareLastSubscribedCategoryNews(string category,int id); // news that clicked and it will be compared to news inside


public:
    static bool signupUser(string name, string username, string email, string password);
    static bool loginUser(string username, string password);
    static bool checkUserExists(string username,string email);
    static void resetPassword(string username,string new_password);


};
#endif // USER_H
