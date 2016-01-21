#include <OpenGL/glu.h>
#include <OpenGL/glu.h>
#include <fstream>
#include <sstream>
#include <istream>
#include <cmath>
#include <stdio.h>
#include <vector>
#include "VectorAndPoint.h"
#include "MyObj.h"

using namespace std;

void MyObj::makeMove (Vector3D position)
{
    translation.x += position.x;
    translation.y += position.y;
    translation.z += position.z;
};
void MyObj::makeScale(Vector3D scale)
{
    scaleFactor.x*=scale.x;
    scaleFactor.y*=scale.y;
    scaleFactor.z*=scale.z;
}
void MyObj::makeRotate(Vector3D rotate)
{
    rotateFactor.x+=rotate.x;
    rotateFactor.y+=rotate.y;
    rotateFactor.z+=rotate.z;
}

void MyObj :: setTextute(std::vector<Point2D> &tex_cord, std::vector<GLuint> &indicies)
{
    texture_coordinates = &tex_cord[0];
    texture_indices = &indicies[0];
}
void MyObj::setMatAmbient(float x0, float x1, float x2, float x3)
{
    mat_ambient[0] = x0;
    mat_ambient[1] = x1;
    mat_ambient[2] = x2;
    mat_ambient[3] = x3;
}
void MyObj::setMatSpecular(float x0, float x1, float x2, float x3)
{
    mat_specular[0] = x0;
    mat_specular[1] = x1;
    mat_specular[2] = x2;
    mat_specular[3] = x3;
}
void MyObj::setMatdiffuse(float x0, float x1, float x2, float x3)
{
    mat_diffuse[0] = x0;
    mat_diffuse[1] = x1;
    mat_diffuse[2] = x2;
    mat_diffuse[3] = x3;
}
void MyObj :: draw()
{
    //Material
    glMaterialfv(GL_FRONT, GL_AMBIENT, this->mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, this->mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, this->mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, this->mat_shininess);
    
    glPushMatrix();
    
    //Transformations
    glTranslatef(this->translation.x, this->translation.y, this->translation.z);
    
    glRotatef(this->rotateFactor.x, 1, 0, 0);
    glRotatef(this->rotateFactor.y, 0, 1, 0);
    glRotatef(this->rotateFactor.z, 0, 0, 1);
    glScalef(this->scaleFactor.x, this->scaleFactor.y, this->scaleFactor.z);
    
    //Rendering
    glBindTexture(GL_TEXTURE_2D, this->textureID);
    glBegin(GL_TRIANGLES);
    glEnable(GL_BLEND);
    
    if (this->texture_coordinates) {
        for(int i = 0; i < num_of_indices; i +=3) {
            
            if(this->normals)
            glNormal3f(this->normals[this->normal_indices[i]].x,
                       this->normals[this->normal_indices[i]].y,
                       this->normals[this->normal_indices[i]].z);
        
            glTexCoord2f(this->texture_coordinates[this->texture_indices[i]].x,
                         this->texture_coordinates[this->texture_indices[i]].y);
            glVertex3f(this->vertices[this->indices[i]].x,
                       this->vertices[this->indices[i]].y,
                       this->vertices[this->indices[i]].z);
            glTexCoord2f(this->texture_coordinates[this->texture_indices[i+1]].x,
                         this->texture_coordinates[this->texture_indices[i+1]].y);
            
            glVertex3f(this->vertices[this->indices[i+1]].x,
                       this->vertices[this->indices[i+1]].y,
                       this->vertices[this->indices[i+1]].z);
            
            glTexCoord2f(this->texture_coordinates[this->texture_indices[i+2]].x,
                         this->texture_coordinates[this->texture_indices[i+2]].y);
            glVertex3f(this->vertices[this->indices[i+2]].x,
                       this->vertices[this->indices[i+2]].y,
                       this->vertices[this->indices[i+2]].z);
        }
    } else {
        for(int i = 0; i < num_of_indices; i +=3) {
            
            if(this->normals)
            glNormal3f(this->normals[this->normal_indices[i]].x,
                       this->normals[this->normal_indices[i]].y,
                       this->normals[this->normal_indices[i]].z);
            
            glVertex3f(this->vertices[this->indices[i]].x,
                       this->vertices[this->indices[i]].y,
                       this->vertices[this->indices[i]].z);
            
            glVertex3f(this->vertices[this->indices[i+1]].x,
                       this->vertices[this->indices[i+1]].y,
                       this->vertices[this->indices[i+1]].z);
            
            glVertex3f(this->vertices[this->indices[i+2]].x,
                       this->vertices[this->indices[i+2]].y,
                       this->vertices[this->indices[i+2]].z);
        }
    }
    
    glDisable(GL_BLEND);
    glEnd();
    
    glPopMatrix();
};

