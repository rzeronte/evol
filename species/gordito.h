//
// Created by darkhead on 14/6/20.
//

#ifndef EVOL_GORDITO_H
#define EVOL_GORDITO_H

#include "../include/Specie.h"
#include "../include/Colors.h"

class gordito : public Specie {
public:
    gordito() {
        this->name = "gordito";
        this->genome.size = 30.0f;

        this->genome.sociability = 75.f;
        this->genome.sexuality = 5.0f;
        this->genome.belligerence = 50;

        this->genome.brain_tick = 0.10f;
        this->morphology.color = Colors::yellow();
        this->updateRacials();
    }

    void evaluate() override {
    }

    void sex(Specie *another) override {

    }

};
#endif //EVOL_GORDITO_H
