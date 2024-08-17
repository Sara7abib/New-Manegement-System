#include "adminwindow.h"
#include "ui_adminwindow.h"
#include <QFileDialog>
#include "news.h"
#include "newsmanagementsystem.h"
#include "admin.h"
#include <QMessageBox>
#include <QLayout>
#include "userwindow.h"
#include <QTableWidget>
#include <QTableWidgetItem>

#include <iostream>
#include <thread>
#include <chrono>
#include <QTimer>
#include <QEventLoop>
#include "mainwindow.h"

using namespace std;

AdminWindow::AdminWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AdminWindow)
{
    ui->setupUi(this);
    current_index = 0;
    //ui->pages->setCurrentIndex(1);
    ui->pages->setCurrentIndex(0);
    viewDashboard();
    //Add Page
    ui->addnewcategory->hide();
    ui->addCategory_input->hide();
    ui->uploadCategoryPhoto_addpage->hide();
    // Modify Page
    ui->addCategorymodified_input->hide();
    ui->addnewcategory_modify->hide();
    ui->uploadCatPhoto_modifypage->hide();

    ui->welcome_back->setText("Welcome Back, " + QString::fromStdString(NewsManagementSystem::currentAdmin->get_name()));

    // Set up a QTimer to update news periodically
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout,[=](){
        if(all_news.empty()){
            return;
        }
        setNewsData();
        current_index = (current_index+1)% all_news.size();
    });

    timer->start(5000); // Update news every 10 seconds


}



void AdminWindow::setNewsData(){
    if(all_news.empty() || all_news.size() == 0){
        current_index = 0;
        ui->newsidlabel_home->setText("");
        ui->newsname_home->setText("");
        //QString news_date = QString::number(news.date.day) + "/" + QString::number(news.date.month) + "/" + QString::number(news.date.year);
        ui->dateposted_home->setText("");
        ui->categoryname_home->setText("");
        ui->imglabel_home->setPixmap(QPixmap(""));
        ui->imglabel_home->setFixedSize(220,220);
        return;
    }
    News news = NewsManagementSystem::news[all_news[current_index].first][all_news[current_index].second];
    ui->newsidlabel_home->setText(QString::number(news.getNewsId()));
    ui->newsname_home->setText(QString::fromStdString(news.title));
    QString news_date = QString::number(news.date.day) + "/" + QString::number(news.date.month) + "/" + QString::number(news.date.year);
    ui->dateposted_home->setText(news_date);
    ui->categoryname_home->setText(QString::fromStdString(news.category));
    ui->imglabel_home->setPixmap(QPixmap(QString::fromStdString(news.image_url)));
    ui->imglabel_home->setFixedSize(220,220);
}

AdminWindow::~AdminWindow()
{
    delete ui;
}



void AdminWindow::on_addnews_button_clicked()
{
    QString title = ui->newstitle_input->text();
    QString category = ui->newscategory_combo->currentText();
    QString description = ui->Description_textarea->toPlainText();
    QString newsphoto = newsphotourl;

    if(title.isEmpty()|| description.isEmpty() || newsphoto.isEmpty()){
        QMessageBox::critical(this,"Empty Input","You must fill all inputs");
        return;
    }

    if(category == "others"){
        QMessageBox::critical(this,"choose another category","You can't choose others as category");
        return;
    }
    News news(title.toStdString(),description.toStdString(),category.toStdString(),newsphoto.toStdString(),Date::currentDate(),NewsTime::getCurrentTime());
    // pointer that carries the address of the current admin object
    //qDebug() << NewsManagementSystem::getCurrentAdmin()
    //qDebug() << NewsManagementSystem::getCurrentAdmin()->secret_code;
    NewsManagementSystem::getCurrentAdmin()->addNews(category.toStdString(),news);
    // you should create another temp veriable here that will store the value after pressing the button of uploading photo or put the variable in the header file in the class

    // Messagebox and move(navigate) to another page
    QMessageBox::information(this,"Success","You Posted news Successfully");
    all_news = NewsManagementSystem::MapToVector();
    current_index=current_index%all_news.size();
    ui->no_categories->setText(QString::number(NewsManagementSystem::noCategories()));
    ui->no_news->setText(QString::number(NewsManagementSystem::noNews()));

    //Erase all inputs
    ui->newstitle_input->clear();
    ui->Description_textarea->clear();
    ui->newscategory_combo->setCurrentIndex(0);
    ui->label_uploadphotoimage->setPixmap(QPixmap());
    newsphoto=NULL;
    newsphotourl = NULL;

    ui->dashboard_btn->setStyleSheet("background-color: grey;");
    ui->addnews_btn->setStyleSheet("background-color: blue;");
    ui->viewallnews_btn->setStyleSheet("background-color: blue;");
    ui->pages->setCurrentIndex(0);


}