void MyObj :: setTextureMapID (int textureID)
{
    this->textureID = textureID;
}

void MyObj::getNormal()
{
    
}

void load_obj (string filename, MyObj **mesh) {
    ifstream myFile;
    string line;
    string delimiter = "/";
    string delimiter2 = "//";
    
    vector<Vector3D> * vertices = new vector<Vector3D>;
    vector<Vector3D> * normals = new vector<Vector3D>;
    vector<Point2D> * tex_cord = new vector<Point2D>;
    vector<GLuint> * indices = new vector<GLuint>;
    vector<GLuint> * normal_indices = new vector<GLuint>;
    vector<GLuint> * tex_indices = new vector<GLuint>;
    
    bool is_text_cord = false;
    
    myFile.open(filename);
    
    if (myFile.is_open()){
        while (getline(myFile, line)) {
            if (line.substr(0,2) == "v ") {
                istringstream s(line.substr(2));
                Vector3D v; s >> v.x; s >> v.y; s >> v.z;
                vertices->push_back(v);
            }  else if (line.substr(0,2) == "f ") {
                istringstream s(line.substr(2));
                string token;
                GLushort a, b, c;
                string s2;
                
                if (is_text_cord) {
                    for(int i = 0; i < 3;  i++) {
                        s >> s2;
                        //Face
                        token = s2.substr(0, s2.find(delimiter));
                        a = ::atof(token.c_str()); a--;
                        s2.erase(0, s2.find(delimiter) + delimiter.length());
                        indices->push_back(a);
                    
                        //UV Indice
                        token = s2.substr(0, s2.find(delimiter));
                        b = ::atof(token.c_str()); b--;
                        s2.erase(0, s2.find(delimiter) + delimiter.length());
                        tex_indices->push_back(b);
                        
                        //Normal Indice
                        token = s2.substr(0, s2.find(delimiter));
                        c = ::atof(token.c_str()); c--; // normal indice
                        normal_indices->push_back(c);
                    }
                } else {
                    for(int i = 0; i < 3;  i++) { 
                        s >> s2;
                        //Face
                        token = s2.substr(0, s2.find(delimiter2));
                        a = ::atof(token.c_str()); a--; // face indice
                        s2.erase(0, s2.find(delimiter2) + delimiter2.length());
                        indices->push_back(a);
                        
                        //Normal Indice
                        token = s2.substr(0, s2.find(delimiter2));
                        c = ::atof(token.c_str()); c--; // normal indice
                        normal_indices->push_back(c);
                    }
                }
                
            } else if (line.substr(0,2) == "vn") {
                istringstream s(line.substr(2));
                Vector3D vn; s >> vn.x; s >> vn.y; s >> vn.z;
                normals->push_back(vn);
                
            } else if (line.substr(0,2) == "vt") {
                is_text_cord = true;
                istringstream s(line.substr(2));
                Point2D uv; s >> uv.x; s >> uv.y;
                tex_cord->push_back(uv);
            }
            
            else if (line[0] == '#') {}
            else {}
        }
        myFile.close();
    } else {
        printf("Unable to open %s\n",filename.c_str());
    }
    
    (*mesh) = new MyObj(*vertices,*normals,*indices, *normal_indices);
    
    if (is_text_cord) {
        (*mesh)->setTextute(*tex_cord,*tex_indices);
    }
};
