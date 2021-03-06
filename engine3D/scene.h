#pragma once
#include "shader.h"
#include "shape.h"
#include "camera.h"
#include <vector>

class Scene : public MovableGLM
{
protected:
	std::vector<Shader*> shaders;
	std::vector<Camera*> cameras; //light will have the properties of camera
	Shape *axisMesh;

	std::vector<int> chainParents;
	int pickedShape;
	static const int scaleFactor = 3;


public:
	int viewIndex = 0;
	glm::vec3 sceneColor = glm::vec3(20,5,0);

	std::vector<Shape*> shapes;
	int destinationIndex;
	glm::vec3 cameraOriginalPosition;
	glm::vec3 destinationPosition;

	enum axis{xAxis,yAxis,zAxis};
	enum transformations{xLocalTranslate,yLocalTranslate,zLocalTranslate,xGlobalTranslate,yGlobalTranslate,zGlobalTranslate,
		xLocalRotate,yLocalRotate,zLocalRotate,xGlobalRotate,yGlobalRotate,zGlobalRotate,xScale,yScale,zScale,xCameraTranslate,yCameraTranslate,zCameraTranslate};
	Scene();
	Scene(glm::vec3 position,float angle,float hwRelation,float near, float far);
	void addShape(int type,int parent);
	void addShape(const std::string& fileName,int parent);
	void addShape(const std::string& fileName,const std::string& textureFileName,int parent);
	void addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices,int parent);
	void addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices,const std::string& textureFileName,int parent, ShapeType type);
	void addShape(int Cyparts,int linkPosition,int parent);
	void addShape(int CylParts,int linkPosition,const std::string& textureFileName,int parent, ShapeType type);
	
	void addShader(const std::string& fileName);
	std::vector<Shape*> GetShapes() { return shapes; }
	std::vector<Camera*> GetCameras() { return cameras; }

	glm::mat4 GetViewProjection(int indx) const;
	glm::mat4 GetShapeTransformation() const;
	void draw(int shaderIndx, int cameraIndx, bool drawAxis);
	
	void shapeLocalTranslation(glm::vec3& v, int indx);
	void shapeTransformation(int type,float amt);
	void shapeTransformation(int type, float amt, glm::vec3 vec);

	void shapeRotation(glm::vec3 v, float ang,int indx);
	void shapeEulerAnglesRotation(float z_angle, float x_angle, int index);//

	void inline setPicked(int pickID) { pickedShape = pickID; }//
	int inline getPicked() { return pickedShape; }//
	float picking(double x,double y);
	
	void resize(int width,int hight,int near,int far);
	//void updateTipPosition(int indx);
	glm::vec3 getTipPosition(int indx);
	glm::vec3 getGoalPosition();//
	glm::vec3 getNextGoalPosition();

	glm::vec3 getAxisDirection(int indx,int axis);
	glm::vec3 getBase(int index);
	void restartAnglesForShape(int indx);//
	glm::mat4 getNormalForShape(int shapeIndex);//

	inline void setParent(int indx,int newValue) {chainParents[indx]=newValue;}
	virtual ~Scene(void);

	
};

