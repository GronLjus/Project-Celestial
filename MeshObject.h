#pragma once
#include <string>
#include "BufferObject.h"
#include "IBounding.h"
#include "TextureResourceObject.h"
#include "BaseObject.h"

namespace Resources
{

	///<summary>This class handles all data that forms a mesh</summary>
	class MeshObject : public BaseObject
	{
		public:
		
			///<summary>This struct represents a vertex of arbitrary dimensions</summary>
			struct Vertex
			{

				private:
					///<summary>The dimensions for the position</summary>
					int vMod;
					///<summary>The dimensions for the texture</summary>
					int vtMod;
					///<summary>The dimensions for the normals</summary>
					int vnMod;
					///<summary>An array containing the vertex's position</summary>
					float* v;
					///<summary>An array containing the vertex's texture-coordinates</summary>
					float* vt;
					///<summary>An array containing the vertex's normals</summary>
					float* vn;
					///<summary>An array containing the vertex's edgenormals</summary>
					float* ven;

				public:

					Vertex(){ vMod = 0; vtMod = 0; vnMod = 0; v = nullptr; vt = nullptr; vn = nullptr; ven = nullptr; }

					///<param name='vM'>[in]The dimensions for the position</param>
					///<param name='vtM'>[in]The dimensions for the texture</param>
					///<param name='vnM'>The dimensions for the normals</param>
					Vertex(int vM,int vtM,int vnM)
					{
					
						vMod = vM;
						vtMod = vtM;
						vnMod = vnM;
						v = new float[vM];
						vt = new float[vtM];
						vn = new float[vnM];
						ven = new float[vnM];
						
						for(int i=0;i<vMod;i++)
						{

							v[i] = -1.0f;

						}
					
						for(int i=0;i<vtMod;i++)
						{

							vt[i] = -1.0f;

						}
					
						for(int i=0;i<vnMod;i++)
						{

							vn[i] = -1.0f;

						}
						
						for(int i=0;i<vnMod;i++)
						{

							ven[i] = -1.0f;

						}
					}

					///<param name='copy'>[in]A pointer ot the vertex to copy data from</param>
					Vertex(Vertex* copy)
					{

						vMod = copy->getVM();
						vtMod = copy->getVTM();
						vnMod = copy->getVNM();
						v = new float[vMod];
						vt = new float[vtMod];
						vn = new float[vnMod];
						ven = new float[vnMod];

						for(int i=0;i<vMod;i++)
						{

							v[i] = copy->getV()[i];

						}
					
						for(int i=0;i<vtMod;i++)
						{

							vt[i] = copy->getVT()[i];

						}
					
						for(int i=0;i<vnMod;i++)
						{

							vn[i] = copy->getVN()[i];
							ven[i] = copy->getVEN()[i];

						}
					}

					///<summary>Gets an array of this vertex's position</summary>
					///<returns>An array containing the position</returns>
					float* getV() const
					{

						return v;

					}
				
					///<summary>Gets an array of this vertex's texture-coordinates</summary>
					///<returns>An array containing the texture</returns>
					float* getVT() const
					{

						return vt;

					}
				
					///<summary>Gets an array of this vertex's normal</summary>
					///<returns>An array containing the normal</returns>
					float* getVN() const
					{

						return vn;

					}

					///<summary>Gets an array of this vertex's edgenormal</summary>
					///<returns>An array containing the edgenormal</returns>
					float* getVEN() const
					{

						return ven;

					}

					///<summary>Gets the dimensions of this vertex's position</summary>
					///<returns>The dimension for the position</returns>
					int getVM() const
					{

						return vMod;

					}
				
					///<summary>Gets the dimensions of this vertex's position</summary>
					///<returns>The dimension for the position</returns>
					int getVTM() const
					{

						return vtMod;

					}
				
					///<summary>Gets the dimensions of this vertex's position</summary>
					///<returns>The dimension for the position</returns>
					int getVNM() const
					{

						return vnMod;

					}

