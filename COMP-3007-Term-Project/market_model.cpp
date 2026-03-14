#include "hinton_market.h"
#include "market_model.h"

#include <sstream>

// ------------------------
// ----- USER SYSTEM ------
// ------------------------
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

// ------------------------
// -- MARKET DATE SYSTEM --
// ------------------------

void MarketDateSystem::add_market_date(MarketDate market_date)
{
    market_dates.push_back(market_date);
}

void MarketDateSystem::make_booking(User *user, uint64_t market_date_index)
{
    uint8_t waitlisted = 0;
    uint64_t waitlist_position = 0;
    std::stringstream s;
    std::string date = market_dates[market_date_index].date.to_string();

    QMessageBox msgBox;
    std::vector<UserId> *booking_list = nullptr;
    uint64_t booking_limit = 0;
    uint64_t *booked = nullptr;

    // handle artisan vendor booking
    if (user->perms.user_type == USER_TYPE_ARTISAN)
    {
        booking_list = &market_dates[market_date_index].artisan_users;
        booking_limit = market_dates[market_date_index].artisan_limit;
        booked = &market_dates[market_date_index].artisan_booked;
    }
    else if (user->perms.user_type == USER_TYPE_FOOD)
    {
        booking_list = &market_dates[market_date_index].food_users;
        booking_limit = market_dates[market_date_index].food_limit;
        booked = &market_dates[market_date_index].food_booked;
    }

    if (booking_list == nullptr) { return; }

    booking_list->push_back(user->id);
    if (booking_list->size() > booking_limit)
    {
        waitlisted = 1;
        waitlist_position = booking_list->size() - booking_limit;
    }
    else
    {
        (*booked)++;
    }

    if (waitlisted == 1)
    {
        // Set up waitlist message
        QString qs = QString("You have been put on a waitlist for. You are in position %1.")
            .arg(waitlist_position);
        msgBox.setText(qs);
        msgBox.exec();
        s << "[Action] Waitlisted in position " << waitlist_position << " for " << date << ".";
        notification_system->add_notification(user->id, s.str());
    }
    else
    {
        s << "[Action] Booked for " << date << ".";
        notification_system->add_notification(user->id, s.str());
    }
}

void MarketDateSystem::cancel_booking(User *user, uint64_t market_date_index)
{
    uint64_t *booked = nullptr;
    std::vector<UserId> *booking_list = nullptr;
    std::stringstream notification_msg;

    if (user->perms.user_type == (USER_TYPE) USER_TYPE_ARTISAN)
    {
        booking_list = &market_dates[market_date_index].artisan_users;
        booked = &market_dates[market_date_index].artisan_booked;
    }
    else if (user->perms.user_type == (USER_TYPE) USER_TYPE_FOOD)
    {
        booking_list = &market_dates[market_date_index].food_users;
        booked = &market_dates[market_date_index].food_booked;
    }

    if (booking_list == nullptr) { return; }

    for (uint32_t i = 0; i < booking_list->size(); i++)
    {
        if (user->id == (*booking_list)[i])
        {
            booking_list->erase(booking_list->begin() + i);
            (*booked)--;
            notification_msg << "[Action] Cancelled booking for " <<
                                market_dates[market_date_index].date.to_string() << ".";
            notification_system->add_notification(user->id, notification_msg.str());

            // Notify waitlist
            notification_msg.str("");
            notification_msg.clear();

            if (booking_list->size() > 0)
            {
                notification_msg << "[Alert] Available spot for " <<
                                    market_dates[market_date_index].date.to_string() << "." <<
                                    " Please respond to waitlist offer.";
                notification_system->add_notification((*booking_list)[*booked], notification_msg.str());
            }

            break;
        }
    }
}

int64_t MarketDateSystem::is_user_booked(User *user, uint64_t market_date_index)
{
    std::vector<UserId> *booking_list = nullptr;
    int64_t booked = 0;

    if (user->perms.user_type == (USER_TYPE) USER_TYPE_ARTISAN)
    {
        booking_list = &market_dates[market_date_index].artisan_users;
        booked =  (int64_t)market_dates[market_date_index].artisan_booked;
    }
    else if (user->perms.user_type == (USER_TYPE) USER_TYPE_FOOD)
    {
        booking_list = &market_dates[market_date_index].food_users;
        booked =  (int64_t)market_dates[market_date_index].food_booked;
    }

    if (booking_list == nullptr) { return -2; }

    for (uint32_t i = 0; i < booked; i++)
    {
        if (user->id == (*booking_list)[i])
        {
            return i;
        }
    }

    return -1;
}

// -------------------------
// -- NOTIFICATION SYSTEM --
// -------------------------
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
