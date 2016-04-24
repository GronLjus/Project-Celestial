#include "stdafx.h"
#include "DrawScene.h"

using namespace Graphics;
using namespace Resources;
using namespace CrossHandlers;

DrawScene::DrawScene()
{

	meshes = new CelestialList<MeshObject*>();
	lights = new CelestialList<ILight*>();
	particles = new CelestialList<IParticleEmitter*>();

	terrain = nullptr;
	terrainSubs = nullptr;

}

void DrawScene::ClearScene()
{

	meshes->Reset();
	lights->Reset();
	particles->Reset();

	terrain = nullptr;
	terrainSubs = nullptr;

}

void DrawScene::AddMesh(MeshObject* mesh)
{

	meshes->AddElement(mesh);

}

void DrawScene::AddLight(ILight* light)
{

	lights->AddElement(light);

}

void DrawScene::AddParticleSystem(IParticleEmitter* pSystem)
{

	particles->AddElement(pSystem);

}

void DrawScene::SetTerrain(TerrainMesh* terrain, CelestialList<int>* terrainSubs)
{

	this->terrain = terrain;
	this->terrainSubs = terrainSubs;

}

CelestialList<MeshObject*>* DrawScene::GetMeshes()
{

	return meshes;

}

CelestialList<ILight*>* DrawScene::GetLights()
{

	return lights;

}

CelestialList<IParticleEmitter*>* DrawScene::GetParticleSystem()
{

	return particles;

}

TerrainMesh* DrawScene::GetTerrain(CelestialList<int>* &terrainSubs)
{

	terrainSubs = this->terrainSubs;
	return terrain;

}

DrawScene::~DrawScene()
{

	delete meshes;
	delete lights;
	delete particles;

}