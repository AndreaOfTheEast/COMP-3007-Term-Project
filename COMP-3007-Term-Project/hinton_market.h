#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cstdint>

#define DebugTrap() __builtin_trap()
#define DebugLog(fmt,...) fprintf(stderr, fmt __VA_OPT__(,) __VA_ARGS__)
#define Assert(b,fmt,...) do { if(!(b)) { DebugLog(fmt __VA_OPT__(,) __VA_ARGS__); DebugTrap(); } } while(0)

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

class User
{
public:
    Permissions perms;
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