					///<summary>Sets the parameter of the vertex's position</summary>
					///<param name='parameter'>[in]The parameter to modify</param>
					///<param name='v'>[in]The value to set</param>
					void SetVertix(int parameter,float v)
					{

						if(parameter < vMod)
						{
					
							this->v[parameter] = v;

						}
					}
				
					///<summary>Sets the parameter of the vertex's texture-coordinate</summary>
					///<param name='parameter'>[in]The parameter to modify</param>
					///<param name='vt'>[in]The value to set</param>
					void SetTextureVertix(int parameter,float vt)
					{

						if(parameter < vtMod)
						{

							this->vt[parameter] = vt;

						}
					}
				
					///<summary>Sets the parameter of the vertex's normal</summary>
					///<param name='parameter'>[in]The parameter to modify</param>
					///<param name='vn'>[in]The value to set</param>
					void SetVertixNormal(int parameter,float vn)
					{

						if(parameter < vnMod)
						{

							this->vn[parameter] = vn;

						}
					}

					///<summary>Sets the parameter of the vertex's edgenormal</summary>
					///<param name='parameter'>[in]The parameter to modify</param>
					///<param name='vn'>[in]The value to set</param>
					void SetVertixEdgeNormal(int parameter, float vn)
					{

						if (parameter < vnMod)
						{

							this->ven[parameter] = vn;

						}
					}

					~Vertex()
					{
					
						if(v != nullptr)
						{
					
							delete[] v;
							v = nullptr;
					
						}

						if(vt != nullptr)
						{

							delete[] vt;
							vt = nullptr;
					
						}
					
						if(vn != nullptr)
						{
				
							delete[] vn;
							vn = nullptr;

						}

						if (ven != nullptr)
						{

							delete[] ven;
							ven = nullptr;

						}
					}
			};

			///<summary>This struct represents a linked list of indexed faces with a arbitrary number of vertices</summary>
			struct Face
			{

				private:
					///<summary>The amount of vertices on this face</summary>
					int vertices;
					///<summary>An array containing the indices for this face</summary>
					int* index;
					///<summary>An array containing the adjecencies for each edge</summary>
					int* adjIndex;
					///<summary>The id of the face</summary>
					int id;

					///<summary>The next face in the material</summary>
					Face* nextMaterialFace;
					///<summary>The next face in the group</summary>
					Face* nextGroupFace;
					///<summary>The next face in the list</summary>
					Face* nextFace;

					///<summary>The material-index for this face</summary>
					int material;
					///<summary>The group-index for this face</summary>
					int group;

					///<summary>Initilizes the face</summary>
					///<param name='size'>[in]The number of vertices</param>
					///<param name='m'>[in]The material of this face</param>
					///<param name='g'>[in]The group of this face</param>
					void init(int size,int m,int g)
					{

						group = g;
						material = m;
						vertices = size;
						nextFace = nullptr;
						nextMaterialFace = nullptr;
						nextGroupFace = nullptr;

						index = new int[size];
						adjIndex = new int[size];

						for(int i=0;i<size;i++)
						{

							index[i] = 0;
							adjIndex[i] = -1;

						}
					}

				public:
					///<summary>Gets the groupvalue of this face</summary>
					///<returns>A value of the faces group</returns>
					int GetGroup() const
					{

						return group;

					}
				
					///<summary>Gets the materialindex of this face</summary>
					///<returns>A value of the faces material</returns>
					int GetMaterial() const
					{

						return material;

					}
				
					///<summary>Gets the number of vertices for this face</summary>
					///<returns>The amount of vertices</returns>
					int getSize() const
					{

						return vertices;

					}

					///<summary>Gets the next face in the list</summary>
					///<returns>A pointer to the next face</returns>
					Face* getNext()
					{

						return nextFace;

					}
					
