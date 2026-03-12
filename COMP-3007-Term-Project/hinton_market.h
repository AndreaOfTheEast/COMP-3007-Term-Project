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
class ComplianceDocs
{
public:
    std::string information;
};

class Permissions
{
public:
    int32_t schedule_date_range;
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
};

class User
{
public:
    UserId id;
    Permissions perms;
    Credentials creds;
    std::string business_name;
    std::string owner_name;
    std::string email;
    std::string mail_address;
    ComplianceDocs compliance_docs;
};

class Admin : public User
{
public:
};

class Operator : public User
{
public:
};

class ArtisanVendor : public User
{
public:
};

class FoodVendor : public User
{
public:
};

class Date
{
public:
    uint64_t day;

    static Date get_now() {
        Date date;
        date.day = (uint64_t)time(0) / 86400ull;
        return date;
    }
};

class Booking
{
public:
    Date date;

    std::vector<UserId> artisan_users;
    std::vector<UserId> food_users;

    uint64_t artisan_limit;
    uint64_t food_limit;
};
