 #include "user.h"
#include<vector>
#include "news.h"
#include<unordered_map>
#include<QString>
#include "account.h"
#include "mainwindow.h"
#include<string>
#include<stack>
#include <algorithm>
#include "newsmanagementsystem.h"
#include <QMessageBox>

using namespace std;

int User::USER_ID_COUNT = 1;
User::User() : Account(){

}


User::User(int id,string name, string username, string email, string password)
    : Account(id, name, username, email, password) {

}



User::User(string name, string username, string email, string password)
    : Account(USER_ID_COUNT++, name, username, email, password) {

}

stack<pair<string, int>> User::displayNewsByCategory(string category) {
    vector<pair<string,int>>categoryVector; // vector<pair<string,int>>
    stack<pair<string, int>> s;
    auto it = NewsManagementSystem::news.find(category);
    if (it != NewsManagementSystem::news.end()) {
        for ( auto& newsItem : it->second) {
            if((newsItem.second.rate>2||newsItem.second.noRatingUsers().first<5)&&!newsItem.second.foundSpammedUser(get_user_name()))
                categoryVector.push_back({category,newsItem.first});

        }
    }
    if(!categoryVector.empty()){
        categoryVector=rateSort(categoryVector);
        for(int i=0;i<categoryVector.size();i++){
            s.push(categoryVector[i]);
        }
        return s;
    }
    else
        return s;
}



vector<pair<string,int>> User::rateSort(vector<pair<string,int>> newsarticles){
    for (int i = 0; i < newsarticles.size() - 1; ++i) {
        int minIndex = i;
        for (int j = i + 1; j < newsarticles.size(); ++j) {
            if (NewsManagementSystem::news[newsarticles[j].first][newsarticles[j].second].rate < NewsManagementSystem::news[newsarticles[minIndex].first][newsarticles[minIndex].second].rate) {
                minIndex = j;
            }
        }
        // Swap the pairs in the vector, not the News objects in the map
        swap(newsarticles[i], newsarticles[minIndex]);
    }
    return newsarticles;
}

vector<pair<string, int>> User::searchByTitle(QString searchtitle) {
    unordered_map<string,unordered_map<int ,News>> ::iterator it;
    unordered_map<int, News> ::iterator it2;
    QString searchTitleLower = searchtitle.toLower();
    vector<pair<string, int>> searchVector;
    for (it =NewsManagementSystem::news.begin(); it != NewsManagementSystem::news.end(); it++) {
        for (it2 = it->second.begin(); it2 != it->second.end(); it2++) {
            QString newsTitleLower = QString::fromStdString(it2->second.title).toLower();
            if ((newsTitleLower.contains(searchTitleLower))&&!it2->second.foundSpammedUser(get_user_name())){
                searchVector.push_back({it2->second.category, it2->first});
            }
        }
    }
    return searchVector;
}
vector<pair<string, int>> User::searchByKeyWord(QString searchkey) {
    unordered_map<string,unordered_map<int ,News>> ::iterator it;
    unordered_map<int, News> ::iterator it2;

    vector<pair<string, int>> searchVector;
    for (it = NewsManagementSystem::news.begin(); it != NewsManagementSystem::news.end(); it++) {

        for (it2 = it->second.begin(); it2 != it->second.end(); it2++) {

            if ((QString::fromStdString(it2->second.description).contains(searchkey))&&!it2->second.foundSpammedUser(get_user_name()))
                searchVector.push_back({it2->second.category, it2->first});

        }
    }
    return searchVector;
}
vector<pair<string, int>> User::searchByDate(QDate searchdate) {
    unordered_map<string,unordered_map<int ,News>> ::iterator it;
    unordered_map<int, News> ::iterator it2;
    vector<pair<string, int>> searchVector;
    for (it = NewsManagementSystem::news.begin(); it != NewsManagementSystem::news.end(); it++) {
        for (it2 = it->second.begin(); it2 != it->second.end(); it2++) {
            if (it2->second.date.toQDate() == searchdate&&!it2->second.foundSpammedUser(get_user_name()))
                searchVector.push_back({it2->second.category, it2->first});
        }
    }
    return searchVector;
}



stack<pair<string,int>>User::showLatestNews(){
    stack<pair<string,int>>latest;
    vector<pair<string,int>> latestNews;
    unordered_map<string,unordered_map<int,News>>::iterator it1;
    unordered_map<int,News>::iterator it2;
    for(it1=NewsManagementSystem::news.begin();it1!=NewsManagementSystem::news.end();it1++){
        for(it2=it1->second.begin();it2!=it1->second.end();it2++){
            if (!it2->second.foundSpammedUser(get_user_name()))
                latestNews.push_back(make_pair(it2->second.category,it2->second.id));
        }
    }
    if(!latestNews.empty()){
        latestNews=sortByDate(latestNews);
        if(latestNews.size()>10)
            for(int i=latestNews.size()-10;i<latestNews.size();i++)
                latest.push(latestNews[i]);
        else
            for(int i= 0;i<latestNews.size();i++)
                latest.push(latestNews[i]);
        return latest;
    }
    else
        return latest;
}

vector<pair<string,int>>User::sortByDate(vector<pair<string,int>> newsarticles){
    for (int i = 0; i < newsarticles.size() - 1; ++i) {
        int minIndex = i;
        for (int j = i + 1; j < newsarticles.size(); ++j) {
            if (NewsManagementSystem::news[newsarticles[j].first][newsarticles[j].second].date.toQDate()<=NewsManagementSystem::news[newsarticles[minIndex].first][newsarticles[minIndex].second].date.toQDate()) {
                minIndex = j;
            }
        }
        // Swap the pairs in the vector, not the News objects in the map
        swap(newsarticles[i], newsarticles[minIndex]);
    }
    return newsarticles;
}




