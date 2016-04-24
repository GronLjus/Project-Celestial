#pragma once

#include "CelestialList.h"
#include "MeshObject.h"
#include "TerrainMesh.h"
#include "ILight.h"
#include "IParticleEmitter.h"

namespace Graphics
{
	///<summary>A collection of objects to draw</summary>
	class DrawScene
	{

		public:
			DrawScene();
			///<summary>Clears the scene</summary>
			void ClearScene();

			///<summary>Adds a mesh to be drawn in the scene</summary>
			///<param val='mesh'>[in]A pointer to the mesh</param>
			void AddMesh(Resources::MeshObject* mesh);
			///<summary>Adds a light to be drawn in the scene</summary>
			///<param val='light'>[in]A pointer to the light</param>
			void AddLight(Resources::ILight* light);
			///<summary>Adds a particlesystem to be drawn in the scene</summary>
			///<param val='pSys'>[in]A pointer to the particlesystem</param>
			void AddParticleSystem(Resources::IParticleEmitter* pSys);
			///<summary>Sets the terrain used by the scene</summary>
			///<param val='terrain'>[in]A pointer to the terrain for the scene to use</param>
			///<param val='terrainSubs'>A pointer to the first element in a list of terrainsubs to draw</param>
			void SetTerrain(Resources::TerrainMesh* terrain, CrossHandlers::CelestialList<int>* terrainSubs);

			///<summary>Gets the meshes to draw</summary>
			///<returns>A pointer to the list of meshes to draw</returns>
			CrossHandlers::CelestialList<Resources::MeshObject*>* GetMeshes();
			///<summary>Gets a list of lights to draw</summary>
			///<returns>A pointer to the list of lights to draw</returns>
			CrossHandlers::CelestialList<Resources::ILight*>* GetLights();
			///<summary>Gets a list of particlesystem to draw</summary>
			///<returns>A pointer to the list of particlesystems to draw</returns>
			CrossHandlers::CelestialList<Resources::IParticleEmitter*>* GetParticleSystem();
			///<summary>Gets the terrain to draw</summary>
			///<param val='terrainSubs'>[out]A pointer to the list of terrainsubs</param>
			///<returns>A pointer to the terrain to draw</returns>
			Resources::TerrainMesh* GetTerrain(CrossHandlers::CelestialList<int>* &terrainSubs);
			
			~DrawScene();

		private:
			///<summary>The meshes in the scene</summary>
			CrossHandlers::CelestialList<Resources::MeshObject*>* meshes;
			///<summary>The lights in the scene</summary>
			CrossHandlers::CelestialList<Resources::ILight*>* lights;
			///<summary>The particlesystems in the scene</summary>
			CrossHandlers::CelestialList<Resources::IParticleEmitter*>* particles;
			///<summary>The terrain in the scene</summary>
			Resources::TerrainMesh* terrain;
			///<summary>A pointer to the first element in a list of terrainsubs to draw</summary>
			CrossHandlers::CelestialList<int>* terrainSubs;

	};
}