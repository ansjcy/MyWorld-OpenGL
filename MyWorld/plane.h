#ifndef plane_h
#define plane_h
#include <math.h>
#include "VectorAndPoint.h"
#include "MyObj.h"
//#include "MyObj.h"
class Plane{
    
public:
    Vector3D translation;
    Vector3D rotateFactor; //angle around the Y axis
    
    //Meshes
    MyObj *body      = nullptr;

    //Camera
    Vector3D lookFrom;
    Vector3D lookAt;
    
    //Other
    float cam_height;
    
    
    Plane() {
        cam_height = 1;
    };
    
    void draw() {
        body->draw();
    };
    
    /*Moves the Beetles by Vector */
    void makeMove(Vector3D position) { //....
        translation.x += position.x;
        translation.y += position.y;
        translation.z += position.z;
        
        body->translation.x += position.x;
        body->translation.y += position.y;
        body->translation.z += position.z;
    };
    void moveTo(Vector3D position)
    {
        translation.x = position.x;
        translation.y = position.y;
        translation.z = position.z;
        
        body->translation.x = position.x;
        body->translation.y = position.y;
        body->translation.z = position.z;

    }
    
    /*Moves the Beetles Forward or backwards */
    void makeMove (float distance) {
        
        int sign = distance < 0 ? 1 : -1;
        
        float new_x  = distance * sinf(this->rotateFactor.y * 3.14159265 / 180);
        float new_z  = distance * cosf(this->rotateFactor.y * 3.14159265 / 180);
        
        this->translation.x += new_x;
        this->translation.z += new_z;
        
        this->body->translation.x += new_x;
        this->body->translation.z += new_z;
        
    };
    /*
     void makeRotate(Vector3D rotation)
     {
     
     
     }
     
     
     */
    
    /* Rotates the Beetless
     */
    void rotateBy (float angle) {
        this->rotateFactor.y += angle;
        
        //Body transformation
        this->body->rotateFactor.y += angle;
    };
    
    /* Rotates the Turret */
    
    void rotateBeetles(float angle) {
        rotateBy (angle);
    }
    
};


#endif /* plane_h */
