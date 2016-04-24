#pragma once
#include "../../Objects/MeshObject/MeshObject.h"
#include "../../../CrossHandler-Tools/ICachableObject.h"
#include "../../../CrossHandler-Tools/BoxedBool.h"

namespace Resources
{

	///<summary>This class contains the data to represent the vertices and the indices of a mesh used of a terrain</summary>
	class TerrainMesh : public CrossHandlers::ICachableObject
	{

		public:
			///<param name='sub'>[in]The sub this mesh represents</param>
			///<param name='levels'>[in]The number of levels this mesh contains</param>
			///<param name='isAlive'>[in]A pointer to the value determening if this object is still alive</param>
			TerrainMesh(int sub,int levels,CrossHandlers::BoxedBool* isAlive);
			///<summary>Gets the sub of this mesh</summary>
			///<returns>A value representing the sub of this mesh</returns>
			int GetSub() const;
			///<summary>Adds vertices to this mesh</summary>
			///<param name='verts'>[in]An array of pointers to the vertices this mesh should use</param>
			///<param name='amount'>[in]The amount of vertices to add</param>
			///<param name='level'>[in]The level to add the vertices to </param>
			void AddVertices(MeshObject::Vertex** verts,int amount,int level);
			///<summary>Adds indices to this mesh</summary>
			///<param name='inds'>[in]An array of ints to add as indices to this mesh</param>
			///<param name='amount'>[in]The amount of indices</param>
			///<param name='level'>[in]The level to add the indices to</param>
			void AddIndices(UINT* inds,int amount,int level);
		
			///<summary>Gets the levels this mesh contains</summary>
			///<returns>The amount of levels</returns>
			int GetLevels() const;

			///<summary>Gets the indices used by this mesh</summary>
			///<param name='amounts'>[out]An array containing the amount of indices for each level</param>
			///<returns>An array of arrays containing the indices used</returns>
			UINT** GetIndices(int* &amounts);
			///<summary>Gets the vertices used by this mesh</summary>
			///<param name='amounts'>[out]An array containing the amount of vertices for each level</param>
			///<returns>An array of arrays containing the pointers to the vertices used</returns>
			MeshObject::Vertex*** GetVertices(int* &amounts);
			///<summary>Sets the vertexbuffer used by this mesh</summary>
			///<param name='buffer'>[in]The buffer to set</param>
			///<param name='level'>[in]The level to add the buffer to</param>
			void SetVertexBuffer(ID3D10Buffer* buffer,int level);
			///<summary>Sets the indexbuffer used by this mesh</summary>
			///<param name='buffer'>[in]The buffer to set</param>
			///<param name='level'>[in]The level to add the buffer to</param>
			void SetIndexBuffer(ID3D10Buffer* buffer,int level);

			///<summary>Gets the indexbuffers used by this mesh, if any</summary>
			///<returns>An array of pointers to the indexbuffers used by this mesh, or nullptr if doesn't use any</returns>
			ID3D10Buffer** GetIndexBuffers();
			///<summary>Gets the vertexbuffers used by this mesh, if any</summary>
			///<returns>An array of pointers to the vertexbuffers used by this mesh, or nullptr if doesn't use any</returns>
			ID3D10Buffer** GetVertexBuffers();

			///<summary>Gets the amount of indices used in a level</summary>
			///<param name='level'>[in]The level to get the indices from</param>
			///<returns>The amount of indices in a level</returns>
			int GetIndexAmount(int level) const;

			///<summary>Checks if the mesh has been buffered already</summary>
			///<returns>Wether or not the mesh has been buffered</returns>
			bool IsBuffered() const;

			virtual void Lock();
			virtual void Unlock();

			virtual void FlagForDeletion();

		private:
			///<summary>The sub of this mesh</summary>
			int sub;
			///<summary>The amount of levels in this mesh</summary>
			int levels;
			///<summary>The double array containing the indices</summary>
			UINT** indices;
			///<summary>The double array containing the vertices</summary>
			MeshObject::Vertex*** vertices;
			///<summary>An array containing the amount of vertices in each level</summary>
			int* vAmounts;
			///<summary>An array containing the amount of indices in each level</summary>
			int* iAmounts;

			///<summary>Whether or not mesh is buffered</summary>
			bool isBuffered;
			///<summary>An array of pointers to the buffers that contains the vertices of this mesh</summary>
			ID3D10Buffer** vBuffers;
			///<summary>An array of pointers to the buffers that contains the indices of this mesh</summary>
			ID3D10Buffer** iBuffers;

			///<summary>The reference to the value determening if this object is still alive</summary>
			CrossHandlers::BoxedBool* isAlive;

			///<summary>If this object will kill itself when the locks become zeo</summary>
			bool willKillMyself;
			///<summary>The number of times this object has been locked</summary>
			int locks;

			~TerrainMesh();
	};

}