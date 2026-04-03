#include "hinton_market.h"
#include "market_model.h"

#include <sstream>

// ------------------------
// ----- USER SYSTEM ------
// ------------------------
bool UserSystem::get_user(Credentials creds, User *user)
{
    user->perms.user_type = USER_TYPE_NULL;

    std::string query_string =
            "SELECT * FROM users"
            " WHERE users.username = :username;";
    QSqlQuery query;
    query.prepare(QString(query_string.c_str()));
    query.bindValue(":username", QString(creds.username.c_str()));
    if(!query.exec())
    {
        fprintf(stderr, "DB Query: %s\n", query.lastError().text().toStdString().c_str());
        Assert(0, "database failed query");
        return(0);
    }

    if(query.next())
    {
        user->id                                                  = UserId{ (uint64_t)query.value(0).toInt() };
        user->creds.username                                      = query.value(1).toString().toStdString();
        user->email                                               = query.value(2).toString().toStdString();
        user->phone_number                                        = query.value(3).toString().toStdString();
        user->mail_address                                        = query.value(4).toString().toStdString();
        user->owner_name                                          = query.value(5).toString().toStdString();
        user->business_name                                       = query.value(6).toString().toStdString();
        user->perms.user_type                                     = (USER_TYPE)query.value(7).toInt(); // andwu: TODO: SUS
        user->compliance_docs.business_licence.number             = query.value(8).toString().toStdString();
        user->compliance_docs.business_licence.expiration_date    = query.value(9).toString().toStdString();
        user->compliance_docs.liability_insurance.policy_number   = query.value(10).toString().toStdString();
        user->compliance_docs.liability_insurance.provider        = query.value(11).toString().toStdString();
        user->compliance_docs.liability_insurance.expiration_date = query.value(12).toString().toStdString();
        user->compliance_docs.food_handler.certification_number   = query.value(13).toString().toStdString();
        user->compliance_docs.food_handler.expiration_date        = query.value(14).toString().toStdString();
        return(1);
    }

    return(0);
}

void UserSystem::add_user(User user)
{
    Assert(0, "TODO: insert to the database");
#if 0
    std::string query_string =
        "INSERT INTO user"
        " (user_id, username, email, phone, mail_address, owner_name, business_name, user_type, bl_licence, bl_expiration_date, li_policy_number, li_provider, li_expiration_date, fh_certification_number, fh_expiration_date)"
        " VALUES (" +
        std::string(user.id) + "," // andwu: TODO: how do we convert??
        user.creds.username + ","
        user.email + ","
        user.phone_number + ","
        user.mail_address + ","
        user.owner_name + ","
        user.business_name + ","
        user.perms.user_type + ","
        user.compliance_docs.business_licence.number + ","
        user.compliance_docs.business_licence.expiration_date + ","
        user.compliance_docs.liability_insurance.policy_number + ","
        user.compliance_docs.liability_insurance.provider + ","
        user.compliance_docs.liability_insurance.expiration_date + ","
        user.compliance_docs.food_handler.certification_number + ","
        user.compliance_docs.food_handler.expiration_date
        + ")";
    QSqlQuery query(QString(query_string.c_str())); // andwu: what??
    {
    }
#endif
}

