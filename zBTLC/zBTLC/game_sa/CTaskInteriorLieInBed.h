#pragma once

#include "plbase/PluginBase_SA.h"
#include "CTaskSimple.h"

class CTaskInteriorLieInBed : public CTaskSimple {
protected:
    CTaskInteriorLieInBed(plugin::dummy_func_t a) : CTaskSimple(a) {}
public:
    
};

//VALIDATE_SIZE(CTaskInteriorLieInBed, 0x);