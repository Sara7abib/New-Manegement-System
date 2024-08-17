#ifndef NEWS_H
#define NEWS_H

#include <string>
#include <iostream>
#include <vector>
#include "review.h"
#include "newstime.h"
#include <stack>
#include <utility>
#include <date.h>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
using namespace std;
class News{
public:
    static int ID_COUNT;
public:
    int id;
    string title;
    string description;
    string image_url;
    Date date;
    float rate;
    string category;
    NewsTime time;
    unordered_set<string> spammedUsers; // will initalize in constructor by zero (to count number how many user spam each news)


public:


    unordered_map<string,Review> reviews; //<username,Review>

public:
    News();
    News(string name,string description,string category,string image_url,Date newsdate,NewsTime newstime); // I used it when assigning news after posting it in admin
    News(int,string,string,string,float,string,unordered_set<string>,unordered_map<string,Review>);
    News(int id,string title ,string description,string category,string photourl);
    News(string,string,string,string);
    News(Date,string,string,int,int,string,string);


    News(string title,string description,string image_url,News &t); // copy constructor
    News(string title,string description,string category,string image_url,News &t);

    static bool sortingNewsDate(News n1,News n2);
    static stack<pair<string,int>> latestNews(string category);



public:
    // setters and getters
    int getNewsId();
    string getNewsTitle();
    string getNewsDescription();
    string getNewsImageUrl();
    Date getNewsDate();
    float getNewsRating();
    string getNewsCategory;
    void pushrate(int user_rate,string username);




    void setRate(float rate);

    pair<int,float> noRatingUsers();
    bool foundSpammedUser(string username);
    int noSpammedUsers();
    string writeNewsInfo();
public:
    static Date LatestNews(string Category);



};

#endif // NEWS_H
