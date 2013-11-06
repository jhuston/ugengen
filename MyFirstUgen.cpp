#include "SC_Plugin.h"
#include "SC_RGen.h"

static InterfaceTable *ft;

struct MyFirstUgen : public Unit {
    float rate, delaysize, fwdhop, readpos;
    int32 writepos;

    float* delayline;
};

extern "C" {
  void load(InterfaceTable* inTable);
  void MyFirstUgen_Ctor(MyFirstUgen *unit);
  void MyFirstUgen_next(MyFirstUgen *unit, int inNumSamples);
  void MyFirstUgen_Dtor(MyFirstUgen *unit);
}

void MyFirstUgen_Ctor( MyFirstUgen *unit) {

    unit->delaysize = SAMPLERATE * 0.02f;
    float rate = IN0(1);
    float delta = (unit->delaysize * rate) / SAMPLERATE;
    unit->fwdhop = delta + 1.0f;
    unit->rate = rate;
    unit->writepos = 0;
    unit->readpos = 0;

    //Allocate the delay line
    unit->delayline = (float*)RTAlloc(unit->mWorld, unit->delaysize * sizeof(float));
    memset(unit->delayline, 0, unit->delaysize * sizeof(float));
    
    SETCALC(MyFirstUgen_next);
    MyFirstUgen_next(unit, 1);
}

void MyFirstUgen_next( MyFirstUgen* unit, int inNumSamples ){

    float *in = IN(0);
    float *out = OUT(0);

    float depth = IN0(2);

    float rate = unit->rate;
    float fwdhop = unit->fwdhop;
    float readpos = unit->readpos;
    float* delayline = unit->delayline;
    int writepos = unit->writepos;
    int delaysize = unit->delaysize;

    float val, delayed, currate;

    currate = IN0(1);

    if(rate != currate){
        rate = currate;
        fwdhop = ((delaysize * rate * 2) / SAMPLERATE) + 1.0f;
    }

    for(int i=0; i< inNumSamples; ++i){
        val = in[i];

        delayline[writepos++] = val;
        if(writepos == delaysize){
            writepos = 0;
        }
        delayed = delayline[(int)readpos];
        readpos += fwdhop;

        while((int)readpos >= delaysize){
            readpos -= delaysize;
        }
        while((int)readpos < 0){
            readpos += delaysize;
        }
        out[i] = val + (delayed * depth);
    }

    unit->rate = rate;
    unit->fwdhop = fwdhop;
    unit->writepos = writepos;
    unit->readpos = readpos;
}

void MyFirstUgen_Dtor( MyFirstUgen* unit){
    RTFree(unit->mWorld, unit->delayline);
}

PluginLoad(MyFirstUgen){
  ft = inTable;

  DefineDtorUnit(MyFirstUgen);
}