void AdminWindow::on_uploadphoto_clicked()
{
    // member in the class in header file (like temp var), You could make it Global var
    newsphotourl=QFileDialog::getOpenFileName(this,tr("Open File"),"C://","JPG IMAGE (*.jpg);; JPEG IMAGE (*.jpeg);; PNG IMAGE (*.png)");
    if(!newsphotourl.isEmpty()){
        QPixmap pixmap(newsphotourl); //for adding image as label;
        ui->label_uploadphotoimage->setPixmap(pixmap);
        ui->label_uploadphotoimage->setScaledContents(true); // Scales the pixmap to fit the size of the QLabel
        ui->label_uploadphotoimage->setFixedSize(250, 300);
    }
}




void AdminWindow::on_newscategory_combo_currentTextChanged(const QString &arg1)
{
    if(arg1 == "others"){
        ui->addnewcategory->show();
        ui->addCategory_input->show();
        ui->uploadCategoryPhoto_addpage->show();
    }else{
        ui->addnewcategory->hide();
        ui->addCategory_input->hide();
        ui->uploadCategoryPhoto_addpage->hide();
    }
}

void AdminWindow::on_addnewcategory_clicked()
{
    QString categoryname = ui->addCategory_input->text();
    QString categoryimg = categoryphoto;

    if(categoryname == "others" || categoryname == "Others"){
        QMessageBox::critical(this,"invalid category name","Write correct name");
        return;
    }

    if(!categoryname.isEmpty() && !categoryimg.isEmpty()){

        auto category_found = NewsManagementSystem::news.find(categoryname.toStdString());
        if(category_found != NewsManagementSystem::news.end()){
            // category exist before
            ui->addCategory_input->clear(); // to delete what has been written
            categoryphoto = NULL;
            //ui->addnewcategory->hide();
            //ui->addCategory_input->hide();
            //ui->uploadCategoryPhoto_addpage->hide();
            QMessageBox::critical(this,"Wrong","This category already exists");
            return;
        }

        int index_insertion = ui->newscategory_combo->count() - 1;
        //NewsManagementSystem::news[categoryname.toStdString()];
        ui->addCategory_input->clear(); // to delete what has been written
        ui->addnewcategory->hide();
        ui->addCategory_input->hide();
        ui->uploadCategoryPhoto_addpage->hide();
        categoryphoto = NULL; // member in class cuz in next time the previous image will be stored so if he doesn't upload photo it will pass cuz in variable the last image path already exists
        ui->newscategory_combo->insertItem(index_insertion,categoryname); //addItem()
        NewsManagementSystem::getCurrentAdmin()->addCategory(categoryname.toStdString(),categoryimg.toStdString());


    }else{
        QMessageBox::critical(this,"Empty Input","You must fill all inputs");
        return;
    }
}

// widgets (containers)
//QWidget *scrollarea_containerr ,*container_ofproductscards;
//It is a pointer that points to the address of existing widgets in ui in qt designer will not create new one

void AdminWindow::on_viewallnews_btn_clicked()
{
    ui->viewallnews_btn->setStyleSheet("background-color: grey;");
    ui->addnews_btn->setStyleSheet("background-color: blue;");
    ui->dashboard_btn->setStyleSheet("background-color: blue;");

    ui->pages->setCurrentIndex(2);
    showingAllNews();

}
// I can't send it by reference cuz it is a pointer not an object

