#include "SC_Plugin.h"
#include "SC_RGen.h"

static InterfaceTable *ft;

struct UGEN_NAME : public Unit {

};

extern "C" {
  void load(InterfaceTable* inTable);
  void UGEN_NAME_Ctor(UGEN_NAME *unit);
  void UGEN_NAME_next(UGEN_NAME *unit, int inNumSamples);
  void UGEN_NAME_Dtor(UGEN_NAME *unit);
}

void UGEN_NAME_Ctor( UGEN_NAME *unit) {
    
    SETCALC(UGEN_NAME_next);
    UGEN_NAME_next(unit, 1);
}

void UGEN_NAME_next( UGEN_NAME* unit, int inNumSamples ){

    float *in = IN(0);
    float *out = OUT(0);

    for(int i=0; i< inNumSamples; ++i){
        val = in[i];

        out[i] = val + (delayed * depth);
    }
}

void UGEN_NAME_Dtor( UGEN_NAME* unit){

}

PluginLoad(UGEN_NAME){
  ft = inTable;

  DefineDtorUnit(UGEN_NAME);
}