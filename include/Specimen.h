//
// Created by darkhead on 10/6/20.
//

#ifndef EVOL_SPECIMEN_H
#define EVOL_SPECIMEN_H

#include "Specie.h"
#include <glm/glm.hpp>

#define MAX_ANGLE_RANDOM_ROTATION 25.0f

#define FRICTION 0.01

#define ESCAPE_SPEED_FACTOR 1.1f

#define FRICTION_FOLLOW 0.025f;
#define FOLLOW_CONFUSION 5.00f;
#define EPSILON_STOP_FOLLOW_GROUP 3.0f;

#define MAX_SPECIMENS_PERCEPTED 5

#define EATING_MARGING 2.5f
#define ENERGY_EATING_FACTOR 2.75f;
#define ENERGY_CONSUME_FACTOR 0.20f

#define FACTOR_NUM_SPECIES_BY_SICE 0.075f
#define SEX_FACTOR 0.025f

#define MAX_SIMULATION_SIZE_UNITS 10000;

inline int fastrand() {
    int g_seed = (214013*g_seed+2531011);
    return (g_seed>>16)&0x7FFF;
}

inline int randInRange(int nMin, int nMax)
{
    return nMin + (int)((double)rand() / (RAND_MAX+1) * (nMax-nMin+1));
}

class Specimen {
public:
    int id;

    Specie specie;

    glm::vec2 position;
    glm::vec2 direction;

    float initial_energy = 0;
    float current_speed = 0;
    float current_distance = 0;

    // Timer for brain tick
    Timer *timer;
    float lastTicks = 0;
    float accumulated_time_brain_thick = 0;

    Uint32 base_color;

    bool dead = false;

    Specimen *followTo = nullptr;

    std::vector<Specimen*> *percepted;
    std::vector<Specimen*> *inside;

    std::vector<Specimen*> *friends;
    std::vector<Specimen*> *enemies;
    std::vector<Specimen*> *eat;
    std::vector<Specimen*> *eated;

    Specimen *mostNearEnemy;
    Specimen *mostNearFriend;
    Specimen *mostNearEat;

    Specimen(int id);

    void setTimer(Timer *timer);

    void evaluate(Uint32 *buffer, int canvas_width, int canvas_height, std::vector<Specimen*> &specimens);

    void move(int limit_x, int limit_y);
    void applyFriction();
    void applyFollowTo();
    void applyEat();
    void applyFollowGroups(std::vector<Specimen*> *group, int noise_movement);
    void applyEscapeGroups(std::vector<Specimen*> *group);

    void applyCollisionLimitsImpulse(int  limit_x, int limit_y);
    void applyImpulse(float angle, float speed);
    void applyImpulseTo(glm::vec2 to, float speed);
    void applyRandomImpulse();
    void applyRandomEscape();

    void getSpecimensPercepted(std::vector<Specimen*> &specimens);

    void think(std::vector<Specimen*> &specimens );
    bool updateBrain();

    void setPosition(int x, int y);
    void setRandomPosition(int width, int height);
    void setRandomDirection();
    void setDirectionForDegree(float deg);

    void setFollowTo(Specimen *followTo);

    int random(int min, int max);

    float degreesToRadians(float angleDegrees) { return angleDegrees * (float) M_PI / (float) 180.0; }
    float radiansToDegrees(float angleRadians) { return angleRadians * (float) 180.0 / (float) M_PI; }

    Specie &getSpecie() ;
    void setSpecie( Specie &specie);

    bool canEat(Specimen &other);
    float getDistanceWithSpecimen(Specimen &other);
    void consumeEnergy();
    void attackTo(Specimen &other, int number_of_attackers);

    bool isSameSpecie(Specie s);
    bool isHungry();
    void sortSpecimensGroupByDistance(std::vector<Specimen*> &specimens);

    void debug(Uint32 *videoBuffer, int screen_width, int screen_height, bool enemies, bool friends, bool eat);

    void applySex(std::vector<Specimen*> &specimens, Specimen *s1, Specimen *s2);
private:

    float time_lived;

public:
};


#endif //EVOL_SPECIMEN_H
