#include "stdafx.h"
#include "TerrainObject.h"
#include <cmath>

using namespace CrossHandlers;
using namespace Resources;
using namespace CelestialMath;

TerrainObject::TerrainObject(std::string file,UINT pointDensity,UINT subs,int width,int height,int depth,int x,int y,int z,MeshObject::Material* base)
{

	this->base = base;
	int len;
	int slength = (int)file.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, file.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, file.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;

	HBITMAP image = (HBITMAP)LoadImage(nullptr, r.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_LOADTRANSPARENT);
	BITMAP map;
	HDC temp = CreateCompatibleDC(nullptr);
	SelectObject(temp,image);
	GetObject(image,sizeof(map),&map);
	rows = map.bmHeight;
	columns = map.bmWidth;

	values = new float*[columns];    

	for(int c=0;c<columns;c++)
	{
	
		values[c] = new float[rows];

		for(int r=0;r<rows;r++)
		{

			values[c][r] = (float)(GetPixel(temp,c,r))/(float)(0x00ffffff);//The maximum value of a pixel

		}
	}

	DeleteObject(image);

	this->pointDensity = pointDensity;
	cacheUsed = 0;
	cacheMax = 12;
	
	int subSquared = (int)pow(2.0f,(float)subs);
	preLoadedSubs = new TerrainMesh*[subSquared];
	usages = new int[subSquared];
	objects = new TerrainMesh*[subSquared];
	isAlive = new BoxedBool*[subSquared];

	for(int i=0;i<subSquared;i++)
	{

		preLoadedSubs[i] = nullptr;
		objects[i] = nullptr;
		usages[i] = 0;
		isAlive[i] = new BoxedBool(false);

	}

	this->subs = (float)subs;

	centerX = (float)x;
	centerY = (float)y;
	centerZ = (float)z;

	this->width = (float)width;
	this->height = (float)height;
	this->depth = (float)depth;


}

float TerrainObject::GetHeightAt(float x,float z)
{
	
	transformPoint(x,z);
	float subWidth = columns/subs;
	float subHeight = rows/subs;
	float xCrement = subWidth/(pointDensity-1);
	float yCrement = subHeight/(pointDensity-1);

	float lowestX = (int)(x/xCrement)*xCrement;
	float lowestZ = (int)(z/yCrement)*yCrement;

	float l1 = (x-lowestX)/xCrement;
	float l2 = (z-lowestZ)/yCrement;
	float l3 = 0;

	float h1 = 0;
	float h2 = 0;
	float h3 = values[(int)lowestX][(int)lowestZ]*height;

	if(lowestX+xCrement < columns)
	{

		h1 = values[(int)(lowestX+xCrement)][(int)(lowestZ)]*height;

	}
	
	if(lowestZ+yCrement < rows)
	{
		
		h2 = values[(int)(lowestX)][(int)(lowestZ+yCrement)]*height;

	}

	if(l1+l2 > 1 && lowestX+xCrement < columns && lowestZ+yCrement < rows)//Guard for underflow
	{

		h3 = values[(int)(lowestX+xCrement)][(int)(lowestZ+yCrement)]*height;

		float old1 = l1;
		l1 = 1-l2;
		l2 = 1-old1;

	}

	l3 = 1-l1-l2;

	return centerY+h1*l1+h2*l2+h3*l3;

}

UINT TerrainObject::GetSubFromCoordinate(float x,float z)
{
	
	transformPoint(x,z);
	prepSubFromCoordinate(x,z);
	int subX = (int)(x/(columns/subs));
	int subY = (int)(z/(rows/subs));
	return subY*(int)subs+subX;

}

bool TerrainObject::prepSubFromCoordinate(float x,float y)
{
	

	if(x >= 0 && y >= 0 && x < columns && y < rows)
	{

		int subX = (int)(x/(columns/subs));
		int subY = (int)(y/(rows/subs));

		int sub = subY*(int)subs+subX;
		usages[sub]++;
		TerrainMesh* retVal = nullptr;

		if(isAlive[sub]->GetValue())
		{
			
			retVal = preLoadedSubs[sub];

		}
		else
		{

			retVal = loadMesh(sub);

		}

		retVal->Lock();
		return true;

	}
	
	return false;

}

