#include "stdafx.h"
#include "PlaneLoader.h"
#include "ResourceHandler.h"
#include <cmath>
using namespace Resources;

MeshObject* PlaneLoader::LoadShape(MeshObject::Material* mat)
{

	MeshObject* mesh = new MeshObject(ResourceLoader::MatID());//The object to load data into
	mesh->AddMaterial(mat);
	MeshObject::Vertex** vertices = new MeshObject::Vertex*[4];
	
	vertices[0] = new MeshObject::Vertex(3,2,3);
	vertices[0]->SetVertix(0,-(0.5f));
	vertices[0]->SetVertix(1,(0.5f));
	vertices[0]->SetVertix(2,(0.5f));

	vertices[0]->SetTextureVertix(0,0);
	vertices[0]->SetTextureVertix(1,1);

	vertices[1] = new MeshObject::Vertex(3,2,3);
	vertices[1]->SetVertix(0,(0.5f));
	vertices[1]->SetVertix(1,(0.5f));
	vertices[1]->SetVertix(2,(0.5f));
	
	vertices[1]->SetTextureVertix(0,1);
	vertices[1]->SetTextureVertix(1,1);

	vertices[2] = new MeshObject::Vertex(3,2,3);
	vertices[2]->SetVertix(0,(0.5f));
	vertices[2]->SetVertix(1,-(0.5f));
	vertices[2]->SetVertix(2,-(0.5f));
	
	vertices[2]->SetTextureVertix(0,1);
	vertices[2]->SetTextureVertix(1,0);

	vertices[3] = new MeshObject::Vertex(3,2,3);
	vertices[3]->SetVertix(0,-(0.5f));
	vertices[3]->SetVertix(1,-(0.5f));
	vertices[3]->SetVertix(2,-(0.5f));
	
	vertices[3]->SetTextureVertix(0,0);
	vertices[3]->SetTextureVertix(1,0);
	
	//Get the normals by calculating the crossproduct of two vectors building the plane		
	//normalX = (wy-vy)*(uz-vz)-(wz-vz)*(uy-vy)
	//normalY = (wz-vz)*(ux-vx)-(wx-vx)*(uz-vz)
	//normalZ = (wx-vx)*(uy-vy)-(wy-vy)*(ux-vx)
	float normalX = (vertices[1]->getV()[1]-vertices[0]->getV()[1])*(vertices[2]->getV()[2]-vertices[0]->getV()[2])-(vertices[1]->getV()[2]-vertices[0]->getV()[2])*(vertices[2]->getV()[1]-vertices[0]->getV()[1]);
	float normalY = (vertices[1]->getV()[2]-vertices[0]->getV()[2])*(vertices[2]->getV()[0]-vertices[0]->getV()[0])-(vertices[1]->getV()[0]-vertices[0]->getV()[0])*(vertices[2]->getV()[2]-vertices[0]->getV()[2]);
	float normalZ = (vertices[1]->getV()[0]-vertices[0]->getV()[0])*(vertices[2]->getV()[1]-vertices[0]->getV()[1])-(vertices[1]->getV()[1]-vertices[0]->getV()[1])*(vertices[2]->getV()[0]-vertices[0]->getV()[0]);
	float length = sqrt(pow(normalX,2)+pow(normalY,2)+pow(normalZ,2));

	//Set the normals
	for(int i=0;i<4;i++)
	{


		vertices[i]->SetVertixNormal(0,normalX/length);
		vertices[i]->SetVertixNormal(1,normalY/length);
		vertices[i]->SetVertixNormal(2,normalZ/length);

	}

	MeshObject::Face* face = new MeshObject::Face(4,0,0);
	face->SetIndex(mesh->AddVertices(vertices,4));
	mesh->SetFirstFace(face);
	mat->SetFirstFace(face);
	mat->AddFace(face);

	delete[] vertices;
	
	mesh->CalculateShapes();
	return mesh;
}

PlaneLoader::~PlaneLoader()
{

}