					///<summary>Gets the next face in the materiallist</summary>
					///<returns>A pointer to the next face</returns>
					Face* getNextInMaterial()
					{

						return nextMaterialFace;

					}

					///<summary>Gets the next face in the grouplist</summary>
					///<returns>A pointer to the next face</returns>
					Face* getGroupNext()
					{

						return nextGroupFace;

					}

					///<param name='size'>[in]The number of vertices</param>
					///<param name='m'>[in]The material of this face</param>
					///<param name='g'>[in]The group of this face</param>
					Face(int size,int m,int g)
					{

						init(size,m,g);

					}

					///<param name='copy'>[in]A pointer to the face to copy</param>
					Face(Face* &copy)
					{
					
						init(copy->getSize(),copy->GetMaterial(),copy->GetGroup());

						for(int i=0;i<vertices;i++)//Copy the indices
						{

							SetIndexAt(i,copy->GetIndexAt(i));
							SetAdjIndexAt(i,copy->GetAdjIndexAt(i));

						}
					}

					///<summary>Gets a index on the face</summary>
					///<param name='place'>[in]Where to get the index from</param>
					///<returns>An index</returns>
					int GetIndexAt(int place) const
					{

						return index[place];

					}

					///<summary>Gets a adjecant index on an edge</summary>
					///<param name='place'>[in]The edge to get the index from</param>
					///<returns>An index</returns>
					int GetAdjIndexAt(int place) const
					{

						return adjIndex[place];

					}
					///<summary>Gets the id of the face</summary>
					///<returns>The id of the face</returns>
					int GetId() const
					{

						return id;

					}

					///<summary>Sets an index on the face</summary>
					///<param name='place'>[in]Where to modify the value</param>
					///<param name='val'>[in]The new index</param>
					void SetIndexAt(int place,int val)
					{

						index[place] = val;

					}

					///<summary>Sets an adjecent-index on the face</summary>
					///<param name='place'>[in]Where to modify the value</param>
					///<param name='val'>[in]The new index</param>
					void SetAdjIndexAt(int place,int val)
					{

						adjIndex[place] = val;

					}

					///<summary>Sets the indices for the face</summary>
					///<param name='val'>[in]An array of indices to set in the face</param>
					void SetIndex(int* val)
					{

						if(index != nullptr)
						{

							delete[] index;

						}

						index = val;

					}

					///<summary>Sets the next face in the list</summary>
					///<param name='face'>[in]A pointer to the face that will be next in the list</param>
					void Setnext(Face* face)
					{

						nextFace = face;

					}
				
					///<summary>Sets the next face in the grouplist</summary>
					///<param name='face'>[in]A pointer to the face that will be next in the list</param>
					void SetnextGroup(Face* face)
					{

						nextGroupFace = face;

					}

					///<summary>Sets the next face in the materiallist</summary>
					///<param name='face'>[in]A pointer to the face that will be next in the list</param>
					void SetnextMaterial(Face* face)
					{

						nextMaterialFace = face;

					}
					///<summary>Sets the material of the face</summary>
					///<param name='val'>[in]The material</param>
					void SetMaterial(int val)
					{

						material = val;

					}
					///<summary>Sets the id of the face</summary>
					///<param name='id'>[in]The id to use</param>
					void SetId(int id)
					{

						this->id = id;

					}

					///<summary>Sets the group of the face</summary>
					///<param name='val'>[in]The group</param>
					void SetGroup(int val)
					{

						group = val;

					}

