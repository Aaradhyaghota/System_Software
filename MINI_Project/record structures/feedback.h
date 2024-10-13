#ifndef FEEDBACK
#define FEEDBACK

#define FEEDBACK_SIZE 100
#include "../functions/server_constants.h"
struct Feedback {
    int id;
    bool reviewed;
    char text[FEEDBACK_SIZE];
};

#endif