void AdminWindow::DeleteExistingChildren(QLayout *existingLayoutforscroll){
    //QLayout *existingLayoutforscroll = scrollarea_containerr->layout();
    if (existingLayoutforscroll != nullptr) {
        QLayoutItem *child;
        while ((child = existingLayoutforscroll->takeAt(0)) != nullptr) {
            if (child->widget()){

                child->widget()->deleteLater();
            }

            delete child;
        }
        delete existingLayoutforscroll;
    }
}

void AdminWindow::showingAllNews(){
    // widgets (containers)
    QWidget *scrollarea_container ,*container_ofnewscards;
    scrollarea_container = ui->scrollAreaWidgetContents_forallnews;
    DeleteExistingChildren(scrollarea_container->layout());

    container_ofnewscards = new QWidget(scrollarea_container); //that container I do by code I will create new QWidget(container) and will make the pointer to point to its address and I can access this QWidget using its pointer using dereference operator

    // layouts
    QVBoxLayout *scrollarea_layout;

    scrollarea_layout = new QVBoxLayout(scrollarea_container);

    QGridLayout *layout_ofnewscards = new QGridLayout(container_ofnewscards);

    // TO check on the map is empty (No News), there is no news in system
    if(NewsManagementSystem::news.empty()){
        QLabel *label_nonews = new QLabel("No News To show");
        layout_ofnewscards->addWidget(label_nonews,1,0);
        scrollarea_layout->addWidget(container_ofnewscards);
        return;
    }

    int no_news=0;
    for(auto it=NewsManagementSystem::news.begin();it!=NewsManagementSystem::news.end();it++){
        for(auto &news_unorderedmap:it->second){
            News news = news_unorderedmap.second;
            QWidget *news_card = new QWidget();
            QVBoxLayout *contentnews_card = new QVBoxLayout();
            news_card->setLayout(contentnews_card);
            QLabel *news_number = new QLabel("#" + QString::number((no_news)+1),news_card);
            QLabel *news_name = new QLabel("Name: " + QString::fromStdString(news.title) ,news_card);
            QLabel *news_code = new QLabel("ID: " + QString::number(news.id),news_card);
            QLabel *news_category = new QLabel("Category: " +QString::fromStdString(news.category),news_card);
            QLabel* news_image = new QLabel(news_card);
            news_image->setPixmap(QPixmap(QString::fromStdString(news.image_url)));
            news_image->setScaledContents(true); // Scales the pixmap to fit the size of the QLabel
            news_image->setFixedSize(250, 350);

            QLabel* news_rating = new QLabel("Rating: " + QString::number(news.rate) + "/5" , news_card);
            QPushButton *viewmore = new QPushButton("View More",news_card);
            viewmore->setStyleSheet("font-weight: bold;font-size:32px;color: white;background-color:#03045e;border-radius:3px;border-style:solid;border-color:black;border-width:1px;");
            viewmore->setFixedWidth(200);
            //no_news++;
            contentnews_card->addWidget(news_number);
            contentnews_card->addWidget(news_image);
            contentnews_card->addWidget(news_code);
            contentnews_card->addWidget(news_name);
            contentnews_card->addWidget(news_category);
            contentnews_card->addWidget(news_rating);
            contentnews_card->addWidget(viewmore);

            news_card->setObjectName("news_card");
            news_card->setStyleSheet("QLabel{font-weight:bold;color:#11405C;}");

            layout_ofnewscards->addWidget(news_card,no_news/2,no_news%2);
            no_news++; // must be after cuz first element index zero will increment first and put it in wrong position


            container_ofnewscards->setObjectName("newsContainers");// object name here is id in css
            container_ofnewscards->setStyleSheet("QWidget#newsContainers > * {border: 1px solid black;background-color: white;border-radius:5px;}");

            connect(viewmore,&QPushButton::clicked,[=](){
                ui->pages->setCurrentIndex(3);
                qDebug() <<"In View More Connect: " << QString::number(news.id) << QString::fromStdString(news.category);

                this->viewMoreNews = news;
                showNewsData(viewMoreNews);
            });



                // you can create 2 pointers of QpushButtons and assign them to Modify and delete buttons in ui page
                // then you can write Lambda slot function for each of one of them here
                // I think here not sepere connect outside , Idk the difference honestly if I wrote connect inside connect or outside it as it's seperate

        }
    }
    scrollarea_layout->addWidget(container_ofnewscards); // I think you don't need cuz it's already added in ui designer you just have a pointer to it and modify on it not making a new one, you can put it withing for(HERE)

}






