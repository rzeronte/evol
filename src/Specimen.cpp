//
// Created by darkhead on 10/6/20.
//

#include "../include/Specimen.h"
#include "../include/Colors.h"
#include <glm/gtx/rotate_vector.hpp>
#include <vector>
#include <iostream>
#include "../include/Drawable.h";

Specimen::Specimen(int id) : id(id)
{
    this->id = id;
    this->dead = false;
    this->position.x = 1;
    this->position.y = 1;

    this->direction.x = 1.0f;
    this->direction.y = 1.0f;

    this->applyRandomImpulse();

    this->base_color = Colors::createRGB(0, 0, 0);

    percepted = new std::vector<Specimen*>;
    enemies   = new std::vector<Specimen*>;
    friends   = new std::vector<Specimen*>;
    eat       = new std::vector<Specimen*>;
    eated     = new std::vector<Specimen*>;
}

void Specimen::evaluate(Uint32 *videoBuffer, int canvas_width, int canvas_height, std::vector<Specimen*> &specimens, int current_global_size)
{
    this->base_color = this->specie.morphology.color;

    this->applyFriction();

    this->getSpecimensPercepted(specimens);

    this->think( specimens, current_global_size );

    this->move(canvas_width, canvas_height);

    this->debug(videoBuffer, canvas_width, canvas_height, true, false, true);
}

bool Specimen::updateBrain()
{
    float ticks     = this->timer->getTicks();
    float deltatime = ticks - this->lastTicks;
    this->lastTicks = ticks;

    accumulated_time_brain_thick += (deltatime / 1000);
    if (accumulated_time_brain_thick >= specie.genome.brain_tick) {
        this->accumulated_time_brain_thick = 0;
        return true;
    }

    return false;
}

void Specimen::think(std::vector<Specimen*> &specimens, int current_global_size)
{

    if (this->updateBrain()) {
        this->base_color *= Colors::white()/25;

        if ( !enemies->empty() ) {
            this->applyEscapeGroups(enemies);

            /*int randon_belligerance = abs(randInRange(1, 99));

            if ( randon_belligerance < this->specie.genome.belligerence ) {
                int total_friends_size = (friends->size() * this->specie.genome.size) / 10.0f;

                if ( (float) total_friends_size  > mostNearEnemy->specie.genome.size) {
                    this->attackTo( *mostNearEnemy );
                    eated->emplace_back( mostNearEnemy );

                } else {
                    this->applyEscapeGroups(enemies);
                }
            }*/

        } else if ( !eat->empty() && isHungry() ) {
            if ( this->canEat( *mostNearEat ) ) {
                this->attackTo( *mostNearEat , 1);
                eated->emplace_back( mostNearEat );
            }
            this->applyFollowGroups(eat, 50);
        } else if ( !friends->empty() ) {
            // sociability
            int random_sociability = abs(randInRange(1, 99));
            if ( random_sociability < this->specie.genome.sociability ) {
                this->applyFollowGroups(friends, 150);
            }

            // sex
            int max_simulation_size = MAX_SIMULATION_SIZE_UNITS;
            if (!isHungry() && current_global_size < max_simulation_size ) {
                float random_sexuality = fabs(randInRange(0, 100)) * 1.0f;
                if ( random_sexuality  < this->specie.genome.sexuality * SEX_FACTOR) {
                    this->applySex(specimens, this, mostNearFriend);
                }
            }
        }

        applyRandomImpulse();
    }

}

void Specimen::setPosition(int x, int y)
{
    this->position.x = x;
    this->position.y = y;
}

void Specimen::setRandomPosition(int width, int height)
{
    this->position.x = this->random(1, width);
    this->position.y = this->random(1, height);
}

void Specimen::setRandomDirection()
{
    float angle = random(0, 360);
    direction.x = 1;
    direction.y = 1;

    direction = glm::normalize(direction);
    direction = glm::rotate(direction, degreesToRadians(angle));
}

void Specimen::setDirectionForDegree(float angle)
{
    direction = glm::normalize(direction);
    direction = glm::rotate(direction, degreesToRadians(angle));
}

int Specimen::random(int min, int max) //range : [min, max)
{
    static bool first = true;
    if (first)
    {
        srand( time(NULL) ); //seeding for the first time only!
        first = false;
    }
    return min + rand() % (( max + 1 ) - min);
}

void Specimen::applyRandomImpulse()
{
    float angle = random(-MAX_ANGLE_RANDOM_ROTATION, MAX_ANGLE_RANDOM_ROTATION);
    this->applyImpulse(angle, this->specie.genome.speed);
}

