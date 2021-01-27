#ifndef MUSIC_PLAYER_H
#define MUSIC_PLAYER_H

#include <iostream>
#include <string>

#include <vlc/vlc.h>

using namespace std;

class Music_Player
{
    libvlc_instance_t *m_inst = NULL;		//libvlc的对象指针
    libvlc_media_player_t *m_player = NULL;
    libvlc_media_t *m_media = NULL;

public:
    Music_Player()
    {
        m_inst = libvlc_new(0, NULL);
    }
    ~Music_Player()
    {
        stop_play();
        libvlc_release(m_inst);
    }

    void start_play(string music_path)
    {
        if(is_media_player_playing() == true)
        {
            stop_play();
        }
        m_media = libvlc_media_new_location(m_inst, ("file://" + music_path).c_str());
        m_player = libvlc_media_player_new_from_media(m_media);
        libvlc_media_release(m_media);
        libvlc_media_player_play(m_player);
    }
    void stop_play()
    {
        if(is_media_player_playing() == true)
        {
            libvlc_media_player_stop(m_player);
            libvlc_media_player_release(m_player);
            m_player = NULL;
        }
    }
    bool is_media_player_playing()
    {
        if(m_player != NULL)
        {
            return libvlc_media_player_is_playing(m_player);
        }
        else
        {
            return false;
        }
    }
};


#endif // MUSIC_PLAYER_H
