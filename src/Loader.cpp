//
// Created by darkhead on 12/6/20.
//

#include <iostream>
#include "../include/Loader.h"

void Loader::LoadSimulationSpecimens(int max_by_specie, Timer *timer, int screen_width, int screen_height, std::vector<Specie> &types, std::vector <Specimen*> &specimens)
{
    float divisor = 1;
    for (int j = 0 ; j < types.size(); j++) {
        int num_specimens =  (int) ( (float) max_by_specie/types[j].genome.size * 75.0f);
        num_specimens *= FACTOR_NUM_SPECIES_BY_SICE;
        std::cout << "Loading " << num_specimens << " " << types[j].name << " | " ;

        for (int i = 0 ; i < num_specimens; i++) {
            auto *specimen = new Specimen( specimens.size() );
            specimen->setSpecie( types[ j ] );
            specimen->setTimer( timer);
            specimen->setRandomPosition(screen_width, screen_height);
            specimen->setRandomDirection();
            specimens.push_back( specimen );
        }
        divisor*=5;
    }

    std::cout << std::endl ;

}

void Loader::ResetSimulation(int max_by_specie, Timer *timer, int screen_width, int screen_height, std::vector<Specie> &types, std::vector <Specimen*> &specimens)
{
    for (int i = 0 ; i < specimens.size(); i++) {
        delete specimens[i];
    }

    specimens.clear();

    Loader::LoadSimulationSpecimens(max_by_specie, timer, screen_width, screen_height, types, specimens);
}