bool TerrainObject::prepSubFromSub(UINT sub)
{

	if(sub < subs*subs)
	{

		usages[sub]++;
		TerrainMesh* retVal = nullptr;

		if(isAlive[sub]->GetValue())
		{
			
			retVal = preLoadedSubs[sub];

		}
		else
		{

			retVal = loadMesh(sub);

		}

		retVal->Lock();
		return true;

	}

	return false;

}

void TerrainObject::transformPoint(float &x,float &z)
{

	x -= centerX;
	z -= centerZ;

	x = clamp((x/(width*0.5f)+1)*0.5f*columns,0.0f,(float)(columns-1));
	z = clamp((z/(depth*0.5f)+1)*0.5f*rows,0.0f,(float)(rows-1));

}

UINT* TerrainObject::GetSubsFromFrustum(Frustum* frustum,int &size,UINT* &outArray)
{
	
	size = 0;
	int pointSize = 0;
	Vector2* worldPoints = frustum->GetUpperCornerPoints(pointSize);
	Vector2* localPoints  = new Vector2[pointSize];

	for(int i=0;i<pointSize;i++)
	{

		localPoints[i] = Vector2(worldPoints[i].x,worldPoints[i].y);
		transformPoint(localPoints[i].x,localPoints[i].y);

	}

	UINT* retVal = outArray;

	if(outArray == nullptr)
	{

		return nullptr;

	}

	int base = 0;
	int max = 4;
	float xCrement = (float)columns/subs;
	float yCrement = (float)rows/subs;
	
	for(int i=0;i<pointSize/3;i++)//Divide the frustum into trapeziums 
	{

		float startX = localPoints[base].x;
		int startIndex = base;

		float endX = localPoints[base].x;
		int endIndex = base;

		for(int k=1;k<4;k++)//Find the max/min x-coordinates
		{
			
			int index = (base+k)%pointSize;

			if(localPoints[index].x < startX)
			{

				startX = localPoints[index].x;
				startIndex = index;

			}
			
			if(localPoints[index].x > endX)
			{

				endX = localPoints[index].x;
				endIndex = index;

			}
		}

		int highIndex = startIndex;
		int lowIndex = startIndex;

		int index2 = (int)clamp((float)((startIndex+1)%max),(float)base,(float)max);
		int index3 = startIndex-1;

		if(index3 < 0)
		{

			index3 = max-1;

		}

		for(int x=(int)(startX/xCrement);x<=(int)(endX/xCrement);x++)//Step through each sub in the xDirection
		{

			float xCoord = (x+1)*xCrement;//Get the x-value of the next sub
			float maxY = localPoints[highIndex].y;
			float minY = localPoints[lowIndex].y;

			if(minY > maxY)
			{

				float tempY = maxY;
				maxY = minY;
				minY = tempY;

			}

			bool exceed1 = false;
			bool exceed2 = false;

			//Find out which localPoints to determine y-values from
			while(xCoord > localPoints[index2].x && !exceed1)
			{

				if(localPoints[index2].y > maxY)
				{

					maxY = localPoints[index2].y;

				}
				
				if(localPoints[index2].y < minY)
				{

					minY = localPoints[index2].y;

				}

				highIndex = index2;
				index2 = (int)clamp(((float)((index2+1)%max)),(float)base,(float)max);
				exceed1 = localPoints[index2].x < localPoints[highIndex].x;

			}
			
			//Find out which points to determine y-values from
			while(xCoord > localPoints[index3].x && !exceed2)
			{
				
				if(localPoints[index3].y > maxY)
				{

					maxY = localPoints[index3].y;

				}
				
				if(localPoints[index3].y < minY)
				{

					minY = localPoints[index3].y;

				}

				lowIndex = index3;
				index3--;

				if(index3 < 0)
				{

					index3 = max-1;

				}

				exceed2 = localPoints[index3].x < localPoints[lowIndex].x;

			}

			if(!exceed1 && !exceed2)//Check if we have circled around
			{

				float xDist1 = xCoord-localPoints[highIndex].x;
				float xDist2 = xCoord-localPoints[lowIndex].x;

				float ratio1 = (localPoints[index2].y-localPoints[highIndex].y)/(localPoints[index2].x-localPoints[highIndex].x);
				float ratio2 = (localPoints[index3].y-localPoints[lowIndex].y)/(localPoints[index3].x-localPoints[lowIndex].x);

				float y1 = localPoints[highIndex].y+xDist1*ratio1;
				float y2 = localPoints[lowIndex].y+xDist2*ratio2;

				if(y1 > maxY)
				{

					maxY = y1;

				}

				if(y2 > maxY)
				{

					maxY = y2;

				}
				
				if(y1 < minY)
				{

					minY = y1;

				}

				if(y2 < minY)
				{

					minY = y2;

				}
			}

			int subYMin = (int)(minY/yCrement);
			int subYMax = (int)(maxY/yCrement);

			for(int y=subYMin;y<=subYMax;y++)//Step through the subs in the y-coordinate
			{

				int subHere = (int)(y*subs+x);
				bool exists = false;

				if(i >= 1)
				{

					for(int k=0;k<size && !exists;k++)
					{

						exists = retVal[k] == subHere;

					}
				}

				if(!exists)
				{

					if(subHere == 9)
					{

						int dbg = 0;

					}

					if(prepSubFromSub(subHere))
					{
					
						retVal[size] =  subHere;
						size++;

					}
				}
			}
		}

		base = 3;
		max = 6;

	}
	
	delete[] localPoints;
	return retVal;

}

