#include "newsmanagementsystem.h"
#include<unordered_map>
#include <iostream>
#include<QFile>
#include<sstream>
#include<QTextStream>
using namespace std;


unordered_map<string,unordered_map<int,News>> NewsManagementSystem::news;

unordered_map<string,string> NewsManagementSystem::categories;

unordered_map<string,Admin> NewsManagementSystem::admins;
unordered_map<string,User> NewsManagementSystem::users;

User *NewsManagementSystem::currentUser = NULL;
Admin *NewsManagementSystem::currentAdmin = NULL;

Admin* NewsManagementSystem::getCurrentAdmin(){
    return currentAdmin;

}

NewsManagementSystem::NewsManagementSystem() {}




vector<string> NewsManagementSystem::allCategories(){
    vector<string> nameofcategories;
    for (auto it : news){ // outer unorderedmap which its key is name of category which is the key
        nameofcategories.push_back(it.first);
    }
    return nameofcategories;
}

int NewsManagementSystem::noNews(){
    int counter = 0;
    for(auto n : news){
        counter+=n.second.size(); // size of inner unordered map
    }
    return counter;
}

int NewsManagementSystem::noCategories(){

    return news.size(); // no of keys = no of categories which is the size of map
}

int NewsManagementSystem::noUsers(){
    return users.size();
}


int NewsManagementSystem::noComments(){
    int counter = 0;
    for (auto n : news){
        for (auto news_ : n.second){
            for (auto c : news_.second.reviews){ //loop through unordered map of reviews
                counter += c.second.comments.size();
            }
        }

    }
    return counter;
}

vector<pair<string,int>> NewsManagementSystem::MapToVector(){
    vector<pair<string,int>> MapToVector;
    for(auto& outerPair : news)
    {
        for(auto& innerPair : outerPair.second)
        {
            MapToVector.push_back(make_pair(innerPair.second.category,innerPair.second.id));
        }
    }
    return MapToVector;
}

void NewsManagementSystem:: writeCategoriesInFiles(QString categoryData){
    QFile catData(categoryData);
    if(!catData.open(QIODevice::WriteOnly)){
        qDebug() <<"Failed to open file.";
        return;
    }
    QTextStream stream(&catData);
    for (auto it = categories.begin(); it != categories.end(); ++it) {
        stream << QString::fromStdString(it->first) << ';' << QString::fromStdString(it->second) << "\n";
    }
    catData.close();
}
void NewsManagementSystem::readCategoryFromFile(QString categoryData){
    QFile catData(categoryData);
    if(!catData.open(QIODevice::ReadOnly)){
        qDebug() <<"Failed to open file.";
        return;
    }
    QTextStream stream(&catData);
    while(!stream.atEnd()){
        QString line = stream.readLine();
        istringstream read(line.toStdString());
        string categoryName;
        getline(read, categoryName, ';');
        string categoryUrl;
        getline(read, categoryUrl);
        categories[categoryName]=categoryUrl;
        // qDebug() << categoryName << " "<< categoryUrl;
    }
    catData.close();
}

void NewsManagementSystem::readNewsDataFromFiles(QString NewsData){
    QFile Data(NewsData);
    int newsCounter=0;
    if (!Data.open(QIODevice::ReadOnly)){
        qDebug() <<"Faild to open file.";
        qDebug() <<"it doesont open";
        return;
    }
    QTextStream stream(&Data);
    while (!stream.atEnd()) {
        QString line = stream.readLine();
        qDebug()<<line;
        if(line.isEmpty())
            continue;
        qDebug() <<line;
        istringstream iss(line.toStdString());
        string category;
        getline(iss, category, ':');
        news[category];
        // qDebug() << category;
        if(line.endsWith(':')){
            QString lineInput = stream.readLine();
            do{
                qDebug() <<lineInput;
                istringstream newsInputs(lineInput.toStdString());
                News n;
                //getline(newsInputs, n.category, '~');
                n.category=category;
                string idStr;
                getline(newsInputs, idStr, '~');
                n.id = stoi(idStr);
                getline(newsInputs, n.title, '~');
                getline(newsInputs, n.description, '~');
                getline(newsInputs, n.image_url,'~');
                string rateStr;
                getline(newsInputs, rateStr, '~');
                n.rate=stoi(rateStr);
                string date;
                getline(newsInputs, date, '~');
                istringstream dateStream(date);
                string day;
                getline(dateStream, day , '/');
                n.date.day=stoi(day);
                string month;
                getline(dateStream, month, '/');
                n.date.month=stoi(month);
                string year;
                getline(dateStream,  year);
                n.date.year=stoi(year);
                string time;
                getline(newsInputs, time, '~') && !time.empty();
                istringstream timeStream(time);
                string hour;
                getline(timeStream, hour , ':');
                n.time.hours=stoi(hour);
                string minute;
                getline(timeStream, minute, ':');
                n.time.minutes=stoi(minute);
                string second;
                getline(timeStream,  second);
                n.time.seconds=stoi(second);
                string usersName;
                if (getline(newsInputs, usersName) && !usersName.empty()) {
                    istringstream userStream(usersName);
                    string userName;
                    while (getline(userStream, userName, ',')) {
                        n.spammedUsers.insert(userName);
                    }
                }
                news[category][n.id]=n;
                lineInput = stream.readLine();
            }while(!lineInput.isEmpty());
            qDebug() <<lineInput;
            newsCounter++;
        }
        else{
            // qDebug() <<category;
            continue;
        }
    }
    Data.close();
    News::ID_COUNT=newsCounter+1;

    qDebug() <<News::ID_COUNT;
}
void NewsManagementSystem:: writeNewsDataInFiles(QString NewsData){
    QFile Data(NewsData);
    if (!Data.open(QIODevice::WriteOnly)){
        qDebug() <<"Faild to open file.";
        qDebug() <<"not open to write";
        return;
    }
    QTextStream stream(&Data);
    for(auto it1=news.begin();it1!=news.end();it1++){
        stream <<QString::fromStdString(it1->first) <<':';
        if(it1->second.size()!=0){
            stream<<'\n';
            for(auto it2=it1->second.begin();it2!=it1->second.end();it2++){
                stream << QString::fromStdString(it2->second.writeNewsInfo()) << '\n';
            }
        }
        else{
            stream<<"No news yet."<<'\n';
        }
        stream <<'\n';
    }
    Data.close();
}