void AdminWindow::on_addnewcategory_modify_clicked()
{
    QString categoryname = ui->addCategorymodified_input->text();
    QString categoryimage = categoryphoto;

    if(categoryname == "others" || categoryname == "Others"){
        QMessageBox::critical(this,"invalid category name","Write correct name");
        return;
    }
    if(!categoryname.isEmpty() && !categoryimage.isEmpty()){

        auto category_found = NewsManagementSystem::news.find(categoryname.toStdString());
        if(category_found != NewsManagementSystem::news.end()){
            // category exist before
            ui->addCategory_input->clear(); // to delete what has been written
            categoryphoto = NULL;
            //ui->addnewcategory->hide();
            //ui->addCategory_input->hide();
            //ui->uploadCatPhoto_modifypage->hide();
            QMessageBox::critical(this,"Wrong","This Category already exists");
            return;
        }

        int index_insertion = ui->newscategory_combo_modify->count() - 1;
        //NewsManagementSystem::news[categoryname.toStdString()]; cuz we make function in admin to add new category
        ui->addCategorymodified_input->clear(); // to delete what has been written
        ui->addnewcategory_modify->hide();
        ui->addCategorymodified_input->hide();
        ui->uploadCatPhoto_modifypage->hide();
        categoryphoto = NULL; // member in class cuz in next time the previous image will be stored so if he doesn't upload photo it will pass cuz in variable the last image path already exists
        ui->newscategory_combo_modify->insertItem(index_insertion,categoryname); //addItem()
        NewsManagementSystem::getCurrentAdmin()->addCategory(categoryname.toStdString(),categoryimage.toStdString());


    }else{
        QMessageBox::critical(this,"Empty Input","You must fill all inputs");
        return;
    }
}


void AdminWindow::on_newscategory_combo_modify_currentTextChanged(const QString &arg1)
{
    if(arg1 == "others"){
        ui->addCategorymodified_input->show();
        ui->addnewcategory_modify->show();
        ui->uploadCatPhoto_modifypage->show();
    }else{
        ui->addCategorymodified_input->hide();
        ui->addnewcategory_modify->hide();
        ui->uploadCatPhoto_modifypage->hide();
    }
}


void AdminWindow::on_uploadnewphoto_clicked()
{
    newsphotourl_modified=QFileDialog::getOpenFileName(this,tr("Open File"),"C://","JPG IMAGE (*.jpg);; JPEG IMAGE (*.jpeg);; PNG IMAGE (*.png)");
    if(!newsphotourl_modified.isEmpty()){
        QPixmap pixmap(newsphotourl_modified); //for adding image as label;
        ui->photomodified->setPixmap(pixmap);
        ui->photomodified->setScaledContents(true); // Scales the pixmap to fit the size of the QLabel
        ui->photomodified->setFixedSize(250, 300);
    }
}


void AdminWindow::on_addnews_btn_clicked()
{
    ui->addnews_btn->setStyleSheet("background-color: grey;");
    ui->dashboard_btn->setStyleSheet("background-color: blue;");
    ui->viewallnews_btn->setStyleSheet("background-color: blue;");

    ui->label_uploadphotoimage->setPixmap(QPixmap(":/imgs/admin/question_mark.png"));
    ui->label_uploadphotoimage->setScaledContents(true); // Scales the pixmap to fit the size of the QLabel
    ui->label_uploadphotoimage->setFixedSize(250, 350);
    vector<string> categories = NewsManagementSystem::allCategories();
    QComboBox *combo_box = ui->newscategory_combo;
    combo_box->clear();
    for(int i = 0; i < categories.size();i++){
        combo_box->insertItem(i,QString::fromStdString(categories[i]));
    }
    combo_box->insertItem(combo_box->count(),"others");
    ui->pages->setCurrentIndex(1);
}







