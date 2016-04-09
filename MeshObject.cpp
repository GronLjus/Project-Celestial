#include "stdafx.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"
#include "MeshObject.h"
#include <math.h>

using namespace Resources;
using namespace CrossHandlers;

MeshObject::Group** MeshObject::getGroups()
{

	return groups;

}

MeshObject::Material** MeshObject::getMaterials()
{

	return materials;

}

MeshObject::Face* MeshObject::GetFirstFace()
{

	return firstFace;

}

MeshObject::MeshObject(MeshObject*& copy,int id)
{

	optimization = copy->IsOptimized();
	this->id = id;

	groupSize = copy->GetGroupsSize();
	materialSize = copy->GetMaterialSize();
	totalIndices = copy->GetTotalIndices();
	materials = nullptr;
	groups = nullptr;

	if(groupSize > 0)
	{

		groups = new Group*[groupSize];

		for(int i=0;i<groupSize;i++)
		{
	
			groups[i] = new Group(copy->getGroups()[i]);

		}
	}
	
	if(materialSize > 0)
	{

		materials = new Material*[materialSize];
		for(int i=0;i<materialSize;i++)
		{
	
			materials[i] = new Material(copy->getMaterials()[i]);

		}
	}

	firstFace = copy->GetFirstFace();
	shapes = new IBounding*[Shape_COUNT];
	maxVertices = 1000;
	vertexAmount = 0;
	totalFaces = 0;
	vertices = new Vertex*[maxVertices];

	for(int i=0;i<Shape_COUNT;i++)
	{

		shapes[i] = copy->GetBoundingObject(CrossHandlers::Shape(i));

	}

	vertexBuffer = nullptr;
	indexBuffer = nullptr;
	instanceBuffers = nullptr;
	instances  = 0;
	isWireFrame = false;

}

MeshObject::MeshObject(int id)
{
	
	this->id = id;
	optimization = false;
	groupSize = 0;
	materialSize = 0;
	totalIndices = 0;
	totalFaces = 0;
	materials = nullptr;
	groups = nullptr;
	firstFace = nullptr;
	lastFace = nullptr;
	shapes = new IBounding*[Shape_COUNT];
	maxVertices = 1000;
	vertexAmount = 0;
	vertices = new Vertex*[maxVertices];

	vertexBuffer = nullptr;
	indexBuffer = nullptr;
	instanceBuffers = nullptr;
	instances  = 0;
	isWireFrame = false;

	for(int i=0;i<Shape_COUNT;i++)
	{

		shapes[i] = nullptr;

	}
}

bool MeshObject::IsWireFrame() const
{

	return isWireFrame;

}

void MeshObject::SetWireFrame(bool wF)
{

	isWireFrame = wF;

}

int MeshObject::GetGroupsSize() const
{

	return groupSize;

}

int MeshObject::GetMaterialSize() const
{

	return materialSize;

}

int MeshObject::GetTotalIndices() const
{

	return totalIndices;

}

void MeshObject::AddGroup(Group* group)
{

	groupSize++;
	extendGroups();
	groups[groupSize-1] = group;

}

void MeshObject::SetFirstFace(Face* &face)
{

	firstFace = face;

}

void MeshObject::AddMaterial(Material* mat)
{

	materialSize++;
	extendMaterials();
	materials[materialSize-1] = mat;

}

void MeshObject::extendGroups()
{
	
	Group** temp = new Group*[groupSize];

	for(int i=0;i< groupSize-1;i++)
	{

		temp[i] = new Group(groups[i]);
		delete groups[i];

	}

	if(groups != nullptr)
	{
	
		delete[] groups;

	}

	groups = temp;

}

void MeshObject::extendMaterials()
{
	
	Material** temp = new Material*[materialSize];

	for(int i=0;i< materialSize-1;i++)
	{

		temp[i] = materials[i];

	}

	if(materials != nullptr)
	{
	
		delete[] materials;

	}

	materials = temp;

}

IBounding* MeshObject::GetBoundingObjectCopy(CrossHandlers::Shape shape)
{

	return shapes[shape]->GetCopy();

}

