#pragma once
#include "hinton_market.h"

class UserSystem
{
public:
    std::vector<User> users;

    User *get_user(Credentials creds);
    void add_user(User user);
};

class MarketDateSystem
{
public:
    std::vector<MarketDate> market_dates;
    void add_market_date(MarketDate market_date);
    void make_booking(User *user, uint64_t market_date_index);
};