void AdminWindow::on_updatenews_button_clicked()
{

    QString id = ui->id_input->text();
    QString title = ui->newstitle_input_modify->text();
    QString category = ui->newscategory_combo_modify->currentText();
    QString description = ui->Description_textarea_modify->toPlainText();
    QString newsphoto = newsphotourl_modified;

    if(title.isEmpty()|| description.isEmpty()){
        QMessageBox::critical(this,"Empty Input","You must fill all inputs");
        return;
    }


    if(category == "others"){
        QMessageBox::critical(this,"choose another category","You can't choose others as category");
        return;
    }
    // Test case to check the category changed or not
    //string old_category = NewsManagementSystem::news[news.category][id.toInt()];

    // we should call here constructor that taking id not generating it randomly either change the category or not


    string old_category = viewMoreNews.category;
    string new_category = category.toStdString();

    News s;
    if(old_category != new_category ){
        if(!newsphoto.isEmpty()){
            s = News(title.toStdString(),description.toStdString(),category.toStdString(),newsphoto.toStdString(),viewMoreNews); //copy const
        }else{
            s = News(title.toStdString(),description.toStdString(),category.toStdString(),viewMoreNews.image_url,viewMoreNews); //copy const
        }


        NewsManagementSystem::getCurrentAdmin()->removeNews(old_category,id.toInt());
        NewsManagementSystem::getCurrentAdmin()->addNews(new_category,s);

    }else {

        // pointer that carries the address of the current admin object
        News s;
        if(!newsphoto.isEmpty()){
            s = News(title.toStdString(),description.toStdString(),newsphoto.toStdString(),viewMoreNews); //copy const
        }else{
            s = News(title.toStdString(),description.toStdString(),viewMoreNews.image_url,viewMoreNews); //copy const
        }

        NewsManagementSystem::getCurrentAdmin()->modifyNews(category.toStdString(),id.toInt(),s);
        // you should create another temp veriable here that will store the value after pressing the button of uploading photo or put the variable in the header file in the class
    }

    QMessageBox::information(this,"You update it Successfully","Succeded");
    all_news = NewsManagementSystem::MapToVector();
    current_index=current_index%all_news.size();
    ui->no_categories->setText(QString::number(NewsManagementSystem::noCategories()));
    ui->no_news->setText(QString::number(NewsManagementSystem::noNews()));

    //Erase all inputs
    ui->id_input->clear();
    ui->newstitle_input_modify->clear();
    ui->Description_textarea_modify->clear();
    ui->newscategory_combo_modify->setCurrentIndex(0);
    ui->photomodified->setPixmap(QPixmap());
    newsphoto=NULL;
    newsphotourl_modified=NULL;


    ui->dashboard_btn->setStyleSheet("background-color: grey;");
    ui->addnews_btn->setStyleSheet("background-color: blue;");
    ui->viewallnews_btn->setStyleSheet("background-color: blue;");
    ui->pages->setCurrentIndex(0);


}


void AdminWindow::on_logout_btn_clicked()
{
    hide();
    NewsManagementSystem::currentAdmin = NULL;
    MainWindow *mainwindow = new MainWindow();
    mainwindow->show();


}


void AdminWindow::on_uploadCatPhoto_modifypage_clicked(){
    categoryphoto=QFileDialog::getOpenFileName(this,tr("Open File"),"C://","JPG IMAGE (*.jpg);; JPEG IMAGE (*.jpeg);; PNG IMAGE (*.png)");

}


void AdminWindow::on_uploadCategoryPhoto_addpage_clicked(){
    categoryphoto=QFileDialog::getOpenFileName(this,tr("Open File"),"C://","JPG IMAGE (*.jpg);; JPEG IMAGE (*.jpeg);; PNG IMAGE (*.png)");

}

