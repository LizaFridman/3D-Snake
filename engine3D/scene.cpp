#define GLEW_STATIC
#include <GL\glew.h>
#include "scene.h"
#include <iostream>
#include "../IK/SnakeScene.h"
#include "GLErrorHandler.h"

using namespace std;
using namespace glm;

Vertex axisVertices[] =
{
	Vertex(glm::vec3(1,0,0),glm::vec2(1, 0), glm::vec3(0, 0, -1),glm::vec3(1,0,0)),
	Vertex(glm::vec3(-1,0,0),glm::vec2(1, 0), glm::vec3(0, 0, -1),glm::vec3(1,0,0)),
	Vertex(glm::vec3(0,1,0),glm::vec2(1, 0), glm::vec3(0, 0, -1),glm::vec3(0,1,0)),
	Vertex(glm::vec3(0,-1,0),glm::vec2(1, 0), glm::vec3(0, 0, -1),glm::vec3(0,1,0)),
	Vertex(glm::vec3(0,0,1),glm::vec2(1, 0), glm::vec3(0, 0, -1),glm::vec3(0,0,1)),
	Vertex(glm::vec3(0,0,-1),glm::vec2(1, 0), glm::vec3(0, 0, -1),glm::vec3(0,0,1)),
};

	unsigned int axisIndices[] = 
	{
		0,1,
		2,3,
		4,5
	};

	Scene::Scene()
	{
		glLineWidth(3);
		cameraOriginalPosition = vec3(0, 0, -20.0f);
		cameras.push_back(new Camera(cameraOriginalPosition, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f),60.0f,1.0f,1.f,500.0f));
		pickedShape = -1;
		
	}

	Scene::Scene(vec3 position,float angle,float hwRelation,float near, float far)
	{
		GLCall(glLineWidth(3));
		cameras.push_back(new Camera(position, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f),angle,hwRelation,near,far));
		//axisMesh = new Shape(axisVertices,sizeof(axisVertices)/sizeof(axisVertices[0]),axisIndices, sizeof(axisIndices)/sizeof(axisIndices[0]));
		pickedShape = -1;
	}

	void Scene::addShape(int Cylparts,int linkPosition,int parent)
	{
		chainParents.push_back(parent);
		shapes.push_back(new Shape(Cylparts,linkPosition));
	}
	void Scene::addShape(int CylParts,int linkPosition,const std::string& textureFileName,int parent, ShapeType type)
	{
		chainParents.push_back(parent);
		shapes.push_back(new Shape(CylParts,linkPosition,textureFileName,type));
	}
	void Scene::addShape(int type,int parent)
	{
		chainParents.push_back(parent);
		shapes.push_back(new Shape(*shapes[type]));
	}

	void Scene::addShape(const std::string& fileName,int parent)
	{
		chainParents.push_back(parent);
		shapes.push_back(new Shape(fileName));
	}

	void Scene::addShape(const std::string& fileName,const std::string& textureFileName,int parent)
	{
		chainParents.push_back(parent);
		shapes.push_back(new Shape(fileName,textureFileName));
	}

	void Scene::addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices,int parent)
	{
		chainParents.push_back(parent);
		shapes.push_back(new Shape(vertices,numVertices,indices,numIndices));
	}
	
	void Scene::addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices, const std::string &textureFlieName,int parent, ShapeType type)
	{
		chainParents.push_back(parent);
		shapes.push_back(new Shape(vertices,numVertices,indices,numIndices,textureFlieName, type));
	}

	void Scene::addShader(const std::string& fileName)
	{
		shaders.push_back(new Shader(fileName));
		if(!axisMesh)
		{	
			axisMesh = new Shape(axisVertices,sizeof(axisVertices)/sizeof(axisVertices[0]),axisIndices, sizeof(axisIndices)/sizeof(axisIndices[0]));
			axisMesh->myScale(vec3(2.0*scaleFactor,2.0*scaleFactor,2.0*scaleFactor));
			axisMesh->myTranslate(vec3(0,0,0.5),1);
		}
	}

	mat4 Scene::GetViewProjection(int indx) const
	{
		return cameras[indx]->GetViewProjection();
	}

	mat4 Scene::GetShapeTransformation() const
	{
		return shapes[pickedShape]->makeTrans();
	}

	void Scene::draw(int shaderIndx,int cameraIndx,bool drawAxis)
	{
		mat4 Normal = makeTrans();
		mat4 MVP = cameras[viewIndex]->GetViewProjection() * Normal;
		vector<mat4> trans;

		shaders[shaderIndx]->Bind();

		for (int i=0; i<shapes.size();i++)
		{
			mat4 Normal1 = mat4(1);
			for (int j = i; chainParents[j] > -1; j = chainParents[j])
			{
				Normal1 =  shapes[chainParents[j]]->makeTrans() * Normal1;
			}
			

			mat4 MVP1 = MVP * Normal1; 
			Normal1 = Normal * Normal1;
			
			if(shaderIndx==0 && drawAxis && chainParents[i]>=0)
			{
				shaders[shaderIndx]->Update(axisMesh->makeTransScale(MVP1), axisMesh->makeTransScale(Normal1), 0, trans, 0, cameras[viewIndex]->getCameraPosition(), sceneColor);
				axisMesh->draw(GL_LINES);
			}

			MVP1 = MVP1 * shapes[i]->makeTransScale(mat4(1));
			Normal1 = Normal1 * shapes[i]->makeTrans();
			trans.push_back(MVP1);
			shaders[shaderIndx]->Update(MVP1,Normal1,i, trans, shapes[i]->getTexture(), cameras[viewIndex]->getCameraPosition(), sceneColor);
			//shaders[shaderIndx]->Update(MVP1,Normal1,cameras[0]->GetPos(), i, transformations);

			if (shapes[i]->type != INVISIBLE) {
				shapes[i]->draw(GL_TRIANGLES);
			}
			/*else 
				shapes[i]->draw(GL_TRIANGLES);*/
		}
		if(shaderIndx==0 )
		{
			shaders[shaderIndx]->Bind();
			shaders[shaderIndx]->Update(cameras[viewIndex]->GetViewProjection()*scale(vec3(10,10,10)),Normal*scale(vec3(10,10,10)),0, trans, 0, cameras[viewIndex]->getCameraPosition(),sceneColor);
			//shaders[shaderIndx]->Update(cameras[0]->GetViewProjection()*scale(vec3(10, 10, 10)), Normal*scale(vec3(10, 10, 10)), cameras[0]->GetPos(), 0, transformations);
			axisMesh->draw(GL_LINES);
		}
	}

	void Scene::shapeRotation(vec3 v, float ang, int indx)
	{
		if (v.x > 0.999)
			shapes[indx]->myRotate(ang, v, xAxis1);
		else
			if (v.z > 0.999)
				shapes[indx]->myRotate(ang, v, zAxis12);
			else
				shapes[indx]->myRotate(ang, v, -1);
	}

	void Scene::shapeEulerAnglesRotation(float z_angle, float x_angle, int index)
	{
		shapes[index]->myRotate(x_angle, vec3(0), xAxis1);
		shapes[index]->myRotate(z_angle, vec3(0), zAxis12);
	}

	void Scene::shapeTransformation(int type,float amt)
	{
		    vec3 newAxis;
			switch (type)
			{
			case xLocalTranslate:
				if(pickedShape ==-1)
					myTranslate(vec3(amt,0,0),1);
				else
				{
					int i = pickedShape;
					for (; chainParents[i] > -1; i = chainParents[i]);
					shapes[i]->myTranslate(vec3(amt,0,0),1);
				}
				break;
			case yLocalTranslate:
				if(pickedShape ==-1)
					myTranslate(vec3(0,amt,0),1);
				else
					{
						int i = pickedShape;
						for (; chainParents[i] > -1; i = chainParents[i]);
						shapes[i]->myTranslate(vec3(0,amt,0),1);
					}
				break;
			case zLocalTranslate:
				if(pickedShape ==-1)
					myTranslate(vec3(0,0,amt),1);
				else
				{
					int i = pickedShape;
					for (; chainParents[i] > -1; i = chainParents[i]);
					shapes[i]->myTranslate(vec3(0,0,amt),1);
				}
				break;
			case xGlobalTranslate:
				if(pickedShape ==-1)
					myTranslate(vec3(amt/5.0,0,0),0);
				else
				{
					int i = pickedShape;
					for (; chainParents[i] > -1; i = chainParents[i]);
					shapes[i]->myTranslate(vec3(amt,0,0),0);
				}
				break;
			case yGlobalTranslate:
				if(pickedShape ==-1)
					myTranslate(vec3(0,amt/5.0,0),0);
				else
				{
					int i = pickedShape;
					for (; chainParents[i] > -1; i = chainParents[i]);
					shapes[i]->myTranslate(vec3(0,amt,0),0);
				}
				break;
			case zGlobalTranslate:
				if(pickedShape ==-1)
					myTranslate(vec3(0,0,amt/5.0),0);
				else
				{
					int i = pickedShape;
					for (; chainParents[i] > -1; i = chainParents[i]);
					shapes[i]->myTranslate(vec3(0,0,amt),0);
				}
				break;
			case xLocalRotate:
				if(pickedShape ==-1)
					myRotate(amt,vec3(1,0,0),xAxis1);
				else
					shapes[pickedShape]->myRotate(amt,vec3(1,0,0),xAxis1);
				break;
			case yLocalRotate:
				if(pickedShape ==-1)
					myRotate(amt,vec3(0,1,0),-1);
				else
					shapes[pickedShape]->myRotate(amt,vec3(0,1,0),-1);
				break;
			case zLocalRotate:
				if(pickedShape ==-1)
					myRotate(amt,vec3(0,0,1),zAxis12);
				else
					shapes[pickedShape]->myRotate(amt,vec3(0,0,1),zAxis12);
			break;
			case xGlobalRotate:
				if(pickedShape ==-1)
					globalSystemRot(amt,vec3(1,0,0),xAxis1);
				else
					shapes[pickedShape]->globalSystemRot(amt,vec3(1,0,0),xAxis1);
				break;
			case yGlobalRotate:
				if(pickedShape ==-1)
					globalSystemRot(amt,vec3(0,1,0),-1);
				else
					shapes[pickedShape]->globalSystemRot(amt,vec3(0,1,0),-1);
				break;
			case zGlobalRotate:
				if(pickedShape ==-1)
					globalSystemRot(amt,vec3(0,0,1),zAxis12);
				else
					shapes[pickedShape]->globalSystemRot(amt,vec3(0,0,1),zAxis12);
			break;
			case xScale:
				if(pickedShape ==-1)
					myScale(vec3(amt,1,1));
				else
					shapes[pickedShape]->myScale(vec3(amt,1,1));
			break;
			case yScale:
				if(pickedShape ==-1)
					myScale(vec3(1,amt,1));
				else
					shapes[pickedShape]->myScale(vec3(1,amt,1));
			break;
			case zScale:
				if(pickedShape ==-1)
					myScale(vec3(1,1,amt));
				else
					shapes[pickedShape]->myScale(vec3(1,1,amt));
			break;
			case xCameraTranslate: //camera plane translate
				if(pickedShape == -1)
					myTranslate(vec3(amt/5.0,0,0),0);
				else
				{
					//newAxis = findAxis(vec3(1,0,0));					
						int i = pickedShape;
					for (; chainParents[i] > -1; i = chainParents[i]);
				
					shapes[i]->translateInSystem(*this,vec3(amt,0,0),0,false);
				}
				break;
			case yCameraTranslate:
				if(pickedShape ==-1)
					myTranslate(vec3(0,amt/5.0,0),0);
				else
					{
						//newAxis = findAxis(vec3(0,1,0));
							int i = pickedShape;
						for (; chainParents[i] > -1; i = chainParents[i]);
						
						shapes[i]->translateInSystem(*this,vec3(0,amt,0),0,false);
					}
			break;
			case zCameraTranslate:
				if(pickedShape ==-1)
					myTranslate(vec3(0,0,amt/5.0),0);
				else
					{
					//	newAxis = findAxis(vec3(0,0,1));
							int i = pickedShape;
						for (; chainParents[i] > -1; i = chainParents[i]);
			
						shapes[i]->translateInSystem(*this,vec3(0,0,amt),0,false);
					}
			break;
			default:
				break;
			}
	}

	void Scene::shapeLocalTranslation(vec3& v, int indx)
	{
		shapes[indx]->myTranslate(v, 1);
	}

	void Scene::shapeTransformation(int type, float amt, vec3 vec)
	{
		vec3 newAxis;
		switch (type)
		{
		case xLocalTranslate:
			if (pickedShape == -1)
				myTranslate(vec, 1);
			else
			{
				int i = pickedShape;
				//for (; chainParents[i] > -1; i = chainParents[i]);
				shapes[i]->myTranslate(vec, 1);
			}
			break;
		case yLocalTranslate:
			if (pickedShape == -1)
				myTranslate(vec, 1);
			else
			{
				int i = pickedShape;
				for (; chainParents[i] > -1; i = chainParents[i]);
				shapes[i]->myTranslate(vec, 1);
			}
			break;
		case zLocalTranslate:
			if (pickedShape == -1)
				myTranslate(vec, 1);
			else
			{
				int i = pickedShape;
				for (; chainParents[i] > -1; i = chainParents[i]);
				shapes[i]->myTranslate(vec, 1);
			}
			break;
		case xGlobalTranslate:
			if (pickedShape == -1)
				myTranslate(vec, 0);
			else
			{
				int i = pickedShape;
				for (; chainParents[i] > -1; i = chainParents[i]);
				shapes[i]->myTranslate(vec, 0);
			}
			break;
		case yGlobalTranslate:
			if (pickedShape == -1)
				myTranslate(vec, 0);
			else
			{
				int i = pickedShape;
				for (; chainParents[i] > -1; i = chainParents[i]);
				shapes[i]->myTranslate(vec, 0);
			}
			break;
		case zGlobalTranslate:
			if (pickedShape == -1)
				myTranslate(vec, 0);
			else
			{
				int i = pickedShape;
				for (; chainParents[i] > -1; i = chainParents[i]);
				shapes[i]->myTranslate(vec, 0);
			}
			break;
		case xLocalRotate:
			if (pickedShape == -1)
				myRotate(amt, vec, xAxis1);
			else
				shapes[pickedShape]->myRotate(amt, vec, xAxis1);
			break;
		case yLocalRotate:
			if (pickedShape == -1)
				myRotate(amt, vec, -1);
			else
				shapes[pickedShape]->myRotate(amt, vec, -1);
			break;
		case zLocalRotate:
			if (pickedShape == -1)
				myRotate(amt, vec, zAxis12);
			else
				shapes[pickedShape]->myRotate(amt, vec, zAxis12);
			break;
		case xGlobalRotate:
			if (pickedShape == -1)
				globalSystemRot(amt, vec, xAxis1);
			else
				shapes[pickedShape]->globalSystemRot(amt, vec, xAxis1);
			break;
		case yGlobalRotate:
			if (pickedShape == -1)
				globalSystemRot(amt, vec, -1);
			else
				shapes[pickedShape]->globalSystemRot(amt, vec, -1);
			break;
		case zGlobalRotate:
			if (pickedShape == -1)
				globalSystemRot(amt, vec, zAxis12);
			else
				shapes[pickedShape]->globalSystemRot(amt, vec, zAxis12);
			break;
		case xScale:
			if (pickedShape == -1)
				myScale(vec3(amt, 1, 1));
			else
				shapes[pickedShape]->myScale(vec3(amt, 1, 1));
			break;
		case yScale:
			if (pickedShape == -1)
				myScale(vec3(1, amt, 1));
			else
				shapes[pickedShape]->myScale(vec3(1, amt, 1));
			break;
		case zScale:
			if (pickedShape == -1)
				myScale(vec3(1, 1, amt));
			else
				shapes[pickedShape]->myScale(vec3(1, 1, amt));
			break;
		case xCameraTranslate: //camera plane translate
			if (pickedShape == -1)
				myTranslate(vec3(amt / 5.0, 0, 0), 0);
			else
			{
				//newAxis = findAxis(vec3(1,0,0));					
				int i = pickedShape;
				for (; chainParents[i] > -1; i = chainParents[i]);

				shapes[i]->translateInSystem(*this, vec3(amt, 0, 0), 0, false);
			}
			break;
		case yCameraTranslate:
			if (pickedShape == -1)
				myTranslate(vec3(0, amt / 5.0, 0), 0);
			else
			{
				//newAxis = findAxis(vec3(0,1,0));
				int i = pickedShape;
				for (; chainParents[i] > -1; i = chainParents[i]);

				shapes[i]->translateInSystem(*this, vec3(0, amt, 0), 0, false);
			}
			break;
		case zCameraTranslate:
			if (pickedShape == -1)
				myTranslate(vec3(0, 0, amt / 5.0), 0);
			else
			{
				//	newAxis = findAxis(vec3(0,0,1));
				int i = pickedShape;
				for (; chainParents[i] > -1; i = chainParents[i]);

				shapes[i]->translateInSystem(*this, vec3(0, 0, amt), 0, false);
			}
			break;
		default:
			break;
		}
	}

	
	
	void Scene::resize(int width,int height,int near,int far)
	{
		GLCall(glViewport(0,0,width,height));
		cameras[viewIndex]->setProjection((float)width/(float)height,near,far);
	}

	float Scene::picking(double x,double y)
	{
		float depth;
		GLCall(glClearColor(0.0,0.0,0.0,0.0));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		draw(1,0,false); 
						
		GLint viewport[4];  
		unsigned char data[4];
		GLCall(glGetIntegerv(GL_VIEWPORT, viewport)); //reading viewport parameters
		GLCall(glReadPixels(x,viewport[3] - y,1,1, GL_RGBA, GL_UNSIGNED_BYTE, data));
		GLCall(glReadPixels(x,viewport[3] - y,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&depth));
		cout<<"depth "<<depth<<endl;
		int pickedID = data[0] + data[1]*256 + data[2]*256*256 - 1;
		if(data[0] == 0 && data[1] == 0 && data[2] == 0)
		{
			pickedShape = -1;
			//std::cout<<"not picked"<<std::endl;
		}
		else
		{
			pickedShape = pickedID;
			std::cout<<"picked "<<pickedID<<std::endl;
		}
		return depth;
	}

	vec3 Scene::getTipPosition(int index)
	{
		mat4 Normal1 = mat4(1);
		if(index > -1)
		{
			for (int j = index;  chainParents[j] > -1; j = chainParents[j])
			{
				Normal1 =  shapes[chainParents[j]]->makeTrans() * Normal1;
			}
			return shapes[index]->getPointInSystem(Normal1,vec3(0,0,1));
			//return shapes[index]->getTipPos(Normal1);
		}
		else
		{
			return shapes[0]->getPointInSystem(mat4(1),vec3(0,0,-1)); 
		}
	}

	glm::vec3 Scene::getGoalPosition()
	{
		return vec3(shapes[linksNum]->makeTrans()*vec4(0, 0, 0, 1));
	}

	glm::vec3 Scene::getNextGoalPosition()
	{
		/*auto velocity = shapes[shapeIndex]->forwardDirection;
		vec3 movement = DISTANCE_DELTA * 2.f* velocity;
		auto directionToMove = vec3(getNormalForShape(shapeIndex) * vec4(movement, 0));
		auto goal = vec3(shapes[shapeIndex]->makeTrans()*vec4(0, 0, 0, 1)) ;
		return goal + directionToMove;*/

		auto translation = glm::translate(shapes[0]->makeTrans(), destinationPosition);
		return	glm::vec3(translation[3]);

		//return vec3(shapes[destinationIndex]->makeTrans()*vec4(0, 0, 0, 1));
	}

	/*glm::vec3 Scene::getGoalPosition()
	{
		switch (direction) {
		case LEFT:

			break;
		case RIGHT:
			break;
		case UP:
			break;
		case DOWN:
			break;
		default:
			return vec3(shapes[linksNum]->makeTrans()*vec4(0, 0, 0, 1));
			break;
		}
	}*/

	vec3 Scene::getBase(int indx)
	{
		mat4 Normal1 = mat4(1);
		if( indx>-1)
		{
			for (int j = indx; chainParents[j] > -1; j = chainParents[j])
			{
				Normal1 =  shapes[chainParents[j]]->makeTrans() * Normal1;
			}
			return shapes[indx]->getPointInSystem(Normal1,vec3(0,0,0));
			//return shapes[indx]->getCenterOfRotation(Normal1);
		}
		else
		{
			return vec3(0,0,0);
		}
	}

	vec3 Scene::getAxisDirection(int indx,int axis)
	{
		if(axis == xAxis)
		{
			mat4 Normal1 = mat4(1);
			for (int j = indx; chainParents[j] > -1; j = chainParents[j])
			{
				Normal1 =  shapes[chainParents[j]]->makeTrans() * Normal1;
			}
			return shapes[indx]->getPointInSystem(Normal1,vec3(1,0,0)); 
				//shapes[indx]->getXdirection(Normal1);
		}
		else
		{
			mat4 Normal1 = mat4(1);
			for (int j = indx; chainParents[j] > -1; j = chainParents[j])
			{
				Normal1 =  shapes[chainParents[j]]->makeTrans() * Normal1;
			}
			return shapes[indx]->getVectorInSystem(Normal1,vec3(0,0,1)); 
				//shapes[indx]->getZdirection(Normal1);
		}

	}

	void Scene::restartAnglesForShape(int indx)
	{
		shapes[indx]->reset_euler_angles();
	}

glm::mat4 Scene::getNormalForShape(int shapeIndex)
{
	mat4 Normal1 = mat4(1);

	if (shapeIndex>-1)
	{
		for (int j = shapeIndex; chainParents[j] > -1; j = chainParents[j])
		{
			Normal1 = shapes[chainParents[j]]->makeTrans() * Normal1;
		}
	}

	return shapes[shapeIndex]->makeTrans(Normal1);
}



Scene::~Scene(void)
{
	for (Shape* shp : shapes)
		{
			delete shp;
		}
	for (Camera* cam : cameras)
		{
			delete cam;
		}
	for(Shader* sdr: shaders)
		{
			delete sdr;
		}
	delete axisMesh;
}