TerrainMesh* TerrainObject::GetSubMesh(UINT subIndex)
{

	if(isAlive[subIndex]->GetValue())
	{

		return preLoadedSubs[subIndex];

	}

	return nullptr;

}

MeshObject::Material* TerrainObject::GetMaterial()
{

	return base;

}

int TerrainObject::GetTotalAmountOfSubs() const
{

	return (int)(subs*subs);

}

TerrainMesh* TerrainObject::loadMesh(int sub)
{

	int subX = sub%(int)subs;
	int subY = (int)(sub/subs);

	float subWidth = width/subs;
	float subHeight = depth/subs;
	float xCrement = subWidth/(pointDensity-1);
	float yCrement = subHeight/(pointDensity-1);

	float localXCrement = xCrement*(columns/width);
	float localYCrement = yCrement*(rows/depth);

	float xStart = (centerX-width*0.5f)+subWidth*subX;//The subs starting position in worldspace
	float yStart = (centerY-depth*0.5f)+subHeight*subY;//The subs starting position in worldspace
	
	float localXStart = columns/subs*subX;//The subs starting position in worldspace
	float localYStart = rows/subs*subY;//The subs starting position in worldspace

	int vSize = pointDensity*pointDensity;
	int iSize = (pointDensity-1)*(pointDensity-1)*6;
	int levels = (vSize/1000)+1;

	if(1000 <= vSize)
	{

		vSize = 1000;
		iSize = (int)(1000*0.5/pointDensity*(pointDensity-1));

	}

	TerrainMesh* mesh = new TerrainMesh(sub,levels,isAlive[sub]);
	UINT* indices = new UINT[iSize];
	MeshObject::Vertex** vertices = new MeshObject::Vertex*[vSize];
	int totalVertices = 0;
	int totalIndices = 0;
	bool newLevel = true;
	int level = 0;

	for(UINT y = 0;y<pointDensity;y++)
	{

		for(UINT x=0;x<pointDensity;x++)
		{

			int max = 2;//Add two vertices per row

			if(y == pointDensity-1 && y%2 == 0)
			{

				max = 1;//Add one vertex per row

			}
			
			if((y%2 == 0 && totalVertices +max*2 > 1000) || (y%2 != 0 && totalVertices+max*x+3 > (UINT)1000))
			{

				newLevel = true;
				mesh->AddVertices(vertices,totalVertices,level);
				mesh->AddIndices(indices,totalIndices,level);

				vSize =pointDensity*pointDensity;
				iSize = (pointDensity-1)*(pointDensity-1)*6;

				if(1000 <= vSize)
				{

					vSize = 1000;
					iSize = 1000*3;

				}

				indices = new UINT[iSize];
				vertices = new MeshObject::Vertex*[vSize];
				totalVertices = 0;
				totalIndices = 0;
				level++;

			}

			if(y%2 == 0 || newLevel)
			{

				for(int i=0;i<max;i++)//Add vertices
				{

					float xNormal = 0;
					float yNormal = 1;
					float zNormal = 0;

					float xPos = centerX+xStart+x*xCrement;
					float zPos = centerZ+yStart+(y+i)*yCrement;

					float uX = (((float)xPos/(float)width)+1)*0.5f;
					float uY = (((float)zPos/(float)depth)+1)*0.5f;

					vertices[totalVertices] = new MeshObject::Vertex(3,2,3); 
					float localX = clamp(localXStart+ x*localXCrement,0,(float)(columns-1));
					float localY = clamp(localYStart+ (y+i)*localXCrement,0,(float)(rows-1));

					float yPos = values[(int)localX][(int)(localY)]*height;
			
					vertices[totalVertices]->SetVertix(0,xPos);
					vertices[totalVertices]->SetVertix(1,centerY+yPos);
					vertices[totalVertices]->SetVertix(2,zPos);

					vertices[totalVertices]->SetTextureVertix(0,uX);
					vertices[totalVertices]->SetTextureVertix(1,uY);

					//calculate normals
					float rightY = 0;
					float leftY = 0;
					float upY = 0;
					float downY = 0;
					float rightDownY = 0;
					float leftUpY = 0;

					if(localX >= localXCrement)
					{
						
						leftY = values[(int)(localX-localXCrement)][(int)(localY)]*height - yPos;

						if(localY+localYCrement < rows)
						{
						
							downY = values[(int)localX][(int)(localY+localYCrement)]*height - yPos;

						}
						
						if(localY >= localYCrement)
						{
							
							upY = values[(int)localX][(int)(localY-localYCrement)]*height - yPos;
							leftUpY = values[(int)(localX-localXCrement)][(int)(localY-localYCrement)]*height - yPos;
							 
						}
					}

					if(localX+localXCrement < columns)
					{
						
						rightY = values[(int)(localX+localXCrement)][(int)(localY)]*height - yPos;

						if(localY+localYCrement < rows)
						{
						
							downY = values[(int)localX][(int)(localY+localYCrement)]*height - yPos;
							rightDownY = values[(int)(localX+localXCrement)][(int)(localY+localYCrement)]*height-yPos;

						}
						
						if(localY >= localYCrement)
						{
							
							upY = values[(int)localX][(int)(localY-localYCrement)]*height - yPos;

						}
					}

					int faces = 6;
					int currFace = 0;
					Vector3* faceNormals = new Vector3[faces];

					if(localX >= localXCrement)
					{
						
						if(localY+localYCrement < rows)
						{
						
							faceNormals[currFace] = Vector3( 
								leftY*yCrement-0,
								0-(-xCrement*yCrement),
								-xCrement*downY-0); //Cross left with down
							currFace++;

						}
						
						if(localY >= localYCrement)
						{
							
							faceNormals[currFace] = Vector3(
								leftUpY*(-yCrement)-(-yCrement)*upY,
								0-(-xCrement)*(-yCrement),
								(-xCrement)*upY-0);//Cross up with leftUp
							currFace++;
							
							faceNormals[currFace] = Vector3(
								0-(-yCrement)*leftY,
								(-yCrement)*(-xCrement)-0,
								(-xCrement)*leftY-leftUpY*(-xCrement)); //Cross left with leftUp
							currFace++;
						}
					}

					if(localX+localXCrement < columns)
					{
						
						if(localY+localYCrement < rows)
						{
						
							faceNormals[currFace] = Vector3(
								rightDownY*yCrement-yCrement*downY,
								0-xCrement*yCrement,
								xCrement*downY-rightDownY*xCrement);//Cross rightdown with down
							currFace++;

							faceNormals[currFace] = Vector3(
								0-yCrement*rightDownY,
								yCrement*xCrement-0,
								xCrement*rightY-rightDownY*xCrement);//Cross rightDown with right
							currFace++;

						}
						
						if(localY >= localYCrement)
						{
							
							faceNormals[currFace] = Vector3(
								rightY*(-yCrement)-0,
								0-xCrement*(-yCrement),
								xCrement*upY-0);//Cross up with right
							currFace++;
							
						}
					}

					for(int i=0;i<currFace;i++)
					{

						xNormal += faceNormals[i].x;
						yNormal += faceNormals[i].y;
						zNormal += faceNormals[i].z;

					}

					delete[] faceNormals;

					xNormal/=currFace;
					yNormal/=currFace;
					zNormal/=currFace;
					float lengthInv = 1/sqrt(xNormal*xNormal+yNormal*yNormal+zNormal*zNormal);

					vertices[totalVertices]->SetVertixNormal(0,xNormal*lengthInv);
					vertices[totalVertices]->SetVertixNormal(1,yNormal*lengthInv);
					vertices[totalVertices]->SetVertixNormal(2,zNormal*lengthInv);

					totalVertices++;

				}

				if(x < pointDensity-1 && y <pointDensity-1)//Set indices for the two faces
				{

					indices[totalIndices+0] = totalVertices-1;
					indices[totalIndices+1] = totalVertices;
					indices[totalIndices+2] = totalVertices-2;

					indices[totalIndices+3] = totalVertices-1;
					indices[totalIndices+4] = totalVertices+1;
					indices[totalIndices+5] = totalVertices;

					newLevel = false;

					totalIndices += 6;

				}
			}
			else
			{
				if(x < pointDensity-1 && y <pointDensity-1)//Set indices for two faces
				{

					indices[totalIndices+0] = totalVertices+x*2;
					indices[totalIndices+1] = totalVertices+(x*2)-(pointDensity*2-1)+2;
					indices[totalIndices+2] = totalVertices+(x*2)-(pointDensity*2-1);

					indices[totalIndices+3] = totalVertices+x*2;
					indices[totalIndices+4] = totalVertices+x*2+2;
					indices[totalIndices+5] = totalVertices+(x*2)-(pointDensity*2-1)+2;

					if(y == pointDensity-2)
					{

						indices[totalIndices+0] = totalVertices+x;
						indices[totalIndices+3] = totalVertices+x;
						indices[totalIndices+4] = totalVertices+x+1;

					}

					totalIndices += 6;

				}
			}
		}
	}
	
	mesh->AddVertices(vertices,totalVertices,level);
	mesh->AddIndices(indices,totalIndices,level);
	preLoadedSubs[sub] = mesh;
	cacheUsed++;

	while(cacheUsed > cacheMax)//Clean up subs
	{

		int lowestIndex = 0;
		int lowestValue = MAXINT;

		for(int i=0;i<subs*subs;i++)
		{

			if(isAlive[i] && usages[i] < lowestValue && i!= sub)
			{

				lowestValue = usages[i];
				lowestIndex = i;
				usages[i]--;
				clamp((float)usages[i],0,(float)usages[i]);

			}
		}

		if(isAlive[lowestIndex]->GetValue())
		{
		
			preLoadedSubs[lowestIndex]->FlagForDeletion();

		}

		cacheUsed--;

	}
	
	return mesh;

}

