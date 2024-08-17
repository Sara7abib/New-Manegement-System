#ifndef DATE_H
#define DATE_H
#include <QDate>
class Date
{
public:
    int day;
    int month;
    int year;
public:
    Date();
    Date(int day,int month,int year);
    Date(QDate qdate); // constructor that takes QDate object and assign its data in Date object
    static Date currentDate();
    QDate toQDate();
};

#endif // DATE_H
