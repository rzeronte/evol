//
// Created by darkhead on 10/6/20.
//

#ifndef EVOL_ANDREITA_H
#define EVOL_ANDREITA_H
#include "../include/Specie.h"

class andreita : public Specie {
public:
    andreita() {
        this->name = "andreito";
        this->genome.size = 10.0f;

        this->genome.sociability = 51.0f;
        this->genome.sexuality = 15.0f;
        this->genome.belligerence = 5;

        this->genome.brain_tick = 0.10f;
        this->genome.belligerence = 75;

        this->morphology.color = Colors::green();
        this->updateRacials();
    }

    void evaluate() override {
    }

    void sex(Specie *another) override {

    }

};

#endif //EVOL_ANDREITA_H
