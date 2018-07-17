
#include "scene.h"
#define EPSILON 1e-4f
#define ROTATION_ANGLE 11.25f
#define DISTANCE_DELTA 0.01f

const int linksNum = 6;
const int maximumIterations = 1;

class IK : public Scene
{
	bool cameraMode;
	glm::vec3 tipPosition;
	glm::vec3 distPosition;
	bool isIKactive;
	double delta;
    float maxDistance;
	float linkLength;

	int headLink;
	//Direction headDirection;
public:
	

	IK(void);
	virtual ~IK(void);
	IK(glm::vec3 position,float angle,float hwRelation,float near, float far);
	
	void init(Vertex *vertices,unsigned int *indices,int verticesSize,int indicesSize);

	void addShape(int type, int parent);
	void addShape(const std::string& fileName, int parent);
	void addShape(const std::string& fileName,const std::string& textureFileName, int parent);
	void addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices, int parent);
	void addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices,const std::string& textureFileName, int parent);
	void addShape(int CylParts,int linkPosition,int parent);
	void addShape(int CylParts,int linkPosition,const std::string& textureFileName,int parent);

	void inline changeMode(){cameraMode = !cameraMode;}
	bool inline getMode() { return cameraMode; }
	
	void IK::UpdateSnakeMovement();
	void makeIKChange();
	
	void reset_euler_angles(int shapeIndex);
	void calculateStep(bool EulerVersion);
	//float Distance(int indx1, int indx2);
	bool inline isActive() const { return isIKactive;} 
	void inline dActivate() { isIKactive = false;}
	void inline Activate() { isIKactive = true; }

	void setDirectionRight();
	void setDirectionLeft();
	void setDirectionUp();
	void setDirectionDown();

	void pick_box();
	void pick_next_box();
	void pick_previous_box();
};