float TerrainObject::clamp(float value,float min,float max)
{

	if(value > max)
	{

		return max;

	}
	else if(value < min)
	{

		return min;

	}

	return value;

}

void TerrainObject::SetSubs(UINT newSubs)
{

	for(int i=0;i<subs*subs;i++)
	{

		if(isAlive[i]->GetValue())
		{
			
			preLoadedSubs[i]->FlagForDeletion();

		}

		delete isAlive;

	}

	delete[] objects;
	delete[] preLoadedSubs;
	delete[] usages;
	delete[] objects;
	delete[] isAlive;

	subs = (float)newSubs;
	int tempSubs = (int)pow(2.0f,(float)newSubs);
	preLoadedSubs = new TerrainMesh*[tempSubs];
	usages = new int[tempSubs];
	objects = new TerrainMesh*[tempSubs];
	isAlive = new BoxedBool*[tempSubs];

	for(int i=0;i<tempSubs;i++)
	{

		preLoadedSubs[i] = nullptr;
		objects[i] = nullptr;
		usages[i] = 0;
		isAlive[i] = false;
		isAlive[i] = new BoxedBool(false);

	}
}

void TerrainObject::SetPointDensity(UINT newSize)
{

	if( newSize >= 1)
	{

		pointDensity = newSize;
		
		for(int i=0;i<subs*subs;i++)
		{

			if(isAlive[i]->GetValue())
			{

				preLoadedSubs[i]->FlagForDeletion();

			}

			usages[i] = 0;

		}
	}
}