void Specimen::applyRandomEscape()
{
    float angle = random(-30, 30);
    this->applyImpulse(angle, this->specie.genome.speed * ESCAPE_SPEED_FACTOR);
}

void Specimen::applyImpulseTo(glm::vec2 to, float speed)
{
    glm::vec2 difference = to - this->position;

    direction = glm::normalize(difference);

    current_speed = speed;

    this->consumeEnergy();

}
void Specimen::applyCollisionLimitsImpulse(int limit_x, int limit_y)
{
    glm::vec2 center;
    center.x = (int) (limit_x/2);
    center.y = (int) (limit_y/2);


    if ( ( (int) position.x <= 0 || (int)position.x >= limit_x) || ( (int)position.y <= 0 || (int)position.y >= limit_y) )  {
        this->applyImpulseTo(center, specie.genome.speed);
    }

}

void Specimen::applyImpulse(float angle, float speed)
{
    direction = glm::normalize(direction);
    direction = glm::rotate(direction, degreesToRadians(angle));

    direction = direction * this->specie.genome.speed;

    current_speed = speed;

    this->consumeEnergy();
}

void Specimen::move(int limit_x, int limit_y)
{
    glm::vec2 old_position = this->position;


    position = position + (direction * current_speed);

    this->applyCollisionLimitsImpulse(limit_x, limit_y );
}

void Specimen::applyFriction()
{
    if (current_speed > 0) {
        current_speed -= FRICTION;
    }
}

void Specimen::getSpecimensPercepted(std::vector<Specimen*> &specimens)
{
    percepted->clear();
    enemies->clear();
    friends->clear();
    eat->clear();
    eated->clear();

    for (int i = 0; i < specimens.size(); i++) {

        if ( this->id == specimens[i]->id) continue;
        if (specimens[i]->dead) continue;

        float d = this->getDistanceWithSpecimen( *specimens[i] );
        if (d <= this->specie.genome.perception) {
            specimens[i]->current_distance = d;
            percepted->emplace_back( specimens[i] );

            if (percepted->size() > MAX_SPECIMENS_PERCEPTED) {
                continue;
            }
        }
    }

    float mostNearEnemy_d = 9999;
    float mostNearFriend_d = 9999;
    float mostNearEat_d = 9999;;

    mostNearFriend = nullptr;
    mostNearEnemy = nullptr;
    mostNearEat = nullptr;

    for (int i = 0; i < percepted->size(); i++) {
        Specimen *s = percepted->at(i);
        float distance = this->getDistanceWithSpecimen( *s );

        if ( isSameSpecie( s->specie )) {
            friends->emplace_back( s );
            if (distance < mostNearFriend_d) {
                mostNearFriend = s;
                mostNearFriend_d = distance;
            }
        } else {
            float percepted_size = s->specie.genome.size;
            float current_size = this->specie.genome.size;

            if ( percepted_size > current_size) {
                enemies->emplace_back(s);
                if (distance < mostNearEnemy_d) {
                    mostNearEnemy = s;
                    mostNearEnemy_d = distance;
                }
            }

            if ( percepted_size < current_size) {
                eat->emplace_back( s );
                if (distance < mostNearEat_d) {
                    mostNearEat = s;
                    mostNearEat_d = distance;
                }
            }
        }
    }
}

void Specimen::setSpecie( Specie &s)
{
    this->specie = Specie(s);
    this->initial_energy = this->specie.genome.energy;
}

bool Specimen::canEat(Specimen &other) {
    glm::vec2 difference = other.position - this->position;

    if (glm::length(difference) <= this->specie.genome.size * 2 * EATING_MARGING) {
        return true;
    }

    return false;
}

float Specimen::getDistanceWithSpecimen(Specimen &other)
{
    glm::vec2 difference = other.position - this->position;

    return glm::length(difference);
}

void Specimen::consumeEnergy()
{
    if (this->specie.genome.energy > 0) {
        this->specie.genome.energy -= this->specie.genome.size * ENERGY_CONSUME_FACTOR;
    }

    if (this->specie.genome.energy <= 0) {
        this->dead = true;
    }
}

void Specimen::setFollowTo(Specimen *followTo) {
    Specimen::followTo = followTo;
    consumeEnergy();
}

