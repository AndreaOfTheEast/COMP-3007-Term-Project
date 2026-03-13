#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <time.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QDialog>
#include <QMessageBox>
#pragma GCC diagnostic pop

#define DebugTrap() __builtin_trap()
#define DebugLog(fmt,...) fprintf(stderr, fmt __VA_OPT__(,) __VA_ARGS__)
#define Assert(b,fmt,...) do { if(!(b)) { DebugLog(fmt __VA_OPT__(,) __VA_ARGS__); DebugTrap(); } } while(0)

//~ andwu: user
struct BusinessLicence
{
    std::string number;
    std::string expiration_date;
};

struct LiabilityInsurance
{
    std::string policy_number;
    std::string provider;
    std::string expiration_date;
};

struct FoodHandler
{
    std::string certification_number;
    std::string expiration_date;
};

class ComplianceDocs
{
public:
    BusinessLicence business_licence;
    LiabilityInsurance liability_insurance;
    FoodHandler food_handler;
};

#define USER_TYPE_LIST(X) \
    X(NULL) \
    X(FOOD) \
    X(ARTISAN) \
    X(OPERATOR) \
    X(ADMIN)

typedef enum : int8_t
{
    #define X(n) USER_TYPE_##n,
    USER_TYPE_LIST(X)
    #undef X
    USER_TYPE__COUNT,
} USER_TYPE;

static std::string user_type_strings[] =
{
    #define X(n) #n,
    USER_TYPE_LIST(X)
    #undef X
};

class Permissions
{
public:
    int32_t schedule_date_range;
    USER_TYPE user_type;
    int8_t // andwu: TODO: idk what these should be
        stall_book : 1,
        stall_book_cancel : 1,
        waitlist_book : 1,
        waitlist_book_cancel : 1;
};

class Credentials
{
public:
    std::string username;

    bool operator==(Credentials creds)
    {
        bool b = username == creds.username;
        return(b);
    }
};

class UserId {
public:
    uint64_t id;
    bool operator==(const UserId user_id)
    {
        bool b = id == user_id.id;
        return b;
    }
};

class User
{
public:
    UserId id;
    Permissions perms;
    Credentials creds;
    std::string business_name;
    std::string phone_number;
    std::string owner_name;
    std::string email;
    std::string mail_address;
    ComplianceDocs compliance_docs;
};

class Admin : public User
{
public:
    Admin() {
        perms.user_type = USER_TYPE_ADMIN;
    }
};

class Operator : public User
{
public:
    Operator() {
        perms.user_type = USER_TYPE_OPERATOR;
    }
};

class ArtisanVendor : public User
{
public:
    ArtisanVendor() {
        perms.user_type = USER_TYPE_ARTISAN;
    }
};

class FoodVendor : public User
{
public:
    FoodVendor() {
        perms.user_type = USER_TYPE_FOOD;
    }
};

class Date
{
public:
    uint64_t day;
    uint64_t month;
    uint64_t year;

    static Date get_now() {
        time_t now = std::time(NULL);
        struct tm *today = localtime(&now);

        Date date;
        date.day = today->tm_mday;
        date.month = today->tm_mon;
        date.year = today->tm_year;
        return date;
    }

    std::string to_string();
};

class MarketDate
{
public:
    Date date;
    std::vector<UserId> artisan_users;
    std::vector<UserId> food_users;
    uint64_t artisan_limit;
    uint64_t food_limit;
};
