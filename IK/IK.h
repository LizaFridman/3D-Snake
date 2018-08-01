#include "scene.h"
#define EPSILON 1e-4f
#define ROTATION_ANGLE 10.f
#define DISTANCE_DELTA 1.f

const int maximumIterations = 1;
const double minDistance = 5.;
/// Snake ///
const int linksNum = 6;
const int headLink = linksNum - 1;
/// Scene Objects ///
const int grassIndex = linksNum + 1;
const int caveIndex = grassIndex + 1;
/// Obsticles ///
const int obsticlesFirstIndex = caveIndex + 1;
const int numOfObsticles = 6;
const int obsticlesLastIndex = obsticlesFirstIndex + numOfObsticles - 1;
/// Food ///
const int foodFirstIndex = obsticlesLastIndex + 1;
const int numOfFood = 3;
const int foodLastIndex = foodFirstIndex + numOfFood - 1;
const int foodPoints = 10;
/// Camera ///
const int skyViewIndex = 0;
const int snakeViewIndex = 1;

enum GameMode { PAUSED, GAME_LOST, GAME_WON, ONGOING};

class IK : public Scene
{
	bool cameraMode;
	glm::vec3 tipPosition;
	bool isIKactive;
	
    float maxDistance;
	float linkLength;

public:
	bool movementActive = true;
	int pointsCounter = 0;
	GameMode gameMode = ONGOING;
	int displayWidth = 1200;
	int displayHeight = 800;
	bool gameOver = false;

	IK(void);
	virtual ~IK(void);
	IK(glm::vec3 position,float angle,float hwRelation,float near, float far);
	void updateViews();
	void update_movement();
	void pausePressed();

	void init(Vertex *vertices,unsigned int *indices,int verticesSize,int indicesSize);
	//void buildLevel();

	void addShape(int type, int parent);
	void addShape(const std::string& fileName, int parent);
	void addShape(const std::string& fileName,const std::string& textureFileName, int parent);
	void addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices, int parent);
	void addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices,const std::string& textureFileName, int parent, ShapeType type);
	void addShape(int CylParts,int linkPosition,int parent);
	void addShape(int CylParts,int linkPosition,const std::string& textureFileName,int parent, ShapeType type);

	void inline changeMode(){cameraMode = !cameraMode;}
	bool inline getMode() { return cameraMode; }
	
	void IK::UpdateSnakeMovement(int dirFactor);
	void changeScene();
	
	void reset_euler_angles(int shapeIndex);
	void calculateSnakeStep();
	void detectCollision();
	bool areShapesCollided(int firstIndex, int otherIndex);
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

	Shape* is_snake_collided();
};