					///<summary>Reduces the face into triangles</summary>
					void Triangulize()
					{

						if(vertices > 3)
						{

							Face* newNext = nullptr;
							Face* lastNext = nullptr;
							int* newIndices = new int[3];
							int* newAdjs = new int[3];
						
							newIndices[0] = index[0];
							newIndices[1] = index[1];
							newIndices[2] = index[2];
							newAdjs[0] = adjIndex[0];
							newAdjs[1] = adjIndex[1];
							newAdjs[2] = adjIndex[2];

							for(int i=2;i<vertices-1;i++)
							{

								bool even = i%2 == 0;
								Face* temp = new Face(3,this->material,group);//Create a new face
								int* tempIndex = new int[3];

								if(even || i == vertices-2)//Inserts indexes based on orientation
								{

								
									tempIndex[0] = index[i-2];
									tempIndex[1] = index[i];
									tempIndex[2] = index[i+1];

									temp->SetAdjIndexAt(0,adjIndex[i-2]);
									temp->SetAdjIndexAt(1,adjIndex[i]);
									temp->SetAdjIndexAt(2,adjIndex[i+1]);

								}
								else
								{

									tempIndex[0] = index[i];
									tempIndex[1] = index[i-1];
									tempIndex[2] = index[i+2];
									
									temp->SetAdjIndexAt(0,adjIndex[i]);
									temp->SetAdjIndexAt(1,adjIndex[i-1]);
									temp->SetAdjIndexAt(2,adjIndex[i+2]);

								}

								temp->SetIndex(tempIndex);//Sets the index for the new face

								if(newNext == nullptr)//Inserts the new face into a tempoary list
								{

									newNext = temp;
									lastNext = newNext;

								}
								else
								{

									lastNext->Setnext(temp);
									lastNext = lastNext->getNext();

								}
							}

							delete[] adjIndex;
							delete[] index;
							index = newIndices;
							adjIndex = newAdjs;
							vertices = 3;
							lastNext->Setnext(nextFace);//Sets the next face last in the temp list
							lastNext->SetnextGroup(nextGroupFace);//Sets the next face last in the temp list
							lastNext->SetnextMaterial(nextMaterialFace);//Sets the next face last in the temp list

							nextFace = newNext;
							nextMaterialFace = newNext;
							nextGroupFace = newNext;

						}
					}
					
					~Face()
					{
				
						delete[] index;
						delete[] adjIndex;

					}
			};
		
			///<summary>This struct represents a group of faces</summary>
			struct Group
			{

				private:
					///<summary>The first face in this group</summary>
					Face* firstFace;
					///<summary>The last face in this group</summary>
					Face* lastFace;
					///<summary>The name of the group</summary>
					std::string groupName;
					///<summary>The amount of vertices in this group</summary>
					int count;
					///<summary>The number of faces in this </summary>
					int faces;
					///<summary>The index where this material start</summary>
					int startingIndex;

				public:
					Group(){count = 0;faces = 0;firstFace = nullptr;}

					///<param name='copy'>[in]A pointer to the group to copy data from</param>
					Group(Group*& copy)
					{

						count = 0;
						faces = 0;
						groupName = copy->getName();

					}

					///<param name='first'>[in]A pointer to the first face in this group</param>
					///<param name='name'>[in]The name of this group</param>
					Group(Face* &first, std::string name){count = 0;faces = 0;firstFace = first;lastFace = first;groupName = name;}
					///<summary>Sets the first face of this group</summary>
					///<param name='first'>[in]A pointer to the first face to use</param>
					void SetFirstFace(Face* &first){firstFace = first;lastFace=firstFace;}
					///<summary>Sets the startingindex of the group</summary>
					///<param name='start'>[in]The new startingindex</param>
					void SetStartingIndex(int start){this->startingIndex = start;}

					///<summary>Gets the first face this group has</summary>
					///<returns>A pointer to the first face used by this group</returns>
					Face* getFirstFace(){return firstFace;}
					///<summary>Gets the name of the group</summary>
					///<returns>The group's name</returns>
					std::string getName() const{return groupName;}
					///<summary>Adds a face to the group</summary>
					///<param name='face'>[in]A pointer to the face to add</param>
					void addFace(Face* face)
					{
						count+= face->getSize();
						faces++; 
						if(firstFace == nullptr)
						{

							firstFace=face;
							lastFace=firstFace;

						}
						else
						{

							lastFace->SetnextGroup(face);

						}
					}
					///<summary>Gets the amount of vertices in this group</summary>
					///<returns>The amount of vertices</returns>
					int getCount() const{return count;}
					///<summary>Gets the amount of faces in this group</summary>
					///<returns>The number of faces</returns>
					int getFaces() const{return faces;}
					///<summary>Gets the starting index of the group</summary>
					///<returns>Where in the indexbuffer the group starts</returns>
					int GetStartingIndex() const{return startingIndex;}

