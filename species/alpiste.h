//
// Created by darkhead on 13/6/20.
//

#ifndef EVOL_ALPISTE_H
#define EVOL_ALPISTE_H

#include "../include/Specie.h"
#include "../include/Colors.h"

class alpiste : public Specie {

    void evaluate() override {
    }

    void sex(Specie *another) override {

    }

public:
    alpiste() {
        this->name = "alpiste";
        this->genome.size = 10.0f;
        this->genome.brain_tick = 0.10f;

        this->genome.sociability = 75.0f;
        this->genome.sexuality = 75.0f;
        this->genome.belligerence = 75.0f;

        this->morphology.color = Colors::magenta();
        this->updateRacials();
    }
};
#endif //EVOL_ALPISTE_H
