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
		headLink = linksNum - 1;
		direction = UP;
		/*distPosition = vec3(1,0,0);
		tipPosition = vec3(0,0,2*linksNum*scaleFactor);
		maxDistance = linksNum*2.0f*scaleFactor;*/
	}

	IK::IK(vec3 position,float angle,float hwRelation,float nearf, float farf) : Scene( position,  angle,  hwRelation,  nearf,  farf)
	{
		cameraMode = false;
		isIKactive = false;
		delta = 1e-1;
		direction = UP;
		headLink = linksNum - 1;
		/*distPosition = vec3(1,0,0);
		tipPosition = vec3(0,0,2*linksNum*scaleFactor);
		maxDistance = linksNum*2.0f*scaleFactor;*/
	}

	IK::~IK(void)
	{
	}

	void IK::init(Vertex *vertices,unsigned int *indices,int verticesSize,int indicesSize)
	{
		myRotate(-90.0f,vec3(1,0,0),-1);
		//addShape(vertices, verticesSize, indices, indicesSize,"./res/textures/plane.png",-1);
		addShape(0,2,"./res/textures/grass.bmp",-1);

		pickedShape = 0;
		shapeTransformation(zScale,scaleFactor/2);

		for (int i = 1; i < linksNum-1; i++)
		{
			pickedShape = i;

			addShape(1,1,"./res/textures/Green-Barbed.bmp",-1);
			shapeTransformation(zScale,scaleFactor/2);	
		
			shapeTransformation(zGlobalTranslate,1.0);
			setParent(i, i - 1);
		}

			pickedShape = linksNum-1;

			addShape(0,3,"./res/textures/Stone_02_COLOR.bmp",-1);
			shapeTransformation(zScale,scaleFactor/2);	
			
			shapeTransformation(zGlobalTranslate,1.0);
			setParent(linksNum - 1, linksNum - 2);

		pickedShape = linksNum;
		
		//addShape(0,"./res/textures/box0.bmp",-1);
		addShape(vertices, verticesSize, indices, indicesSize,"./res/textures/box0.bmp",-1);
		shapeTransformation(xScale,0.5);
		shapeTransformation(yScale,0.5);
		shapeTransformation(zScale,0.5);
		shapeTransformation(xGlobalTranslate, -8.0f);
		shapeTransformation(yGlobalTranslate, -3.0f);
		shapeTransformation(zGlobalTranslate, -5.0f);
		pickedShape = 0;

		distPosition = getGoalPosition();
		tipPosition = getTipPosition(linksNum-1);
		maxDistance = length(tipPosition);
		linkLength = maxDistance / linksNum;

		pickedShape = linksNum;
	}

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
	
	void IK::calculateStep(bool EulerVersion)
	{
		const auto start = getDistination(0);
		
		vector<vec3> linkTipPositions;
		for (int i = 0; i < linksNum; i++)
		{
			linkTipPositions.push_back(getDistination(i));
		}
		linkTipPositions.push_back(getTipPosition(linksNum - 1));

		//std::cout << "==> Starting Step 1" << std::endl;

		auto last_tip_position = linkTipPositions[linksNum];
		auto factor = 0.1f;
		auto goal = factor * getGoalPosition() + (1.0f - factor) * last_tip_position;
		auto targetDiff = goal - last_tip_position;
		auto distance = dot(targetDiff,targetDiff);

		if(distance > 0.5f)
		{
			goal = last_tip_position + 0.5f * normalize(targetDiff);
		}

		for (int i = linkTipPositions.size() - 1 ; i > 0 ; i--)
		{
			linkTipPositions[i] = goal;
			auto direction = normalize(linkTipPositions[i - 1] - goal);
			goal = goal + (float)scaleFactor * direction;
		}

		linkTipPositions[0] = goal;

		//std::cout << "==> Starting Step 2" << std::endl;
		goal = start;

		for (uint i = 1; i < linkTipPositions.size(); i++)
		{
			linkTipPositions[i - 1] = goal;
			auto direction = normalize(linkTipPositions[i] - goal);
			goal = goal + (float)scaleFactor * direction;
		}

		linkTipPositions[linksNum] = goal;

		//Rotation
		//std::cout << "==> Starting Rotation" << std::endl;
		auto yAxis = vec3(0, 1, 0);
		auto zAxis = vec3(0, 0, 1);

		for (uint i = 0;i < linksNum; i++)
		{
			reset_euler_angles(i);

			auto currentLinkTip = linkTipPositions[i];
			auto nextLinkTip = linkTipPositions[i + 1];

			auto diff = nextLinkTip - currentLinkTip;
			auto direction = normalize(diff);
			auto rAxis = cross(zAxis, direction);

			if(length(rAxis) > EPSILON)
			{
				auto proj_on_xy = normalize(direction - dot(direction, zAxis) * zAxis);

				//Euler angles calculation
				auto z_angle = degrees(acos(clamp(dot(yAxis, proj_on_xy), -1.0f, 1.0f)));
				auto x_angle = degrees(acos(clamp(dot(zAxis, direction), -1.0f, 1.0f)));
				
				auto xAxis = cross(yAxis, zAxis);
				if (dot(proj_on_xy, xAxis) > 0)
				{
					z_angle = -z_angle;
				}

				rAxis = normalize(rAxis);// maybe normalize before the 'if'?
				auto r_mat = rotate(x_angle, rAxis);
				yAxis = vec3(r_mat * vec4(yAxis, 0));
				zAxis = direction;
				shapeEulerAnglesRotation(z_angle, -x_angle, i);
			}
		}
	}

	void IK::setDirectionRight() {
		Sleep(50);

		for (int i = headLink; i > 0; i--)
		{
			pickedShape = i;
			shapeTransformation(yLocalRotate, -ROTATION_ANGLE);
		}

		if (direction == UP) {
			direction = RIGHT;
		}
		else if (direction == DOWN) {
			//shapeTransformation(yLocalRotate, ROTATION_ANGLE);
			direction = LEFT;
			//shapeTransformation(xLocalTranslate, .5f);
		}
		else if (direction == LEFT) {
			direction = UP;
		}
		else {
			direction = DOWN;
		}
		
	}
	void IK::setDirectionLeft() {
		Sleep(50);
		for (int i = headLink; i > 0; i--)
		{
			pickedShape = i;
			shapeTransformation(yLocalRotate, ROTATION_ANGLE);
		}

		if (direction == UP) {
			direction = LEFT;
		}
		else if (direction == DOWN) {
			//shapeTransformation(yLocalRotate, ROTATION_ANGLE);
			direction = RIGHT;
			//shapeTransformation(xLocalTranslate, .5f);
		}
		else if (direction == LEFT) {
			direction = DOWN;
		}
		else {
			direction = UP;
		}
	}

	void IK::setDirectionUp() {
		Sleep(50);
		/*
		auto angle = 0;
		if (direction == RIGHT) {
			angle = ROTATION_ANGLE;
		}
		else if (direction == DOWN) {
			angle = 2 * ROTATION_ANGLE;
		}
		else if (direction == LEFT) {
			angle = -ROTATION_ANGLE;
		}

		for (int i = 0; i < linksNum - 1; i++)
		{
			pickedShape = i;
			shapeTransformation(yLocalRotate, angle);
		}
		
		direction = UP;
	}

	void IK::setDirectionDown() {
		Sleep(50);

		/*auto angle = 0;
		if (direction == RIGHT) {
			angle = -ROTATION_ANGLE;
		}
		else if (direction == LEFT) {
			angle = ROTATION_ANGLE;
		}
		for (int i = 0; i < linksNum - 1; i++)
		{
			pickedShape = i;
			shapeTransformation(yLocalRotate, angle);
		}

		direction = DOWN;*/
		UpdateSnakeMovement();
	}

	void IK::UpdateSnakeMovement() {
		Sleep(30);
		//for (int i = 1; i < linksNum - 1; i++)
		//{
			/*
			# Get the direction of the head after rotation
			# Save the previous location of the head
			# translate in that direction (might be just the Y Axis)
			# go over the other links and replace the location with the previous
			one (might need to copy rotation too)
			*/

		/*
		for (int i = linkTipPositions.size() - 1 ; i > 0 ; i--)
		{
			linkTipPositions[i] = goal;
			auto direction = normalize(linkTipPositions[i - 1] - goal);
			goal = goal + (float)scaleFactor * direction;
		}
		*/
			pickedShape = headLink;
			shapeTransformation(yLocalTranslate, DISTANCE_DELTA);
			/* 
			Direction = After the rotation is done, get the NEW tip position of the head and using the base position
			of the head we get the new direction.

			*/
			/*for(int i=headLink; )
			switch (direction) {
			case LEFT:
					shapeTransformation(xGlobalTranslate, DISTANCE_DELTA);
				break;
			case RIGHT:
					shapeTransformation(xGlobalTranslate, -DISTANCE_DELTA);
				break;
			case UP:
					shapeTransformation(zLocalTranslate, DISTANCE_DELTA);
				break;
			case DOWN:
					shapeTransformation(zLocalTranslate, -DISTANCE_DELTA);
				break;
			default:
				break;
			}*/
			//shapes[i]->update();
		//}
	}
	
	void IK::makeIKChange()
	{
		distPosition=getGoalPosition();
		tipPosition = getTipPosition(linksNum - 1);
		auto distFromBase = distance(getDistination(0), distPosition);

		//Check if within reach
		//std::cout << "base_distance = " << distFromBase << std::endl;
		//std::cout << "max_distance = " << maxDistance << std::endl;
		auto diff = (maxDistance - delta);
		//std::cout << "max-distance - delta = " << diff << std::endl;

		// Unreachable
		if(distFromBase >= diff)
		{
			std::cout << "De-activating" << std::endl;
			dActivate();
			return;
		}

		// Rotate Links
		calculateStep(true);

		//Check if reached to destination
		tipPosition = getTipPosition(linksNum - 1);
		//distPosition = getGoalPosition();
		
		//std::cout << "tipPosition = " << tipPosition.x << ", " << tipPosition.y << ", " << tipPosition.z << std::endl;
		//std::cout << "distPosition = " << distPosition.x << ", " << distPosition.y << ", "<< distPosition.z << std::endl;
		
		auto dist = distance(tipPosition, distPosition);
		//std::cout << "Distance = " << dist << std::endl;
		
		
		if(dist < delta)
		{
			std::cout << "Reached Destination!!" << std::endl;
			dActivate();
			return;
		}
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




	