IBounding* MeshObject::GetBoundingObject(CrossHandlers::Shape shape)
{

	return shapes[shape];

}

void MeshObject::SetBoundingObject(CrossHandlers::Shape shape,IBounding* object)
{

	if(shapes[shape] != nullptr)
	{

		delete shapes[shape];
	
	}

	shapes[shape] = object;

}

void MeshObject::CalculateShapes()
{

	int dims = 3;
	float* biggest = new float[dims];
	float* pos = new float[dims];
	float* smallest = new float[dims];
	float* dimens = new float[dims];

	for(int i=0;i<dims;i++)
	{

		biggest[i] = MININT;
		smallest[i] = MAXINT;
		dimens[i] = 0;

	}

	for(int i=0;i<vertexAmount;i++)//Go through each vertex
	{

		for(int k=0;k<vertices[i]->getVM();k++)//Go through each dimension
		{

			if(k < dims)
			{
				
				//Find the biggest smalles value
				float compare = vertices[i]->getV()[k];

				if(compare < smallest[k])
				{

					smallest[k] = compare;

				}
					
				if(compare > biggest[k])
				{

					biggest[k] = compare;

				}
			}
		}
	}

	for(int i=0;i<dims;i++)//Get the dimensions and centerpos of boundingobjects
	{

		dimens[i] = biggest[i] - smallest[i];

		if(dimens[i] < 0)
		{

			dimens[i] *= -1;

		}

		pos[i] = biggest[i] - dimens[i]*0.5f;

	}

	shapes[Shape_BOX] = new BoundingBox(dimens[0],dimens[1],dimens[2],pos[0],pos[1],pos[2]);
	shapes[Shape_SPHERE] = new BoundingSphere(pos[0],pos[1],pos[2],sqrt(dimens[0]*dimens[0]+dimens[1]*dimens[1]+dimens[2]*dimens[2])*0.5f);

	delete[] pos;
	delete[] biggest;
	delete[] smallest;
	delete[] dimens;

}

int** getEdges(MeshObject::Face* faceToGetFrom,int indexToLookFor,int &indexOfI)
{

	indexOfI = -1;

	//Go through each index to find the edge
	for(int j=0;j<faceToGetFrom->getSize() && indexOfI == -1;j++)
	{

		if(faceToGetFrom->GetIndexAt(j) == indexToLookFor)
		{

			indexOfI = j;

		}
	}

	int edgeAfter = indexOfI;
	int edgeBefore = (indexOfI+2)%3;

	int indexAfter = (indexOfI+1)%3;
	int indexBefore = edgeBefore;

	//The face is a trianglestrip
	if(faceToGetFrom->getSize() > 3)
	{

		edgeAfter = max(indexOfI+1,faceToGetFrom->getSize()-1);
		edgeBefore = max(indexOfI-1,0);

		indexAfter = max(indexOfI+2,faceToGetFrom->getSize()-1);
		indexBefore = max(indexOfI-2,0);

		if(indexOfI == faceToGetFrom->getSize()-1)
		{

			indexAfter = faceToGetFrom->getSize()-2;

		}

		if(indexOfI == 0)
		{

			indexBefore = 1;

		}

	}

	int* edges = new int[2];//Every vertex has two edges
	int* indices = new int[2];
	edges[0] = edgeBefore;
	edges[1] = edgeAfter;
	indices[0] = indexBefore;
	indices[1] = indexAfter;
	int** retVal = new int*[2];
	retVal[0] = edges;
	retVal[1] = indices;
	return retVal;

}

