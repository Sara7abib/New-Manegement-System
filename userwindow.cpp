#include "userwindow.h"
#include "ui_userwindow.h"
#include<vector>
#include<unordered_map>
#include<QLabel>
#include<QPushButton>
#include<QString>
#include<QGridLayout>
#include<QScrollArea>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QWidget>
#include<QListWidget>
#include<QListWidgetItem>
#include<QSettings>
#include<stack>
#include<QMessageBox>
#include <QTimer>
#include <memory>
#include"account.h"
#include"newsmanagementsystem.h"
#include <QTextEdit>
#include "user.h"
#include "adminwindow.h"
#include <QDebug>
#include "mainwindow.h"

UserWindow::UserWindow(QWidget *parent)
    : QMainWindow(parent),newsColumns(1),categoryColumns(3)
    , ui(new Ui::UserWindow)
{
    ui->setupUi(this);
    QTimer::singleShot(0, this, &UserWindow::on_Home_clicked);




    // Connect to a QTimer::singleShot() with a timeout of 0 milliseconds
    QTimer::singleShot(0, [this]() {
        sendMessageUpdates(NewsManagementSystem::currentUser);
    });




}

UserWindow::~UserWindow()
{
    delete ui;
}



void UserWindow::on_Home_clicked(){ // <<<<<<
    ui->stackedWidget->setCurrentIndex(0);
    QWidget *firstWidget = ui->stackedWidget->widget(0);
    stack<pair<string,int>>latestStack=NewsManagementSystem::currentUser->showLatestNews();
    //qDebug() << QString::fromStdString(latestStack.top().first) ;
    setupNewsDisplay(firstWidget,latestStack);
}
void UserWindow::displayCategory(string category)
{
    ui->stackedWidget->setCurrentIndex(1);
    QWidget* secondWidget = ui->stackedWidget->widget(1);

    qDebug() << "HERE Clicked on Category";
    // tlama awl mra ydos 3la category fa hoa mshafsh wla news gwa category dh
    auto it_category_found = NewsManagementSystem::currentUser->subscribedCategories.find(category);
    if(it_category_found == NewsManagementSystem::currentUser->subscribedCategories.end()){
        NewsManagementSystem::currentUser->subscribedCategories[category] = make_pair(false,-1);
    }



    stack<pair<string,int>>newsStack=NewsManagementSystem::currentUser->displayNewsByCategory(category);
    setupNewsDisplay(secondWidget,newsStack);
}
void UserWindow::showNews(News &n, QGridLayout *layout, int index) {
    QWidget *newsWidget = new QWidget();
    QVBoxLayout *newsLayout = new QVBoxLayout(newsWidget);
    QLabel *Title = new QLabel(QString::fromStdString(n.title));
    QLabel *Image= new QLabel();
    QPixmap pix(QString::fromStdString(n.image_url));

    QString date = QString::number(n.date.day) + "/" + QString::number(n.date.month) + "/"  + QString::number(n.date.year) ;
    QLabel *dateLabel = new QLabel(date);
    QPushButton *detailsButton = new QPushButton("More Details");
    detailsButton->setStyleSheet("font-size:22px;color:white;background-color:#0077B6;border-radius: 10px;");
    newsLayout->addWidget(Title);
    newsLayout->addWidget(Image);
    int w = Image->width();
    int h = Image->height();
    Image->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
    newsLayout->addWidget(dateLabel);
    newsLayout->addWidget(detailsButton);
    newsWidget->setLayout(newsLayout);

    int row = index / newsColumns;
    int col = index % newsColumns;
    layout->addWidget(newsWidget, row, col);

    connect(detailsButton, &QPushButton::clicked, [&]() {
        handleDetailsButtonClick(n);
    });
}

