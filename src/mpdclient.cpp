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
        mpd_idle list = mpd_run_idle_mask( m_Connection, mpd_idle::MPD_IDLE_PLAYER );

        m_Notify.sendNotify( "Player change", std::to_string((list ^ mpd_idle::MPD_IDLE_PLAYER) == 0), std::filesystem::path("") );
    }
}
