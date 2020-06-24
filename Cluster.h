//
// Created by darkhead on 15/6/20.
//

#ifndef EVOL_CLUSTER_H
#define EVOL_CLUSTER_H

#include <vector>
#include <glm/gtx/rotate_vector.hpp>
#include "include/Specimen.h"
#include <glm/gtx/string_cast.hpp>

class Cluster {
public:
    Cluster(int id, const std::string &name, std::vector<Specimen *> &totalSpecimens) : id(id), name(name), total_specimens(totalSpecimens)
    {
        inertia_factor = 0.75f;
        global_best_position = glm::vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
    }

    int id;
    std::string name;

    std::vector<Specimen*> &total_specimens;
    std::vector<Specimen *> specimens;
    Specimen *best_specimen;
    glm::vec2 global_best_position;

    float inertia_factor;

    float coefficient_attraction_best_personal = 0.9;
    float coefficient_attraction_best_global = 0.1;

    void update()
    {
        this->setVelocityFrame();
        this->randomSpeed(best_specimen);
    }

    glm::vec2 setVelocityFrame()
    {
        glm::vec2 best_cluster_position = this->global_best_position;

        for (int i = 0; i < specimens.size(); i++) {
            Specimen *s = specimens.at(i);

            s->getSpecimensPercepted(this->total_specimens);

            if (specimens[i]->id != best_specimen->id) {
                setVelocityFrameForSpecimen( s, best_cluster_position );
            }
            updateBestPositionsForSpecimen( s );

        }

        this->best_specimen = this->getSpecimenAlpha();
        this->global_best_position = best_specimen->position;
    }

    void setVelocityFrameForSpecimen(Specimen *s, glm::vec2 best_cluster_position)
    {
        glm::vec2 new_speed;

        glm::vec2 best_position_from_experience = s->cluster_info.best_position.position;

        float r1 = (float) abs(getRandomFactor() - 1);
        float r2 = (float) abs(getRandomFactor() - 1);

        new_speed  = inertia_factor * s->cluster_info.current_speed;
        new_speed += coefficient_attraction_best_personal * r1 * (best_position_from_experience - s->position);
        new_speed += coefficient_attraction_best_global * r2 * (best_cluster_position - s->position);

        s->speed = new_speed * 0.075f;

        assert(!isnan(s->speed.x));
        assert(!isnan(s->speed.y));

        s->cluster_info.current_speed = s->speed;
    }

    void updateBestPositionsForSpecimen(Specimen *s)
    {
        float points_best;

        float points_current_speed = getRankingForPosition(s);

        Types::PositionRanking best_position;
        best_position.position = s->position + s->speed;
        best_position.points = points_current_speed;

        if ( points_current_speed > points_best ) {
            s->cluster_info.best_position = best_position;
        }

        s->cluster_info.best_positions.push_back( best_position );

    }

    float getRankingForPosition(Specimen *s)
    {
        glm::vec2 position_enemies = getPositionCenterForGroup(*s->enemies);
        glm::vec2 position_friends = getPositionCenterForGroup(*s->friends);
        glm::vec2 position_food    = getPositionCenterForGroup(*s->eat);

        if (s->enemies->size() == 0) {
            return best_specimen->cluster_info.best_position.points;
        }

        float distance_enemies = glm::length( (s->position + s->speed) - position_enemies);
        float distance_friends = glm::length( (s->position + s->speed) - position_friends);
        float distance_food    = glm::length( (s->position + s->speed) - position_food);

        float points = distance_enemies * RANKING_FACTOR;

        return points;
    }

    void setInitialVelocity()
    {
        std::cout << "setInitialVelocity for " << this->name << std::endl;

        for (int i = 0; i < specimens.size(); i++) {
            glm::vec2 speed;
            float angle = (float) abs(Types::random(1, 360));
            speed.x = 1;
            speed.y = 1;

            speed = glm::normalize(speed);
            speed = glm::rotate(speed, glm::radians(angle));

            assert(!isnan(speed.x));
            assert(!isnan(speed.y));

            specimens.at(i)->speed = speed;
        }
    }

