#pragma once
#include "hinton_market.h"

//~ andwu: user system
class UserSystem
{
public:
    std::vector<User> users;

    User *get_user(Credentials creds);
};

//~ andwu: market
class Market
{
public:
    uint64_t max_bookings;
    std::vector<User *> bookings;
};

//~ andwu: booking system
class BookingSystem
{
public:
    std::vector<Market> markets;

    void add_market(Market market);

    std::vector<Market *> get_markets_in_range(Date start, Date end);
};