void AdminWindow::showNewsData(News &news){
    // widgets (containers)
    QWidget *scrollarea_container ,*container_ofnews;
    scrollarea_container = ui->scrollAreaWidgetContents_newsdata;
    DeleteExistingChildren(scrollarea_container->layout());

    container_ofnews = new QWidget(scrollarea_container); //that container I do by code I will create new QWidget(container) and will make the pointer to point to its address and I can access this QWidget using its pointer using dereference operator

    // layouts
    QVBoxLayout *scrollarea_layout;

    scrollarea_layout = new QVBoxLayout(scrollarea_container);

    QVBoxLayout *layout_ofnewscards = new QVBoxLayout(container_ofnews);


    QLabel *news_img = new QLabel(container_ofnews);
    news_img->setPixmap(QPixmap(QString::fromStdString(news.image_url)));
    news_img->setScaledContents(true); // Scales the pixmap to fit the size of the QLabel
    news_img->setFixedSize(250, 350);

    QLabel *news_title = new QLabel(QString::fromStdString(news.title),container_ofnews);
    news_title->setStyleSheet("font-weight:bold;font-size:31px;");
    news_title->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);


    QWidget *news_info = new QWidget(container_ofnews);
    QHBoxLayout *layout_news_info = new QHBoxLayout(news_info);
    news_info->setLayout(layout_news_info);


    QLabel *news_category = new QLabel(news_info);
    news_category->setText("Category: " + QString::fromStdString(news.category));
    QLabel *news_rate = new QLabel(news_info);
    if(news.rate == -1){
        news_rate->setText("Rating: " + QString::number(0) + "/5");
    }else{
        news_rate->setText("Rating: " + QString::number(news.rate) + "/5");
    }
    QLabel *news_date_posted = new QLabel(news_info);
    QString newsdate = "Date Posted: " + QString::number(news.date.day) + "/" + QString::number(news.date.month) + "/" + QString::number(news.date.year);
    news_date_posted->setText(newsdate);

    layout_news_info->addWidget(news_category);
    layout_news_info->addWidget(news_rate);
    layout_news_info->addWidget(news_date_posted);

    QLabel *news_description = new QLabel(container_ofnews);
    news_description->setText(QString::fromStdString(news.description));
    news_description->setWordWrap(true);

    QWidget *news_btns = new QWidget(container_ofnews);
    QHBoxLayout *layout_news_btns = new QHBoxLayout(news_btns);
    news_btns->setLayout(layout_news_btns);

    QPushButton *modify_btn = new QPushButton("Modify",news_btns);
    QPushButton *delete_btn = new QPushButton("Delete",news_btns);

    layout_news_btns->addWidget(modify_btn);
    layout_news_btns->addWidget(delete_btn);
    //-----------
    // main layout will add all the widgets inside it

    layout_ofnewscards->addWidget(news_img);
    layout_ofnewscards->addWidget(news_title);
    layout_ofnewscards->addWidget(news_info);
    layout_ofnewscards->addWidget(news_description);
    layout_ofnewscards->addWidget(news_btns);

    scrollarea_layout->addWidget(container_ofnews); // without that nothing appears

    modify_btn->setStyleSheet("font-weight:bold;font-size:30px;background-color: black;color:white;");
    delete_btn->setStyleSheet("font-weight:bold;font-size:30px;background-color: #800000;color:white;");
    connect(modify_btn,&QPushButton::clicked,[=](){
        //qDebug() << "News Object in Delete: " << QString::number(news_object.id) << QString::fromStdString(news_object.category);
        //qDebug() <<"In Modify Connect Page: " << QString::number(viewMoreNews.id) << QString::fromStdString(viewMoreNews.category);
        // Set data first in the modify page then change the page
        ui->id_input->setText(QString::number(viewMoreNews.id));
        ui->id_input->setReadOnly(true);
        ui->newstitle_input_modify->setText(QString::fromStdString(viewMoreNews.title));
        ui->Description_textarea_modify->setText(QString::fromStdString(viewMoreNews.description));
        ui->photomodified->setPixmap(QPixmap(QString::fromStdString(viewMoreNews.image_url)));
        ui->photomodified->setScaledContents(true); // Scales the pixmap to fit the size of the QLabel
        ui->photomodified->setFixedSize(250, 350);
        ui->newstitle_input_modify->setText(QString::fromStdString(viewMoreNews.title));


        // Delete all items in it first then add
        //to know the number of items in the QComboBox use count() it is one based
        QComboBox *combo_modify = ui->newscategory_combo_modify;
        if(combo_modify->count() > 0){
            combo_modify->clear();
        }
        vector<string> categories = NewsManagementSystem::allCategories();
        for(int i = 0; i < categories.size();i++){
            //combo_modify->addItem(QString::fromStdString(categories[i]));
            combo_modify->insertItem(i,QString::fromStdString(categories[i]));
        }
        combo_modify->insertItem(combo_modify->count(),"others");
        int index = combo_modify->findText(QString::fromStdString(viewMoreNews.category));
        if (index != -1) {
            combo_modify->setCurrentIndex(index);
        }




        ui->addCategorymodified_input->hide();
        ui->addnewcategory_modify->hide();

        ui->pages->setCurrentIndex(4); // change to modify page

    });

    connect(delete_btn,&QPushButton::clicked,[=](){
        NewsManagementSystem::getCurrentAdmin()->removeNews(viewMoreNews.category,viewMoreNews.id);
        if(NewsManagementSystem::news[viewMoreNews.category].size() == 0){   // to delete category if it doesn't have news after deleting it
            NewsManagementSystem::news.erase(viewMoreNews.category);
        }
        all_news = NewsManagementSystem::MapToVector();
        qDebug() << QString::number(all_news.size());
        // you must handle the case if there is only one news left and you delete it so vector size will be 0
        // 0 mod 0 >> error so program crashes
        if(all_news.size() > 0){
            current_index=current_index%all_news.size();
        }else{
            current_index = 0;
            ui->newsidlabel_home->setText("");
            ui->newsname_home->setText("");
            //QString news_date = QString::number(news.date.day) + "/" + QString::number(news.date.month) + "/" + QString::number(news.date.year);
            ui->dateposted_home->setText("");
            ui->categoryname_home->setText("");
            ui->imglabel_home->setPixmap(QPixmap(""));
            ui->imglabel_home->setFixedSize(220,220);
        }
        ui->pages->setCurrentIndex(2);
        showingAllNews();

    });

    container_ofnews->setStyleSheet("background-color: white;");





}

