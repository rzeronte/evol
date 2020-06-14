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
        this->genome.size = 12.5f;
        this->genome.speed = 1.75f;
        this->genome.perception = 200.0f;
        this->genome.energy = 1000.0f;
        this->genome.brain_tick = 0.10f;

        this->genome.sociability = 25.0f;
        this->genome.sexuality = 35.0f;
        this->genome.belligerence = 75;

        this->morphology.color = Colors::green();
    }

    void evaluate() override {
    }

    void sex(Specie *another) override {

    }

};

#endif //EVOL_ANDREITA_H
