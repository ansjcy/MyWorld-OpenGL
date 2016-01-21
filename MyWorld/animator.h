#ifndef BeetlesSim_animator_h
#define BeetlesSim_animator_h

#include <cmath>
#include "beetles.h"

class Animator {
        bool driveBack;
        bool rotated;
    public:
        Beetles *beetles;
        float distance = 0;
        float distance_moved = 0;
        float angle = 0;
        float angle_moved = 0;
    
    Animator(Beetles *beetles, float distance, float angle) :
        beetles(beetles), distance(distance), angle(angle) {
            this->angle_moved = 0;
            this->distance_moved = 0;
            this->driveBack = false;
            this->rotated = false;
    };
    
    void animate(float distance, float angle) {
        if (!this->driveBack && !this->rotated) {
            this->beetles->makeMove(distance);
            this->distance_moved += std::abs(distance);
            if (this->distance_moved >= this->distance) {
                this->distance_moved = 0;
                this->driveBack = true;
            }
            
        } else if (this->driveBack && !this->rotated) {
            this->beetles->rotateBeetles(angle);
            this->angle_moved += angle;
            if (this->angle_moved >= this->angle) {
                this->angle_moved = 0;
                this->rotated = true;
            }
        } else if (this->driveBack && this->rotated) {
            this->beetles->makeMove(distance);
            this->distance_moved += std::abs(distance);
            if (this->distance_moved >= this->distance) {
                this->distance_moved = 0;
                this->driveBack = false;
            }
        } else if (!this->driveBack && this->rotated) {
            this->beetles->rotateBeetles(angle);
            this->angle_moved += angle;
            if (this->angle_moved >= this->angle) {
                this->angle_moved = 0;
                this->rotated = false;
            }
        }
    }
};

#endif
