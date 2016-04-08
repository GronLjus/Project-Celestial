#include "stdafx.h"
#include "ResourceLoader.h"
#include "OBJFileLoader.h"
#include "ImageFileLoader.h"
#include "TRNFileLoader.h"
#include "PlaneLoader.h"
#include "GUITextBoxLoader.h"
#include "GUILayoutLoader.h"
#include "GUIImageLoader.h"
#include "LightParticleEmitter.h"
#include "SkyboxParticle.h"
#include "PointLight.h"
#include "CelScriptBinLoader.h"
#include "CelScriptSourceLoader.h"
#include "GridLoader.h"

using namespace std;
using namespace Resources;
using namespace CrossHandlers;
using namespace CelestialMath;

int ResourceLoader::matID = 0;

void ResourceLoader::Init(Graphics::CardHandler* &card, TextContainer* outText)
{

	for (int i = 0; i<fileTexLoaders; i++)
	{

		texFileLoaders[i]->Init(card, outText);

	}

	for (int i = 0; i<fileMatLoaders; i++)
	{

		matFileLoaders[i]->Init(card, outText);

	}

	matColourLoader->Init(card);

	for (int i = 0; i<meshLoaders; i++)
	{

		objectFileLoaders[i]->Init(card, outText);

	}

	for (int i = 0; i<terrainLoaders; i++)
	{

		terrainFileLoaders[i]->Init(card, outText);

	}

	for (int i = 0; i < fileScriptLoaders; i++)
	{

		scriptFileLoaders[i]->Init(card, outText);

	}

	/*compiledScriptLoader->Init(card);
	sourceScriptLoader->Init(card);*/

}

ResourceLoader::ResourceLoader()
{

	meshLoaders = 1;
	terrainLoaders = 1;
	fileMatLoaders = 2;
	fileTexLoaders = 1;
	fileScriptLoaders = 2;
	shapeLoaders = Shape_NA;
	gUILoaders = GUIObjects_NA;

	matFileLoaders = new IFileMaterialLoader*[fileMatLoaders];
	matFileLoaders[0] = new MTLFileLoader();
	matFileLoaders[1] = new ImageFileLoader();

	texFileLoaders = new IFileTextureLoader*[fileTexLoaders];
	texFileLoaders[0] = new ImageFileLoader();

	matColourLoader = new ColourLoader();

	objectFileLoaders = new IFileMeshLoader*[meshLoaders];
	objectFileLoaders[0] = new OBJFileLoader();

	terrainFileLoaders = new IFileTerrainLoader*[terrainLoaders];
	terrainFileLoaders[0] = new TRNFileLoader();

	shapeLoads = new IShapeLoader*[shapeLoaders];
	shapeLoads[Shape_PLANE] = new PlaneLoader();
	shapeLoads[Shape_GRID] = new GridLoader();

	gUILoads = new IGUILoader*[gUILoaders];
	gUILoads[GUIObjects_TEXTBOX] = new GUITextBoxLoader();
	gUILoads[GUIObjects_LAYOUT] = new GUILayoutLoader();
	gUILoads[GUIObjects_IMAGE] = new GUIImageLoader(this);

	scriptFileLoaders = new IFileCLLoader*[fileScriptLoaders];
	scriptFileLoaders[0] = new CelScriptBinLoader();
	scriptFileLoaders[1] = new CelScriptSourceLoader();

}

int ResourceLoader::MatID()
{

	return matID++;

}

string ResourceLoader::getExtension(string file)
{

	string extension = "";
	bool stop = false;

	for(int i = file.length()-1;!stop && i>= 0;i--)
	{

		stop = file[i] == '.';

		if(!stop)
		{

			extension += file[i];

		}
	}

	string temp = extension;

	for(UINT i=0;i< extension.length();i++)
	{

		extension[i] = temp[extension.length()-1-i];

	}

	return extension;

}

MeshObject* ResourceLoader::LoadMeshFromFile(string file)
{

	bool found = false;
	string extension = getExtension(file);
	int i = 0;

	for(i=0;i<meshLoaders && !found;i++)
	{

		const string* extensions = objectFileLoaders[i]->Extension();

		for(int k=0;k<objectFileLoaders[i]->GetNrExtensions() && !found;k++)
		{
		
			found = extensions[k] == extension;

		}
	}

	if(found)
	{
	
		MeshObject* obj = objectFileLoaders[i - 1]->Load(file);
		obj->OptimizeLevels(true);
		return obj;

	}

	return nullptr;

}

ImageResourceObject** ResourceLoader::LoadBitMapesFromFile(std::string file, unsigned int &size)
{

	bool found = false;
	string extension = getExtension(file);
	int i = 0;

	for (i = 0; i<meshLoaders && !found; i++)
	{

		const string* extensions = texFileLoaders[i]->Extension();

		for (int k = 0; k<texFileLoaders[i]->GetNrExtensions() && !found; k++)
		{

			found = extensions[k] == extension;

		}
	}

	if (found)
	{

		return texFileLoaders[i - 1]->LoadBitMaps(file, size);

	}

	return nullptr;

}

MeshObject* ResourceLoader::LoadGrid(unsigned int cells, float gridSize) const
{

	MeshObject* obj = ((GridLoader*)shapeLoads[Shape_GRID])->LoadGrid(cells,gridSize);
	obj->OptimizeLevels(true);
	return obj;

}

