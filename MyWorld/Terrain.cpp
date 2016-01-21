#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <string.h>
#include <math.h>
#include <utility>
#include <vector>
#include "VectorAndPoint.h"
#include <stdlib.h>
#include "Terrain.h"


Terrain::Terrain(int maxSize)
{
	minGridSize =1;
    maxGridSize = maxSize;
	numVertices = 0;
	vertices = NULL;
	numQuads = 0;
    quads = NULL;
	
    vertices = new TerrainVertex[(maxGridSize+1)*(maxGridSize+1)];
    quads = new TerrainQuad[maxGridSize*maxGridSize];

	mat_ambient[0] = 0.0;
	mat_ambient[1] = 0.0;
	mat_ambient[2] = 0.0;
	mat_ambient[3] = 1.0;
    
	mat_specular[0] = 0.0;
	mat_specular[1] = 0.0;
	mat_specular[2] = 0.0;
	mat_specular[3] = 1.0;
    
	mat_diffuse[0] = 0.9;
	mat_diffuse[1] = 0.5;
	mat_diffuse[2] = 0.0;
	mat_diffuse[3] = 1.0;
    
	mat_shininess[0] = 0.0;
    
}
bool Terrain::InitGrid(int gridSize, Vector3D origin, double gridLength, double gridWidth)
{
    Vector3D rowCount;
    Vector3D iter;
	int currentVertex = 0;
    
	Vector3D colAtom,rowAtom;
	colAtom.x = gridLength;
	colAtom.y = 0;
	colAtom.z = 0;
    rowAtom.x = 0;
    rowAtom.y = 0;
    rowAtom.z = -gridWidth;
    colAtom*=(float)(1.0/gridSize);
    rowAtom*=(float)(1.0/gridSize);
    
	numVertices=(gridSize+1)*(gridSize+1);
	// Starts at front left corner of grid 
	rowCount.Set(origin.x,origin.y,origin.z);

    srand( (unsigned)time( NULL ) );
    for(int i=0; i< gridSize+1; i++)
    {
        for(int j=0; j< gridSize+1; j++)
        {
            // compute vertex position along grid row (along x)
            iter.x = rowCount.x + j * colAtom.x;
            if(!(i > gridSize*1/4 && i < 3*gridSize/4 && j < gridSize*3/4 && j > gridSize*1/4))
                iter.y = rowCount.y + j * colAtom.y + rand()%6 ;
            else
                iter.y = rowCount.y + j * colAtom.y;
            iter.z = rowCount.z + j * colAtom.z;
            vertices[currentVertex].position.Set(iter.x,iter.y,iter.z);
            currentVertex++;
        }
        // go to next row (using z)...
        rowCount += rowAtom;
    }
	numQuads=(gridSize)*(gridSize);
	int currentQuad=0;

	for(int j=0; j < gridSize; j++)
	{
		for(int k=0; k < gridSize; k++)
		{
			// Counterclockwise order
			quads[currentQuad].vertices[0]=&vertices[j*    (gridSize+1)+k];
			quads[currentQuad].vertices[1]=&vertices[j*    (gridSize+1)+k+1];
			quads[currentQuad].vertices[2]=&vertices[(j+1)*(gridSize+1)+k+1];
			quads[currentQuad].vertices[3]=&vertices[(j+1)*(gridSize+1)+k];
			currentQuad++;
		}
	}
	this->GetNormalVec();

	return true;
}


void Terrain::DrawGrid(int gridSize)
{
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    
    int currentQuad=0;
	for(int j=0; j< gridSize; j++)
	{
		for(int k=0; k< gridSize; k++)
		{
            glBindTexture(GL_TEXTURE_2D, this->textureID); // right face of cube
			glBegin(GL_QUADS);
			
			glNormal3f(quads[currentQuad].vertices[0]->normal.x,
				       quads[currentQuad].vertices[0]->normal.y,
					   quads[currentQuad].vertices[0]->normal.z);
            glTexCoord2f(0.0, 0.0);
			glVertex3f(quads[currentQuad].vertices[0]->position.x,
				       quads[currentQuad].vertices[0]->position.y,
					   quads[currentQuad].vertices[0]->position.z);
			
			glNormal3f(quads[currentQuad].vertices[1]->normal.x,
				       quads[currentQuad].vertices[1]->normal.y,
					   quads[currentQuad].vertices[1]->normal.z);
            glTexCoord2f(1.0, 0.0);
			glVertex3f(quads[currentQuad].vertices[1]->position.x,
				       quads[currentQuad].vertices[1]->position.y,
					   quads[currentQuad].vertices[1]->position.z);
			
			glNormal3f(quads[currentQuad].vertices[2]->normal.x,
				       quads[currentQuad].vertices[2]->normal.y,
					   quads[currentQuad].vertices[2]->normal.z);
            glTexCoord2f(1.0, 1.0);
			glVertex3f(quads[currentQuad].vertices[2]->position.x,
				       quads[currentQuad].vertices[2]->position.y,
					   quads[currentQuad].vertices[2]->position.z);
			
			glNormal3f(quads[currentQuad].vertices[3]->normal.x,
				       quads[currentQuad].vertices[3]->normal.y,
					   quads[currentQuad].vertices[3]->normal.z);
            glTexCoord2f(0.0, 1.0);
			glVertex3f(quads[currentQuad].vertices[3]->position.x,
				       quads[currentQuad].vertices[3]->position.y,
					   quads[currentQuad].vertices[3]->position.z);
			glEnd();
			currentQuad++;
		}
	}
}

void Terrain::GetNormalVec() 
{
	int currentQuad=0;

	for(int j=0; j< this->maxGridSize; j++)
	{
		for(int k=0; k< this->maxGridSize; k++)
		{
			Vector3D n0,n1,n2,n3,e0,e1,e2,e3,ne0,ne1,ne2,ne3;
			
			quads[currentQuad].vertices[0]->normal.LoadZero();
			quads[currentQuad].vertices[1]->normal.LoadZero();
			quads[currentQuad].vertices[2]->normal.LoadZero();
			quads[currentQuad].vertices[3]->normal.LoadZero();
			e0 = quads[currentQuad].vertices[1]->position - quads[currentQuad].vertices[0]->position; 
			e1 = quads[currentQuad].vertices[2]->position - quads[currentQuad].vertices[1]->position; 
			e2 = quads[currentQuad].vertices[3]->position - quads[currentQuad].vertices[2]->position; 
			e3 = quads[currentQuad].vertices[0]->position - quads[currentQuad].vertices[3]->position; 
			e0.Normalize();
			e1.Normalize();
			e2.Normalize();
			e3.Normalize();
			
			n0 = e0.CrossProduct(-e3);
			n0.Normalize();
			quads[currentQuad].vertices[0]->normal += n0;
			
			n1 = e1.CrossProduct(-e0);
			n1.Normalize();
			quads[currentQuad].vertices[1]->normal += n1;

			n2 = e2.CrossProduct(-e1);
			n2.Normalize();
			quads[currentQuad].vertices[2]->normal += n2;

			n3 = e3.CrossProduct(-e2);
			n3.Normalize();
			quads[currentQuad].vertices[3]->normal += n3;
			
			quads[currentQuad].vertices[0]->normal.Normalize();
			quads[currentQuad].vertices[1]->normal.Normalize();
			quads[currentQuad].vertices[2]->normal.Normalize();
			quads[currentQuad].vertices[3]->normal.Normalize();

			currentQuad++;
		}
	}
}
