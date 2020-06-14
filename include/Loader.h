//
// Created by darkhead on 12/6/20.
//

#ifndef EVOL_LOADER_H
#define EVOL_LOADER_H

#include <vector>
#include "Specimen.h"

class Loader {

public:
    static void LoadSimulationSpecimens( int max_by_specie, Timer *timer, int screen_width, int screen_height, std::vector<Specie> &types, std::vector <Specimen*> &specimens );
    static void ResetSimulation(int max_by_specie, Timer *timer, int screen_width, int screen_height, std::vector<Specie> &types, std::vector <Specimen*> &specimens);
};


#endif //EVOL_LOADER_H