void User::bookMarkingNews(int id,string category,bool flag){
    pair<string,int>currunt;
    currunt.first=category;
    currunt.second=id;
    if (flag) {
        favouriteList.push_front(currunt);
    } else {
        for (auto it = favouriteList.begin(); it != favouriteList.end(); ++it) {
            if (*it == currunt) {
                favouriteList.erase(it);
                break;
            }
        }
    }
}
list<pair<string,int>> User::getFvourite(){
    return favouriteList;
}
void User::reportAsSpam(News &n ){

    //n.spammedUsers.insert(this->get_id());
    n.spammedUsers.insert(NewsManagementSystem::currentUser->get_user_name());


}
stack<pair<string,int>> User::displayTrendingNews(){
    stack<pair<string,int>>trending;
    vector<pair<string,int>> trendingNews;
    unordered_map<string,unordered_map<int,News>>::iterator it1;
    unordered_map<int,News>::iterator it2;
    for(it1=NewsManagementSystem::news.begin();it1!=NewsManagementSystem::news.end();it1++){
        for(it2=it1->second.begin();it2!=it1->second.end();it2++){
            int difference=Date::currentDate().toQDate().daysTo(it2->second.date.toQDate());
            if(difference>=-2&&difference<=0&&it2->second.rate>=4&&!it2->second.foundSpammedUser(get_user_name())){
                int i=Date::currentDate().toQDate().daysTo(it2->second.date.toQDate());
                qDebug()<<i;
                trendingNews.push_back(make_pair(it2->second.category,it2->second.id));
            }
        }
    }
    if(!trendingNews.empty()){
        trendingNews = rateSort(trendingNews);
        for(int i=0;i<trendingNews.size();i++){
            trending.push(trendingNews[i]);
        }
        return trending;
    }
    else
        return trending;
}




void User::compareLastSubscribedCategoryNews(string category,int id){
    News news_clicked = NewsManagementSystem::news[category][id];
    News last_news = NewsManagementSystem::news[category][subscribedCategories[category].second]; // that is in subscribed category

    // when he click on any category if he subscribed or not we will add its key in map and assign boolean to false and id to -1 but check first it is not already exist in unordered map
    // check if the id is -1 either it is false or true , meaning it is the first news so don't compare
    if(subscribedCategories[category].second == -1){
        NewsManagementSystem::currentUser->subscribedCategories[category] = make_pair(subscribedCategories[category].first,id);
        return;
    }



    // compare

    if(last_news.date.year > news_clicked.date.year){
        return;

    }else if (last_news.date.year < news_clicked.date.year){
        NewsManagementSystem::currentUser->subscribedCategories[category] = make_pair(subscribedCategories[category].first,id);

    }else{
        if(last_news.date.month > news_clicked.date.month){
            return;

        }else if (last_news.date.month < news_clicked.date.month){
            NewsManagementSystem::currentUser->subscribedCategories[category] = make_pair(subscribedCategories[category].first,id);

        }else{
            if(last_news.date.day > news_clicked.date.day){
                return;
            }else if (last_news.date.day < news_clicked.date.day){
                NewsManagementSystem::currentUser->subscribedCategories[category] = make_pair(subscribedCategories[category].first,id);

            }else{ // days are same , we will compare by time
                if(last_news.time.hours > news_clicked.time.hours){
                    return;
                }else if (last_news.time.hours < news_clicked.time.hours){
                    NewsManagementSystem::currentUser->subscribedCategories[category] = make_pair(subscribedCategories[category].first,id);

                }else{
                    if(last_news.time.minutes > news_clicked.time.minutes){
                        return;
                    }else if (last_news.time.minutes < news_clicked.time.minutes){
                        NewsManagementSystem::currentUser->subscribedCategories[category] = make_pair(subscribedCategories[category].first,id);

                    }else{
                        if(last_news.time.seconds > news_clicked.time.seconds){
                            return;
                        }else if (last_news.time.seconds < news_clicked.time.seconds){
                            NewsManagementSystem::currentUser->subscribedCategories[category] = make_pair(subscribedCategories[category].first,id);

                        }else{
                            return;
                        }
                    }
                }
            }
        }
    }

    // You can add many conditions in one if condition and use and && and or ||

    // or you can convert them to QDate and QTime it will be easier in comparison




}



bool User::signupUser(string name, string username, string email, string password){

    if(Validation::checkEmailExists(email) || Validation::checkUsernameExists(username)){
        return false;
    }

    qDebug() << "username: " << QString::fromStdString(username) << "- Password: " << QString::fromStdString(password);
    User newuser(name,username,email,Validation::encryptPassword(password));

    NewsManagementSystem::users[newuser.get_user_name()] = newuser;
    qDebug () << "I'm in Register function:  " << QString::number(newuser.get_id()) << "-" << QString::fromStdString(newuser.get_user_name()) << "-" << QString::fromStdString(newuser.get_password());
    return true;
}

bool User::loginUser(string username, string password){
    auto user_itrator = NewsManagementSystem::users.find(username);
    if (user_itrator != NewsManagementSystem::users.end()){
        if(Validation::comparePassword(password,user_itrator->second.password)){
            NewsManagementSystem::currentUser = &NewsManagementSystem::users[username];
            return true;
        }



    }
    return false;

}


// Forget Password
bool User::checkUserExists(string username,string email){
    auto user_itrator = NewsManagementSystem::users.find(username);
    if (user_itrator != NewsManagementSystem::users.end()){
        if(user_itrator->second.email == email){
            return true;
        }
    }
    return false;

}



void User::resetPassword(string username,string new_password){
    NewsManagementSystem::users[username].password = Validation::encryptPassword(new_password);
}







