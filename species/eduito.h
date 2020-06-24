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
        this->genome.size = 10.0f;
        this->genome.brain_tick = 0.10f;

        this->genome.sociability = 75.0f;
        this->genome.sexuality = 99.0f;
        this->genome.belligerence = 75.0f;

        this->morphology.color = Colors::red();
        this->updateRacials();

    }

    void evaluate() override {
    }

    void sex(Specie *another) override {

    }

};

#endif //EVOL_EDUITO_H
