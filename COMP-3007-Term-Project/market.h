#pragma once
#include "hinton_market.h"

//~ andwu: user system
class UserSystem
{
public:
    std::vector<User> users;

    User *get_user(Credentials creds);
};

//~ andwu: booking system
class BookingSystem
{
public:
    std::vector<User> users;

    User *get_user(Credentials creds);
};
