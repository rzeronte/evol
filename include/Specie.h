//
// Created by darkhead on 9/6/20.
//

#ifndef EVOL_SPECIE_H
#define EVOL_SPECIE_H


#include <string>
#include <iostream>
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

    void updateRacials()
    {
        this->genome.speed = 10.0f / (this->genome.size * 0.5f ) ;
        this->genome.perception = this->genome.size * 15.25;
        this->genome.energy = (1 / this->genome.size*2) * 10000.0f;

        std::cout << "Type: : " << this->name << std::endl;
        std::cout << "Speed: " << this->genome.speed << std::endl;
        std::cout << "perception: " << this->genome.perception << std::endl;
        std::cout << "energy: " << this->genome.energy << std::endl << std::endl;
    }
};


#endif //EVOL_SPECIE_H
