//
// Created by darkhead on 9/6/20.
//

#ifndef EVOL_GENOME_H
#define EVOL_GENOME_H


#include "Timer.h"

class Genome {

public:
    float speed = 1;
    float energy = 100;
    float size = 1;
    float brain_tick = 1;
    float perception = 10;

    int sociability = 100;
    int belligerence = 100;
    int sexuality = 100;
};


#endif //EVOL_GENOME_H
