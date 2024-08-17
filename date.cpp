#include "date.h"

Date::Date() {}

Date::Date(int day,int month,int year){
    this->day = day;
    this->month = month;
    this->year = year;
}
QDate Date::toQDate(){
    return QDate(year,month,day);
}


Date::Date(QDate qdate){
    this->day = qdate.day();
    this->month = qdate.month();
    this->year = qdate.year();
}
Date Date::currentDate(){
    QDate currentDate = QDate::currentDate(); // to get current date
    return Date(currentDate);

}