					~Group()
					{
				
					}
			};
		
			///<summary>This struct represents a material that's part of the mesh</summary>
			struct Material
			{
			
				enum MaterialType{MaterialType_AMBIANT,MaterialType_DIFFUSE,MaterialType_NORMAL};

				private:
					///<summary>The first face in this material</summary>
					Face* firstFace;
					///<summary>The last face in this group</summary>
					Face* lastFace;
					///<summary>The diffusetexture of this material</summary>
					CrossHandlers::TextureResourceObject* diffuse;
					///<summary>The ambienttexture of this material</summary>
					CrossHandlers::TextureResourceObject* ambient;
					///<summary>The normaltexture of this material</summary>
					CrossHandlers::TextureResourceObject* normal;
					///<summary>The illumindex of this material</summary>
					byte illum;
					///<summary>This materials alpha-value</summary>
					float alpha;
					///<summary>This materials lightrefractionindex</summary>
					float lightRefraction;
					///<summary>An array containing the tfilter</summary>
					float* tFilter;
					///<summary>The amount of vertices in this material</summary>
					int count;
					///<summary>The number of faces in this material</summary>
					int faces;
					///<summary>The index where this material start</summary>
					int startingIndex;
					///<summary>The name of the material</summary>
					std::string materialName;

				public:
					///<param name='name'>[in]The name of the material</param>
					Material(std::string name)
					{

						count = 0;
						faces = 0;
						firstFace = nullptr;
						diffuse = nullptr;
						ambient = nullptr;
						normal = nullptr;
						materialName=name;
						tFilter = new float[3];
						tFilter[0] = 0;
						tFilter[1] = 0;
						tFilter[2] = 0;
						alpha = 1;
				
					}
				
					///<param name='copy'>[in]A pointer to the material to copy data from</param>
					Material(Material*& copy)
					{
					
						count = copy->GetCount();
						faces = copy->GetCount();
						tFilter = new float[3];
						materialName = copy->GetName();
						diffuse = copy->GetDiffuse();
						ambient = copy->GetAmbient();
						normal = copy->GetNormal();
						illum = copy->Getillum();
						tFilter[0] = copy->GetTFilter()[0];
						tFilter[1] = copy->GetTFilter()[1];
						tFilter[2] = copy->GetTFilter()[2];
						firstFace = nullptr;

					}

					///<summary>Gets the diffusetexture from this material</summary>
					///<returns>The diffuse of this material</returns>
					CrossHandlers::TextureResourceObject* GetDiffuse(){ return diffuse; }
					///<summary>Gets the ambienttexture from this material</summary>
					///<returns>The ambient of this material</returns>
					CrossHandlers::TextureResourceObject* GetAmbient(){ return ambient; }
					///<summary>Gets the normaltexture from this material</summary>
					///<returns>The normalmap of this material, will be nullptr if normalmap is missing</returns>
					CrossHandlers::TextureResourceObject* GetNormal(){ return normal; }
					///<summary>Gets the illum-index of this material</summary>
					///<returns>The index of the illum-model</returns>
					byte Getillum() const{return illum;}
					///<summary>Gets the alpha value of this material</summary>
					///<returns>The alpha-value</returns>
					float GetAlpha() const{return alpha;}
					///<summary>Gets the lightrefraction of the material</summary>
					///<returns>How light refracts through this material</returns>
					float GetLightRefraction() const{return lightRefraction;}
					///<summary>Gets the materials t-filter</summary>
					///<returns>And array of the t-filers of this material</returns>
					float* GetTFilter() const{return tFilter;}
					///<summary>Gets the name of this material</summary>
					///<returns>The materials name</returns>
					std::string GetName() const{return materialName;}
					///<summary>Gets the first face of the material</summary>
					///<returns>A pointer to the first face in the underlying list</returns>
					Face* GetFirstFace() {return firstFace;}
					///<summary>Gets the amount of vertices</summary>
					///<returns>The number of vertices in this material</returns>
					int GetCount() const{return count;}
					///<summary>Gets the number of faces in this material</summary>
					///<returns>The amount of faces</returns>
					int GetFaces() const{return faces;}
					///<summary>Gets the starting index of the material</summary>
					///<returns>Where in the indexbuffer the material starts</returns>
					int GetStartingIndex() const{return startingIndex;}

