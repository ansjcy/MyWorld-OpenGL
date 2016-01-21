#include "VectorAndPoint.h"

class spherebox{
public:
    spherebox(Vector3D translate=Vector3D(0,0,0));
    void computeBoundingSphereAverage(Vector3D* vertices, unsigned int vertex_num);
    int TestSphereSphere(spherebox a, spherebox b);
    Vector3D center ;
    float radious ;
    Vector3D translate;
};
