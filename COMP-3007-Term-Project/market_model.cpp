#include "hinton_market.h"
#include "market_model.h"

#include <sstream>

// ------------------------
// ----- USER SYSTEM ------
// ------------------------
User *UserSystem::get_user(Credentials creds)
{
    Assert(0, "TODO: select from the database");
    return(0);
}

void UserSystem::add_user(User user)
{
    Assert(0, "TODO: insert to the database");
}

std::vector<User> UserSystem::get_user_list() {
    return(users);
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
    // andwu: TODO: maybe the booking table should be: booking_id, date, create_timestamp, user_id
    Assert(0, "TODO: insert a booking");
}

int MarketDateSystem::make_booking(UserId user, uint64_t market_date_index)
{
    Assert(0, "TODO: we want to insert a booking into the booking list"
            "for this date and type."
            "Then query what that position is and the limit,"
            "so that we can have a popup about being waitlist/booked");
    return(0);
}

void MarketDateSystem::cancel_booking(UserId user, uint64_t market_date_index)
{
    Assert(0, "TODO: we want to remove a booking, add a notification for waitlister that are ready");
}

int64_t MarketDateSystem::is_user_booked(UserId user, uint64_t market_date_index)
{
    Assert(0, "TODO: check if this market date is booked by the user");
    return(-1);
}

// -------------------------
// -- NOTIFICATION SYSTEM --
// -------------------------
std::vector<std::string> NotificationSystem::get_notifications(UserId id) {
    Assert(0, "TODO: query for notifications");
    return(std::vector<std::string>{});
}

void NotificationSystem::add_notification(UserId id, std::string content) {
    Assert(0, "TODO: insert notification");
}
