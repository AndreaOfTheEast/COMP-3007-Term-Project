#include "hinton_market.h"
#include "market.h"

//~ andwu: user system
User *UserSystem::get_user(Credentials creds)
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

void UserSystem::add_user(User user)
{
    users.push_back(user);
}

std::string Date::to_string()
{
    char buff[1024];
    snprintf(buff, sizeof(buff), "%02lu/%02lu/%lu", day, (month + 1), (year + 1900));
    return std::string(buff);
}

// ~nico: booking system
void MarketDateSystem::add_market_date(MarketDate market_date)
{
    market_dates.push_back(market_date);
}

void MarketDateSystem::make_booking(User *user, uint64_t market_date_index)
{
    uint8_t waitlisted = 0;
    uint64_t waitlist_position = 0;

    QMessageBox msgBox;

    // handle artisan vendor booking
    if (user->perms.user_type == USER_TYPE_ARTISAN)
    {
        market_dates[market_date_index].artisan_users.push_back(user->id);
        if (market_dates[market_date_index].artisan_users.size() > market_dates[market_date_index].artisan_limit) {
            waitlisted = 1;
            waitlist_position = market_dates[market_date_index].artisan_users.size() - market_dates[market_date_index].artisan_limit;
        }

    }
    // handle food vendors booking
    else if (user->perms.user_type == USER_TYPE_FOOD)
    {
        market_dates[market_date_index].food_users.push_back(user->id);
        if (market_dates[market_date_index].food_users.size() > market_dates[market_date_index].food_limit) {
            waitlisted = 1;
            waitlist_position = market_dates[market_date_index].food_users.size() - market_dates[market_date_index].food_limit;
        }
    }

    // TODO: add date to the message
    // set up waitlist message
    QString qs = QString("You have been put on a waitlist. You are in position %1.")
            .arg(waitlist_position);

    if (waitlisted == 1) {
        msgBox.setText(qs);
        msgBox.exec();
    }
}
