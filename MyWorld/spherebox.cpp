//
//  spherebox.cpp
//  MyWorld
//
//  Created by 吴星瑶 on 16/1/11.
//  Copyright © 2016年 jason. All rights reserved.
//

#include "spherebox.h"
#include "VectorAndPoint.h"
#include "math.h"


int spherebox::TestSphereSphere(spherebox a, spherebox b)

{
    
    float dist2 = (a.center.x+a.translate.x-(b.center.x+b.translate.x))*(a.center.x+a.translate.x-(b.center.x+b.translate.x))+(a.center.y+a.translate.y-(b.center.y+b.translate.y))*(a.center.y+a.translate.y-(b.center.y+b.translate.y))+
(a.center.z+a.translate.z-(b.center.z+b.translate.z))*(a.center.z+a.translate.z-(b.center.z+b.translate.z));
    
    float radisum = a.radious + b.radious ;
    
    if(dist2 < radisum * radisum)
        
        return 1 ;
    
    else
        
        return 0 ;
    
}

void spherebox::computeBoundingSphereAverage(Vector3D* vertices, unsigned int vertex_num)
{
    //Compute the center point
    Vector3D total ;
    total.x = 0 ;
    total.y = 0 ;
    total.z = 0 ;
    for(int i = 0 ; i < vertex_num ; i ++)
    {
        total.x += vertices[i].x ;
        total.y += vertices[i].y ;
        total.z += vertices[i].z ;
    }// end for
    
    total.x /= vertex_num ;
    total.y /= vertex_num ;
    total.z /= vertex_num ;
    center = total ;
    
    //Compute the radious
    float r = 0 ;
    for(int i = 0 ; i < vertex_num ; i ++)
    {
        Vector3D temp ;
        //Vec3Sub(temp, total, vertices[i]);
        temp.x=vertices[i].x-total.x;
        temp.y=vertices[i].y-total.y;
        temp.z=vertices[i].z-total.z;
        float length = 0 ;
        length = sqrt(temp.x*temp.x+temp.y*temp.y+temp.z*temp.z);
        if(length > r)
            r = length ;
    }// end for
    
    radious = r ;
}// end for computeBoundingSphereAverage