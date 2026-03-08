#pragma once
#include "hinton_market.h"

//~ andwu: user system
class UserSystem
{
public:
    std::vector<User> users;

    User *get_user(Credentials creds)
    {
        User *user = 0;
        for(uint64_t ui = 0;
                ui < users.size();
                ui += 1)
        {
            if(users[ui].creds == creds)
            {
                user = &users[ui];
                break;
            }
        }
        return(user);
    }
};

//~ andwu: booking system
class BookingSystem
{
public:
    std::vector<User> users;

    User *get_user(Credentials creds)
    {
        User *user = 0;
        for(uint64_t ui = 0;
                ui < users.size();
                ui += 1)
        {
            if(users[ui].creds == creds)
            {
                user = &users[ui];
                break;
            }
        }
        return(user);
    }
};
