//
// Created by darkhead on 15/6/20.
//

#include <vector>
#include <glm/vec2.hpp>
#include "Specie.h"

#ifndef EVOL_TYPES_H
#define EVOL_TYPES_H

class Types {

public:
    struct chartFrameInfo {
        float second;
        int population;
    };

    struct TypesContainer {
        Specie specie;
        std::vector<chartFrameInfo> frameInfo;
    };

    static int random(int min, int max) //range : [min, max)
    {
        static bool first = true;
        if (first)
        {
            srand( time(NULL) ); //seeding for the first time only!
            first = false;
        }

        int r = min + rand() % (( max + 1 ) - min);

        return -1 * r;
    }

};


#endif //EVOL_TYPES_H
