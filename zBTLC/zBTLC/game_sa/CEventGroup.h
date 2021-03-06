#pragma once

#include "plbase/PluginBase_SA.h"

class CEventGroup {
protected:
    void *vtable;
public:
    class CPed   *m_pPed;
    unsigned int  m_dwCount;
    void         *m_apEvents[16];
};

VALIDATE_SIZE(CEventGroup, 0x4C);