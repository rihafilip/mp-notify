#ifndef MPDCLIENT_H
#define MPDCLIENT_H
#include "notify.h"

#include <mpd/client.h>

class MpdClient
{
private:
    Notify m_Notify;
    mpd_connection * m_Connection;

public:
    MpdClient();
    ~MpdClient();

    void listen ();
};

#endif // MPDCLIENT_H
