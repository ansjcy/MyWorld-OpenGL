//
//  controller.cpp
//  MyWorld
//
//  Created by Ah on 1/10/16.
//  Copyright © 2016 jason. All rights reserved.
//

#include "controller.hpp"
/*
 These:
 lookFromx
 lookFromy
 lookFromz
 
 upx
 upy
 upz
 
 lookAtx
 lookAty
 lookAtz
 camera...............
 glutMouseFunc(mouse);
 glutMotionFunc(mouseMotionHandler);
 glutKeyboardFunc(keyboard);
 
 */

extern float lookFromx;// Camera X  Position
extern float lookFromy;	// Camera Y Position
extern float lookFromz;		// Camera Z Position
extern float lookAtx;		// Camera is looking at
extern float lookAty;
extern float lookAtz;
extern float upx;			// Up Vector
extern float upy;
extern float upz;
extern bool closeWindow;
extern int choicess;
#define PI 3.1415926
#define ANG 180

//float x,y, z;
float controller::rx = 0, controller::ry = 0;//, rz = 0;
bool controller::isCreated = false;
bool controller::flag = false;
float controller::ration = 1.0f;
int controller::lastX = 0, controller::lastY = 0;
bool controller::buttonState = 0;
float controller::headx = 0, controller::heady=1.0f, controller::headz = 0.0f;

static void RotateMatrix(Vector3D& vec, float ry, float rx)
{
    rx = rx/ANG*PI;
    ry = ry/ANG*PI;
    vec.y = cos(rx)*vec.y-sin(rx)*vec.z;
    vec.z = sin(rx)*vec.y + cos(rx)*vec.z;
    //rotate around x axis
    vec.x = cos(ry)*vec.x + sin(ry)*vec.z;
    vec.z = -sin(ry)*vec.x + cos(ry)*vec.z;
}


void controller::mouse(int button, int state, int x, int y)
{
    
    if(state == GLUT_DOWN)
    {
        lastX = x;
        lastY = y;
        buttonState = state;
    }
    else
    {
        buttonState = GLUT_UP;
    }
    cout << "mouse.\n";
}

void controller::mouseMotionHandler(int xMouse, int yMouse)
{
    //obj->lookFrom
    
    if(buttonState == GLUT_DOWN)
    {
        float lim = 100.0;
        Vector3D dest(lookAtx, lookAty, lookAtz) ;
        Vector3D src(lookFromx,lookFromy,lookFromz);
        Vector3D dirc = dest - src;
        
        Vector3D dir = dest - src;
        Vector3D head( upx, upy, upz);
        head.Normalize();
        dir.Normalize();
        Vector3D xx = dir.CrossProduct(head);
        xx.Normalize();
        int dx, dy;
        dx = xMouse - lastX;
        dy = yMouse - lastY;
        lastX = xMouse, lastY = yMouse;
        float signx = 1, signy = 1;
        if(dx < 0)
            signx = -1;
        if(dy < 0)
            signy = -1;
      //  signy = 0;
        Vector3D tempX = Vector3D(xx.x*signx, xx.y*signx, xx.z*signx);
        Vector3D tempY = Vector3D(head.x*signy, head.y*signy, head.z*signy);
        Vector3D res;
        float ry, rx;
        ry = fabs(dy)/lim;
        rx = fabs(dx)/lim;
        tempX = tempX * rx;
        tempY = tempY * ry;
        res = (tempX + tempY)*ration;
        lookAtx = res.x + src.x + dirc.x;
        lookAty = res.y + src.y + dirc.y;
        lookAtz = res.z + src.z + dirc.z;        
        cout << upx << ", " << upy << ", " << upz << "\n";
        cout << lookFromx << ", " << lookFromy << ", " << lookFromx << endl;
        isCreated = true;
    }
}
    


extern Plane * plane;
enum Action {TRANSLATE, NAVIGATE, NAVIGATE2, CHOOSE, LIGHT, LIGHTD, LIGHTS, LIGHTA};
//                      trans,view  rotate  choose

extern enum Action nowAction;
extern GLfloat light_position0[4]; //1:infinity
extern GLfloat light_diffuse[4];  //diffusion..
extern GLfloat light_specular[4];  //"mirror"...
extern GLfloat light_ambient[4];  //RGBA, environment..
extern float camerax ;		// Camera X Position
extern float cameray ;	// Camera Y Position
extern float cameraz;