MeshObject* ResourceLoader::LoadBasicShape(Resources::Shape shape, MeshObject::Material* material)
{

	if (material == nullptr)
	{

		material = matColourLoader->LoadMaterial(178,178,178,"DEF",1.0f,0.3f);

	}

	if(shape < Shape_NA)
	{

		MeshObject* obj = shapeLoads[shape]->LoadShape(material);
		obj->OptimizeLevels(true);
		return obj;

	}
	
	return nullptr;

}

IParticleEmitter* ResourceLoader::LoadParticleEffectFromFile(ParticleSystem system, BaseParticle base, string file)
{

	bool found = false;
	string extension = getExtension(file);
	int i = 0;

	for (i = 0; i<fileTexLoaders && !found; i++)
	{

		const string* extensions = texFileLoaders[i]->Extension();

		for (int k = 0; k<texFileLoaders[i]->GetNrExtensions() && !found; k++)
		{

			found = extensions[k] == extension;

		}
	}

	if (found)
	{

		if (system == ParticleSystem_Light)
		{

			return new LightParticleEmitter(base, false, texFileLoaders[i - 1]->LoadTexture(file));

		}

		if (system == ParticleSystem_SkyBox)
		{

			return new SkyboxParticle(base, texFileLoaders[i - 1]->LoadTexture(file));

		}
	}

	return nullptr;
}

IParticleEmitter* ResourceLoader::LoadParticleEffect(ParticleSystem system, BaseParticle base)
{
	
	if (system == ParticleSystem_Light)
	{

		return new LightParticleEmitter(base, false, nullptr);

	}

	return nullptr;

}

GUIObject* ResourceLoader::LoadGUIObject(GUIObjects type, GUISnap hor, GUISnap ver, std::string file)
{

	if (type != GUIObjects_NA)
	{

		return gUILoads[type]->Create(hor, ver, file);

	}

	return nullptr;
}

CelScriptCompiled* ResourceLoader::LoadCLScript(std::string path)
{

	CelScriptCompiled* compiled = nullptr;
	std::string  ext = getExtension(path);
	bool found = false;
	string extension = getExtension(path);
	int i = 0;

	for (i = 0; i<fileScriptLoaders && !found; i++)
	{

		const string* extensions = scriptFileLoaders[i]->Extension();

		for (int k = 0; k<scriptFileLoaders[i]->GetNrExtensions() && !found; k++)
		{

			found = extensions[k] == extension;

		}
	}

	if (found)
	{

		compiled = scriptFileLoaders[i - 1]->Load(path);

	}

	return compiled;
}

void ResourceLoader::SaveCompiledScript(string path, CelScriptCompiled* script)
{

	if (scriptFileLoaders[0] != nullptr)
	{

		((CelScriptBinLoader*)scriptFileLoaders[0])->SaveBinary(path, script);

	}
}

ILight* ResourceLoader::LoadLight(LightType light, BaseLightDesc lightDesc)
{

	ILight* lightPointer = nullptr;

	if (light != LightType_NA)
	{


		if (light == LightType_POINT)
		{

			lightPointer = new PointLight(lightDesc);

		}
	}

	return lightPointer;

}

TerrainObject* ResourceLoader::LoadTerrainFromFile(string file)
{

	bool found = false;
	string extension = getExtension(file);
	int i = 0;

	for(i=0;i<terrainLoaders && !found;i++)
	{

		const string* extensions = terrainFileLoaders[i]->Extension();

		for(int k=0;k<terrainFileLoaders[i]->GetNrExtensions() && !found;k++)
		{
		
			found = extensions[k] == extension;

		}
	}

	if(found)
	{
	
		return terrainFileLoaders[i-1]->Load(file);

	}

	return nullptr;

}

MeshObject::Material** ResourceLoader::LoadMaterialsFromFile(std::string file,int &size)
{

	bool found = false;
	string extension = getExtension(file);
	int i = 0;

	for(i=0;i<fileMatLoaders && !found;i++)
	{

		const string* extensions = matFileLoaders[i]->Extension();

		for(int k=0;k<matFileLoaders[i]->GetNrExtensions() && !found;k++)
		{
		
			found = extensions[k] == extension;

		}
	}

	if(found)
	{
	
		return matFileLoaders[i-1]->LoadMaterial(file,size);

	}

	return nullptr;
}

MeshObject::Material* ResourceLoader::LoadMaterialsFromColour(int* colours,std::string name,float df,float af)
{

	return matColourLoader->LoadMaterial(colours[0],colours[1],colours[2],name,df,af);

}

ResourceLoader::~ResourceLoader()
{

	for(int i=0;i<meshLoaders;i++)
	{

		delete objectFileLoaders[i];

	}
	
	for(int i=0;i<terrainLoaders;i++)
	{

		delete terrainFileLoaders[i];

	}
	
	for(int i=0;i<fileMatLoaders;i++)
	{

		delete matFileLoaders[i];

	}

	for (int i = 0; i<fileTexLoaders; i++)
	{

		delete texFileLoaders[i];

	}

	delete matColourLoader;

	for(int i=0;i<shapeLoaders;i++)
	{

		delete shapeLoads[i];

	}

	for (int i = 0; i<gUILoaders; i++)
	{

		delete gUILoads[i];

	}

	for (int i = 0; i<fileScriptLoaders; i++)
	{

		delete scriptFileLoaders[i];

	}

	delete[] gUILoads;
	delete[] shapeLoads;
	delete[] objectFileLoaders;
	delete[] terrainFileLoaders;
	delete[] matFileLoaders;
	delete[] texFileLoaders;
	delete[] scriptFileLoaders;

}