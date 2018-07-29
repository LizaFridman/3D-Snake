#include "IK.h"
#include <iostream>
#include <Windows.h>
using namespace std;
using namespace glm;


	void printArr(vector<int> arr)
	{
		
		std::cout<<"array"<<std::endl;
		for (int i = 0; i < arr.size(); i++)
		{
			std::cout<<i<<"  ";
		}
		std::cout<<std::endl;
		for(int p : arr)
		{
			std::cout<<p<<"  ";
		}
		cout<<std::endl;
	}
	
	IK::IK(void)
	{
		cameraMode = false;
		isIKactive = false;
		delta = 1e-1;
		//headDirection = direction;
		//direction = UP;
		destinationPosition = vec3(1,0,0);
		tipPosition = vec3(0,0,2*linksNum*scaleFactor);
		maxDistance = linksNum*2.0f*scaleFactor;
	}

	IK::IK(vec3 position,float angle,float hwRelation,float nearf, float farf) : Scene( position,  angle,  hwRelation,  nearf,  farf)
	{
		cameraMode = false;
		isIKactive = false;
		delta = 1e-1;
		//direction = UP;
		destinationPosition = vec3(1,0,0);
		tipPosition = vec3(0,0,2*linksNum*scaleFactor);
		maxDistance = linksNum*2.0f*scaleFactor;
	}

	IK::~IK(void)
	{
	}

	void IK::init(Vertex *vertices, unsigned int *indices, int verticesSize, int indicesSize)
	{
		myRotate(-90.0f, vec3(1, 0, 0), -1);
		//myRotate(90.0f, vec3(0, 0, 1), -1);
		pickedShape = -1;
		shapeTransformation(yCameraTranslate, 100.f);
		shapeTransformation(zCameraTranslate,250.f);
		//addShape(vertices, verticesSize, indices, indicesSize,"./res/textures/plane.png",-1);

		addShape(0, 2, "./res/textures/Green-Barbed.bmp", -1);
		//shapeTransformation(xLocalRotate, -90.f);
		pickedShape = 0;
		shapeTransformation(zScale, scaleFactor);

		for (int i = 1; i < headLink; i++)
		{
			addShape(1, 1, "./res/textures/Green-Barbed.bmp", -1);
			pickedShape = i;
			shapeTransformation(zScale, scaleFactor);

			shapeTransformation(zGlobalTranslate, 1.0);
			setParent(i, i - 1);
		}

		addShape(0, 3, "./res/textures/Green-Barbed.bmp", -1);
		pickedShape = headLink;
		shapeTransformation(zScale, scaleFactor);

		shapeTransformation(zGlobalTranslate, 1.0);
		setParent(headLink, headLink - 1);

		pickedShape = 0;
		// distination point

		//addShape(0,"./res/textures/box0.bmp",-1);
		addShape(vertices, verticesSize, indices, indicesSize, "./res/textures/box0.bmp", -1);
		pickedShape = linksNum;
		destinationIndex = pickedShape;

		shapeTransformation(xScale, 0.5);
		shapeTransformation(yScale, 0.5);
		shapeTransformation(zScale, 0.5);
		//shapeTransformation(xGlobalTranslate, -8.0f);
		//shapeTransformation(yGlobalTranslate, -3.0f);
		shapeTransformation(zGlobalTranslate, (linksNum + 3) * 4.0f);
		pickedShape = 0;

		//distPosition = getGoalPosition();

		tipPosition = getTipPosition(headLink);
		maxDistance = length(tipPosition);
		linkLength = maxDistance / linksNum;
		//pickedShape = linksNum;
	}

	/*void IK::buildLevel() {
		pickedShape = 0;
		// distination point
		pickedShape = linksNum;

		//addShape(0,"./res/textures/box0.bmp",-1);
		addShape(vertices, verticesSize, indices, indicesSize, "./res/textures/box0.bmp", -1);
		shapeTransformation(xScale, 0.5);
		shapeTransformation(yScale, 0.5);
		shapeTransformation(zScale, 0.5);
		shapeTransformation(xGlobalTranslate, -8.0f);
		shapeTransformation(yGlobalTranslate, -3.0f);
		shapeTransformation(zGlobalTranslate, -5.0f);
		pickedShape = 0;

	}*/

	void IK::addShape(int CylParts,int linkPosition,int parent)
	{
		
		__super::addShape(CylParts,linkPosition,parent);
	}

	void IK::addShape(int CylParts,int linkPosition,const std::string& fileName,int parent)
	{	
		__super::addShape(CylParts,linkPosition,fileName,parent);
	}

	void IK::addShape(int type, int parent) 
	{
		
		__super::addShape(type,parent);
	}

	void IK::addShape(const std::string& fileName, int parent)
	{
		
		__super::addShape(fileName,parent);
	}

	void IK::addShape(const std::string& fileName,const std::string& textureFileName, int parent)
	{
		
		__super::addShape(fileName,textureFileName,parent);
	}

	void IK::addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices, int parent)
	{
		
		__super::addShape(vertices,numVertices,indices,numIndices,parent);
	}
	
	void IK::addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices, const std::string &textureFlieName, int parent)
	{
		
		__super::addShape(vertices,numVertices,indices,numIndices,textureFlieName,parent);
	}
	
	void IK::calculateSnakeStep()
	{
		movementActive = false;
		vector<vec3> linkTipPositions;
		auto tailBase = getBase(0);
		auto nextGoal = getNextGoalPosition();
		for (int i = 0; i < linksNum; i++)
		{
			linkTipPositions.push_back(getBase(i));
		}
		linkTipPositions.push_back(getTipPosition(headLink));

		//std::cout << "==> Starting Step 1" << std::endl;

		auto last_tip_position = linkTipPositions[linksNum];
		auto factor = 0.1f;
		/*
		auto velocity = shapes[headLink]->forwardDirection;
		vec3 movement = DISTANCE_DELTA * 2.f* velocity;
		auto directionToMove = vec3(getNormalForShape(headLink) * vec4(movement, 0));
		auto goal = getTipPosition(headLink) + directionToMove;//factor * getGoalPosition() + (1.0f - factor) * last_tip_position;*/
		auto goal = factor * nextGoal + (1.0f - factor) * last_tip_position;;
		auto targetDiff = goal - last_tip_position;
		auto distance = dot(targetDiff, targetDiff);

		if (distance > 0.5f)
		{
			goal = last_tip_position + 0.5f * normalize(targetDiff);
		}

		for (int i = linkTipPositions.size() - 1; i > 0; i--)
		{
			linkTipPositions[i] = goal;
			auto direction = normalize(linkTipPositions[i - 1] - goal);
			goal = goal + direction * (float)scaleFactor;
		}

		//std::cout << "==> Starting Step 2" << std::endl;
		auto temp = linkTipPositions[0];
		linkTipPositions[0] = goal;
		/*goal = temp;

		for (uint i = 1; i < linkTipPositions.size(); i++)
		{
			linkTipPositions[i - 1] = goal;
			auto direction = normalize(linkTipPositions[i] - goal);
			goal = goal + direction * (float)scaleFactor;
		}

		linkTipPositions[linksNum] = goal;
		*/
		auto translationVec = linkTipPositions[0] - tailBase;
		shapeLocalTranslation(translationVec, 0);

		//Rotation
		//std::cout << "==> Starting Rotation" << std::endl;
		auto yAxis = vec3(0, 1, 0);
		auto zAxis = vec3(0, 0, 1);

		for (uint i = 0; i < linksNum; i++)
		{
			reset_euler_angles(i);

			auto currentLinkTip = linkTipPositions[i];
			auto nextLinkTip = linkTipPositions[i + 1];

			auto diff = nextLinkTip - currentLinkTip;
			auto newZaxis = normalize(diff);
			auto rAxis = cross(zAxis, newZaxis);

			if (rAxis == vec3(0))
			{
				movementActive = false;
				return;
			}

			rAxis = normalize(rAxis);

			if (length(rAxis) > EPSILON)
			{
				auto xAxis = cross(yAxis, zAxis);
				auto proj_on_xy = normalize(newZaxis - dot(newZaxis, zAxis) * zAxis);
				auto z_angle = degrees(acos(clamp(dot(yAxis, proj_on_xy), -1.0f, 1.0f)));
				auto x_angle = degrees(acos(clamp(dot(zAxis, newZaxis), -1.0f, 1.0f)));


				if (dot(proj_on_xy, xAxis) > 0)
				{
					z_angle = -z_angle;
				}

				yAxis = vec3(rotate(x_angle, rAxis) * vec4(yAxis, 0));
				zAxis = newZaxis;
				shapeEulerAnglesRotation(z_angle, -x_angle, i);
			}
		}
		movementActive = true;
	}
		

	Shape & IK::getSnakeHead()
	{
		return *(shapes[headLink]);
	}

	void IK::setDirectionRight() {
		//Sleep(50);
		/*
		Shape& head = getSnakeHead();
		auto rotation = glm::rotate(-ROTATION_ANGLE, head.upDirection);
		auto velDir = rotation * vec4(head.forwardDirection, 0);
		head.forwardDirection = glm::vec3(velDir);*/

		pickedShape = destinationIndex;
		shapeTransformation(xLocalTranslate, -DISTANCE_DELTA*2);

		movementActive = true;
		//Activate();
	}
	void IK::setDirectionLeft() {
		//Sleep(50);
		/*
		Shape& head = getSnakeHead();
		//auto velocity = shapes[headLink]->forwardDirection;
		auto rotation = glm::rotate(ROTATION_ANGLE, head.upDirection);
		auto velDir = rotation * vec4(head.forwardDirection, 0);
		head.forwardDirection = glm::vec3(velDir);*/

		pickedShape = destinationIndex;
		shapeTransformation(xLocalTranslate, DISTANCE_DELTA*2);

		movementActive = true;
		//Activate();
	}

	void IK::setDirectionUp() {
		//Sleep(50);
		/*
		Shape& head = getSnakeHead();
		auto right = head.getRightDirection();
		auto rotation = glm::rotate(ROTATION_ANGLE, right);
		auto forward = rotation * glm::vec4(head.forwardDirection, 0);
		auto up = rotation * glm::vec4(head.upDirection, 0);
		head.forwardDirection = glm::vec3(forward);
		head.upDirection = glm::vec3(up);*/
		
		pickedShape = destinationIndex;
		shapeTransformation(zLocalTranslate, DISTANCE_DELTA);
		//calculateSnakeStep();

		movementActive = true;
		//Activate();
	}

	void IK::setDirectionDown() {
		//Sleep(50);
		/*
		Shape& head = getSnakeHead();
		auto right = head.getRightDirection();
		auto rotation = glm::rotate(-ROTATION_ANGLE, right);
		auto forward = rotation * glm::vec4(head.forwardDirection, 0);
		auto up = rotation * glm::vec4(head.upDirection, 0);
		head.forwardDirection = glm::vec3(forward);
		head.upDirection = glm::vec3(up);*/
		
		pickedShape = destinationIndex;
		shapeTransformation(zLocalTranslate, -DISTANCE_DELTA);

		movementActive = true;
		//Activate();
	}

	void IK::UpdateSnakeMovement(int dirFactor) {
		Sleep(50);
		

		auto prevLinkDirection = getAxisDirection(headLink, xAxis);

		
		//auto prevDirection = vec3(shapes[pickedShape]->getTraslate());
		/*auto prevRotate = shapes[pickedShape]->getRotationMatrix();
		auto preTrans0 = shapes[pickedShape]->getTranslationMatrix(0);
		auto preTrans1 = shapes[pickedShape]->getTranslationMatrix(1);
		auto prevCenter = shapes[pickedShape]->getCenterOfRotation(mat4(1));*/
		pickedShape = headLink;
		shapeTransformation(xLocalTranslate, 0.01f, prevLinkDirection);
		

		/*for (int i = 1 ; i < linksNum; i++) {
			pickedShape = i;
			
			shapes[pickedShape + 1]->setRotationMatrix(prevRotate);
		}*/

		for (int i = headLink - 1; i > -1; i--) {
			
			auto temp = getAxisDirection(i, xAxis);
			//shapes[i]->changeCenterOfRotation(prevCenter);
			pickedShape = i;
			shapeTransformation(xLocalTranslate, 0.01f, normalize(prevLinkDirection));
			prevLinkDirection = temp;
			//pickedShape = i;
			//shapeTransformation(xGlobalTranslate, 0.01f, snakeDirection);
		}
		
			pickedShape = 0;
			shapeTransformation(zLocalTranslate, dirFactor * DISTANCE_DELTA);
			
		//}
	}
	
	void IK::makeIKChange()
	{
		destinationPosition=getNextGoalPosition();
		tipPosition = getTipPosition(headLink);
		auto distFromBase = distance(getBase(0), destinationPosition);

		//Check if within reach
		//std::cout << "base_distance = " << distFromBase << std::endl;
		//std::cout << "max_distance = " << maxDistance << std::endl;
		auto diff = (maxDistance - delta);
		//std::cout << "max-distance - delta = " << diff << std::endl;
		// Unreachable
		/*if(distFromBase >= diff)
		{
			std::cout << "De-activating" << std::endl;
			dActivate();
			return;
		}*/

		// Rotate Links
		calculateSnakeStep();

		//Check if reached to destination
		tipPosition = getTipPosition(headLink);
		//std::cout << "tipPosition = " << tipPosition.x << ", " << tipPosition.y << ", " << tipPosition.z << std::endl;
		//std::cout << "distPosition = " << distPosition.x << ", " << distPosition.y << ", "<< distPosition.z << std::endl;
		
		auto dist = distance(tipPosition, destinationPosition);
		//std::cout << "Distance = " << dist << std::endl;
		if(dist < delta)
		{
			std::cout << "Reached Destination!!" << std::endl;
			dActivate();
			return;
		}

		/*pickedShape = 0;
		shapeTransformation(zLocalTranslate, 0.05f);*/
	}

void IK::reset_euler_angles(int shapeIndex)
{
	shapes[shapeIndex]->reset_euler_angles();
}

void IK::pick_box()
	{
		if (pickedShape != linksNum)
		{
			pickedShape = linksNum;
		}
		else
		{
			pickedShape = 0;
		}
	}

	void IK::pick_next_box()
	{
		pickedShape = (pickedShape + 1) % linksNum;
	}

	void IK::pick_previous_box()
	{
		pickedShape = (linksNum + pickedShape - 1) % linksNum;
	}




	