void NewsManagementSystem:: readNewsReviewFromFiles(QString NewsReview ){
    QFile review(NewsReview);
    if (!review.open(QIODevice::ReadOnly)){
        qDebug() <<"Faild to open file.";
        qDebug()<<"no thing open";
        return;
    }
    QTextStream stream(&review);
    while (!stream.atEnd()) {
        QString line = stream.readLine();
        qDebug() << line;
        if(line.isEmpty())
            continue;
        istringstream iss(line.toStdString());
        string category;
        getline(iss, category, ';');
        string idStr;
        int newsId;
        getline(iss, idStr, ';');
        newsId = stoi(idStr);
        string userName;
        getline(iss, userName, ';');
        Review r;
        string rateStr;
        getline(iss, rateStr, ';');
        r.rating=stoi(rateStr);
        if(line.endsWith(':')){
            QString lineInput = stream.readLine();
            do{
                qDebug() << lineInput;
                istringstream reviewInputs(lineInput.toStdString());
                string comment;
                getline(reviewInputs, comment, '~');
                Date datePosted;
                string date;
                getline(reviewInputs,date);
                istringstream dateStream(date);
                string day;
                getline(dateStream, day , '-');
                datePosted.day=stoi(day);
                string month;
                getline(dateStream, month, '-');
                datePosted.month=stoi(month);
                string year;
                getline(dateStream,  year);
                datePosted.year=stoi(year);
                r.comments.push_back(make_pair(comment,datePosted));
                lineInput=stream.readLine();
                qDebug() << lineInput;
            }while(!lineInput.isEmpty());
            news[category][newsId].reviews[userName]=r;
        }
        else{
            news[category][newsId].reviews[userName]=r;
        }
    }
    review.close();
}

