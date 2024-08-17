#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>
#include "news.h"
#include "newsmanagementsystem.h"
namespace Ui {
class AdminWindow;
}

class AdminWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdminWindow(QWidget *parent = nullptr);
    ~AdminWindow();

private slots:
    void on_addnews_button_clicked();

    void on_uploadphoto_clicked();

    //void on_newscategory_combo_currentIndexChanged(int index);

    void on_addnewcategory_clicked();

    void on_newscategory_combo_currentTextChanged(const QString &arg1);

    void on_viewallnews_btn_clicked();

    void on_addnewcategory_modify_clicked();

    void on_newscategory_combo_modify_currentTextChanged(const QString &arg1);

    void on_uploadnewphoto_clicked();

    //void on_updatenews_button_clicked();

    void on_addnews_btn_clicked();





    void on_updatenews_button_clicked();

    void on_logout_btn_clicked();

    void on_uploadCatPhoto_modifypage_clicked();

    void on_uploadCategoryPhoto_addpage_clicked();

    void on_dashboard_btn_clicked();
void closeEvent(QCloseEvent *);


private:
    Ui::AdminWindow *ui;

public:
    QString newsphotourl;
    QString newsphotourl_modified;

    QString categoryphoto; // either in add page or modify page

public:
    // not sure about it I may write all code in slot function of button view all news btn;
    // these functions must be here not in NewsManagementSystem to inherit ui to can access widgets in qt designer
    void DeleteExistingChildren(QLayout *existingLayoutforscroll);
    void showingAllNews(); // I can send layouts in parameter as reference or pointers but I will try it first


public:
    News viewMoreNews;
    void setViewMoreNews(News news);
    News getViewMoreNews();

    void viewnewstimer();

    void setNewsData(vector<pair<string,int>> all_news,int current_index);
    void setNewsData();

public:
    int current_index;

public:
    void showNewsData(News &news);
    void viewDashboard();

public:
    vector<pair<string,int>> all_news = NewsManagementSystem::MapToVector(); // because of that not updated in news , either add , update , or delete and if deleted it has string and id of news that doesn't exist in the original unordered map so it access garbage

// 3 cases : add - modify - delete
};

#endif // ADMINWINDOW_H