					///<summary>Sets the first face in the list</summary>
					///<param name='face'>[in]A pointer to the first face to use</param>
					void SetFirstFace(Face* &face){firstFace = face;lastFace = firstFace;}
					///<summary>Adds a face to the material</summary>
					///<param name='face'>[in]A pointer to the face to add</param>
					void AddFace(Face* face)
					{
						count+= face->getSize();
						faces++; 

						if(firstFace == nullptr)
						{

							firstFace=face;
							lastFace=firstFace;

						}
						else
						{

							lastFace->SetnextMaterial(face);

						}
					}

					///<summary>Sets the diffuse-texture</summary>
					///<param name='tex'>[in]A pointer to the texture to use</param>
					void SetDiffuse(CrossHandlers::TextureResourceObject* tex){ diffuse = tex; }
					///<summary>Sets the Ambient-texture</summary>
					///<param name='tex'>[in]A pointer to the texture to use</param>
					void SetAmbient(CrossHandlers::TextureResourceObject* tex){ ambient = tex; }
					///<summary>Sets the normal-texture</summary>
					///<param name='tex'>[in]A pointer to the texture to use</param>
					void SetNormal(CrossHandlers::TextureResourceObject* tex){ normal = tex; }
					///<summary>Sets the illumination-index to use</summary>
					///<param name='illum'>[in]The new index to use</param>
					void SetIllum(byte illum){this->illum = illum;}
					///<summary>Sets the alpha-value of the material</summary>
					///<param name='a'>[in]The new alpha-value</param>
					void SetAlpha(float a){alpha = a;}
					///<summary>Sets a new lightrefraction to use</summary>
					///<param name='lr'>[in]The new lightrefraction to use</param>
					void SetLightRefraction(float lr){lightRefraction = lr;}
					///<summary>Sets the T-filter for the material</summary>
					///<param name='tF'>[in]An array to the tfilter to COPY data from</param>
					void SetFilter(float* tF){tFilter[0] = tF[0];tFilter[1] = tF[1];tFilter[2] = tF[2];}
					///<summary>Sets the startingindex of the material</summary>
					///<param name='start'>[in]The new startingindex</param>
					void SetStartingIndex(int start){this->startingIndex = start;}
				
					///<summary>Resets the amount of vertices to 0</summary>
					void ResetCount(){count = 0;}
					///<summary>Increases count</summary>
					///<param name='vertices'>[in]How much to increase by</param>
					void IncrementCount(int vertices){count += vertices;}

					///<summary>Releases all texture allocated by this material, must be called at the end of its life</summary>
					void Release()
					{

						if(diffuse != nullptr)
						{

							delete diffuse;
							diffuse = nullptr;

						}

						if (ambient != nullptr )
						{

							delete ambient;
							ambient = nullptr;

						}

						if (normal != nullptr)
						{

							delete normal;
							normal = nullptr;

						}
					}