void TerrainObject::SetX(float x)
{

	this->centerX = x;
	
	for(int i=0;i<subs*subs;i++)
	{

		if(isAlive[i]->GetValue())
		{

			preLoadedSubs[i]->FlagForDeletion();

		}

		usages[i] = 0;

	}
}

void TerrainObject::SetY(float y)
{

	this->centerY = y;
		
	for(int i=0;i<subs*subs;i++)
	{
		
		if(isAlive[i]->GetValue())
		{

			preLoadedSubs[i]->FlagForDeletion();

		}

		usages[i] = 0;

	}
}

void TerrainObject::SetZ(float z)
{

	this->centerZ = z;
	
		
	for(int i=0;i<subs*subs;i++)
	{
		
		if(isAlive[i]->GetValue())
		{

			preLoadedSubs[i]->FlagForDeletion();

		}

		usages[i] = 0;

	}
}
		
void TerrainObject::SetWidth(float w)
{

	this->width = w;
		
	for(int i=0;i<subs*subs;i++)
	{
		
		if(isAlive[i]->GetValue())
		{

			preLoadedSubs[i]->FlagForDeletion();

		}

		usages[i] = 0;

	}
}

void TerrainObject::SetHeight(float h)
{

	this->height = h;
		
	for(int i=0;i<subs*subs;i++)
	{
		
		if(isAlive[i]->GetValue())
		{

			preLoadedSubs[i]->FlagForDeletion();

		}

		usages[i] = 0;

	}
}

void TerrainObject::SetDepth(float d)
{

	this->depth = d;
		
	for(int i=0;i<subs*subs;i++)
	{
		
		if(isAlive[i]->GetValue())
		{

			preLoadedSubs[i]->FlagForDeletion();

		}

		usages[i] = 0;

	}
}

TerrainObject::~TerrainObject()
{

	base->Release();
	delete base;

	for(int i=0;i<subs*subs;i++)
	{

		if(isAlive[i]->GetValue())
		{
		
			preLoadedSubs[i]->FlagForDeletion();

		}

		delete isAlive[i];

	}

	delete[] preLoadedSubs;
	delete[] usages;
	delete[] objects;
	delete[] isAlive;

	for(int c=0;c<columns;c++)
	{
	
		delete[] values[c];

	}

	delete[] values;

}