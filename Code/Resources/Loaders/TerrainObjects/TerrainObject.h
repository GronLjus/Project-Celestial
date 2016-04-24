#pragma once
#include "TerrainMesh.h"
#include "../../../CrossHandler-Tools/Frustum.h"
#include <string>

namespace Resources
{
	///<summary>This class encapsulates a object containing the data of a terrain</summary>
	class TerrainObject
	{

		public:

			///<param name='map'>[in]The name of the file to load heightvalues from</param>
			///<param name='pointDensity'>[in]The density of the terrain, value must have the range[1,MAXINT]</param>
			///<param name='subs'>[in]The amount of subs</param>
			///<param name='width'>[in]The width</param>
			///<param name='height'>[in]The height</param>
			///<param name='depth'>[in]The depth</param>
			///<param name='x'>[in]The x-value</param>
			///<param name='y'>[in]The y-value</param>
			///<param name='z'>[in]The z-value</param>
			///<param name='base'>A[in] pointer to the meshobject to use as a base</param>
			TerrainObject(std::string map,UINT pointDensity,UINT subs,int width,int height,int depth,int x,int y,int z,MeshObject::Material* base);

			///<summary>Gets the height of the terrain at a point</summary>
			///<param name='x'>[in]The x-value of the coordinate in worldspace</param>
			///<param name='z'>[in]The z-value of the coordinate in worldspace</param>
			///<returns>The height of the terrain</returns>
			float GetHeightAt(float x,float z);
			///<summary>Gets the index of the meshobject that contains a certain pixel</summary>
			///<param name='x'>[in]The x-value of the coordinate in worldspace</param>
			///<param name='z'>[in]The z-value of the coordinate in worldspace</param>
			///<returns>The index to the meshobject representing the sub of the terrain, it will be locked and will need to be unlocked when done</returns>
			UINT GetSubFromCoordinate(float x,float z);
			///<summary>Gets the subs contained by the frustum</summary>
			///<param name='frustum'>[in]A pointer to the frustum to check</param>
			///<param name='size'>[out]The size of the array</param>
			///<param name='outArray'>[in]The reference to the array to use, if nullptr the interior array will be used instead, this can cause strange behavior to occur if the array is not cleared before the function is called again</param>
			///<returns>An array of the indices to the meshobjects representing the subs, all of which are locked and will need to be unlocked when done</returns>
			UINT* GetSubsFromFrustum(CrossHandlers::Frustum* frustum,int &size,UINT* &outArray);

			///<summary>Gets the submesh of the terrain</summary>
			///<param name='subIndex'>[in]The index of the sub to get</param>
			///<returns>A pointer to the mesh if loaded, otherwise nullptr</returns>
			TerrainMesh* GetSubMesh(UINT subIndex);

			///<summary>Gets the material used by this terrain</summary>
			///<returns>A pointer to the material this terrain uses</returns>
			MeshObject::Material* GetMaterial();
			///<summary>Sets the amount of subs in the terrain</summary>
			///<param name='newSubs'>[in]The new amount of subs</param>
			void SetSubs(UINT newSubs);
			///<summary>Sets the pointdensity of the terrain</summary>
			///<param name='newSize'>[in]The new density of the terrain, value must have the range[1,MAXINT]</param>
			void SetPointDensity(UINT newSize);

			///<summary>Sets the x-value of the center</summary>
			///<param name='x'>[in]The new x-value</param>
			void SetX(float x);
			///<summary>Sets the y-value of the center</summary>
			///<param name='y'>[in]The new y-value</param>
			void SetY(float y);
			///<summary>Sets the z-value of the center</summary>
			///<param name='z'>[in]The new z-value</param>
			void SetZ(float z);
		
			///<summary>Sets the width of the terrain</summary>
			///<param name='w'>[in]The new width</param>
			void SetWidth(float w);
			///<summary>Sets the height of the terrain</summary>
			///<param name='h'>[in]The new height</param>
			void SetHeight(float h);
			///<summary>Sets the depth of the terrain</summary>
			///<param name='d'>[in]The new depth</param>
			void SetDepth(float d);

			///<summary>Gets the amount of total subs  in this object</summary>
			///<returns>The total amount of subs</returns>
			int GetTotalAmountOfSubs() const;

			~TerrainObject();

		private:
			///<summary>The amount of rows in the underlying data</summary>
			int rows;
			///<summary>The amount of columns in the underlying data</summary>
			int columns;

			///<summary>A doublearray containing the heightvalues with values from [0,1]</summary>
			float** values;
		
			///<summary>Where this heightmap should be centered in the x-axis</summary>
			float centerX;
			///<summary>Where this heightmap should be centered in the y-axis</summary>
			float centerY;
			///<summary>Where this heightmap should be centered in the z-axis</summary>
			float centerZ;
		
			///<summary>The maximum width of the object in units</summary>
			float width;
			///<summary>The maximum height of the object in units</summary>
			float height;
			///<summary>The maximim depth of the object in units</summary>
			float depth;

			///<summary>The amount of subsquares the object should be divided in</summary>
			float subs;
			///<summary>The distance between points in pixels</summary>
			UINT pointDensity;

			///<summary>An array determining if a subcache is alive</summary>
			CrossHandlers::BoxedBool** isAlive;
			///<summary>The current amount of used cache</summary>
			int cacheUsed;
			///<summary>The maximum amount of preloaded meshes</summary>
			int cacheMax;
			///<summary>An array containing whic cahces are being used</summary>
			int* usages;
			///<summary>An array of pointers to cached meshes</summary>
			TerrainMesh** preLoadedSubs;
		
			///<summary>A pointer to the material to use as a base</summary>
			MeshObject::Material* base;

			///<summary>An array of pointers to the subs</summary>
			TerrainMesh** objects;

			///<summary>Loads a mesh from a sub</summary>
			///<param name='sub'>[in]The sub</param>
			///<returns>A pointer to the newly created meshobject</returns>
			TerrainMesh* loadMesh(int sub);
		
			///<summary>Prepares the meshobject that contains a certain pixel</summary>
			///<param name='x'>[in]The x-value of the coordinate</param>
			///<param name='y'>[in]The y-value of the coordinate</param>
			///<returns>A value determining the sucess</returns>
			bool prepSubFromCoordinate(float x,float y);
			///<summary>Prepares the meshobject that contains a certain pixel</summary>
			///<param name='sub'>[in]The sub to prepare</param>
			///<returns>A value determining the sucess</returns>
			bool prepSubFromSub(UINT sub);

			///<summary>Clamps a value between min and max</summary>
			///<param name='value'>[in]The value to clamp</param>
			///<param name='min'>[in]The min of the clamp</param>
			///<param name='max'>[in]The max of the clamp</param>
			///<returns>A value clamped between min and max</returns>
			float clamp(float value,float min,float max);

			///<summary>Transform a point from  world to local</summary>
			///<param name='x'>[in/out]The x-coordinate to transform</param>
			///<param name='z'>[in/out]The y-coordinate to transform</param>
			void transformPoint(float &x,float &z);
	};
}