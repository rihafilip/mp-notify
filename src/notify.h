#ifndef NOTIFY_H
#define NOTIFY_H

#include <string>
#include <filesystem>

class Notify
{
public:
    Notify( const std::string & appName );
    ~Notify();
    bool sendNotify ( const std::string & head, const std::string & body, const std::filesystem::path & image );
};

#endif // NOTIFY_H
