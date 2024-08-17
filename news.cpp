#include "news.h"
#include "newsmanagementsystem.h"
#include <QDebug>


int News::ID_COUNT = 1;
News::News() { // Constructor

}

News::News(string title,string description,string category,string photourl){
    this->id=ID_COUNT++;
    this->title=title;
    this->description=description;
    image_url=photourl;
    this->category=category;
}
News::News(Date d,string t,string desc,int i,int r,string cat,string image) {

    id=i;
    title=t;
    description=desc;
    image_url=image;
    date=d;
    rate=r;
    category=cat;
}
News::News(int id,string title ,string description,string category,float rating,string photourl,unordered_set<string> spammedUsers,unordered_map<string,Review> newsReviews){

    this->id = id;
    this->title = title;
    this->description = description;
    this->category = category;
    this->rate = rating;
    this->image_url = photourl;

    this->spammedUsers = spammedUsers;
    reviews = newsReviews;

}

News::News(int id,string title ,string description,string category,string photourl){
    this->id = id;
    this->title = title;
    this->description = description;
    this->category = category;
    this->image_url = photourl;

}

News::News(string title,string description,string image_url,News &t) // copy constructor(you give it object and it assigns some of its value members in this news new object
{

    this->title = title;
    this->description = description;
    this->image_url = image_url;

    this->id = t.id;
    this->category = t.category;
    this->date = t.date;
    this->time = t.time;

    this->rate = t.rate;
    this->spammedUsers = t.spammedUsers;
    this->reviews = t.reviews;
}

News::News(string title,string description,string category,string image_url,News &t){
    this->title = title;
    this->description = description;
    this->image_url = image_url;
    this->category = category;

    this->rate = t.rate;
    this->id = t.id;
    this->date = t.date;
    this->time = t.time;
    this->spammedUsers = t.spammedUsers;
    this->reviews = t.reviews;

}

int News::getNewsId(){
    return this->id;
}

// true > don't swap
// false > swap
bool News::sortingNewsDate(News n1,News n2){
    if(n1.date.year < n2.date.year)
        return true;
    else if (n1.date.year == n2.date.year){
        if(n1.date.month < n2.date.month)
            return true;
        else if(n1.date.month == n2.date.month)
            if(n1.date.day < n2.date.day)
                return true;
            else if (n1.date.day > n2.date.day)
                return false;
            else
                return true;
        else
            return false;

    }else{
        return false;
    }

}


stack<pair<string,int>> News::latestNews(string category){
    vector<News> news;
    stack <pair<string,int>> s;
    for(auto it : NewsManagementSystem::news[category]){
        news.push_back(it.second);
    }
    sort(news.begin(),news.end(),sortingNewsDate);
    for(int i = 0; i < news.size();i++){
        s.push(make_pair(news[i].category,news[i].id));
    }
    return s;
}

Date News::LatestNews(string Category){
    pair<string,int> latestn = News::latestNews(Category).top();
    return NewsManagementSystem::news[latestn.first][latestn.second].date;

}

void News:: pushrate(int user_rate,string username) {
    reviews[username].rating = user_rate;
    setRate(noRatingUsers().second / noRatingUsers().first);
    qDebug() << "In Push rate Function user rating is : " << QString::number(user_rate);
    qDebug() << "In push rate function Avg rating: " << QString::number(rate);


}

void News::setRate(float rate){
    this->rate=rate;
}


pair<int,float> News::noRatingUsers(){
    pair<int,float> counter_users_summationrating;
    int counter_users = 0;
    float summation_rating = 0;
    for (auto s : reviews){
        if(s.second.rating !=-1){
            counter_users++;
            summation_rating+=s.second.rating;
        }
    }
    counter_users_summationrating.first = counter_users;
    counter_users_summationrating.second = summation_rating;
    return counter_users_summationrating;
}

//Constructor
News::News(string name,string description,string category,string image_url,Date newsdate,NewsTime newstime){

    this->title = name;
    this->description = description;
    this->category = category;
    this->image_url = image_url;
    this->date = newsdate;
    this->time = newstime;
    this->id = ID_COUNT++; //to assign it first then increment
    this->rate = -1;

}

bool News::foundSpammedUser(string username){
    auto it = spammedUsers.find(username);
    if (it != spammedUsers.end()) {
        return true;
    } else {
        return false;
    }
}
int News::noSpammedUsers(){
    return spammedUsers.size();
}

string News::writeNewsInfo() {
    string s = /*this->category + "~" + */to_string(this->id) + "~" + this->title + "~" + this->description + "~" + this->image_url + "~" +to_string(this->rate) + "~" + to_string(this->date.day) + "/" + to_string(this->date.month) + "/" + to_string(this->date.year) + "~" + to_string(this->time.hours) + ":" + to_string(this->time.minutes) + ":" + to_string(this->time.seconds);
    if (!spammedUsers.empty()) {
        s += '~';
        int index = 0;
        for (auto it = this->spammedUsers.begin(); it != this->spammedUsers.end(); ++it, ++index) {
            s += *it;
            if (index != this->spammedUsers.size() - 1) {
                s += ",";
            }
        }
        return s;
    }
}
