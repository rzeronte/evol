//
// Created by darkhead on 9/6/20.
//

#ifndef EVOL_SPECIE_H
#define EVOL_SPECIE_H


#include <string>
#include "Behavior.h"
#include "Genome.h"
#include "Morphology.h"

class Specie {
public:
    Specie();

    // Identifier
    std::string name;

    // Attributes
    Behavior   behavior;
    Genome     genome;
    Morphology morphology;


    virtual void evaluate() {

    }

    virtual void sex( Specie *another) {

    }
};


#endif //EVOL_SPECIE_H
