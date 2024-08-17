#ifndef REVIEW_H
#define REVIEW_H

#include <string>
#include <date.h>
#include <vector>
using namespace std;
class Review{
public:
    //int userID; // or pointer
    vector<pair<string,Date>> comments;
    float rating = -1;
    //Date date; cuz it will not if user has more than one comment we can't track each date of each comment
public:
    Review();
    Review(string comment, Date date_posted);
    Review(int rating);


};

#endif // REVIEW_H
