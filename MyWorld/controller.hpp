//
//  controller.hpp
//  MyWorld
//
//  Created by Ah on 1/10/16.
//  Copyright © 2016 jason. All rights reserved.
//

#ifndef controller_hpp
#define controller_hpp

#include <stdio.h>
#include "plane.h"
#include <iostream>
#include "objin.h"
using namespace std;

/*
 glutMouseFunc(mouse);
 glutMotionFunc(mouseMotionHandler);
 glutKeyboardFunc(keyboard);
 void mouse(int button, int state, int x, int y);
 void mouseMotionHandler(int xMouse, int yMouse);
 void keyboard(unsigned char key, int x, int y);
 float lookFromx = 0;		// Camera X  Position
 float lookFromy = 0;	// Camera Y Position
 float lookFromz = radius;		// Camera Z Position
 float theta = 0;		// Camera X angle
 float phy = 80;		// Camera Y angle
 float upx = 0;			// Up Vector
 float upy = 1;
 float upz = 0;
 //angle
 float lookAtx = 0;		// Camera is looking at
 float lookAty = 0;
 float lookAtz = 0;
 */

class controller
{
public:
    static float ration;
    static int lastX, lastY;
    static float headx, heady, headz;
    static float rx, ry;
    static bool buttonState;
    static GLMmodel* obj;
    static bool flag;
    static bool isCreated;
   // controller(Plane* aim):obj(aim){}
    static void mouse(int button, int state, int x, int y);
    static void mouseMotionHandler(int xMouse, int yMouse);
    static void keyboard(unsigned char key, int x, int y);
    static void synPlane();
   // ~controller(){delete obj;}
    
};













#endif /* controller_hpp */
