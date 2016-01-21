#ifndef BeetlesSim_Beetles_h
#define BeetlesSim_Beetles_h
#include <math.h>
#include "VectorAndPoint.h"
class Beetles {
    
    public:
    Vector3D translation;
    Vector3D rotateFactor; //angle around the Y axis
    
    //Meshes
    MyObj *body      = nullptr;
    MyObj *(*wheels) = nullptr;
    
    //Camera
    Vector3D lookFrom;
    Vector3D lookAt;
    
    //Other
    int num_of_wheels;
    float cam_height;
  
    bool hit = false;
    
    
  
    Beetles() {
        this->num_of_wheels = 0;
        this->cam_height = 1;
    };
    
    
    void set_wheels (std::vector<MyObj *> &wheels) {
        if(this->wheels){
            free(this->wheels);
        }
        
        this->num_of_wheels = (int) wheels.size();
        this->wheels = &wheels[0];
        
        for(int i = 0; i < num_of_wheels; i++) {
            this->wheels[i]->translation.x += this->translation.x;
            this->wheels[i]->translation.y += this->translation.y-0.4;
            this->wheels[i]->translation.z += this->translation.z;
        }
    };
    
    void draw() {
      if (this->hit) {
        this->body->mat_diffuse[0] = 1.0;
        this->body->mat_diffuse[1] = 0.0;
        this->body->mat_diffuse[2] = 0.0;
      }
        this->body->draw();
        
        for (int i = 0; i < num_of_wheels; i++) {
            this->wheels[i]->draw();
        }
        
    };
    
    /*Moves the Beetles by Vector */
    void makeMove(Vector3D position) {
        this->translation.x += position.x;
        this->translation.y += position.y;
        this->translation.z += position.z;
        
        this->body->translation.x += position.x;
        this->body->translation.y += position.y;
        this->body->translation.z += position.z;
        
        for(int i = 0; i < num_of_wheels; i++) {
            this->wheels[i]->translation.x += position.x;
            this->wheels[i]->translation.y += position.y;
            this->wheels[i]->translation.z += position.z;
        }
        
    };
    
    /*Moves the Beetles Forward or backwards */
    void makeMove (float distance) {
      
        int sign = distance < 0 ? 1 : -1;
      
        float new_x  = distance * sinf(this->rotateFactor.y * 3.14159265 / 180);
        float new_z  = distance * cosf(this->rotateFactor.y * 3.14159265 / 180);
        
        this->translation.x += new_x;
        this->translation.z += new_z;
        
        this->body->translation.x += new_x;
        this->body->translation.z += new_z;
        
        
        for(int i = 0; i < num_of_wheels; i++) {
            this->wheels[i]->translation.x += new_x;
            this->wheels[i]->translation.z += new_z;
            //this->wheels[i]->rotateFactor.x += 5 * -sign;
        }
    };
    
    
    /* Rotates the Beetless
     */
    void rotateBy (float angle) {
        this->rotateFactor.y += angle;
        
        //Body transformation
        this->body->rotateFactor.y += angle;
        
        //Wheels transformation
        for (int i = 0; i < num_of_wheels; i++) {
            this->wheels[i]->rotateFactor.y += angle;
        }
    };
    
    /* Rotates the Turret */
    
    void rotateBeetles(float angle) {
        rotateBy (angle);
    }
  
};

#endif
