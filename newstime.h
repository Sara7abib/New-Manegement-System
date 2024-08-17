#ifndef NEWSTIME_H
#define NEWSTIME_H
#include <QTime>
class NewsTime
{
public:
    int hours;
    int minutes;
    int seconds;
    // handle cases that if he write 06 or 6

public:
    NewsTime();
    NewsTime(int hours,int minutes,int seconds);
    NewsTime(QTime time); // to give it QTime object and from it assign data to members

    QTime toQTime();
    // if i made it static I will have to send NewsTime object in parameters
    // if not lik that way I can access directly the current object and its members using this

    // don't need to make function convert QTime to time cuz I already made that idea by constructor



    static NewsTime getCurrentTime(); // returns NewsTime object you can use it directly


    //Copy Constructor gives it object of same datatype(same class) and it copy its data and assign it to this
    //NewsTime(NewsTime &time);


};

#endif // NEWSTIME_H
