#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "EulerAngles.h"
#define V_UP vec3(0,1,0);
#define V_DOWN vec3(0,-1,0);
#define V_LEFT vec3(1,0,0);
#define V_RIGHT vec3(-1,0,0);

class MovableGLM : public EulerAngles
{
public :
	enum Direction { NONE = 0, RIGHT, LEFT, UP, DOWN };
	
private:
	glm::mat4 translateMat[2];
	glm::mat4 rotateMat;
	//vec3 findAxis2(vec3 vec, int indx);

	glm::vec3 scaleFactor;
	

	//glm::vec3 getTipPos(glm::mat4 &prevTransformations);
	//glm::vec3 getRootPos(glm::mat4 &prevTransformations);
	
	//glm::vec3 getXdirection(glm::mat4 &prevTransformations);
	//glm::vec3 getZdirection(glm::mat4 &prevTransformations);
protected:
	Direction direction;

	glm::vec3 findAxis(glm::vec3 vec) const;

public:
	glm::vec3 v = glm::vec3(0, 0, 1);
	glm::vec3 normal = glm::vec3(0, 1, 0);
	//vec3 findAxis(vec4 &vec, int indx);
	//vec3 findAxis2(vec4 &vec, int indx);
	MovableGLM();
	glm::mat4 makeTransScale(glm::mat4 &prevTransformations) const;
	//glm::mat4 makeTransScale() const;
	glm::mat4 makeTrans(glm::mat4 &prevTransformations) const;
	glm::mat4 makeTrans() const;

	glm::mat4 getRotationMatrix() { return rotateMat; }
	void setRotationMatrix(glm::mat4 newMatrix) { rotateMat = newMatrix; }

	glm::mat4 getTranslationMatrix(int indx) { return translateMat[indx]; }
	void setTranslationMatrix(glm::mat4 newMatrix, int indx) { translateMat[indx] = newMatrix; }

	void myRotate(float ang, glm::vec3 &vec, int indx);
	void myTranslate(glm::vec3 &vec, int indx);
	void myScale(glm::vec3 &vec);
	void globalSystemRot(float ang, glm::vec3 &vec, int indx);
	void changeCenterOfRotation(glm::vec3 &Pvec);
	void zeroTrans();
	void getTraslate(glm::vec4 &vec);
	void copyTranslations(MovableGLM *mGLM);
	void translateInSystem(const MovableGLM  &system,glm::vec3 &vec, int indx, bool toScale);

	void update();
	void set_velocity(glm::vec3 v);
	void add_velocity(glm::vec3 v);
	
	Direction getDirection() { return direction; }

	glm::vec4 getTraslate();
	glm::mat4 getRotate();

	glm::vec3 getPointInSystem(glm::mat4 &prevTransformations,glm::vec3 point);
	glm::vec3 getVectorInSystem(glm::mat4 &prevTransformations,glm::vec3 vec);
	glm::vec3 getCenterOfRotation(glm::mat4 &prevTransformations);
	//float getTraslateZ() { return makeTrans()[3][2]; }
	//inline void rotateVec4(vec4 &vec) { vec = rotateMat[0] *vec; }

	float findAngleBetweenTwoVectors(glm::vec3 baseVec, glm::vec3 newVec);
};