void Specimen::applyFollowTo()
{
    if (this->followTo != nullptr) {

        float speed = this->specie.genome.speed * random(0, 10) * FRICTION_FOLLOW;

        glm::vec2 position = this->followTo->position;
        position.x +=  random(0, 10) * FOLLOW_CONFUSION;
        position.y +=  random(0, 10) * FOLLOW_CONFUSION;

        applyImpulseTo(position, speed);
    }

}

void Specimen::setTimer(Timer *timer) {
    Specimen::timer = timer;
}

void Specimen::debug(Uint32 *videoBuffer, int screen_width, int screen_height, bool draw_enemies, bool draw_friends, bool draw_eat)
{
    int x1 = this->position.x;
    int y1 = this->position.y;


    if (draw_enemies) {
        for (int i = 0; i < enemies->size(); i++) {
            int x2 = enemies->at(i)->position.x;
            int y2 = enemies->at(i)->position.y;
            Drawable::drawLine2D( x1, y1, x2, y2, videoBuffer, screen_width, screen_height, this->specie.morphology.color );
        }
    }

    if (draw_friends) {
        for (int i = 0; i < friends->size(); i++) {
            int x2 = friends->at(i)->position.x;
            int y2 = friends->at(i)->position.y;
            Drawable::drawLine2D( x1, y1, x2, y2, videoBuffer, screen_width, screen_height, this->specie.morphology.color );
        }
    }

    if (draw_eat) {
        for (int i = 0; i < eated->size(); i++) {
            int x2 = eated->at(i)->position.x;
            int y2 = eated->at(i)->position.y;
            Drawable::drawLine2D( x1, y1, x2, y2, videoBuffer, screen_width, screen_height, this->specie.morphology.color );
        }
    }

}

bool Specimen::isSameSpecie(Specie s)
{
    if (this->specie.name == s.name) {
        return true;
    }
    return false;
}

void Specimen::applyFollowGroups(std::vector<Specimen*> *group, int noise_movement)
{

    glm::vec2 r;

    for (int i = 0; i < group->size(); i++) {
        r.x += group->at(i)->position.x;
        r.y += group->at(i)->position.y;
    }

    r.x /= group->size();
    r.y /= group->size();

    float shift = noise_movement;
    r.x += random(-shift, shift);
    r.y += random(-shift, shift);

    glm::vec2 difference = r - this->position;
    float length = glm::length(difference);

    float size = specie.genome.size * (float)EPSILON_STOP_FOLLOW_GROUP;
    if (length >= size) {
        direction = r - this->position;
        direction = glm::normalize(direction);
    }

    this->consumeEnergy();
}

void Specimen::applyEscapeGroups(std::vector<Specimen*> *group)
{
    glm::vec2 r;

    for (int i = 0; i < group->size(); i++) {
        r.x += group->at(i)->position.x;
        r.y += group->at(i)->position.y;
    }

    r.x /= group->size();
    r.y /= group->size();

    current_speed = this->specie.genome.speed;
    direction = this->position - r;
    direction = glm::normalize(direction);

    this->consumeEnergy();

}


void Specimen::sortSpecimensGroupByDistance(std::vector<Specimen*> &v)
{
    const int n = v.size();

    Specimen *aux;
    for (int i = 1 ; i < n ; i++) {
        for (int j = 0 ; j < (n - i); j++) {
            if (v[j]->current_distance > v[j+1]->current_distance) {
                aux = v[j];
                v[j] = v[j+1];
                v[j+1] = aux;
            }
        }
    }
}

void Specimen::applyEat()
{
    for (int i = 0; i < eat->size(); i++) {
        eat->at(i)->dead = true;
    }
}

bool Specimen::isHungry()
{
    return specie.genome.energy < initial_energy * 0.75;
}


void Specimen::attackTo(Specimen &other, int num_attackants)
{
    float energy_factor = ENERGY_EATING_FACTOR;
    other.specie.genome.energy -= ( (other.specie.genome.size / this->specie.genome.size) * energy_factor) / num_attackants;
    this->specie.genome.energy += ( (other.specie.genome.size / this->specie.genome.size) * energy_factor   );

    if (this->specie.genome.energy <= 0) {
        this->dead = true;
    }
}

void Specimen::applySex(std::vector<Specimen*> &specimens, Specimen *s1, Specimen *s2)
{
    s1->already_sex = true;
    s2->already_sex = true;

    auto *specimen = new Specimen( specimens.size() );
    specimen->setSpecie( this->specie );
    specimen->setTimer( timer);
    specimen->setPosition(this->position.x, this->position.y);
    specimen->specie.genome.energy = initial_energy;
    specimen->setRandomDirection();
    specimens.push_back( specimen );
}