void controller::keyboard(unsigned char key, int x, int y)
{
    float ratio = 0.12;
    switch (key) {
        case 'q':
        {
            Vector3D dest(lookAtx, lookAty, lookAtz) ;
            Vector3D src(lookFromx,lookFromy,lookFromz);
            Vector3D dirc = dest - src;
            dirc.Normalize();
            dirc = dirc*ratio;
            lookFromx += dirc.x;
            lookFromy += dirc.y;
            lookFromz += dirc.z;
            lookAtx += dirc.x;
            lookAty += dirc.y;
            lookAtz += dirc.z;
            isCreated = true;
            break;
        }
        case 'n':
                if (nowAction != NAVIGATE)
                {
                    nowAction = NAVIGATE;
                    
                    lookFromx = plane->lookFrom.x;
                    lookFromy = plane->lookFrom.y;
                    lookFromz = plane->lookFrom.z;
                    
                    lookAtx = plane->lookAt.x + 4;
                    lookAty = plane->lookAt.y + 5;
                    lookAtz = plane->lookAt.z;
                    cout << "----------" <<lookFromx << ", " << lookFromy << ", " << lookFromz << endl;

                    cout << "---------" << lookAtx << ", " << lookAty << ", " << lookAtz << endl;
                }
                else
                {
                    nowAction = TRANSLATE;
                    lookAtx = 0;
                    lookAty = 0;
                    lookAtz = 0;
                    
                    lookFromx = camerax;
                    lookFromy = cameray;
                    lookFromz = cameraz;
                }
                break;
            case 'm':
                if(nowAction == NAVIGATE)
                    nowAction = NAVIGATE2;
                else if(nowAction == NAVIGATE2)
                    nowAction = NAVIGATE;
                break;
            case 'a':
            {
                if(nowAction == NAVIGATE)
                {
                    float hypotenuse = sqrt((lookFromx-lookAtx)*(lookFromx-lookAtx)+(lookAtz-lookFromz)*(lookAtz-lookFromz));
                    float xForward = (lookAtz-lookFromz)/hypotenuse;
                    float zForward = -(lookAtx-lookFromx)/hypotenuse;
                    lookAtx+=xForward;
                    lookFromx+=xForward;
                    lookAtz+=zForward;
                    lookFromz+=zForward;
                    plane->makeMove(Vector3D(xForward,0,zForward));
                    plane->lookFrom.x+=xForward;
                    plane->lookFrom.z+=zForward;
                    plane->lookAt.x+=xForward;
                    plane->lookAt.z+=zForward;
                }
                else if (nowAction == CHOOSE)
                {
                    //chooseAnObj(2);
                }
                break;
            }
            case 'd':
            {
                if(nowAction == NAVIGATE){
                    float hypotenuse = sqrt((lookFromx-lookAtx)*(lookFromx-lookAtx)+(lookAtz-lookFromz)*(lookAtz-lookFromz));
                    float xForward = -(lookAtz-lookFromz)/hypotenuse;
                    float zForward = (lookAtx-lookFromx)/hypotenuse;
                    lookAtx+=xForward;
                    lookFromx+=xForward;
                    lookAtz+=zForward;
                    lookFromz+=zForward;
                    plane->makeMove(Vector3D(xForward,0,zForward));
                    plane->lookFrom.x+=xForward;
                    plane->lookFrom.z+=zForward;
                    plane->lookAt.x+=xForward;
                    plane->lookAt.z+=zForward;
                    
                }
                else if(nowAction == CHOOSE)
                {
                    //chooseAnObj(3);
                }
                else if (nowAction == LIGHT)
                {
                    light_position0[1] -=10;
                }
                else if (nowAction == LIGHTD)
                {
                    light_diffuse[1] -=0.1;
                }
                else if (nowAction == LIGHTS)
                {
                    light_specular[1] -=0.1;
                }
                else if (nowAction == LIGHTA)
                {
                    light_ambient[1] -=0.1;
                }
                break;
            }
            case 'w':
            {
                if(nowAction == NAVIGATE)
                {
                    float hypotenuse = sqrt((lookFromx-lookAtx)*(lookFromx-lookAtx)+(lookAtz-lookFromz)*(lookAtz-lookFromz));
                    float xForward = (lookAtx-lookFromx)/hypotenuse;
                    float zForward = (lookAtz-lookFromz)/hypotenuse;
                    lookAtx+=xForward;
                    lookFromx+=xForward;
                    lookAtz+=zForward;
                    lookFromz+=zForward;
                    plane->makeMove(Vector3D(xForward,0,zForward));
                    plane->lookFrom.x+=xForward;
                    plane->lookFrom.z+=zForward;
                    plane->lookAt.x+=xForward;
                    plane->lookAt.z+=zForward;
                }
                else if (nowAction == CHOOSE)
                {
                    //chooseAnObj(0);
                    
                }
                else if (nowAction == LIGHT)
                {
                    light_position0[0] +=10;
                }
                else if (nowAction == LIGHTD)
                {
                    light_diffuse[0] +=0.1;
                }
                else if (nowAction == LIGHTS)
                {
                    light_specular[0] +=0.1;
                }
                else if (nowAction == LIGHTA)
                {
                    light_ambient[0] +=0.1;
                }
                break;
            }
            case 's':
            {
                if(nowAction == NAVIGATE){
                    float hypotenuse = sqrt((lookFromx-lookAtx)*(lookFromx-lookAtx)+(lookAtz-lookFromz)*(lookAtz-lookFromz));
                    float xForward = (lookAtx-lookFromx)/hypotenuse;
                    float zForward = (lookAtz-lookFromz)/hypotenuse;
                    lookAtx-=xForward;
                    lookFromx-=xForward;
                    lookAtz-=zForward;
                    lookFromz-=zForward;
                    plane->makeMove(Vector3D(xForward,0,zForward));
                    plane->lookFrom.x-=xForward;
                    plane->lookFrom.z-=zForward;
                    plane->lookAt.x-=xForward;
                    plane->lookAt.z-=zForward;
                }
                else if (nowAction == CHOOSE)
                {
                    //chooseAnObj(1);
                }
                else if (nowAction == LIGHT)
                {
                    light_position0[0] -=10;
                }
                else if (nowAction == LIGHTD)
                {
                    light_diffuse[0] -=0.1;
                }
                else if (nowAction == LIGHTS)
                {
                    light_specular[0] -=0.1;
                }
                else if (nowAction == LIGHTA)
                {
                    light_ambient[0] -=0.1;
                }
                break;
            }
            case 'e':
            {
                if (nowAction == LIGHT)
                {
                    light_position0[1] +=10;
                }
                else if (nowAction == LIGHTD)
                {
                    light_diffuse[1] +=0.1;
                }
                else if (nowAction == LIGHTS)
                {
                    light_specular[1] +=0.1;
                }
                else if (nowAction == LIGHTA)
                {
                    light_ambient[1] +=0.1;
                }
                break;
            }
                //        case 'd':
                //        {
                //
                //        }
            case 'r':
            {
                if (nowAction == LIGHT)
                {
                    light_position0[2] +=10;
                }
                else if (nowAction == LIGHTD)
                {
                    light_diffuse[2] +=0.1;
                }
                else if (nowAction == LIGHTS)
                {
                    light_specular[2] +=0.1;
                }
                else if (nowAction == LIGHTA)
                {
                    light_ambient[2] +=0.1;
                }
            }
            case 'f':
            {
                if (nowAction == LIGHT)
                {
                    light_position0[2] -=10;
                }
                else if (nowAction == LIGHTD)
                {
                    light_diffuse[2] -=0.1;
                }
                else if (nowAction == LIGHTS)
                {
                    light_specular[2] -=0.1;
                }
                else if (nowAction == LIGHTA)
                {
                    light_ambient[2] -=0.1;
                }
                break;
            }
        case '9':
        {
            if(nowAction!=LIGHT)
                nowAction = LIGHT;
            else
                nowAction = TRANSLATE;
            break;
        }
        case '8':
        {
            if(nowAction!=LIGHTS)
                nowAction = LIGHTS;
            else
                nowAction = TRANSLATE;
            break;
        }
        case '7':
        {
            if(nowAction!=LIGHTA)
                nowAction = LIGHTA;
            else
                nowAction = TRANSLATE;
            break;
        }
        case '6':
        {
            if(nowAction!=LIGHTD)
                nowAction = LIGHTD;
            else
                nowAction = TRANSLATE;
            break;
        }
        case 27:
        {
            closeWindow = true;
        }
        case '`':
        {
            choicess = (choicess+1)%6;
        }
        default:
            break;
    }
}


void controller::synPlane()
{
    if(false)
    {
        float ratio = -.2f;
        obj->position[0] = lookFromx;
        obj->position[1] = lookFromy;
        obj->position[2] = lookFromz;
        Vector3D dest(lookAtx, lookAty, lookAtz) ;
        Vector3D src(lookFromx,lookFromy,lookFromz);
        Vector3D dirc = dest - src;
        //dirc.Normalize();
        dirc = dirc * ratio;
        Vector3D zz(0,0,1);
       // Vector3D tp = dirc.CrossProduct(zz);
       // tp.Normalize();
       // obj->body->translation = src + dirc;
       // obj->body->rotateFactor =
        float shuttle = 50000000*random()*1.0/LONG_MAX;
        
        obj->position[0] = src.x+-dirc.x + shuttle;
        obj->position[1] = src.y+-dirc.y + shuttle;
        obj->position[2] = src.z+-dirc.z + shuttle;
      //  cout << "shuttle: " << shuttle << endl;
        //obj->translation = obj->translation + dirc;
        //obj->translation -=
        isCreated = false;
    
    }
}
