#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "adminwindow.h"
#include "userwindow.h"
#include "validation.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->secretcode_input_register->hide();
    ui->secretcode_input_login->hide();
    ui->secretkey_FP->hide();
    ui->newpassword_widget->hide();
    ui->typeacc_combo_login->setCurrentText("User");
    ui->typeacc_combo_register->setCurrentText("User");
    ui->acctype_FP->setCurrentText("User");

    ui->stackedWidget->setCurrentIndex(0); // it's by default index 0 appeared if you don't set it

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_backbtn_registerpage_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

////QMessageBox::warning(this,"login faild","invalid username");
void MainWindow::on_registerbtn_registerpage_clicked()
{
    string name = ui->nameinput_register->text().toStdString();
    string username = ui->usernameinput_register->text().toStdString();
    string email = ui->emailinput_register->text().toStdString();
    string password = ui->passwordinput_register->text().toStdString();
    string account_type = ui->typeacc_combo_register->currentText().toStdString();

    if(name.empty() || username.empty() || email.empty() || password.empty()){
        QMessageBox::critical(this,"Registeration faild","You must fill all inputs");
        return;
    }

    if(Validation::checkemail(email) && Validation::checkpassword(password)){
        if(account_type == "Admin"){
            string secret_code = ui->secretcode_input_register->text().toStdString();
            if(secret_code.empty()){
                QMessageBox::critical(this,"Registeration faild","You must fill all inputs");
                return;
            }
            bool check = Admin::signupAdmin(name,username,email,password,secret_code);
            if(check){
                QMessageBox::information(this,"Success","You have an account in the system");
                QMessageBox::information(this,"Secret Key","Your secret key is: " + QString::fromStdString(Admin::getSecretKey(username)));
                //ui->verticalLayout->addWidget(QLabel(""));
                ui->stackedWidget->setCurrentIndex(2);
            }else{
                QMessageBox::critical(this,"Registeration faild","This account already exists before or secret code is wrong");
                return;
            }
        }else{ //Account_type = user
            bool check = User::signupUser(name,username,email,password);
            if(check){
                QMessageBox::information(this,"Success","You have an account in the system");
                ui->stackedWidget->setCurrentIndex(2);
            }else{
                QMessageBox::critical(this,"Registeration faild","This account already exists before");
                return;
            }

        }
    }else{
        QMessageBox::critical(this,"Registeration faild","Invalid email or password");
        return;
    }



}


void MainWindow::on_typeacc_combo_register_currentTextChanged(const QString &arg1)
{
    if(arg1 == "Admin"){
        ui->secretcode_input_register->show();
    }else{
        ui->secretcode_input_register->hide();
    }
}


void MainWindow::on_register_btn_home_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_login_btn_home_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_typeacc_combo_login_currentTextChanged(const QString &arg1)
{
    if(arg1 == "Admin"){
        ui->secretcode_input_login->show();
    }else{
        ui->secretcode_input_login->hide();
    }
}


void MainWindow::on_login_btn_loginpage_clicked()
{
    string username = ui->username_login_input->text().toStdString();
    string password = ui->password_login_input->text().toStdString();
    string acc_type = ui->typeacc_combo_login->currentText().toStdString();
    if(username.empty() ||  password.empty()){
        QMessageBox::critical(this,"login faild","You must fill all inputs");
        return;
    }

    if(acc_type == "Admin"){
        string secret_key = ui->secretcode_input_login->text().toStdString();
        bool check = Admin::loginAdmin(username,password,secret_key);
        if(check){
            QMessageBox::information(this,"Success","Login success");
            hide();
            AdminWindow *admin_window = new AdminWindow();
            admin_window->show();

        }else{
            QMessageBox::critical(this,"login faild","Invalid username or password");

        }
    }else{ // user
        bool check = User::loginUser(username,password);
        if(check){
            QMessageBox::information(this,"Success","Login success");
            hide();
            UserWindow *user_window = new UserWindow();
            user_window->show();

        }else{
            QMessageBox::critical(this,"login faild","Invalid username or password");

        }

    }
}


void MainWindow::on_acctype_FP_currentTextChanged(const QString &arg1)
{
    if(arg1 == "Admin"){
        ui->secretkey_FP->show();
    }else{
        ui->secretkey_FP->hide();
    }
}


void MainWindow::on_resetpassword_btn_clicked()
{
    QString username = ui->usernameinput_FP->text();
    QString email = ui->emailinput_FP->text();
    QString acc_type = ui->acctype_FP->currentText();
    if(username.isEmpty() || email.isEmpty()){
        QMessageBox::critical(this,"Reset Password faild","You must fill all inputs");
        return;
    }

    if(acc_type == "Admin"){
        QString secret_key = ui->secretkey_FP->text();
        if(Admin::checkAdminExists(username.toStdString(),email.toStdString(),secret_key.toStdString())){
            ui->newpassword_widget->show();
        }else{
            QMessageBox::critical(this,"Reset Password faild","Invalid data");
            return;
        }
    }else{ //user
        if(User::checkUserExists(username.toStdString(),email.toStdString())){
            ui->newpassword_widget->show();
        }else{
            QMessageBox::critical(this,"Reset Password faild","Invalid data");
            return;
        }

    }

}


void MainWindow::on_backbtn_FP_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_confirmbtn_reset_clicked()
{
    QString username = ui->usernameinput_FP->text();
    QString new_password = ui->newpasswordinput_FP->text();
    QString new_password_confirm = ui->confirnnewpasswordinput_FP->text();
    QString acc_type = ui->acctype_FP->currentText();
    if(new_password.isEmpty() || new_password_confirm.isEmpty()){
        QMessageBox::critical(this,"Reset Password faild","You must fill all inputs");
        return;
    }

    if(!Validation::checkpassword(new_password.toStdString()) || new_password != new_password_confirm){
        QMessageBox::critical(this,"Reset Password faild","Invalid data");
        return;
    }




    if(acc_type == "Admin"){
        Admin::resetPassword(username.toStdString(),new_password.toStdString());
    }else{
        User::resetPassword(username.toStdString(),new_password.toStdString());
    }

    //ui->newpasswordinput_FP->setReadOnly(true);
    //ui->confirnnewpasswordinput_FP->setReadOnly(true);
    QMessageBox::information(this,"Reset Password success","You changed Your password, Please Log in");
    //ui->confirmbtn_reset->setEnabled(false);
    ui->newpasswordinput_FP->clear();
    ui->confirnnewpasswordinput_FP->clear(); // it erases input
    ui->newpassword_widget->hide();

    ui->usernameinput_FP->clear();
    ui->emailinput_FP->clear();
    ui->secretkey_FP->clear();
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_forgetpassword_commandLink_login_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_back_btn_loginpage_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::closeEvent(QCloseEvent *event)  {
    NewsManagementSystem:: writeCategoriesInFiles("CategoriesData.txt");
    NewsManagementSystem:: writeNewsDataInFiles("NewsData.txt");
    NewsManagementSystem:: writeNewsReviewInFiles("NewsReview.txt");
    NewsManagementSystem::writeUserInfoInFils("UserData.txt");
    NewsManagementSystem::writeAdminToFile();
    QMainWindow::closeEvent(event);
}
