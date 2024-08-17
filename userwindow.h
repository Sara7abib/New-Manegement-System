#ifndef USERWINDOW_H
#define USERWINDOW_H

#include <QMainWindow>
#include "newsmanagementsystem.h"


#include<QString>
#include<QSettings>
#include<QPushButton>
#include"user.h"
#include<QGridLayout>
#include<QListWidget>
namespace Ui {
class UserWindow;
}

class UserWindow : public QMainWindow
{
    Q_OBJECT

public:
    void setupNewsDisplay(QWidget* , std::stack<std::pair<std::string, int>>);
    void clearLayout(QLayout* );
    int newsColumns;
    int categoryColumns;
    string category;

    QPushButton *favourite;

    void showNews(News& , QGridLayout* , int);
    void showOneNew(News& , QGridLayout *);
    void displayCategory( string );
    void searchDisplay(vector<pair<string,int>>);
    void notFound(QGridLayout *);
    void categoryWidget();
    void showCategory(string , QGridLayout *, int ,QString);
    void lrate(int,News&);
    void toggleFavorite(News);
private slots:
    void on_search_clicked();
    void handleDetailsButtonClick(News&);

    void on_Trending_clicked();

    void on_Home_clicked();

    void on_Categories_clicked();
    void on_favourote_clicked();
    void on_logout_btn_clicked();
void closeEvent(QCloseEvent *);
public:
    void sendMessageUpdates(User *currentUser);
public:
    explicit UserWindow(QWidget *parent = nullptr);
    ~UserWindow();

private:
    Ui::UserWindow *ui;
};

#endif // USERWINDOW_H
