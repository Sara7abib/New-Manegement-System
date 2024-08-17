#include "newstime.h"

NewsTime::NewsTime() {
    hours=0;
    minutes=0;
    seconds=0;
}

NewsTime::NewsTime(int hours,int minutes,int seconds){
    this->hours = hours;
    this->minutes = minutes;
    this->seconds = seconds;

}
NewsTime::NewsTime(QTime time){
    this->hours = time.hour();
    this->minutes = time.minute();
    this->seconds = time.second();
}
QTime NewsTime::toQTime(){
    QTime qtime(hours, minutes, seconds);
    return qtime;
}
NewsTime NewsTime::getCurrentTime(){
    QTime currentTime = QTime::currentTime();
    return NewsTime(currentTime);

}


