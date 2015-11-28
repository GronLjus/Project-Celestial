#pragma once
#include <string>
#include "IFileMeshLoader.h"
#include "IFileTerrainLoader.h"
#include "IFileMaterialLoader.h"
#include "IFileTextureLoader.h"
#include "IMaterialLoader.h"
#include "IShapeLoader.h"
#include "IGUILoader.h"
#include "IFileCLLoader.h"
#include "ResourceContainer.h"
#include "ResourceObject.h"
#include "TextContainer.h"

namespace Resources
{

	enum Shape{Shape_PLANE,Shape_NA};
	///<summary>This class manages the internal resources of the engine</summary>
	class ResourceLoader
	{

		private:

			///<summary>The unique materialId-counter</summary>
			static int matID;
			///<summary>The amount of fileloaders this manages for meshes</summary>
			int meshLoaders;
			///<summary>The amount of fileloaders this manages for terrains</summary>
			int terrainLoaders;
			///<summary>The amount of fileloaders this manages for materials</summary>
			int fileMatLoaders;
			///<summary>The amount of fileloaders this manages for textures</summary>
			int fileTexLoaders;
			///<summary>The amount of loaders this manages for shapes</summary>
			int shapeLoaders;
			///<summary>The amount of loaders this manages for GUI-Objects</summary>
			int gUILoaders;
			int fileScriptLoaders;

			///<summary>Gets the extension of a path</summary>
			///<param name='file'>[in]The name of the file to get</param>
			std::string getExtension(std::string file);
			///<summary>An array of pointers to the loaders used to load meshes</summary>
			IFileMeshLoader** objectFileLoaders;
			///<summary>An array of pointers to the loaders used to load terrains</summary>
			IFileTerrainLoader** terrainFileLoaders;
			///<summary>An array of pointer to the loaders used to load material from files</summary>
			IFileMaterialLoader** matFileLoaders;
			///<summary>An array of pointer to the loaders used to load texture from files</summary>
			IFileTextureLoader** texFileLoaders;
			///<summary>An array of pointer to the loader used to load simple colours</summary>
			IMaterialLoader* matColourLoader;
			///<summary>An array of pointers to the loaders used to load shapes</summary>
			IShapeLoader** shapeLoads;
			///<summary>An array of pointers to the loaders used to load GUI-objects</summary>
			IGUILoader** gUILoads;
			IFileCLLoader** scriptFileLoaders;


		public:
			///<summary>Gets a unique Id for materials</summary>
			///<returns>The id for the calling material</returns>
			static int MatID();
			ResourceLoader();

			TerrainObject* LoadTerrainFromFile(std::string file);

			///<summary>Loads a mesh from a file</summary>
			///<param val='file'>[in]The path of the file</param>
			///<returns>The id for the mesh</returns>
			MeshObject* LoadMeshFromFile(std::string file);
			///<summary>Loads a basic shape</summary>
			///<param val='shape'>[in]The shape to load</param>
			///<param val='material'>[in]The material to use</param>
			///<returns>The id for the mesh</returns>
			MeshObject* LoadBasicShape(Shape shape, MeshObject::Material* material);

			///<summary>Loads a particlesystem from a file</summary>
			///<param val='system'>[in]The type of system to load</param>
			///<param val='base'>[in]The basics of the particlesystem</param>
			///<param val='file'>[in]The path of the file</param>
			///<returns>The id for the system</returns>
			IParticleEmitter* LoadParticleEffectFromFile(ParticleSystem system, BaseParticle base, std::string file);
			///<summary>Loads a particlesystem</summary>
			///<param val='system'>[in]The type of system to load</param>
			///<param val='base'>[in]The basics of the particlesystem</param>
			///<returns>The id for the system</returns>
			IParticleEmitter* LoadParticleEffect(ParticleSystem system, BaseParticle base);

			///<summary>Loads a GUIObject</summary>
			///<param val='type'>[in]The type of object to load</param>
			///<param val='hor'>[in]How the object should snap horizontally</param>
			///<param val='size'>[in]The size of the object</param>
			///<returns>The id for the object</returns>
			GUIObject* LoadGUIObject(GUIObjects type, GUISnap hor, GUISnap ver);

			///<summary>Loads celscript from a file</summary>
			///<param val='path'>[in]The path of the file to load</param>
			///<returns>A pointer to the compiled script</returns>
			CelScriptCompiled* LoadCLScript(std::string path);
			///<summary>Saves a compiled script to file</summary>
			///<param val='path'>[in]The path to a file to save in</param>
			///<param val='script'>[in]A pointer to the script to save</param>
			void SaveCompiledScript(std::string path, CelScriptCompiled* script);

			///<summary>Loads a light</summary>
			///<param val='lightDesc'>[in]How the light should look</param>
			///<returns>The id for the light</returns>
			ILight* LoadLight(LightType light, BaseLightDesc lightDesc);

			MeshObject::Material** LoadMaterialsFromFile(std::string file,int &size);
			MeshObject::Material* LoadMaterialsFromColour(int* colours,std::string name,float df,float af);

			///<summary>Initializes The handler and its underobjects</summary>
			///<param name='card'>[in]The device to load from</param>
			void Init(Graphics::CardHandler* &card, TextContainer* outText);
			~ResourceLoader();

	};
}