void MeshObject::OptimizeLevels(bool convertFacesToTriangles)
{

	if(!optimization)
	{
		
		Face* lastMatFace = nullptr;

		Face*** facesPerIndex = new Face**[vertexAmount];
		int* faceIndices = new int[vertexAmount];
		int* faceMaxes = new int[vertexAmount];
		totalFaces = 0;
		totalIndices = 0;

		for(int i=0;i<vertexAmount;i++)
		{

			faceMaxes[i] = 8;
			facesPerIndex[i] = new Face*[faceMaxes[i]];
			faceIndices[i] = 0;

		}

		Face* face = firstFace;

		while(face != nullptr)//Go through every Face
		{

			if(convertFacesToTriangles)
			{
					
				face->Triangulize();

			}
			
			for(int k=0;k<face->getSize();k++)
			{

				int index = face->GetIndexAt(k);
				facesPerIndex[index][faceIndices[index]] = face;//Add the face to each index
				faceIndices[index]++;

				if(faceIndices[index] >= faceMaxes[index])//Extend faces
				{
					
					faceMaxes[index] += 8;
					Face** newFacesPerIndex = new Face*[faceMaxes[index]];

					for(int i=0;i<faceIndices[index];i++)
					{

						newFacesPerIndex[i] = facesPerIndex[index][i];
						
					}

					delete[] facesPerIndex[index];

					facesPerIndex[index] = newFacesPerIndex;

				}
			}

			face->SetId(totalFaces);
			totalFaces++;
			totalIndices += face->getSize();
			face = face->getNext();

		}
		
		//Go through each index
		for(int i=0;i<vertexAmount;i++)
		{
			
			//Go through each face
			for(int k=0;k<faceIndices[i];k++)
			{
				
				int indexOfI;
				int** edgeData = getEdges(facesPerIndex[i][k],i,indexOfI);
				int* edges = edgeData[0];
				int* indices = edgeData[1];

				//Go through each edge
				for(int j=0;j<2;j++)
				{

					//Edge is empty
					if(facesPerIndex[i][k]->GetAdjIndexAt(edges[j]) == -1)
					{

						bool foundEdge = false;
						int iValue = facesPerIndex[i][k]->GetIndexAt(indices[j]);

						//Go through each face with the edgeindex
						for(int h=0;h<faceIndices[iValue] && !foundEdge;h++)
						{
							
							//Make sure the second face is not the first face
							if(facesPerIndex[iValue][h]->GetId() != facesPerIndex[i][k]->GetId())
							{

								int indexOfH;
								int** edgeData2 = getEdges(facesPerIndex[iValue][h],iValue,indexOfH);
								int* edges2 = edgeData2[0];
								int* indices2 = edgeData2[1];

								//Go through each edge in the second face
								for(int l=0;l<2 && !foundEdge;l++)
								{
							
									//Found the edge sharing the index with the first face
									foundEdge = facesPerIndex[iValue][h]->GetIndexAt(indices2[l]) == i;

									if(foundEdge)
									{

										int face = max(max(indexOfH,indices2[l])-2,0);//Find which face the edge exists on
										int survivor = (face+2+face+1+face)-indices2[l]-indexOfH;
										facesPerIndex[i][k]->SetAdjIndexAt(edges[j],facesPerIndex[iValue][h]->GetIndexAt(survivor));
								
									}
								}

								delete[] indices2;
								delete[] edges2;
								delete[] edgeData2;

							}
						}
					}
				}

				delete[] indices;
				delete[] edges;
				delete[] edgeData;

			}
		}
		
		optimization = true;

		for(int i=0;i<vertexAmount;i++)
		{

			delete[] facesPerIndex[i];

		}

		delete[] facesPerIndex;
		delete[] faceIndices;
		delete[] faceMaxes;

	}
}

void MeshObject::AddFace(Face* face)
{

	if(firstFace == nullptr)
	{

		firstFace = face;
		lastFace = face;

	}
	else
	{

		lastFace->Setnext(face);
		lastFace = face;

	}

	face->SetId(totalFaces);
	totalFaces++;
	totalIndices += face->getSize();

}

int* MeshObject::AddVertices(Vertex** v,int vSize)
{

	int* retVal = new int[vSize];


	//Add new vertices to the mesh
	for(int k=0;k<vSize;k++)
	{

		retVal[k] = vertexAmount;
		addVertex(v[k]);

	}

	return retVal;

}

void MeshObject::addVertex(Vertex* v)
{

	if(vertexAmount >= maxVertices)
	{

		extendVertices();

	}

	vertices[vertexAmount] = v;
	vertexAmount++;

}

