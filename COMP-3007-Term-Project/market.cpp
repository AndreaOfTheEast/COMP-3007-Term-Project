#include "hinton_market.h"
#include "market.h"

#include <sstream>

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
    if (waitlisted == 1)
    {
        // set up waitlist message
        QString qs = QString("You have been put on a waitlist. You are in position %1.")
            .arg(waitlist_position);
        msgBox.setText(qs);
        msgBox.exec();

        std::stringstream s;
        s << "[Action] Waitlisted in position " << waitlist_position << "for " << market_dates[market_date_index].date.to_string() << ".";
        notification_system->add_notification(user->id, s.str());
    } else {
        std::stringstream s;
        s << "[Action] Booked for " << market_dates[market_date_index].date.to_string() << ".";
        notification_system->add_notification(user->id, s.str());
    }
}

std::vector<std::string> NotificationSystem::get_notifications(UserId id) {
    std::vector<std::string> user_notifs;

    for (uint64_t i = 0; i < notifications.size(); i++) {
        if (notifications[i].id == id) {
            user_notifs.push_back(notifications[i].content);
        }
    }

    return user_notifs;
}

void NotificationSystem::add_notification(UserId id, std::string content) {
    Notification notification;
    notification.id = id;
    notification.content = content;
    notifications.push_back(notification);
}