					///<summary>Releases one texture the material has allocated</summary>
					///<param name='mat'>[in]Which texture to release</param>
					void Release(MaterialType mat)
					{

						if (mat == MaterialType_DIFFUSE && diffuse != nullptr)
						{
					
							delete diffuse;
							diffuse = nullptr;

						}

						if (mat == MaterialType_AMBIANT && ambient != nullptr)
						{
					
							delete ambient;
							ambient = nullptr;

						}

						if (mat == MaterialType_NORMAL &&normal != nullptr)
						{
					
							delete normal;
							normal = nullptr;

						}
					}

					~Material()
					{

						delete[] tFilter;

						if (diffuse != nullptr)
						{

							delete diffuse;

						}

						if (ambient != nullptr )
						{

							delete ambient;

						}

						if (normal != nullptr)
						{

							delete normal;

						}
					}
			};

			///<param name='id'>[in]The unique id of the mesh</param>
			MeshObject(int id);

			///<param name='copy'>[in]A pointer to the meshobject to copy from</param>
			///<param name='id'>[in]The unique id of the mesh</param>
			MeshObject(MeshObject*& copy,int id);

			virtual unsigned char* Update(CrossHandlers::Message* mess) { return nullptr; }
			///<summary>Gets all groups in use by this mesh</summary>
			///<returns>An array of pointers to groups</returns>
			Group** getGroups();
			///<summary>Gets the number of groups used</summary>
			///<returns>The number of groups</returns>
			int GetGroupsSize() const;
			///<summary>Adds a new group to this mesh</summary>
			///<param name='group'>[in]A pointer to the group to add</param>
			void AddGroup(Group* group);
			///<summary>Adds a face to the mesh</summary>
			///<param name='face'>[in]A pointer to the face to add</param>
			void AddFace(Face* face);

			///<summary>Gets all materials in use by this mesh</summary>
			///<returns>An array of pointers to the materials</returns>
			Material** getMaterials();
			///<summary>Gets the number of materials used</summary>
			///<returns>The number of materials</returns>
			int GetMaterialSize() const;
			///<summary>Adds a new material to this mesh</summary>
			///<param name='material'>[in]A pointer to the material to add</param>
			void AddMaterial(Material* material);

			///<summary>Sets the first face to be used by the entire mesh</summary>
			///<param name='face'>[in]A pointer to the face used</param>
			void SetFirstFace(Face* &face);

			///<summary>Gets a copy of a boundingobject</summary>
			///<param name='shape'>[in]The shape to get a copy from</param>
			///<returns>A pointer to a created copy, nullptr if the bounding-shape doesn't exist</returns>
			CrossHandlers::IBounding* GetBoundingObjectCopy(CrossHandlers::Shape shape);
			///<summary>Get a bounding object related to this mesh</summary>
			///<param name='shape'>[in]The shape of the volume to get</param>
			///<returns>A pointer to a volume, nullptr if the bounding-shape doesn't exist</returns>
			CrossHandlers::IBounding* GetBoundingObject(CrossHandlers::Shape shape);
			///<summary>Sets a bounding object to this mesh</summary>
			///<param name='shape'>[in]The shape of the volume to set</param>
			///<param name='object'>[in]A pointer to the object to set</param>
			void SetBoundingObject(CrossHandlers::Shape shape,CrossHandlers::IBounding* object);
			///<summary>Calculates the appropiate volumes from the mesh</summary>
			void CalculateShapes();

			///<summary>Get the number of on a level</summary>
			///<returns>The amount of vertices</returns>
			int GetVertices() const;
			///<summary>Gets the total amount of indices used</summary>
			///<returns>The total amount of indices used</returns>
			int GetTotalIndices() const;