void MeshObject::extendVertices()
{

	int increment = 100;

	maxVertices += increment;
	Vertex** newVertices = new Vertex*[maxVertices];

	for(int i=0;i<vertexAmount;i++)
	{

		newVertices[i] = vertices[i];

	}

	delete[] vertices;
	vertices = newVertices;

}

bool MeshObject::IsOptimized() const
{

	return optimization;

}

MeshObject::Vertex* MeshObject::GetVertex(int place)
{

	return vertices[place];

}

int MeshObject::GetVertices() const
{

	return vertexAmount;

}
		
void MeshObject::SetInstances(int inst)
{
	
	if(instanceBuffers != nullptr)
	{

		for(int i=0;i<instances;i++)
		{

			delete instanceBuffers[i];

		}

		delete[] instanceBuffers;
		instanceBuffers = nullptr;

	}

	instances = inst;
	instanceBuffers = new BufferObject*[inst];

	for(int i=0;i<inst;i++)
	{

		instanceBuffers[i] = nullptr;

	}
}

BufferObject* MeshObject::GetBuffer(BufferTypes type,int which)
{

	if(type == BufferTypes_VERTEX)
	{

		return vertexBuffer;

	}
	else if(type == BufferTypes_INDEX)
	{

		return indexBuffer;

	}
	else if(type == BufferTypes_INSTANCE && which >= 0 && which < instances && instanceBuffers != nullptr)
	{

		return instanceBuffers[which];

	}

	return nullptr;

}

void MeshObject::SetBuffer(BufferTypes type, BufferObject* buffer,int which)
{
	
	if(type == BufferTypes_VERTEX)
	{
		 
		if(vertexBuffer != nullptr)
		{

			delete vertexBuffer;

		}

		vertexBuffer = buffer;

	}
	else if(type == BufferTypes_INDEX)
	{
		
		if(indexBuffer != nullptr)
		{

			delete indexBuffer;

		}

		indexBuffer = buffer;

	}
	else if(type == BufferTypes_INSTANCE && which >= 0 && which < instances && instanceBuffers != nullptr)
	{

		if(instanceBuffers[which] != nullptr)
		{

			delete instanceBuffers[which];

		}

		instanceBuffers[which] = buffer;

	}
}

void MeshObject::Release()
{

	for(int i=0;i< materialSize;i++)
	{

		if(materials[i] != nullptr)
		{
		
			materials[i]->Release();

		}
	}

	if(vertexBuffer != nullptr)
	{

		delete vertexBuffer;
		vertexBuffer = nullptr;

	}

	if(indexBuffer != nullptr)
	{

		delete indexBuffer;
		indexBuffer = nullptr;

	}

	if(instanceBuffers != nullptr)
	{

		for(int i=0;i<instances;i++)
		{

			delete instanceBuffers[i];

		}

		delete[] instanceBuffers;
		instanceBuffers = nullptr;

	}
}

MeshObject::~MeshObject()
{

	if(vertexBuffer != nullptr)
	{

		delete vertexBuffer;
		vertexBuffer = nullptr;

	}

	if(indexBuffer != nullptr)
	{

		delete indexBuffer;
		indexBuffer = nullptr;

	}
	
	if(instanceBuffers != nullptr)
	{

		for(int i=0;i<instances;i++)
		{

			delete instanceBuffers[i];

		}

		delete[] instanceBuffers;
		instanceBuffers = nullptr;

	}

	for(int i=0;i<vertexAmount;i++)
	{

		delete vertices[i];

	}

	delete[] vertices;

	for(int i=0;i< materialSize;i++)
	{

		delete materials[i];

	}
	
	delete[] materials;

	if(groups != nullptr)
	{
	
		for(int i=0;i< groupSize;i++)
		{

			if(groups[i] != nullptr)
			{
		
				delete groups[i];

			}
		}
	
		delete[] groups;

	}

	Face* face = firstFace;

	while(face != nullptr)
	{

		Face* temp = face->getNext();
		delete face;
		face = temp;

	}

	for(int i=0;i<Shape_COUNT;i++)
	{

		if(shapes[i] != nullptr)
		{

			delete shapes[i];

		}
	}

	delete[] shapes;

}