    void setInitialPositions(int w, int h)
    {
        std::cout << "setInitialPositions for " << this->name << std::endl;

        for (int i = 0; i < specimens.size(); i++) {
            specimens[i]->setRandomPosition(w, h);
        }
    }

    glm::vec2 getBestPositionFromExperience(Specimen *s, float &get_points)
    {
        float points = 0;
        glm::vec2 best_position;

        for (int i = 0; i < s->cluster_info.best_positions.size(); i++) {
            if ( s->cluster_info.best_positions.at(i).points > points ) {
                best_position = s->cluster_info.best_positions.at(i).position;
                get_points = s->cluster_info.best_positions.at(i).points;
            }
        }

        return best_position;
    }

    glm::vec2 getBestPositionFromCluster()
    {
        float points = 0;
        glm::vec2 best_position;

        for (int i = 0; i < specimens.size(); i++) {
            Specimen *s = specimens.at(i);
            for (int j = 0; j < s->cluster_info.best_positions.size(); j++) {
                if ( s->cluster_info.best_positions.at(j).points > points ) {
                    best_position = s->cluster_info.best_positions.at(j).position;
                }
            }
        }

        return best_position;
    }

    bool existId(int id)
    {
        for (int i = 0; i < specimens.size(); i++) {
            if (id == specimens.at(i)->id) {
                return true;
            }
        }

        return false;
    }

    Specimen *getSpecimenAlpha()
    {
        Specimen *selected = nullptr;
        int distance = 0;

        assert(specimens.size() > 0);

        for (int i = 0; i < specimens.size(); i++) {
            Specimen *s = specimens.at(i);
            for (int j = 0; j < s->enemies->size(); j++) {
                float d = this->getDistanceBetweenSpecimens( s, s->enemies->at(j) );
                if (d >= distance ) {
                    selected = s;
                    distance = d;
                }
            }
        }

        if (selected == nullptr) {
            selected = this->best_specimen;
        }

        assert(selected != nullptr);

        return selected;
    }

    void addSpecimen(Specimen *s)
    {
        s->cluster_info.cluster_id = this->id;

        Types::PositionRanking best_position;
        best_position.position = s->position;
        best_position.points = 0;

        s->cluster_info.best_positions.push_back( best_position );
        s->cluster_info.current_speed = s->speed;

        assert(!isnan(s->cluster_info.current_speed.x));
        assert(!isnan(s->cluster_info.current_speed.y));

        s->cluster_info.best_position = best_position;

        this->specimens.push_back( s );
    }

    double getRandomFactor()
    {
        double r = ((double) rand() / (RAND_MAX)) + 1;

        return r;
    }

    float getDistanceBetweenSpecimens(Specimen *s1, Specimen *s2)
    {
        glm::vec2 difference = s1->position - s2->position;

        return glm::length(difference);
    }

    glm::vec2 getPositionCenterForGroup(std::vector<Specimen*> &group) {
        glm::vec2 r;
        r.x = 0;
        r.y = 0;

        for (int i = 0; i < group.size(); i++) {
            r.x += group.at(i)->position.x;
            r.y += group.at(i)->position.y;
        }

        r.x /= group.size();
        r.y /= group.size();

        return r;
    }

    void randomSpeed(Specimen *s)
    {
        float angle = Types::random(-MAX_ANGLE_RANDOM_ROTATION, MAX_ANGLE_RANDOM_ROTATION);
        s->speed = glm::normalize(s->speed);
        s->speed = glm::rotate(s->speed, glm::radians(angle));

        s->speed = s->speed * s->specie.genome.speed * this->inertia_factor;

        s->cluster_info.current_speed = s->speed;

        glm::vec2 dest = s->position + s->speed;

        if ( (dest.x <= 0 || dest.x > SCREEN_WIDTH) || (dest.y <= 0 || dest.y > SCREEN_HEIGHT) ) {
            glm::vec2 center(SCREEN_WIDTH/2, SCREEN_WIDTH/2);

            center = center - s->position;
            center = glm::normalize(center);
            center = center * s->specie.genome.speed;
            s->cluster_info.current_speed = center;
            s->speed = center;
        }

    }
};
#endif //EVOL_CLUSTER_H
