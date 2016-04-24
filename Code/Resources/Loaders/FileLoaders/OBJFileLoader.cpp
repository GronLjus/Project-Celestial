#include "../../../../stdafx.h"
#include "OBJFileLoader.h"
#include "../../ResourceHandler.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;
using namespace Resources;
using namespace CelestialMath;

OBJFileLoader::OBJFileLoader()
{

	this->mtlLoader = new MTLFileLoader();
	this->colLoader = new ColourLoader();

}

MeshObject* OBJFileLoader::Load(std::string text)
{

	MeshObject* retVal= new MeshObject(ResourceLoader::MatID());//The object to load data into
	retVal->AddMaterial(colLoader->LoadMaterial(255,255,255,"default",0.8f,0.2f));//Adds a default material
	bool hasDef = false;

	ifstream fileIn;
	fileIn.open(text,ios::in);
	string line;

	string groupName;
	bool groupInited = false;

	int group = -1;
	UINT smoothinGroup = 0;

	bool materialInited = false;
	int currentMaterial = 0;

	vector<vector<MeshObject::Face*>> smoothingGroups;

	MeshObject::Face* currentFace = nullptr;

	float compareEpsilon = 0.000002f;
	vector<vector<float>> vertices;
	vector<vector<float>> textureVertices;
	vector<vector<float>> vertexNormals;
	vector<int> reflectVertexIndex;
	vector<int> faces;

	vector<vector<int>> allVertices;
	vector<vector<int>> allVerticesNormal;
	vector<vector<int>> allVerticesTexture;

	int largestVertex = 0;
	int largestTextureVertex = 0;
	int largestVertexNormal = 0;

	while(fileIn.good())
	{

		getline(fileIn,line);

		string first = getWord(0,line);

		if(first == "mtllib")
		{

			int matSize = 0;
			MeshObject::Material** mats = mtlLoader->LoadMaterial(getWord(1,line),matSize);//Load the materials into a seperate meshobject

			if (mats != nullptr)
			{

				int start = 0;

				for (int i = 0; i < matSize; i++)
				{

					retVal->AddMaterial(mats[i]);

				}

				delete[] mats;

			}
		}

		if(first == "g")
		{

			groupName = getWord(1,line);
			groupInited = false;

		}
		
		if(first == "v")
		{

			addVertices(largestVertex,vertices,line);
			bool isSame = false;
			int ind = -1;

			for(UINT i=0;i<vertices.size()-1 && !isSame;i++)
			{

				bool different = false;

				for (UINT k = 0; k<vertices[i].size() && k <vertices.back().size() && !different; k++)
				{

					different = abs(vertices[i][k]-vertices.back()[k]) > compareEpsilon;

				}

				isSame = !different;

				if(isSame)
				{

					ind = i;

				}
			}

			reflectVertexIndex.push_back(ind);

		}
			
		if(first == "vt")
		{

			addVertices(largestTextureVertex,textureVertices,line);
			
		}
			
		if(first == "vn")
		{

			addVertices(largestVertexNormal,vertexNormals,line);
			
		}

		if(first == "s")
		{

			smoothinGroup = (int)(convertStringToFloat(getWord(1,line)))-1;

			while(smoothinGroup >= smoothingGroups.size())
			{

				smoothingGroups.push_back(vector<MeshObject::Face*>());

			}
		}

		if(first == "usemtl")
		{

			bool stop = false;
			materialInited = false;
			string name = getWord(1,line);

			for(int i=1;i<retVal->GetMaterialSize() && !stop;i++)//Find the index of the material
			{

				string name2 = retVal->getMaterials()[i]->GetName();
				stop = name2 == name;

				if(stop)
				{

					currentMaterial = i;

				}
			}
		}

		if(first == "f")
		{

			vector<int> faceValues;
			int size = getWords(line)-1;

			for(int i=0;i<size;i++)//Go through each vertex
			{

				string vertexLine = getWord(i+1,line);
				string tempString = "";
				vector<int> tempVS;
				int code = 0;

				for(UINT32 k=0;k<vertexLine.length();k++)
				{
					
					if(vertexLine[k] != '/')
					{

						tempString+= vertexLine[k];

					}

					if(vertexLine[k] == '/' || k == vertexLine.length()-1)
					{

						int temp = -1;

						if(tempString != "")
						{
						
							temp = convertStringToInt(tempString)-1;

						}

						if(code == 0 && reflectVertexIndex[temp] != -1)
						{

							temp = reflectVertexIndex[temp];

						}

						tempVS.push_back(temp);
						tempString = "";
						code++;

					}
				}

				int index = 0;
				bool same = false;

				for (UINT k = 0; k<allVertices.size() && !same; k++)
				{
					
					same = (allVertices[k][0] == tempVS[0]);//Compare positions

					if(same)
					{

						index = k;

					}
				}

				if(!same)
				{
					
					allVertices.push_back(tempVS);
					allVerticesNormal.push_back(vector<int>());
					allVerticesTexture.push_back(vector<int>());
					faces.push_back(0);
					index = allVertices.size()-1;

				}
				
				//Add the texturevertices
				if(tempVS.size() >= 1)
				{

					allVerticesTexture[index].push_back(tempVS[1]);

				}

				//Add the normals
				if(tempVS.size() >= 2)
				{

					allVerticesNormal[index].push_back(tempVS[2]);

				}

				bool exists = false;

				for(int k=0;k<faceValues.size() && !exists;k++)
				{

					exists = faceValues[k] == index;//Compare indices

				}

				if(!exists)
				{

					faceValues.push_back(index);
					faces[index]++;

				}
			}
			
			MeshObject::Face* face = new MeshObject::Face(faceValues.size(),currentMaterial,group);
			int* indexes = new int[faceValues.size()];

			for (UINT k = 0; k<faceValues.size(); k++)
			{

				indexes[k] = faceValues[k];

			}

			face->SetIndex(indexes);
			retVal->AddFace(face);
			
			if(!groupInited)
			{

				retVal->AddGroup(new MeshObject::Group(face,groupName));
				group++;
				groupInited = true;

			}
			else
			{
				
				retVal->getGroups()[group]->addFace(face);

			}

			if(!materialInited)
			{

				materialInited = true;
				retVal->getMaterials()[currentMaterial]->SetFirstFace(face);

			}
			else
			{

				retVal->getMaterials()[currentMaterial]->AddFace(face);

			}

			face->SetMaterial(currentMaterial);
			face->SetGroup(group);
			currentFace = face;

			if(smoothinGroup < smoothingGroups.size())
			{
			
				smoothingGroups[smoothinGroup].push_back(face);

			}
		}
	}

	//Add the vertex to the mesh
	MeshObject::Vertex** verts = new MeshObject::Vertex*[allVertices.size()];

	for (UINT i = 0; i<allVertices.size(); i++)
	{
		
		int* indices = new int[3];

		for (UINT k = 0; k < 3; k++)
		{

			indices[k] = -1;

			if(allVertices[i].size() > k)
			{
						
				indices[k] = allVertices[i][k];

			}
		}

		verts[i] = new MeshObject::Vertex(largestVertex,largestTextureVertex,largestVertexNormal);

		//Add postion coordinates
		int start = 0;

		if(indices[0] != -1)
		{

			start = vertices[indices[0]].size();

			for(UINT32 j = 0;j< vertices[indices[0]].size();j++)//Add a value to a positions parameter
			{
							
				verts[i]->SetVertix(j,vertices[indices[0]][j]);

			}
		}

		//Calculate textures
		float* textures = new float[2];
		
		for(int j=0;j<2 && j< largestVertexNormal;j++)
		{

			textures[j] = 0;

		}

		for (UINT k = 0; k<allVerticesTexture[i].size(); k++)
		{
			
			for(int j=0;j<2 && j< largestVertexNormal;j++)
			{
			
				textures[j] += textureVertices[allVerticesTexture[i][k]][j];

			}
		}
		
		for(int j=0;j<2 && j< largestVertexNormal;j++)
		{

			textures[j] /= allVerticesTexture[i].size();
			verts[i]->SetTextureVertix(j,textures[j]);

		}

		//Calculate normal
		float* normals = new float[3];
		
		for(int j=0;j<3 && j< largestVertexNormal;j++)
		{

			normals[j] = 0;

		}

		for (UINT k = 0; k<allVerticesNormal[i].size(); k++)
		{

			for(int j=0;j<3 && j< largestVertexNormal;j++)
			{
			
				normals[j] += vertexNormals[allVerticesNormal[i][k]][j];

			}
		}

		float length = 0;

		for(int j=0;j<3 && j< largestVertexNormal;j++)
		{

			normals[j] /= allVerticesNormal[i].size();
			length += normals[j]*normals[j];

		}

		length = 1/(sqrt(length));
		
		for(int j=0;j<3 && j< largestVertexNormal;j++)
		{

			normals[j] *= length;
			verts[i]->SetVertixNormal(j,normals[j]);

		}
		
		delete[] textures;
		delete[] normals;
		delete[] indices;

	}

	delete[] retVal->AddVertices(verts,allVertices.size());
	delete[] verts;

	double epsilon = 0.0000000001;//Guard for over/underflows

	/*TODO: Fix smoothingroups
	for(int i=0;i<smoothingGroups.size();i++)
	{

		for(int k=0;k< smoothingGroups[i].size();k++)//Go through every face in a smoothinggroup
		{

			for(int j =0;j< smoothingGroups[i][k]->getSize()/retVal->GetVertex(smoothingGroups[i][k]->GetIndexAt(0)).getVNM();j++)//Go through each vertexnormal in a face
			{

				if(smoothingGroups[i][k]->getVN()[j*smoothingGroups[i][k]->getVNM()] < 0)
				{

					int joined = 1;
					float* normals = new float[smoothingGroups[i][k]->getVNM()];
					vector<float> temp = getNormal(smoothingGroups[i][k]);

					for(int h=0;h< temp.size();h++)
					{

						normals[h] = temp[h];

					}

					for(int h=0;h< smoothingGroups[i].size();h++)//Go through every face in a smoothinggroup
					{

						if(h != k && smoothingGroups[i][h]->getVM() == smoothingGroups[i][k]->getVM())
						{

							for(int u =0;u< smoothingGroups[i][h]->getSize()/smoothingGroups[i][h]->getVM();u++)//Go through each vertex in a face
							{

								bool check = true;

								for(int v=0;v<smoothingGroups[i][h]->getVM() && check;v++)
								{

									float tempf = smoothingGroups[i][k]->getV()[u*smoothingGroups[i][k]->getVM()+v] - smoothingGroups[i][h]->getV()[u*smoothingGroups[i][h]->getVM()+v];
									check = tempf < epsilon && tempf > -epsilon;//Close enough to zero

								}

								if(check)
								{
									
									joined++;
									vector<float> temp2 = getNormal(smoothingGroups[i][h]);

									for(int w=0;w< temp2.size();w++)
									{

										normals[w] += temp2[w];

									}
								}
							}
						}
					}

					for(int h=0;h< temp.size();h++)
					{

						normals[h] /= joined;

					}

					delete[] normals;

				}
			}
		}
	}*/

	retVal->CalculateShapes();
	fileIn.close();
	return retVal;

}

