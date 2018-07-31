
#include "scene.h"
#define EPSILON 1e-4f
#define ROTATION_ANGLE 10.f
#define DISTANCE_DELTA 10.f

const int linksNum = 6;
const int maximumIterations = 1;
const int headLink = linksNum - 1;

const int grassIndex = linksNum + 1;

class IK : public Scene
{
	bool cameraMode;
	glm::vec3 tipPosition;
	glm::vec3 destinationPosition;
	bool isIKactive;
	double delta;
    float maxDistance;
	float linkLength;
	//glm::mat4 prevRotate;
	//Direction headDirection;
public:
	bool movementActive = true;
	int pointsCounter = 0;

	int displayWidth = 1200;
	int displayHeight = 800;

	IK(void);
	virtual ~IK(void);
	IK(glm::vec3 position,float angle,float hwRelation,float near, float far);
	
	void init(Vertex *vertices,unsigned int *indices,int verticesSize,int indicesSize);
	//void buildLevel();

	void addShape(int type, int parent);
	void addShape(const std::string& fileName, int parent);
	void addShape(const std::string& fileName,const std::string& textureFileName, int parent);
	void addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices, int parent);
	void addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices,const std::string& textureFileName, int parent);
	void addShape(int CylParts,int linkPosition,int parent);
	void addShape(int CylParts,int linkPosition,const std::string& textureFileName,int parent);

	void inline changeMode(){cameraMode = !cameraMode;}
	bool inline getMode() { return cameraMode; }
	
	void IK::UpdateSnakeMovement(int dirFactor);
	void makeIKChange();
	
	void reset_euler_angles(int shapeIndex);
	void calculateSnakeStep();
	Shape & getSnakeHead();

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