			///<summary>Check if the mesh is optimized</summary>
			///<returns>If the mesh is optimized</returns>
			bool IsOptimized() const;
			///<summary>Optimize the mesh so all indices are in order after meshes</summary>
			///<param name='convertFacesToTriangles'>[in]Whether the faces should be triangulized</param>
			void OptimizeLevels(bool convertFacesToTriangles);
			///<summary>Add vertices to the mesh</summary>
			///<param name='v'>[in]An array of pointers to vertices to add</param>
			///<param name='vSize'>[in]The amount of vetices</param>
			///<returns>An array containing the indices for each vertex</returns>
			int* AddVertices(Vertex** v,int vSize);//Returns the indices
			///<summary>Gets a vertex from the mesh</summary>
			///<param name='place'>[in]On which location the vertex is located at</param>
			///<returns>A pointer to the vertex</returns>
			Vertex* GetVertex(int place);
			///<summary>Gets the first face in this mesh</summary>
			///<returns>The pointer to the first face</returns>
			Face* GetFirstFace();

			///<summary>Sets the amount of buffers for the instances, also kills the current buffers</summary>
			///<param name='amount'>The amount of instancebuffers to use</param>
			void SetInstances(int amount);
			///<summary>Gets the buffers from the mesh</summary>
			///<param name='type'>[in]The type of the buffer to get</param>
			///<param name='which'>[in(optional)]Which buffer to load</param>
			///<returns>An array of pointers to the chosen buffers</returns>
			CrossHandlers::BufferObject* GetBuffer(CrossHandlers::BufferTypes type,int which);
			///<summary>Sets the buffers used by this mesh and releases those used</summary>
			///<param name='type'>[in]The type of buffer to set</param>
			///<param name='buffer'>[in]A pointers to the buffers</param>
			///<param name='which'>[in(optional)]Which buffer to load</param>
			void SetBuffer(CrossHandlers::BufferTypes type, CrossHandlers::BufferObject* buffer,int which);

			///<summary>Releases the resources used by this mesh, must be called at the end of it's life</summary>
			void Release(); 

			bool IsWireFrame() const;
			void SetWireFrame(bool wf);

			virtual ~MeshObject();

		private:
			///<summary>The meshes unique Id</summary>
			int id;
			///<summary>An array of pointers to volumes for this meshobject</summary>
			CrossHandlers::IBounding** shapes;
			///<summary>Extends the array containing groups</summary>
			void extendGroups();
			///<summary>Extends the array containing materials</summary>
			void extendMaterials();
			///<summary>Extends the array containing vertices</summary>
			void extendVertices();

			///<summary>The number of materials this mesh has</summary>
			int materialSize;
			///<summary>The number of groups this mesh has</summary>
			int groupSize;
			///<summary>The total number of indices</summary>
			int totalIndices;
			///<summary>The total amount of faces</summary>
			int totalFaces;

			///<summary>A pointer to the first face in this mesh</summary>
			Face* firstFace;
			Face* lastFace;
			///<summary>An array of pointers to the groups being used by this mesh</summary>
			Group** groups;
			///<summary>An array of pointers to the materials being used by this mesh</summary>
			Material** materials;
		
			///<summary>Adds a vertex to the mesh</summary>
			///<param name='v'>[in]A pointer to the vertex to add</param>
			void addVertex(Vertex* v);
			///<summary>If the mesh has been optimized</summary>
			bool optimization;
			///<summary>An array containing all vertices used by this mesh</summary>
			Vertex** vertices;
			///<summary>The current amount of vertices</summary>
			int vertexAmount;
			///<summary>The current max amount of vertices</summary>
			int maxVertices;

			///<summary>A pointer to the array of vertexbuffers used by this mesh</summary>
			CrossHandlers::BufferObject* vertexBuffer;
			///<summary>A pointer to the array of indexbuffers used  by this mesh</summary>
			CrossHandlers::BufferObject* indexBuffer;
			///<summary>An array of pointers to use as instances by the mesh</summary>
			CrossHandlers::BufferObject** instanceBuffers;
			///<summary>The amount of buffers in the instancearray</summary>
			int instances;

			bool isWireFrame;
	};
}