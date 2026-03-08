#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <chrono>

#define DebugTrap() __builtin_trap()
#define DebugLog(fmt,...) fprintf(stderr, fmt __VA_OPT__(,) __VA_ARGS__)
#define Assert(b,fmt,...) do { if(!(b)) { DebugLog(fmt __VA_OPT__(,) __VA_ARGS__); DebugTrap(); } } while(0)

//~ time
#define SecondsInADay 86400ull

class Date
{
public:
    uint64_t day;

    static Date date_now(void) // andwu: WARN: REALLY WRONG
    {
        uint64_t t = (uint64_t)time(NULL);
        Date date;
        date.day = t / SecondsInADay;
        return(date);
    }
};

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

class User
{
public:
    Permissions perms;
    Credentials creds;
    std::string name;
    std::string business_name;
    std::string owner_name;
    std::string email;
    std::string mail_address;
    ComplianceDocs compliance_docs;
};

class Admin : User
{
public:
};

class Organizer : User
{
public:
};

class ArtisanVendor : User
{
public:
};

class FoodVendor : User
{
public:
};