void OBJFileLoader::addVertices(int &max, vector<vector<float>> &vertices,string line)
{

	vertices.push_back(vector<float>());
	int temp = getWords(line)-1;

	if(temp >max)
	{

		max = temp;

	}

	for(int i=1;i<temp+1;i++)
	{

		vertices.back().push_back(convertStringToFloat(getWord(i,line)));

	}
}

int OBJFileLoader::convertStringToInt(string what) const
{
	std::istringstream iss(what);
    int x;
	iss >>  x;
    return x;
 
}

float OBJFileLoader::convertStringToFloat(string what) const
{
	std::istringstream iss(what);
    float x;
	iss >>  x;
    return x;
 
}

int OBJFileLoader::getWords(string which) const
{
	
	int spaces = 0;

	for(UINT i=0;i<which.length();i++)
	{

		if(which[i] == ' ' && i <which.length()-1)
		{

			spaces++;

		}
	}

	return spaces+1;

}

string OBJFileLoader::getWord(int what, string which) const
{

	string word = "";
	int spaces = 0;

	for(UINT i=0;i<which.length() && spaces <= what;i++)
	{

		if(which[i] == ' ')
		{

			spaces++;

		}
		else if(spaces == what)
		{

			word += which[i];

		}
	}

	return word;

}

std::string* OBJFileLoader::Extension() const
{

	return extensions;

}

void OBJFileLoader::Init(Graphics::CardHandler* &card, TextContainer* outText)
{

	extens = 2;
	extensions = new string[extens];
	extensions[0] = "obj";
	extensions[1] = "3dobj";
	mtlLoader->Init(card, outText);
	colLoader->Init(card);

}

int OBJFileLoader::GetNrExtensions() const
{

	return extens;

}

OBJFileLoader::~OBJFileLoader()
{

	delete mtlLoader;
	delete colLoader;
	delete[] extensions;

}