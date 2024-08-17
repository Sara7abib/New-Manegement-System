#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include "admin.h"
#include "user.h"
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_backbtn_registerpage_clicked();

    void on_registerbtn_registerpage_clicked();

    void on_typeacc_combo_register_currentTextChanged(const QString &arg1);

    void on_register_btn_home_clicked();

    void on_login_btn_home_clicked();

    void on_typeacc_combo_login_currentTextChanged(const QString &arg1);

    void on_login_btn_loginpage_clicked();

    void on_acctype_FP_currentTextChanged(const QString &arg1);

    void on_resetpassword_btn_clicked();

    void on_backbtn_FP_clicked();

    void on_confirmbtn_reset_clicked();

    void on_forgetpassword_commandLink_login_clicked();

    void on_back_btn_loginpage_clicked();
void closeEvent(QCloseEvent *);
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
