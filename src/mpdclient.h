#ifndef MPDCLIENT_H
#define MPDCLIENT_H
#include "notify.h"

#include <mpd/client.h>

class MpdClient
{
private:
    Notify m_Notify;
    mpd_connection * m_Connection;

    std::string retrieveAllTag ( mpd_song * song, mpd_tag_type tag );
    void notifyPlay();
    void downloadArt( const std::filesystem::path & path );

public:
    MpdClient();
    ~MpdClient();

    void listen ();
};

#endif // MPDCLIENT_H
