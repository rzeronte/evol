//
// Created by darkhead on 10/6/20.
//

#ifndef EVOL_EDUITO_H
#define EVOL_EDUITO_H

#include "../include/Specie.h"
#include "../include/Colors.h"

class eduito : public Specie {
public:
    eduito() {
        this->name = "eduito";
        this->genome.size = 20.0f;
        this->genome.speed = 1.5f;
        this->genome.perception = 150.0f;

        this->genome.sociability = 25.f;
        this->genome.sexuality = 25.0f;
        this->genome.belligerence = 25;

        this->genome.energy = 1000.0f;
        this->genome.brain_tick = 0.10f;
        this->morphology.color = Colors::red();
    }

    void evaluate() override {
    }

    void sex(Specie *another) override {

    }

};

#endif //EVOL_EDUITO_H
