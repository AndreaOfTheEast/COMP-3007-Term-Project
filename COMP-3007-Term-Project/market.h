#pragma once
#include "hinton_market.h"

//~ andwu: user system
class UserSystem
{
public:
    std::vector<User> users;

    User *get_user(Credentials creds);
    void add_user(User user);
};

//~ andwu: booking system
class BookingSystem
{
public:
    std::vector<Booking> bookings;

    void add_booking(Booking booking);
};