void UserSystem::update_user(
        User user)
{
    std::string query_string =
        "UPDATE users SET"
        " username = :username, email = :email, phone = :phone,"
        " mail_address = :mail, owner_name = :owner, business_name = :business,"
        " user_type = :usertype, bl_licence = :compliance_licence,"
        " bl_expiration_date = :compliance_exp, li_policy_number = :liability_policy,"
        " li_provider = :liability_insurance, li_expiration_date = :liability_exp,"
        " fh_certification_number = :food_certification, fh_expiration_date = :food_expiration"
        " WHERE users.user_id = :id";

    QSqlQuery query;
    query.prepare(QString(query_string.c_str()));
    {
        query.bindValue(":id", (int)user.id.id);
        query.bindValue(":username", QString(user.creds.username.c_str()));
        query.bindValue(":email", QString(user.email.c_str()));
        query.bindValue(":phone", QString(user.phone_number.c_str()));
        query.bindValue(":mail", QString(user.mail_address.c_str()));
        query.bindValue(":owner", QString(user.owner_name.c_str()));
        query.bindValue(":business", QString(user.business_name.c_str()));
        query.bindValue(":usertype", (int)user.perms.user_type);
        query.bindValue(":compliance_licence", QString(user.compliance_docs.business_licence.number.c_str()));
        query.bindValue(":compliance_exp", QString(user.compliance_docs.business_licence.expiration_date.c_str()));
        query.bindValue(":liability_policy", QString(user.compliance_docs.liability_insurance.policy_number.c_str()));
        query.bindValue(":liability_insurance", QString(user.compliance_docs.liability_insurance.provider.c_str()));
        query.bindValue(":liability_exp", QString(user.compliance_docs.liability_insurance.expiration_date.c_str()));
        query.bindValue(":food_certification", QString(user.compliance_docs.food_handler.certification_number.c_str()));
        query.bindValue(":food_expiration", QString(user.compliance_docs.food_handler.expiration_date.c_str()));
    }
    Assert(query.exec(), "update user failed");
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

void MarketDateSystem::add_market_date(
        Date date, uint64_t food_limit, uint64_t artisan_limit)
{
    std::string query_string =
        "INSERT INTO market_dates (year, month, day)  VALUES(:year, :month, :day)";
    QSqlQuery query;
    query.prepare(QString(query_string.c_str()));
    query.bindValue(":year", QString::fromStdString(std::to_string(date.year)));
    query.bindValue(":month", QString::fromStdString(std::to_string(date.month)));
    query.bindValue(":day", QString::fromStdString(std::to_string(date.day)));
    query.exec();
}

std::string MarketDateSystem::date_str_from_id(MarketDateId market_date_id)
{
    std::string query_string =
        "SELECT * FROM market_dates WHERE market_dates.id = :id";
    QSqlQuery query;
    query.prepare(QString(query_string.c_str()));
    query.bindValue(":id", QString::fromStdString(std::to_string(market_date_id.id)));
    query.exec();
    std::string str = "";
    if(query.next())
    {
        uint64_t year = (uint64_t)query.value("year").toInt();
        uint64_t month = (uint64_t)query.value("month").toInt();
        uint64_t day = (uint64_t)query.value("day").toInt();
        return(std::to_string(day) + "/" + std::to_string(month) + "/" + std::to_string(year));
    }
    return(str);
}

int MarketDateSystem::make_booking(UserId user, MarketDateId market_date_id)
{
//    Assert(0, "TODO: we want to insert a booking into the booking list"
//            "for this date and type."
//            "Then query what that position is and the limit,"
//            "so that we can have a popup about being waitlist/booked");

    // GET USER TYPE
    std::string query_string;
    QSqlQuery query;
    query_string = std::string("SELECT user_type FROM users where user_id = :id;");
    query.prepare(QString(query_string.c_str()));

    query.bindValue(":id", QString::fromStdString(std::to_string(user.id)));
//    query.exec();
    Assert(query.exec(), "Query for User Types fail");

    int userType;
    while(query.next()){
        userType = query.value("user_type").toInt();
    }

    if (userType != USER_TYPE_ARTISAN && userType != USER_TYPE_FOOD){
        return(-1);
    }

    // CHECK IF ALREADY BOOKED
    int ayear = -1;
    int aday = -1;
    int amonth = -1;
    query_string = std::string("SELECT year, month, day FROM market_dates where id = :id");
    query.prepare(QString(query_string.c_str()));
    query.bindValue(":id", QString::fromStdString(std::to_string(market_date_id.id)));
    Assert(query.exec(), "Query for if user already booked fail");
    while(query.next()){
        ayear = query.value("year").toInt();
        amonth = query.value("month").toInt();
        aday = query.value("day").toInt();
    }
    query_string = std::string("SELECT user_id FROM bookings where user_id = :id AND for_year = :year AND for_month = :month AND for_day = :day");
    query.prepare(QString(query_string.c_str()));
    query.bindValue(":id", QString::fromStdString(std::to_string(user.id)));
    query.bindValue(":month", QString::fromStdString(std::to_string(amonth)));
    query.bindValue(":day", QString::fromStdString(std::to_string(aday)));
    query.bindValue(":year", QString::fromStdString(std::to_string(ayear)));
    Assert(query.exec(), "Query for if user already booked fail");
    while(query.next()){
        return -1;
    }

    // QUERY THE USER TYPE
    query_string = std::string(
                "SELECT user_type FROM users"
                " WHERE user_id = ?");
    query.prepare(QString(query_string.c_str()));
    query.addBindValue((int)user.id);
    query.exec();
    int user_type = 0;
    if(query.next())
    {
        user_type = query.value(0).toInt();
    }

    query_string = std::string(
                "SELECT COUNT(booking_id) FROM bookings JOIN users "
                "WHERE market_id = ? AND users.user_id = bookings.user_id AND users.user_type = ?");
    query.prepare(QString(query_string.c_str()));
    query.addBindValue((int)market_date_id.id);
    query.addBindValue(user_type);

    Assert(query.exec(), "Fail counting bookings");
    int numBookings = 0;

    if(query.next())
    {
        numBookings = query.value(0).toInt();
    }

    // GET LIMIT
    int limit = 0;
    int year = 0;
    int month = 0;
    int day = 0;

    if (userType == USER_TYPE_ARTISAN){
        query_string = std::string("SELECT artisan_limit,year,day,month FROM market_dates where id = :id;");
        query.prepare(QString(query_string.c_str()));

        query.bindValue(":id", QString::fromStdString(std::to_string(market_date_id.id)));
        Assert(query.exec(), "Query for Limit fail");
        while(query.next()){
            limit = query.value("artisan_limit").toInt();
            year = query.value("year").toInt();
            month = query.value("month").toInt();
            day = query.value("day").toInt();
        }
    }
    else if(userType == USER_TYPE_FOOD){
        query_string = std::string("SELECT food_limit,year,day,month FROM market_dates where id = :id;");
        query.prepare(QString(query_string.c_str()));

        query.bindValue(":id", QString::fromStdString(std::to_string(market_date_id.id)));
        Assert(query.exec(), "Query for Limit fail");
        while(query.next()){
            limit = query.value("food_limit").toInt();
            year = query.value("year").toInt();
            month = query.value("month").toInt();
            day = query.value("day").toInt();
        }
    }

    // INSERT BOOKING (unsure if need to insert time created or not)
    query_string = "INSERT INTO bookings (for_year, for_month, for_day, user_id, market_id) VALUES(:year, :month, :day, :user_id, :market_id);";
    query.prepare(QString(query_string.c_str()));
    query.bindValue(":year", QString::fromStdString(std::to_string(year)));
    query.bindValue(":month", QString::fromStdString(std::to_string(month)));
    query.bindValue(":day", QString::fromStdString(std::to_string(day)));
    query.bindValue(":user_id", QString::fromStdString(std::to_string(user.id)));
    query.bindValue(":market_id", QString::fromStdString(std::to_string(market_date_id.id)));
    Assert(query.exec(), "Insert Booking fail");

    QMessageBox msgBox;
    std::stringstream s;
    std::string date = std::to_string(month) + "/" + std::to_string(day + 1) + "/" + std::to_string(year + 1900);

    // if waitlisted (# of bookings already reached the limit) put message
    if (numBookings >= limit)
    {
        int waitlist_position = numBookings + 1 - limit;

        // Set up waitlist message
        QString qs = QString("You have been put on a waitlist for. You are in position %1.")
            .arg(waitlist_position);
        msgBox.setText(qs);
        msgBox.exec();

        s << "[Action] Waitlisted in position " << waitlist_position << " for " << date << ".";
        notification_system->add_notification(user, s.str());

        query_string = "UPDATE users SET is_waitlist = 1 WHERE user_id = ?";
        query.prepare(QString(query_string.c_str()));
        query.addBindValue((int)user.id);

        if(query.exec())
        {
            qDebug() << "Successfully put vendor in waitlist!";
        }
    }
    else
    {
        s << "[Action] Booked for " << date << ".";
        notification_system->add_notification(user, s.str());

        if(user_type == USER_TYPE_FOOD)
        {
            query_string = std::string(
                        "UPDATE market_dates SET food_booked = food_booked + 1"
                        " WHERE id = ?");
        }
        else if(user_type == USER_TYPE_ARTISAN)
        {
            query_string = std::string(
                        "UPDATE market_dates SET artisan_booked = artisan_booked + 1"
                        " WHERE id = ?");
        }
        query.prepare(QString(query_string.c_str()));
        query.addBindValue((int)market_date_id.id);
        query.exec();
    }

    return(0);
}

void MarketDateSystem::cancel_booking(UserId user, MarketDateId market_date_id)
{
    // andwu: TODO: maybe we use a delete style of: booking_id, user_id
    std::stringstream notification_msg;
    QSqlQuery query;

    std::string query_string =
            "DELETE FROM bookings WHERE user_id = ? AND market_id = ?";
    query.prepare(QString(query_string.c_str()));
    query.addBindValue((int)user.id);
    query.addBindValue((int)market_date_id.id);

    if(query.exec())
    {
        if (query.numRowsAffected() > 0)
        {
            qDebug() << "Successfully deleted booking, rows affected" <<
                        query.numRowsAffected();
        }
        else
        {
            qDebug() << "No rows were found.";
            return;
        }
    }

    // CANCELLATION ACTION NOTIFICATION
    query_string = std::string(
                "SELECT year, month, day FROM market_dates"
                " WHERE id = ?");
    query.prepare(QString(query_string.c_str()));
    query.addBindValue((int)market_date_id.id);

    int year, month, day;
    if(query.exec() && query.next())
    {
        year  = query.value(0).toInt() + 1900;
        month = query.value(1).toInt();
        day   = query.value(2).toInt() + 1;
    }
    else
    {
        qDebug() << "Something went wrong with getting market_date!";
        return;
    }

    QDate date(year, month, day);
    QString formatted_date = date.toString("MM/dd/yyyy");
    notification_msg << "[Action] Cancelled " << formatted_date.toStdString()
                     << ".";
    notification_system->add_notification(user, notification_msg.str());

    // UPDATE MARKET DATE AVAILABILITY
    // Get the user type
    query_string = std::string("SELECT user_type FROM users where user_id = :id;");
    query.prepare(QString(query_string.c_str()));

    query.bindValue(":id", QString::fromStdString(std::to_string(user.id)));
    Assert(query.exec(), "Query for User Types fail");

    int userType;
    while(query.next()){
        userType = query.value("user_type").toInt();
    }

    // Update availability
    if(userType == USER_TYPE_FOOD)
    {
        query_string = std::string(
                    "UPDATE market_dates SET food_booked = food_booked + 1"
                    " WHERE id = ? ");
    }
    else if (userType == USER_TYPE_ARTISAN)
    {
        query_string = std::string(
                    "UPDATE market_dates SET artisan_booked = artisan_booked + 1"
                    " WHERE id = ? ");
    }

    query.prepare(QString(query_string.c_str()));
    query.addBindValue((int)market_date_id.id);
    query.exec();

    // NOTIFY WAITLIST
    query_string = std::string(
                "SELECT user_id FROM bookings"
                " WHERE market_id = ?"
                " ORDER BY creation_date ASC");
    query.prepare(QString(query_string.c_str()));
    query.addBindValue((int)market_date_id.id);
    query.exec();

    UserId waitlist_user_id = { 0 };
    waitlist_user_id.id = 0;

    if(query.next())
    {
        waitlist_user_id.id = (uint64_t)query.value(0).toInt();
    }

    if (waitlist_user_id.id == 0) { return; }

    notification_msg.str("");
    notification_msg.clear();

    notification_msg << "[Alert] Available spot for " << formatted_date.toStdString() << ".";
    notification_system->add_notification(waitlist_user_id, notification_msg.str());

    // BOOK WAITLIST VENDOR
    query_string = std::string(
                "UPDATE bookings SET is_waitlist = 0"
                " WHERE user_id = ? AND market_id = ?");
    query.prepare(QString(query_string.c_str()));
    query.addBindValue((int)waitlist_user_id.id);
    query.addBindValue((int)market_date_id.id);
    query.exec();

#if 0
    uint64_t *booked = nullptr;
    uint64_t *limit = nullptr;
    std::vector<UserId> *booking_list = nullptr;
    std::stringstream notification_msg;

    if (user->perms.user_type == (USER_TYPE) USER_TYPE_ARTISAN)
    {
        booking_list = &market_dates[market_date_index].artisan_booking.users;
        booked = &market_dates[market_date_index].artisan_booking.booked;
        limit = &market_dates[market_date_index].artisan_booking.limit;
    }
    else if (user->perms.user_type == (USER_TYPE) USER_TYPE_FOOD)
    {
        booking_list = &market_dates[market_date_index].food_booking.users;
        booked = &market_dates[market_date_index].food_booking.booked;
        limit = &market_dates[market_date_index].food_booking.limit;
    }

    if (booking_list == nullptr) { return; }

    for (uint32_t i = 0; i < booking_list->size(); i++)
    {
        if (user->id == (*booking_list)[i])
        {
            booking_list->erase(booking_list->begin() + i);


            // Check if it's a waitlist position
            if (i > *limit)
            {
                // WAITLIST
                notification_msg << "[Action] Cancelled waitlist for " <<
                                    market_dates[market_date_index].date.to_string() << ".";
                notification_system->add_notification(user->id, notification_msg.str());
            }
            else
            {
                // BOOKING
                notification_msg << "[Action] Cancelled booking for " <<
                                    market_dates[market_date_index].date.to_string() << ".";
                notification_system->add_notification(user->id, notification_msg.str());
                (*booked)--;

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
            }

            break;
        }
    }
#endif
}

bool is_date_real(MarketDateId market_date_id)
{
    std::string query_string =
            "SELECT user_id FROM bookings"
            " WHERE id = :id";
    QSqlQuery query;
    query.prepare(QString(query_string.c_str()));
    {
        query.bindValue(":id", (int)market_date_id.id);
    }
    Assert(query.exec(), "Query for if user already booked fail");
    if(query.next()){
        return 1;
    }
    return 0;
}

bool MarketDateSystem::is_user_booked(UserId user_id, MarketDateId market_date_id)
{
//    Assert(0, "TODO: check if this market date is booked by the user");
    std::string query_string;
    QSqlQuery query;

    int year = -1;
    int day = -1;
    int month = -1;
    query_string = std::string("SELECT year, month, day FROM market_dates where id = :id");
    query.prepare(QString(query_string.c_str()));
    query.bindValue(":id", QString::fromStdString(std::to_string(market_date_id.id)));
//    query.exec();
    Assert(query.exec(), "Query for if user already booked fail");
    while(query.next()){
        year = query.value("year").toInt();
        month = query.value("month").toInt();
        day = query.value("day").toInt();
    }
    query_string = std::string("SELECT user_id FROM bookings where user_id = :id AND for_year = :year AND for_month = :month AND for_day = :day");
    query.prepare(QString(query_string.c_str()));
    query.bindValue(":id", QString::fromStdString(std::to_string(user_id.id)));
    query.bindValue(":month", QString::fromStdString(std::to_string(month)));
    query.bindValue(":day", QString::fromStdString(std::to_string(day)));
    query.bindValue(":year", QString::fromStdString(std::to_string(year)));
//    query.exec();
    Assert(query.exec(), "Query for if user already booked fail");
    while(query.next()){
        return 1;
    }
    return 0;

//    std::string query_string =
//        "SELECT * FROM bookings"
//        " ORDER BY bookings.year ASC, bookings.month ASC, bookings.day ASC";
//    QSqlQuery query;
//    query.prepare(QString(query_string.c_str()));
//    {
//        for(;query.next();)
//        {
//            UserId booking_user_id = UserId{ (uint64_t)query.value(4).toInt() };
//            if(booking_user_id == user_id)
//            {
//                return(1);
//            }
//        }
//    }
//    return(0);
}

bool MarketDateSystem::has_any_market_dates(void)
{
    std::string query_string =
            "SELECT * FROM market_dates;";
    QSqlQuery query;
    query.prepare(QString(query_string.c_str()));
    if(!query.exec())
    {
        fprintf(stderr, "DB Query: %s\n", query.lastError().text().toStdString().c_str());
        Assert(0, "database failed query");
        return(0);
    }

    return(query.next());
}

// -------------------------
// -- NOTIFICATION SYSTEM --
// -------------------------
std::vector<std::string> NotificationSystem::get_notifications(UserId id) {
//    Assert(0, "TODO: query for notifications");

    std::vector<std::string> notifications;

    std::string query_string =
            "SELECT content FROM notifications"
            " WHERE notifications.user_id = ? OR notifications.for_all_users != 0"
            " ORDER BY year ASC, month ASC, day ASC";

    QSqlQuery query;
    if(query.prepare(QString(query_string.c_str())))
    {
        query.addBindValue((int)id.id);
        query.exec();
        {
            for(;query.next();)
            {
                std::string content = query.value(0).toString().toStdString();
                notifications.push_back(content);
            }
        }
    }

    return(notifications);
}

void NotificationSystem::add_notification(UserId id, std::string content) {
    std::string query_string =
        "INSERT INTO notifications (year, month, day, user_id, content)"
        " VALUES(?, ?, ?, ?, ?)";
    QSqlQuery query;
    query.prepare(QString(query_string.c_str()));

    QDate date = QDate::currentDate();
    query.addBindValue(date.year());
    query.addBindValue(date.month());
    query.addBindValue(date.day());

    query.addBindValue((int)id.id);
    query.addBindValue(content.c_str());
    query.exec();
}
