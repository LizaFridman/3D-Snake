#include "IK.h"
#include "shape.h"
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
		//headDirection = direction;
		//direction = UP;
		//destinationPosition = vec3(1,0,0);
		tipPosition = vec3(0,0,2*linksNum*scaleFactor);
		maxDistance = linksNum*2.0f*scaleFactor;
	}

	IK::IK(vec3 position,float angle,float hwRelation,float nearf, float farf) : Scene( position,  angle,  hwRelation,  nearf,  farf)
	{
		cameraMode = false;
		isIKactive = false;
		//direction = UP;
		//destinationPosition = vec3(1,0,0);
		tipPosition = vec3(0,0,2*linksNum*scaleFactor);
		maxDistance = linksNum*2.0f*scaleFactor;

		cameras.push_back(new Camera(glm::vec3(0), glm::vec3(0.0f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, -1.0f), angle, hwRelation, nearf, farf));
	}

	
	void IK::updateViews()
	{
		int snake = headLink - 2;
		auto pos = getBase(snake);
		auto direction = glm::normalize(getTipPosition(snake) - getBase(snake));

		cameras[snakeViewIndex]->pos = glm::vec3(pos.x, pos.z, pos.y - 15);
		cameras[snakeViewIndex]->forward = glm::normalize((glm::vec3(direction.x, direction.z, 0) * (1.0f / 2.0f)) + glm::vec3(0, 0, 0.5));

		cameras[skyViewIndex]->pos = glm::vec3(pos.x, pos.z, pos.y - 50);
		cameras[skyViewIndex]->forward = glm::vec3(0, 0, 1);
	}

	void IK::pausePressed() {
		if (gameMode == ONGOING)
			gameMode = PAUSED;
		else {
			gameMode = ONGOING;
		}
	}

	IK::~IK(void)
	{
	}

	void IK::init(Vertex *vertices, unsigned int *indices, int verticesSize, int indicesSize)
	{
		myRotate(-90.0f, vec3(1, 0, 0), -1);
		//myRotate(90.0f, vec3(0, 0, 1), -1);
		pickedShape = -1;
    
		//shapeTransformation(yCameraTranslate, 100.f);
		//shapeTransformation(zCameraTranslate, 250.f);
		//addShape(vertices, verticesSize, indices, indicesSize,"./res/textures/plane.png",-1);
		cameraOriginalPosition = GetCameras()[0]->getCameraPosition();

		addShape(0, 2, "./res/textures/red_snake.jpg", -1, SNAKE_LINK);
    
		//shapeTransformation(xLocalRotate, -90.f);
		pickedShape = 0;
		shapeTransformation(zScale, scaleFactor);

		for (int i = 1; i < headLink; i++)
		{
			addShape(1, 1, "./res/textures/red_snake.jpg", -1, SNAKE_LINK);
			pickedShape = i;
			shapeTransformation(zScale, scaleFactor);

			shapeTransformation(zGlobalTranslate, 1.0);
			setParent(i, i - 1);
		}

		addShape(0, 3, "./res/textures/Tiger_Tiled_bmp.bmp", -1, SNAKE_LINK);
		pickedShape = headLink;
		shapeTransformation(zScale, scaleFactor);

		shapeTransformation(zGlobalTranslate, 1.0);
		setParent(headLink, headLink - 1);

		pickedShape = 0;
		shapeTransformation(zGlobalTranslate, -15.0);

		//// Pointer shape ////
		addShape(vertices, verticesSize, indices, indicesSize, "./res/textures/box0.bmp", -1, INVISIBLE);
		pickedShape = linksNum;
		destinationIndex = pickedShape;

		shapeTransformation(xScale, 0.5);
		shapeTransformation(yScale, 0.5);
		shapeTransformation(zScale, 0.5);
		shapeTransformation(zGlobalTranslate, (linksNum + 3) * 4.0f);

		/// Grass ///
		addShape(vertices, verticesSize, indices, indicesSize, "./res/textures/grass.bmp", -1, FLOOR);
		pickedShape = grassIndex;
		auto width = 160, height = 120;
		shapeTransformation(yGlobalTranslate, -5);
		shapeTransformation(zScale, height);
		shapeTransformation(xScale, width);

		/// Cave ///
		addShape(vertices, verticesSize, indices, indicesSize, "./res/textures/Stone_02_COLOR.bmp", -1, CAVE);
		pickedShape = caveIndex;
		shapeTransformation(zGlobalTranslate, height / 2 - 10.);
		//shapeTransformation(xGlobalTranslate, width/2);
		shapeTransformation(zScale, scaleFactor * 2);
		shapeTransformation(xScale, scaleFactor * 4);
		shapeTransformation(yScale, scaleFactor * 2);
		
		float minWidth = width / 1.3, maxWidth = -width / 1.3;
		float minHeight = -height / 1.2, maxHeight = height / 2;
		/// Obsticles ///
		for (int i = obsticlesFirstIndex; i <= obsticlesLastIndex; i++) {
			addShape(vertices, verticesSize, indices, indicesSize, "./res/textures/Stone_02_NRM.bmp", -1, OBSTICLE_BOX);
			pickedShape = i;
			auto xRand = minWidth + rand() / (RAND_MAX / (maxWidth - minWidth));
			auto zRand = minHeight + rand() / (RAND_MAX / (maxHeight - minHeight));
			shapeTransformation(xGlobalTranslate, xRand);
			shapeTransformation(zGlobalTranslate, zRand);

			shapeTransformation(xScale, scaleFactor);
			shapeTransformation(yScale, scaleFactor);
			shapeTransformation(zScale, scaleFactor);
		}

		/// Apples ///
		for (int i = foodFirstIndex; i <= foodLastIndex; i++) {
			addShape(vertices, verticesSize, indices, indicesSize, "./res/textures/apple.jpg", -1, FOOD);
			pickedShape = i;
			auto xRand = (-width / 2) + rand() / (RAND_MAX / (width));
			auto zRand = (-height / 2) + rand() / (RAND_MAX / (height));
			shapeTransformation(xGlobalTranslate, xRand);
			shapeTransformation(zGlobalTranslate, zRand);

			shapeTransformation(xScale, scaleFactor);
			shapeTransformation(yScale, scaleFactor);
			shapeTransformation(zScale, scaleFactor);
		}

		/// Walls ///

		/// North
		addShape(vertices, verticesSize, indices, indicesSize, "./res/textures/Stone_02_COLOR.bmp", -1, WALL);
		pickedShape++;
		shapeTransformation(zGlobalTranslate, height / 2);
		shapeTransformation(xScale, scaleFactor*(width / 2));
		shapeTransformation(zScale, scaleFactor*2);
		/// South
		addShape(vertices, verticesSize, indices, indicesSize, "./res/textures/Stone_02_COLOR.bmp", -1, WALL);
		pickedShape++;
		shapeTransformation(zGlobalTranslate, -height/1.2);
		shapeTransformation(xScale, scaleFactor*(width / 2));
		shapeTransformation(zScale, scaleFactor * 2);
		/// East
		addShape(vertices, verticesSize, indices, indicesSize, "./res/textures/Stone_02_COLOR.bmp", -1, WALL);
		pickedShape++;
		shapeTransformation(xGlobalTranslate, -width / 1.3);
		shapeTransformation(zScale, scaleFactor*(height / 2));
		shapeTransformation(xScale, scaleFactor * 2);
		/// West
		addShape(vertices, verticesSize, indices, indicesSize, "./res/textures/Stone_02_COLOR.bmp", -1, WALL);
		pickedShape++;
		shapeTransformation(xGlobalTranslate, width / 1.3);
		shapeTransformation(zScale, scaleFactor*(height / 2));
		shapeTransformation(xScale, scaleFactor * 2);

		////////////////////////////////////////
		pickedShape = 0;
		tipPosition = getTipPosition(headLink);
		maxDistance = length(tipPosition);
		linkLength = maxDistance / linksNum;
		//pickedShape = linksNum;


	}

	/*void IK::buildLevel(boxVertices,boxIndices,sizeof(boxVertices)/sizeof(boxVertices[0]), sizeof(boxIndices)/sizeof(boxIndices[0])) {
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

	void IK::addShape(int CylParts,int linkPosition,const std::string& fileName,int parent, ShapeType type)
	{	
		__super::addShape(CylParts,linkPosition,fileName,parent, type);
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
	
	void IK::addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices, const std::string &textureFlieName, int parent, ShapeType type)
	{
		
		__super::addShape(vertices,numVertices,indices,numIndices,textureFlieName,parent, type);
	}
	
	void IK::calculateSnakeStep()
	{
		vector<vec3> linkTipPositions;
		for (int i = 0; i < linksNum; i++)
		{
			linkTipPositions.push_back(getBase(i));
		}
		linkTipPositions.push_back(getTipPosition(headLink));

		//std::cout << "==> Starting Step 1" << std::endl;

		auto tailBase = getBase(0);
		//auto nextGoal = getNextGoalPosition();

		auto last_tip_position = linkTipPositions[linksNum];
		auto factor = 0.1f;
		/*
		auto velocity = shapes[headLink]->forwardDirection;
		vec3 movement = DISTANCE_DELTA * 2.f* velocity;
		auto directionToMove = vec3(getNormalForShape(headLink) * vec4(movement, 0));
		auto goal = getTipPosition(headLink) + directionToMove;//factor * getGoalPosition() + (1.0f - factor) * last_tip_position;*/
		auto goal = getNextGoalPosition();//factor * nextGoal + (1.0f - factor) * last_tip_position;;
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
		goal = temp;

		for (int i = 1; i < linkTipPositions.size(); i++)
		{
			linkTipPositions[i - 1] = goal;
			auto direction = normalize(linkTipPositions[i] - goal);
			goal = goal + direction * (float)scaleFactor;
		}

		linkTipPositions[linksNum] = goal;
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
	}
		

	Shape & IK::getSnakeHead()
	{
		return *(shapes[headLink]);
	}

	void IK::setDirectionRight() {
		//Sleep(50);
		destinationPosition = glm::vec3(-50,0,-50);
	}
	void IK::setDirectionLeft() {
		//Sleep(50);
		destinationPosition = glm::vec3(50, 0, -50);
	}

	void IK::setDirectionUp() {
		//Sleep(50);
		destinationPosition = glm::vec3(0, 50, 0);
	}

	void IK::setDirectionDown() {
		//Sleep(50);
		destinationPosition = glm::vec3(0, -50, 0);
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
	
	void IK::changeScene()
	{
		detectCollision();
		calculateSnakeStep();
		updateViews();
	}

	void IK::detectCollision() {
		for (int i = linksNum + 1; i < shapes.size(); i++) {
			auto type = shapes[i]->type;
			if(type != INVISIBLE && areShapesCollided(headLink, i)){
				switch (type) {
				case OBSTICLE_BOX:
				case WALL:
					gameMode = GAME_LOST;
					break;
				case FOOD:
					pointsCounter+=foodPoints;
					shapes[i]->type = INVISIBLE;
					break;
				case CAVE:
					gameMode = GAME_WON;
					break;
				case FLOOR:
				case SNAKE_LINK:
				case INVISIBLE:
				default:
					break;
				}
				std::cout << "Snake head collided with shape index: " << i << " type: " << type << std::endl;
			}
		}
	}

	bool IK::areShapesCollided(int firstIndex, int otherIndex) {
		auto firstPosition = getBase(firstIndex);
		auto otherPosition = getBase(otherIndex);
		auto dist = distance(firstPosition, otherPosition);
		return dist < minDistance;
	}

void IK::reset_euler_angles(int shapeIndex)
{
	shapes[shapeIndex]->reset_euler_angles();
}

void IK::update_movement()
{
	auto direction = glm::normalize(getTipPosition(headLink) - getBase(headLink))  * 0.1f;
	pickedShape = 0;

	int i = pickedShape;
	for (; chainParents[i] > -1; i = chainParents[i]);
	shapes[i]->myTranslate(direction, 0);
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

Shape* IK::is_snake_collided() {
	for (int i = 0; i < linksNum ; i++) {
			for (int j = linksNum + 1; j < shapes.size(); j++) {
				if (shapes[i]->type != INVISIBLE &&
					shapes[j]->type != INVISIBLE &&
					shapes[j]->collides_with(shapes[i])) {
					return shapes[j];
				}
			}
		
	}

	return NULL;
}