#ifndef GET_TIME_H
#define GET_TIME_H

#include <sys/time.h>

class Get_Time
{
    struct timeval m_timeval;
public:
    __int128 get_tv_time_now()
    {
        gettimeofday(&m_timeval, NULL);
        return (m_timeval.tv_sec * 1000000 + m_timeval.tv_usec);
    }
};

#endif // GET_TIME_H