void UserWindow::setupNewsDisplay(QWidget* widget, std::stack<std::pair<std::string, int>> newsStack) { // <<<<<
    QLayout* existingLayout = widget->layout();
    clearLayout(existingLayout);
    QVBoxLayout* layout = new QVBoxLayout(widget);
    QScrollArea* scrollArea = new QScrollArea(widget);
    QWidget* containerWidget = new QWidget(scrollArea);
    QGridLayout* gridLayout = new QGridLayout(containerWidget);
    if(!newsStack.empty()){
        stack<pair<string,int>>stackCopy=newsStack;
        int maxSize=stackCopy.size();
        qDebug() <<maxSize;
        for (int index = 0; index < maxSize; ++index) {
            pair<string, int>& topElement = stackCopy.top();
            string category = topElement.first;
            int id = topElement.second;
            stackCopy.pop();
            //qDebug() << QString::fromStdString(category) <<" "<< id;
            showNews(NewsManagementSystem::news[category][id], gridLayout, index);
            qDebug() <<NewsManagementSystem::news[category][id].date.year ;

        }
    }
    else{
        QWidget *newsWedig = new QWidget();
        QVBoxLayout *newsLayout = new QVBoxLayout(newsWedig);
        QLabel *notFound = new QLabel("There is no news to display.");
        notFound->setStyleSheet("QLabel {font-size: 50px; text-align: center; }");
        newsLayout->addWidget(notFound);
        newsWedig->setLayout(newsLayout);
        gridLayout->addWidget(newsWedig);
    }
    scrollArea->setWidget(containerWidget);
    scrollArea->setWidgetResizable(true);
    layout->addWidget(scrollArea);
    widget->setLayout(layout);
}
void UserWindow::clearLayout(QLayout* layout) {
    if (layout) {
        QLayoutItem* item;
        while ((item = layout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete layout; // Delete the existing layout
    }

}




void UserWindow::handleDetailsButtonClick(News &n) {
    ui->stackedWidget->setCurrentIndex(3);
    QWidget *fourthWidget = ui->stackedWidget->widget(3);
    QLayout* existingLayout = fourthWidget->layout();
    clearLayout( existingLayout);
    QVBoxLayout *layout = new QVBoxLayout(fourthWidget);
    QScrollArea *scrollArea = new QScrollArea(fourthWidget);
    QWidget *containerWidget = new QWidget(scrollArea);
    QGridLayout *gridLayout = new QGridLayout(containerWidget);

    qDebug() << "HERE" ;

    showOneNew(n, gridLayout);
    scrollArea->setWidget(containerWidget);
    scrollArea->setWidgetResizable(true);
    layout->addWidget(scrollArea);
    fourthWidget->setLayout(layout);

}
void UserWindow::showOneNew(News &n, QGridLayout *layout) { // <<<<<<<<<
    QWidget *newsWidget = new QWidget();
    QVBoxLayout *newsLayout = new QVBoxLayout(newsWidget);
    QLabel *Title = new QLabel(QString::fromStdString(n.title));
    QLabel *description = new QLabel(QString::fromStdString(n.description));
    description->setWordWrap(true);
    QLabel *Image= new QLabel();
    QPixmap pix(QString::fromStdString(n.image_url));
    int w = Image->width();
    int h = Image->height();
    Image->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
    QString date = QString::number(n.date.day) + "/" + QString::number(n.date.month) + "/"  + QString::number(n.date.year) ;
    QLabel *dateLabel = new QLabel(date);
    QWidget *rateWidget=new QWidget(newsWidget);
    QLabel *labelRate = new QLabel("Rate:",rateWidget);
    labelRate->move(0,35);
    QLabel *rate = new QLabel(QString::number(n.rate),rateWidget);
    rate->move(30,35);
    QLabel *rateEnter = new QLabel("Your Rate");
    rateEnter->setFixedSize(QSize(50,50));
    QPushButton *pushrate =new QPushButton("Enter the rate");
    QLineEdit *lineEdit=new QLineEdit();
    QWidget *inner=new QWidget(newsWidget);
    QHBoxLayout *innerWidget= new QHBoxLayout();
    favourite= new QPushButton();
    innerWidget->addWidget(rateWidget);
    innerWidget->addWidget(favourite);
    inner->setLayout(innerWidget);
    favourite->setIcon(QIcon(":/image/Empty.png"));
    favourite->setIconSize(QSize(30,30));
    favourite->setCheckable(true);
    favourite->setFixedSize(50,50);
    favourite->setStyleSheet("border: none;");
    pair<string,int>currunt;
    currunt.first=n.category;
    currunt.second=n.id;
    bool found=false;
    if(!NewsManagementSystem::currentUser->getFvourite().empty()){
        for(auto listItem:NewsManagementSystem::currentUser->getFvourite()){
            if(currunt==listItem){
                favourite->setChecked(true);
                favourite->setIcon(QIcon(":/image/Filled.png"));
                found = true;
                break;
            }
        }
        if (!found) {
            favourite->setChecked(false);
            favourite->setIcon(QIcon(":/image/Empty.png"));        }
    }
    else{
        favourite->setChecked(false);
        favourite->setIcon(QIcon(":/image/Empty.png"));
    }

    // report as spam section
    QWidget *noSpammedUsersWidget=new QWidget(newsWidget);
    QLabel *spamLabel = new QLabel("Number of users reported it as spam: ",noSpammedUsersWidget);
    spamLabel->move(0,30);
    QLabel *noSpammedUsers = new QLabel(QString::number(n.noSpammedUsers()),noSpammedUsersWidget);
    noSpammedUsers->move(200,30);
    QWidget *spamWidget=new QWidget(newsWidget);
    QHBoxLayout *spamLayout = new QHBoxLayout();
    QPushButton *reportSpam = new QPushButton("Report as Spam", newsWidget);
    reportSpam->setFixedSize(QSize(150, 50));
    reportSpam->setStyleSheet("font-weight:bold;font-size:18px;color:white;background-color:#0077B6;border-radius: 10px;");
    spamLayout->addWidget(noSpammedUsersWidget);
    spamLayout->addWidget(reportSpam);
    spamWidget->setLayout(spamLayout);
    QLabel *write_comment_Label = new QLabel("Write a comment");
    write_comment_Label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    write_comment_Label->setStyleSheet("font-weight: bold;font-size:20px;");
    QWidget *writecomment = new QWidget(newsWidget);
    QVBoxLayout *writecomment_section = new QVBoxLayout(writecomment);
    writecomment->setLayout(writecomment_section);

    // Create a QTextEdit widget that user will write comment in it
    QTextEdit *textEdit = new QTextEdit(writecomment);
    // properties
    textEdit->setReadOnly(false);
    textEdit->setPlaceholderText("Enter your comment here...");






    QPushButton *submit_comment = new QPushButton("Submit",writecomment); // its slot function downthere
    submit_comment->setStyleSheet("font-weight:bold;font-size:22px;color:white;background-color:#03045E;");
    writecomment_section->addWidget(textEdit);
    writecomment_section->addWidget(submit_comment);







    // View all comments
    QWidget *viewcomment = new QWidget(newsWidget);
    QVBoxLayout *viewecomment_section = new QVBoxLayout(viewcomment);
    viewcomment->setLayout(viewecomment_section);

    for(auto review : n.reviews){ // .first is id of user , .second is object of review
        for (auto comment: review.second.comments){ // loop through vector of pair<string comment,date of comment> for each user id found in ordered map -
            QWidget *each_comment = new QWidget(viewcomment);
            QGridLayout *eachcomment_section = new QGridLayout(each_comment);
            each_comment->setLayout(eachcomment_section);

            QLabel *user_info = new QLabel(each_comment);
            auto user_data = NewsManagementSystem::users[review.first];
            QString user_text = QString::fromStdString(user_data.get_name()) + "  " + QString::number(user_data.get_id());
            user_info->setText(user_text);
            user_info->setStyleSheet("color: #0D1282");

            QLabel *datePosted = new QLabel(each_comment);
            QString comment_Date = QString::number(comment.second.day) + "/" + QString::number(comment.second.month) + "/" + QString::number(comment.second.year);
            datePosted->setText(comment_Date);
            datePosted->setStyleSheet("color: #0D1282");

            QLabel *user_comment = new QLabel(each_comment);
            user_comment->setText(QString::fromStdString(comment.first));
            user_comment->setWordWrap(true);


            eachcomment_section->addWidget(user_info,0 ,0);
            eachcomment_section->addWidget(datePosted,0 ,1);
            eachcomment_section->addWidget(user_comment,1 ,0,1,2);


            each_comment->setStyleSheet("background-color: #F5F5F5;font-size:15px;font-weight:bold;border-radius:5px;");

            viewecomment_section->addWidget(each_comment);
        }


    }



    connect(submit_comment, &QPushButton::clicked, [=,&n]() { // must send news by reference to be updated or I must use NewsManagementSystem::news[][] in everything, like in getting user iterator to check if there is review object before or not
        QString comment_text = textEdit->toPlainText();
        if(comment_text.isEmpty()){
            QMessageBox::critical(this,"Empty Input","You Can't submit an empty comment");
            return;
        }



        //first we must check if this user made comment before in that news or not , so we will search by key
        //if it is found so we push back in vector of comments, if not we must create Review object and assign it as the value of user id key in unordered map of reviews

        auto it_userkey = n.reviews.find(NewsManagementSystem::currentUser->get_user_name());
        if(it_userkey != n.reviews.end()){ // this user is found as key in map so it must have already review object,so just push back comment in it
            NewsManagementSystem::news[n.category][n.id].reviews[NewsManagementSystem::currentUser->get_user_name()].comments.push_back(make_pair(comment_text.toStdString(),Date::currentDate()));// instead of sending n as reference we change in in the unordered map
        }
        else{ // that user id not found as key in unordered map so it doesn't have Review object so we must create it
            Review review(comment_text.toStdString(),Date::currentDate());
            NewsManagementSystem::news[n.category][n.id].reviews[NewsManagementSystem::currentUser->get_user_name()] = review; // reviews unordered map takes user id as key and Review object as value
        }
        QMessageBox::information(this,"You added a comment successfully","Succeded");
        textEdit->clear();


        // The problem is how to make this new comment appear in same page in ui without refreshing it
        QWidget *each_comment = new QWidget(viewcomment);
        QGridLayout *eachcomment_section = new QGridLayout(each_comment);
        each_comment->setLayout(eachcomment_section);

        QLabel *user_info = new QLabel(each_comment);
        auto user_data = NewsManagementSystem::users[NewsManagementSystem::currentUser->get_user_name()];
        QString user_text = QString::fromStdString(user_data.get_name()) + "  " + QString::number(user_data.get_id());
        user_info->setText(user_text);
        user_info->setStyleSheet("color: #0D1282");

        QLabel *datePosted = new QLabel(each_comment);
        QString comment_Date = QString::number(Date::currentDate().day) + "/" + QString::number(Date::currentDate().month) + "/" + QString::number(Date::currentDate().year);
        datePosted->setText(comment_Date);
        datePosted->setStyleSheet("color: #0D1282");

        QLabel *user_comment = new QLabel(each_comment);
        user_comment->setText(comment_text);
        user_comment->setWordWrap(true);

        eachcomment_section->addWidget(user_info,0 ,0);
        eachcomment_section->addWidget(datePosted,0 ,1);
        eachcomment_section->addWidget(user_comment,1 ,0,1,2);

        // background color code : #FEFDED
        //#FEFAF6
        each_comment->setStyleSheet("background-color: #F5F5F5;font-size:15px;font-weight:bold;border-radius:5px;");

        viewecomment_section->addWidget(each_comment);



    });



    newsLayout->addWidget(Title);
    newsLayout->addWidget(Image);
    newsLayout->addWidget(description);
    newsLayout->addWidget(dateLabel);
    newsLayout->addWidget(inner);
    newsLayout->addWidget(spamWidget);
    newsLayout->addWidget(rateEnter);
    newsLayout->addWidget(lineEdit);
    newsLayout->addWidget(pushrate);

    // view write comment section
    newsLayout->addWidget(write_comment_Label);
    newsLayout->addWidget(writecomment);
    // view all comments section
    newsLayout->addWidget(viewcomment);

    newsWidget->setLayout(newsLayout);
    layout->addWidget(newsWidget);


    connect(pushrate, &QPushButton::clicked, [=,&n]() {
        QString text = lineEdit->text(); // Get the text from lineEdit
        int value = text.toInt(); // Convert text to integer
        qDebug() << "Showing rate" <<n.rate<<" ";
        lrate(value, n);
        qDebug() << "Showing rate after" <<n.rate<<" ";
        rate->setText(QString::number(n.rate));
        lineEdit->clear();
    });





    connect(favourite, &QPushButton::clicked, this, [this,n]() {
        toggleFavorite(n);
    });


    connect(reportSpam, &QPushButton::clicked, [=,&n]() {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Report as Spam","Are you sure you want to report this as spam?", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            NewsManagementSystem::currentUser->reportAsSpam(n);
            // for (auto it =n.spammedUsers.begin(); it != n.spammedUsers.end(); ++it) {
            //     // Access the element
            //     qDebug() << *it << " "<<'/';
            // }

            if(!NewsManagementSystem::currentUser->getFvourite().empty()){
                for(auto listItem:NewsManagementSystem::currentUser->getFvourite()){
                    if(currunt==listItem){
                        NewsManagementSystem::currentUser->bookMarkingNews(currunt.second,currunt.first,false);
                    }
                }
            }
            noSpammedUsers->setText(QString::number(n.noSpammedUsers()));
            QMessageBox::information(this, "Reported", "The news article has been reported as spam.");
            noSpammedUsers->setText(QString::number(n.noSpammedUsers()));
            qDebug() << "Reported as Spam";

            on_Home_clicked();
        }
    });
}
void UserWindow::toggleFavorite(News n) {
    bool addOrDelete=true;
    bool isFavorite = favourite->isChecked();
    if (isFavorite) {
        favourite->setChecked(true);
        favourite->setIcon(QIcon(":/image/Filled.png"));
    } else {
        favourite->setChecked(false);
        favourite->setIcon(QIcon(":/image/Empty.png"));
        addOrDelete=false;
    }
    NewsManagementSystem::currentUser->bookMarkingNews(n.id,n.category,addOrDelete);
}

void UserWindow::on_search_clicked()
{
    QString word = ui->lineEdit->text();
    vector<pair<string,int>> searchResult;
    if(ui->Title->isChecked()){
        searchResult= NewsManagementSystem::currentUser->searchByTitle(word);
        searchDisplay(searchResult);
    }
    if(ui->Date->isChecked()){
        QDate date = QDate::fromString(word, "yyyy-MM-dd");
        searchResult= NewsManagementSystem::currentUser->searchByDate(date);
        searchDisplay(searchResult);
    }
    if(ui->keyword->isChecked()){
        searchResult= NewsManagementSystem::currentUser->searchByKeyWord(word);
        searchDisplay(searchResult);
    }
}
void UserWindow::searchDisplay(vector<pair<string,int>> searchResult)
{

    ui->stackedWidget->setCurrentIndex(2);
    QWidget *thirdWidget = ui->stackedWidget->widget(2);
    QLayout *existingLayout = thirdWidget->layout();
    clearLayout(existingLayout);
    QVBoxLayout *layout = new QVBoxLayout(thirdWidget);
    QScrollArea *scrollArea = new QScrollArea(thirdWidget);
    QWidget *containerWidget = new QWidget(scrollArea);
    QGridLayout *gridLayout = new QGridLayout(containerWidget);
    if(!searchResult.empty()){
        for (int i = 0; i <searchResult.size(); ++i) {
            showNews(NewsManagementSystem::news[searchResult[i].first][searchResult[i].second], gridLayout, i);

        }
    }
    else{
        notFound(  gridLayout);
    }
    scrollArea->setWidget(containerWidget);
    scrollArea->setWidgetResizable(true);
    layout->addWidget(scrollArea);
    thirdWidget->setLayout(layout);
    ui->lineEdit->setFocus();
}

void UserWindow::notFound(QGridLayout * layout) {
    QWidget *newsWidget = new QWidget();
    QVBoxLayout *newsLayout = new QVBoxLayout(newsWidget);
    QLabel *notFoundLabel = new QLabel("Not Found");
    notFoundLabel->setStyleSheet("QLabel {font-size: 100px; text-align: center; }");
    newsLayout->addWidget(notFoundLabel);
    newsWidget->setLayout(newsLayout);
    layout->addWidget(newsWidget);
}

void UserWindow::on_Categories_clicked(){
    ui->stackedWidget->setCurrentIndex(1);
    categoryWidget();
}

void UserWindow::categoryWidget(){
    ui->stackedWidget->setCurrentIndex(1);
    QWidget* TheirdWidget = ui->stackedWidget->widget(1);
    QLayout* existingLayout = TheirdWidget->layout();
    clearLayout(existingLayout);
    QVBoxLayout* layout = new QVBoxLayout(TheirdWidget);
    QScrollArea* scrollArea = new QScrollArea(TheirdWidget);
    QWidget* containerWidget = new QWidget(scrollArea);
    QGridLayout* gridLayout = new QGridLayout(containerWidget);
    int index=0;
    for (auto it=NewsManagementSystem::categories.begin();it!=NewsManagementSystem::categories.end();++it) {
        QString category_url= QString::fromStdString(it->second);
        showCategory(it->first, gridLayout, index,category_url);
        index++;
    }
    scrollArea->setWidget(containerWidget);
    scrollArea->setWidgetResizable(true);
    layout->addWidget(scrollArea);
    TheirdWidget->setLayout(layout);
}


void UserWindow::showCategory(string category, QGridLayout *layout, int index,QString category_url) {
    //QString qCategory=QString::fromStdString(category);
    QWidget *categoryWidget = new QWidget();
    QVBoxLayout *newsLayout = new QVBoxLayout(categoryWidget); // <<<<<<<<<<<<
    QPushButton *subscribe = new QPushButton(categoryWidget);



    bool subscribed_category;
    auto category_found = NewsManagementSystem::currentUser->subscribedCategories.find(category);
    if(category_found == NewsManagementSystem::currentUser->subscribedCategories.end()){ // category not found in the keys of unordered map -- assign it directly without comparison this news
        NewsManagementSystem::currentUser->subscribedCategories[category] = make_pair(false,-1);
        subscribed_category = false;
    }else{

        subscribed_category = NewsManagementSystem::currentUser->subscribedCategories[category].first;
    }


    // Trues mean subscribed
    // False means not subscribe
    if(subscribed_category){
        subscribe->setStyleSheet("background-color: #808080 ;color : white;font-weight: bold;font-size:25px;");
        subscribe->setText("UnSubscribe");
    }else{
        subscribe->setStyleSheet("background-color: #FF0000 ;color : white;font-weight: bold;font-size:25px;");
        subscribe->setText("Subscribe");
    }



    //subscribe->setFixedSize(100,100);
    connect(subscribe, &QPushButton::clicked, [=]() mutable{
        if(subscribed_category){ // if he already subscribed make him not subscribed
            NewsManagementSystem::currentUser->subscribedCategories[category].first = false;
            subscribed_category = false;
            subscribe->setStyleSheet("background-color: #FF0000 ;color : white;font-weight: bold;font-size:25px;");
            subscribe->setText("Subscribe");
            qDebug() << QString::number(NewsManagementSystem::currentUser->subscribedCategories[category].first);
        }else{
            NewsManagementSystem::currentUser->subscribedCategories[category].first = true;
            subscribed_category = true;
            subscribe->setStyleSheet("background-color: #808080 ;color : white;font-weight: bold;font-size:25px;");
            subscribe->setText("UnSubscribe");
            qDebug() << QString::number(NewsManagementSystem::currentUser->subscribedCategories[category].first);
        }
    });


    QPushButton *buttonCategory = new QPushButton(/*qCategory*/);
    QPixmap pix(category_url);
    buttonCategory->setFixedSize(300, 300); // Set the button's fixed size first
    buttonCategory->setIcon(QIcon(pix));
    buttonCategory->setIconSize(buttonCategory->size()); // Set the icon size to match the button's size
    buttonCategory->setFlat(true);
    newsLayout->addWidget(buttonCategory);
    newsLayout->addWidget(subscribe);

    categoryWidget->setLayout(newsLayout);
    int row = index / categoryColumns;
    int col = index % categoryColumns;
    layout->addWidget(categoryWidget, row, col);
    connect(buttonCategory, &QPushButton::clicked, this, [this, category]() {
        displayCategory(category);
    });
}



void UserWindow::lrate(int user_rate,News& n)
{
    if (user_rate > 5 || user_rate < 0) {
        QMessageBox* box = new QMessageBox(QMessageBox::Warning, "Warning", "The Rate Must Be Between 0 to 5", QMessageBox::Ok, this);
        box->exec();
    }
    else if(user_rate==NULL){
        QMessageBox* box = new QMessageBox(QMessageBox::Warning, "Warning", "The Inpute Is Empty ,Enter the rate first", QMessageBox::Ok, this);
        box->exec();
    }

    else{
        n.pushrate(user_rate ,NewsManagementSystem::currentUser->get_user_name()) ;
        QMessageBox msgBox;
        msgBox.setText("The Rate has been changed.");
        msgBox.exec();
    }
}
void UserWindow::on_favourote_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    QWidget* thirdWidget = ui->stackedWidget->widget(2);
    QLayout* existingLayout = thirdWidget->layout();
    clearLayout(existingLayout);
    QVBoxLayout* layout = new QVBoxLayout(thirdWidget);
    QScrollArea* scrollArea = new QScrollArea(thirdWidget);
    QWidget* containerWidget = new QWidget(scrollArea);
    QGridLayout* gridLayout = new QGridLayout(containerWidget);
    int index=0;
    auto favouriteList = NewsManagementSystem::currentUser->getFvourite();
    if(!favouriteList.empty()){
        for (auto it = favouriteList.begin(); it != favouriteList.end(); ++it) {
            //if(!NewsManagementSystem::news[it->first][it->second].foundSpammedUser(u.get_id())){
            showNews(NewsManagementSystem::news[it->first][it->second], gridLayout, index);
            index++;
        }
    }
    else{
        QWidget *favouriteWedig = new QWidget();
        QVBoxLayout *favouriteLayout = new QVBoxLayout(favouriteWedig);
        QLabel *notFavourite = new QLabel("No favourite news yet.");
        notFavourite->setStyleSheet("QLabel {font-size: 50px; text-align: center; }");
        favouriteLayout->addWidget(notFavourite);
        favouriteWedig->setLayout(favouriteLayout);
        gridLayout->addWidget(favouriteWedig);
    }
    scrollArea->setWidget(containerWidget);
    scrollArea->setWidgetResizable(true);
    layout->addWidget(scrollArea);
    thirdWidget->setLayout(layout);
}


