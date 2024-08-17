#include "review.h"

Review::Review() {}


Review::Review(int rating) {
    this->rating = rating;
}

Review::Review(string comment, Date date_posted) {

    this->rating = -1;
    this->comments.push_back(make_pair(comment,date_posted));
}
