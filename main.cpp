#include "mainwindow.h"
#include "adminwindow.h"
#include <QApplication>
#include "newsmanagementsystem.h"
#include "userwindow.h"
#include "admin.h"
#include "news.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // Read data from files
    NewsManagementSystem::readCategoryFromFile("CategoriesData.txt");
    NewsManagementSystem::readNewsDataFromFiles("NewsData.txt");
    NewsManagementSystem::readNewsReviewFromFiles("NewsReview.txt");
    NewsManagementSystem::readUserFromFile("UserData.txt");
    NewsManagementSystem::readAdminFromFile("AdminData.txt");
    MainWindow w;
    w.show();
    return a.exec();
}
