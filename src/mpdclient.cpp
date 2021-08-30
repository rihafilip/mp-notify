#include "mpdclient.h"
#include <stdexcept>
#include <iostream>
#include <fstream>

MpdClient::MpdClient()
    : m_Notify("MP-Notify"), m_Connection(nullptr)
{
    m_Connection = mpd_connection_new("localhost", 6600, 0);
    if (m_Connection == nullptr)
        throw std::runtime_error("Unable to establish MPD connection");
}

MpdClient::~MpdClient()
{
    mpd_connection_free(m_Connection);
}

void MpdClient::listen()
{
    while (true)
    {
        mpd_idle list = mpd_run_idle_mask(m_Connection, mpd_idle::MPD_IDLE_PLAYER);
        if ((list ^ mpd_idle::MPD_IDLE_PLAYER) != 0)
            continue;

        mpd_status *status = mpd_run_status(m_Connection);
        mpd_state state = mpd_status_get_state(status);
        switch (state)
        {
        case MPD_STATE_PLAY:
            notifyPlay();
            break;

        case MPD_STATE_STOP:
            m_Notify.sendNotify("Paused", "", std::filesystem::path(""));
            break;
        case MPD_STATE_PAUSE:
            m_Notify.sendNotify("Stopped", "", std::filesystem::path(""));
            break;

        default:
            break;
        }

        mpd_status_free(status);
    }
}

void MpdClient::notifyPlay()
{
    mpd_song *song = mpd_run_current_song(m_Connection);
    if (song == nullptr)
        return;

    std::string head = retrieveAllTag(song, mpd_tag_type::MPD_TAG_ARTIST);
    std::string body = retrieveAllTag(song, mpd_tag_type::MPD_TAG_TITLE);

    using namespace std::filesystem;
    path cacheDir = absolute(getenv("HOME")) /= path(".cache") /= path("mp-notify");

    if (exists(cacheDir) && !is_directory(cacheDir))
        throw std::runtime_error("Cache path is not directory");
    else if (!exists(cacheDir) && !create_directories(cacheDir))
        throw std::runtime_error("Error creating cache directory");

    path artFile = cacheDir /= (head + body + ".png");
    std::cout << artFile << std::endl;
    if ( !exists(artFile) )
    {
        downloadArt( artFile );
    }

    m_Notify.sendNotify(head, body, artFile);

    mpd_song_free(song);
}

void MpdClient::downloadArt( const std::filesystem::path & path )
{
    constexpr int BUFFER_SIZE = 255;
    char buffer [BUFFER_SIZE];
    std::ofstream of { path.generic_string(), std::ios_base::out | std::ios_base::binary };

    if (!mpd_send_command(m_Connection, "albumart", nullptr))
        throw std::runtime_error("Error while requesting album art");

    // TODO check good writing
    while ( mpd_recv_binary( m_Connection, buffer, BUFFER_SIZE ) )
        of.write( buffer, BUFFER_SIZE );
}

std::string MpdClient::retrieveAllTag(mpd_song *song, mpd_tag_type tag)
{
    std::string output{};
    const char *value = nullptr;
    int i = 0;
    while ((value = mpd_song_get_tag(song, tag, i)) != nullptr)
    {
        if (i != 0)
            output.append(", ");
        output.append(value);
        ++i;
    }

    if (output.empty())
        return "?";
    else
        return output;
}
