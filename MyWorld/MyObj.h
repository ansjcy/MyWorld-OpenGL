#ifndef _MyObj
#define _MyObj
#include <GLUT/glut.h>
#include <OpenGL/OpenGL.h>
#include "VectorAndPoint.h"
#include <vector>
using namespace std;
class MyObj {
public:
    //num of data for obj
    int num_of_vertices;
    int num_of_normals;
    int num_of_indices;
    //data for Obj
    Vector3D *vertices = NULL;
    Vector3D *normals = NULL;
    Point2D *texture_coordinates = NULL;
    GLuint *indices = NULL;
    GLuint *normal_indices = NULL;
    GLuint *texture_indices = NULL;

    //translation, scale, and rotate
    Vector3D translation;		
    Vector3D scaleFactor;		
    Vector3D rotateFactor;
    float mat_ambient[4];
    float mat_specular[4];
    float mat_diffuse[4];
    float mat_shininess[1];
    int textureID;
    
    //Functions
    MyObj(){}
    MyObj (std::vector<Vector3D> &vertices,
           std::vector<Vector3D> &normals,
           std::vector<GLuint> &indices,
           std::vector<GLuint> &normal_indices
           ){
        this->num_of_vertices = (int) vertices.size();
        this->num_of_normals = (int) normals.size();
        this->num_of_indices = (int) indices.size();
        
        this->vertices = &vertices[0];
        this->normals = &normals[0];
        this->indices = &indices[0];
        this->normal_indices = &normal_indices[0];
        
        this->mat_ambient[0] = 0.2;
        this->mat_ambient[1] = 0.2;
        this->mat_ambient[2] = 0.2;
        this->mat_ambient[3] = 1.0;
        this->mat_specular[0] = 0.0;
        this->mat_specular[1] = 0.0;
        this->mat_specular[2] = 0.0;
        this->mat_specular[3] = 1.0;
        this->mat_diffuse[0] = 1.0;
        this->mat_diffuse[1] = 1.0;
        this->mat_diffuse[2] = 1.0;
        this->mat_diffuse[3] = 1.0;
        this->mat_shininess[0] = 0;
        
        this->translation.x = this->translation.y = this->translation.z = 0.6f;
        this->scaleFactor.x = this->scaleFactor.y = this->scaleFactor.z = 1.0f;
        this->rotateFactor.x = this->rotateFactor.y = this->rotateFactor.z = 0.0f;
        
    };

    void makeMove(Vector3D position);
    void makeScale(Vector3D scale);
    void makeRotate(Vector3D rotate);
    void setMatAmbient(float x0, float x1, float x2, float x3);
    void setMatSpecular(float x0, float x1, float x2, float x3);
    void setMatdiffuse(float x0, float x1, float x2, float x3);
    
    void draw ();
    void setTextureMapID (int textureID);
    void setTextute(std::vector<Point2D> &tex_cord, std::vector<GLuint> &indicies);
    void getNormal();
};


void load_obj (std::string filename, MyObj **mesh);

#endif