void AdminWindow::on_dashboard_btn_clicked()
{
    ui->dashboard_btn->setStyleSheet("background-color: grey;");
    ui->addnews_btn->setStyleSheet("background-color: blue;");
    ui->viewallnews_btn->setStyleSheet("background-color: blue;");


    ui->pages->setCurrentIndex(0);
    viewDashboard();
}


void AdminWindow::viewDashboard(){
    // Initialize Label Dashboard Admin
    ui->no_categories->setText(QString::number(NewsManagementSystem::noCategories()));
    ui->no_news->setText(QString::number(NewsManagementSystem::noNews()));
    ui->no_users->setText(QString::number(NewsManagementSystem::noUsers()));
    ui->no_comments->setText(QString::number(NewsManagementSystem::noComments()));



    QPushButton *back_btn = ui->back_btn;
    QPushButton *next_btn = ui->next_btn;
    back_btn->setIcon(QIcon(":/imgs/admin/left.png"));
    next_btn->setIcon(QIcon(":/imgs/admin/right.png"));

    ui->user_table->setRowCount(NewsManagementSystem::noUsers());
    int row=0;
    for (auto user : NewsManagementSystem::users){
        //setItem(row, col, item);
        ui->user_table->setItem(row,0,new QTableWidgetItem(QString::number(user.second.get_id())));
        ui->user_table->setItem(row,1,new QTableWidgetItem(QString::fromStdString(user.second.get_name())));
        ui->user_table->setItem(row,2,new QTableWidgetItem(QString::fromStdString(user.second.get_email())));
        ui->user_table->setItem(row,3,new QTableWidgetItem(QString::fromStdString(user.second.get_password())));
        row++;
    }


    connect(next_btn,&QPushButton::clicked,[=](){
        if(all_news.empty()){
            return;
        }
        current_index = (current_index+1)% all_news.size();
        setNewsData();


    });


    connect(back_btn,&QPushButton::clicked,[=](){
        if(all_news.empty()){
            return;
        }
        current_index = (current_index-1)% all_news.size();
        setNewsData();
    });



}

void AdminWindow::closeEvent(QCloseEvent *event)  {
    NewsManagementSystem:: writeCategoriesInFiles("CategoriesData.txt");
    NewsManagementSystem:: writeNewsDataInFiles("NewsData.txt");
    NewsManagementSystem:: writeNewsReviewInFiles("NewsReview.txt");
    NewsManagementSystem::writeUserInfoInFils("UserData.txt");
    NewsManagementSystem::writeAdminToFile();
    QMainWindow::closeEvent(event);
}
