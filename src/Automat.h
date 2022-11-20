#ifndef AUTOMAT_H
#define AUTOMAT_H

#include <stdint.h>

class Automat
{
    uint32_t m_previousMillis = 0;
    uint32_t m_interval = -1;

public:
    Automat(uint32_t interval)
    {
        m_interval = interval;
    }

    bool tick(uint32_t timeMillis)
    {
        if (timeMillis - m_previousMillis > m_interval)
        {
            m_previousMillis = timeMillis;
            return true;
        }
        else
        {
            return false;
        }
    }
};

#endif