void UserWindow::on_Trending_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    QWidget* secondWidget = ui->stackedWidget->widget(1);
    stack<pair<string,int>>newsStack=NewsManagementSystem::currentUser->displayTrendingNews();
    setupNewsDisplay(secondWidget,newsStack);
    //categoryWidget();
}


void UserWindow::sendMessageUpdates(User *currentUser){
    for(auto category : currentUser->subscribedCategories){
        if(category.second.first && category.second.second != -1){ // true , news already viewed before
            Date dateOfNewsStored = NewsManagementSystem::news[category.first][category.second.second].date;
            Date latestNewsInCategory = News::LatestNews(category.first); // name of each category he subscribed
            if(dateOfNewsStored.toQDate() < latestNewsInCategory.toQDate()){
                QString text = QString::fromStdString(category.first) + " has news not checked";
                QMessageBox::information(this,"Update",text);
            }
        }else if(category.second.first && category.second.second == -1){ // true, -1 (no news viewed before but he subscribed to this category
            QString text = QString::fromStdString(category.first) + " has news not checked";
            QMessageBox::information(this,"Update",text);
        }


    }

}

void UserWindow::on_logout_btn_clicked()
{
    hide();
    NewsManagementSystem::currentUser = NULL;
    MainWindow *mainwindow = new MainWindow();
    mainwindow->show();
}

void UserWindow::closeEvent(QCloseEvent *event)  {
    NewsManagementSystem:: writeCategoriesInFiles("CategoriesData.txt");
    NewsManagementSystem:: writeNewsDataInFiles("NewsData.txt");
    NewsManagementSystem:: writeNewsReviewInFiles("NewsReview.txt");
    NewsManagementSystem::writeUserInfoInFils("UserData.txt");
    NewsManagementSystem::writeAdminToFile();
    QMainWindow::closeEvent(event);
}
