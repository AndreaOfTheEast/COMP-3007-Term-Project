#include "hinton_market.h"
#include "market.h"

//~ andwu: user system
User *
UserSystem::get_user(Credentials creds)
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