void NewsManagementSystem:: writeNewsReviewInFiles(QString NewsReview ){
    QFile reviewFile(NewsReview);
    if (!reviewFile.open(QIODevice::WriteOnly)){
        qDebug() <<"Faild to open file.";
        qDebug()<<"no thing open";
        return;
    }
    QTextStream stream(&reviewFile);
    for(auto it1=news.begin();it1!=news.end();it1++){
        for(auto it2=it1->second.begin();it2!=it1->second.end();it2++){
            if(it2->second.reviews.size()!=0){
                for(auto it3=it2->second.reviews.begin();it3!=it2->second.reviews.end();it3++){
                    stream<<QString::fromStdString(it2->second.category)<<';'<<QString::number(it2->second.id)<<';'<<QString::fromStdString(it3->first)<<';'<<QString::number(it3->second.rating)<<':';
                    if(it3->second.comments.size()!=0){
                        stream<<'\n';
                        for(auto v=it3->second.comments.begin();v!=it3->second.comments.end();v++){
                            stream<<QString::fromStdString(v->first)<<'~'<<QString::number(v->second.day)<<'-'<<QString::number(v->second.month)<<'-'<<QString::number(v->second.year)<<'\n';
                        }
                        stream<<'\n';
                    }
                    else
                        stream <<"No comments yet."<<'\n'<<'\n';
                }
            }
        }
    }
    reviewFile.close();
}
void NewsManagementSystem::writeUserInfoInFils(QString userInfo){
    QFile userData(userInfo);
    if (!userData.open(QIODevice::WriteOnly)){
        qDebug() <<"Faild to open file.";
        qDebug()<<"no thing open";
        return;
    }
    QTextStream stream(&userData);
    for(auto it1=users.begin();it1!=users.end();it1++){
        stream << QString::number(it1->second.get_id())<<'~'<<QString::fromStdString(it1->second.get_name())<<'~'<<QString::fromStdString(it1->first)<<'~'<<QString::fromStdString(it1->second.get_email())<<'~'<<QString::fromStdString(it1->second.get_password());
        if(!it1->second.getFvourite().empty()&&it1->second.getFvourite().size()!=1){
            stream<<'~';
            int x=0;
            for(auto listItem:it1->second.getFvourite()){
                if(x!=1){
                stream<<QString::fromStdString(listItem.first)<<','<<QString::number(listItem.second)<<';';
                //qDebug()<<"category"<<listItem.first<<"id"<<listItem.second;
                }
                else
                  stream<<QString::fromStdString(listItem.first)<<','<<QString::number(listItem.second);
                x--;
            }
        }
        else if(it1->second.getFvourite().size()==1){
            stream<<'~';
            stream<<QString::fromStdString(it1->second.getFvourite().front().first)<<','<<QString::number(it1->second.getFvourite().front().second);

        }
        stream<<'\n';
    }
    stream<<'\n';
    userData.close();
}

void NewsManagementSystem:: readUserFromFile(QString fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
        return;
    QTextStream stream(&file);
    list<pair<string,int>>favourite;
    while(!stream.atEnd()) {
        QString line = stream.readLine();
        istringstream iss(line.toStdString());
        if (line.isEmpty()) // Check for empty lines
            break;
        User user;

        string idStr;
        getline(iss, idStr, '~');
        user.id = stoi(idStr);
        getline(iss,user.name , '~');
        getline(iss, user.user_name, '~');
        getline(iss, user.email, '~');
        getline(iss, user.password, '~');
        string list1;
        if(getline(iss,list1) && (!list1.empty())){
            //QString line2 = list.readLine();
            istringstream list(list1);
            pair<string,int> pairFav;
            string favlist;
            while(getline(list,favlist,';')&& (!favlist.empty())){
                string favCategory;
                string favNews;
                istringstream issFav(favlist);
                getline(issFav, favCategory, ',');
                getline(issFav, favNews, ',');
                pairFav.first=favCategory;
                pairFav.second=stoi(favNews);
                favourite.push_back(pairFav);
            }

            user.favouriteList=favourite;

        }
        users[user.user_name]=user;
    }
    file.close();
    User::USER_ID_COUNT=users.size()+1;
}
void NewsManagementSystem:: readAdminFromFile(string fileName) {
    QString fileNem =QString::fromStdString(fileName);
    QFile file(fileNem);
    if (!file.open(QIODevice::ReadOnly))
        return;
    QTextStream stream(&file);

    while(!stream.atEnd()) {
        QString line = stream.readLine();

        istringstream iss(line.toStdString());
        if (line.isEmpty()) // Check for empty lines
            break;
        Admin admin;

        string idStr;
        getline(iss, idStr, '~');
        admin.id = stoi(idStr);
        getline(iss,admin.name , '~');
        getline(iss, admin.user_name, '~');
        getline(iss, admin.email, '~');
        getline(iss, admin.password, '~');
        getline(iss, admin.secret_code);



        admins[admin.user_name]=admin;
        Admin::ADMIN_ID_COUNT++;
    }
    file.close();
}
void NewsManagementSystem::writeAdminToFile() {
    QFile file("AdminData.txt");
    if (!file.open(QIODevice:: WriteOnly | QIODevice::Text)) {
        qDebug() << "Could not open file for writing";
        return;
    }
    //file.resize(0);
    QTextStream out (&file);
    unordered_map<string,Admin> ::iterator it;
    for (it=admins.begin();it!=admins.end() ;it++){
        //Admin* currentAdmin= NewsManagementSystem::getCurrentAdmin();
        QString adminId =QString::number(it->second.get_id());
        QString adminName =QString::fromStdString(it->second.get_name());
        QString adminUserName =QString::fromStdString(it->second.get_user_name());
        QString adminEmail =QString::fromStdString(it->second.get_email());
        QString adminPassword =QString::fromStdString(it->second.get_password());
        QString adminSecretKey =QString::fromStdString(it->second.secret_code);
        //QString adminscount =QString::number(admins.size());
        out << adminId << "~" << adminName << "~" << adminUserName
            << "~" << adminEmail << "~" <<  adminPassword
            << "~"<<  adminSecretKey <<"\n";
    }
    file.close();
        //<< "adminSecretKey"
}
