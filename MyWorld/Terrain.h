struct TerrainVertex
{
	Vector3D position;
	Vector3D normal;
};
struct TerrainQuad
{
	// pointers to vertices of each quad
	TerrainVertex *vertices[4];	
};
class Terrain
{
private:
	int maxGridSize;
	int minGridSize;
    
	int numVertices;
	TerrainVertex *vertices;

	int numQuads;
	TerrainQuad *quads;
	
	GLfloat mat_ambient[4];
    GLfloat mat_specular[4];
    GLfloat mat_diffuse[4];
	GLfloat mat_shininess[1];
public:
    int textureID = -1;
	Terrain(int maxGridSize = 40);
	bool InitGrid(int gridSize, Vector3D origin, double gridLength, double gridWidth);
	void DrawGrid(int gridSize);
	void UpdateGrid();
	void GetNormalVec();
    void setTextureID(int textureID) {
        this->textureID = textureID;
    };
};

