#pragma once
#include "ShaderContainer.h"
#include "CelestialMath.h"
#include "CameraFrame.h"
#include "ShadowMapConstants.h"
#include "DrawScene.h"
#include "CelestialBufferHandler.h"
#include "TextContainer.h"
#include "ViewObject.h"
#include "GraphicalMesh.h"
#include <string>

using namespace CelestialMath;

namespace Graphics
{

	///<summary>This class encapsulates the "shaders.hlsl" file</summary>
	class CelestialShader
	{
	
		public:
			CelestialShader();

			HRESULT Init(ID3D10Device1* card, GraphicQuality gQ, DrawingStyle dS, CrossHandlers::TextureResourceObject* backBuffer,CrossHandlers::TextContainer* errorOut);

			void SetVertexBuffers(ID3D10Buffer* vertices, ID3D10Buffer* indices);
			void ToggleWireFrameMode(bool val,bool light);
			void ToggleNormalSpikes(bool val);
			void ToggleGlobalBorders(bool val);

			bool SetQuality(GraphicQuality gQ, CrossHandlers::TextureResourceObject* backBuffer);
			bool SetDrawing(DrawingStyle dS);

			void NothingToDraw();
			void StartDrawing();
			void StartDrawing(Entities::ViewObject* scene, unsigned int flip);
			void DrawScene(Entities::ViewObject* scene, GraphicalMesh* meshes, unsigned int flip);
			void DrawScene(Graphics::DrawScene* scene, int flip);
			void FinishDrawing();

			void SetCamera(CrossHandlers::CameraFrame* cam);
			CelestialBufferHandler* GetBufferHandler();

			///<summary>Releases all references used by the shader, must be called when the shaders usage is over</summary>
			void Release();

			~CelestialShader();

		private:
			
			enum Technique{ Technique_PREDRAW, Technique_TERRAIN, Technique_GEOMETRY, Technique_SHADOW, Technique_LIGHT, Technique_SCREEN, Technique_PARTICLE, Technique_DEBUG, Technique_SIZE };
			enum Type{Type_SOLIDS,Type_WIREFRAME,Type_SIZE};
			enum PreDrawCode{PreDrawCode_SIZE};
			enum GeometryCode{GeometryCode_GEO,GeometryCode_GEOBORDERS,GeometryCode_SIZE};
			enum ShadowCode{ ShadowCode_TRNMAP, ShadowCode_TRNVOLUME, ShadowCode_GEOMAP, ShadowCode_GEOVOLUME, ShadowCode_GEOUMBRAVOLUME, ShadowCode_GEOPENUMBRAVOLUME, ShadowCode_GEOSSVOLUME, ShadowCode_SSPCSSMAJOR, ShadowCode_SIZE };
			enum LightCode{ LightCode_POINTNOSHADOWS, LightCode_POINTSHADOWMAPPING, LightCode_POINTUMBRASHADOWMAPPING, LightCode_POINTSSPCSS, LightCode_SIZE };
			enum ParticleCode{ ParticleCode_LIGHT, ParticleCode_SKYBOX, ParticleCode_SIZE };
			enum ScreenCode{ScreenCode_MOTIONBLUR,ScreenCode_GRID,ScreenCode_SIZE};
			enum DebugCode{DebugCode_GNSPIKES,DebugCode_TNSPIKES,DebugCode_SIZE};

			enum BlendCode{BlendCode_OPAQUE,BlendCode_ADD,BlendCode_ALPHA,BlendCode_SIZE};
			enum DepthCode{ DepthCode_RW, DepthCode_ZFAILSINGLE, DepthCode_ZFAILREVERSE, DepthCode_STCHCK, DepthCode_LRGCHCK, DepthCode_SIZE };
			enum RastState{ RastState_BACKCULL, RastState_FRONTCULL, RastState_NOCULL, RastState_WIREFRAME, RastState_SIZE };
			enum SampleState{SampleState_SINGLE,SampleState_ARRAY,SampleState_COMPARRAY,SampleState_SIZE};

			enum SRVRegisters{ SRVRegisters_AMB, SRVRegisters_DIFF, SRVRegisters_NORM, SRVRegisters_DEPTH, SRVRegisters_SHADOWMAP, SRVRegisters_SHADOWSTENCIL, SRVRegisters_SHADOWINFO, SRVRegisters_PARTMAT, SRVRegisters_SCREEN, SRVRegisters_VELOCITY, SRVRegisters_SIZE };
			enum BufferRegisters{BufferRegisters_FRAME, BufferRegisters_RENDER, BufferRegisters_LIGHT, BufferRegisters_SIZE};
			enum SamplerRegisters{SamplerRegisters_SAMPLER, SamplerRegisters_SIZE};

			enum VertexShaders{VertexShaders_VSPREDRAW,
				VertexShaders_VS,VertexShaders_VSTERRAIN,
				VertexShaders_VSSHADOW,
				VertexShaders_VSTERRAINSHADOW,
				VertexShaders_VSLIGHT,
				VertexShaders_VSSCREEN,
				VertexShaders_VSPARTICLE,
				VertexShaders_SIZE};

			enum GeometryShaders{
				GeometryShaders_GSSKYCUBE,
				GeometryShaders_GSTriangles,
				GeometryShaders_GSTrianglesBorders,
				GeometryShaders_GSSHADOW,
				GeometryShaders_GSSHADOWVolumes, 
				GeometryShaders_GSSHADOWUmbraVolume,
				GeometryShaders_GSSHADOWPenUmbraVolume,
				GeometryShaders_GSSHADOWSSVolume,
				GeometryShaders_GSLIGHT,
				GeometryShaders_GSSCREEN,
				GeometryShaders_GSPARTICLE,
				GeometryShaders_GSNORMALSPIKE,
				GeometryShaders_SIZE};

			enum PixelShaders{ PixelShaders_SKYBOX, 
				PixelShaders_MAPPER, 
				PixelShaders_SHADOWMAPPER,
				PixelShaders_SHADOWDBG,
				PixelShaders_POINTLIGHTER, 
				PixelShaders_POINTLIGHTERSM, 
				PixelShaders_POINTLIGHTERUMBRASM, 
				PixelShaders_POINTLIGHTERSSPCSS,
				PixelShaders_SSVDEPTH,
				PixelShaders_SSGAUSSMAJOR,
				PixelShaders_SSGAUSSMINOR,
				PixelShaders_MOTIONBLUR, 
				PixelShaders_GRID, 
				PixelShaders_LIGHTPARTICLE, 
				PixelShaders_DEBUGOUT, 
				PixelShaders_SIZE };
			
			enum MRTVal{ MRTVal_AMB, MRTVal_DIFF, MRTVal_NORM, MRTVal_VEL, MRTVal_OLD, MRTVal_DEPTH, MRTVal_SIZE };
			enum MRTPlaces{ MRTPlaces_TargetStart = 0, MRTPlaces_TargetEnd = 5, MRTPlaces_DepthStart = 5, MRTPlaces_DepthEnd = 7 };

			///<summary>Draws the terrain to the screen</summary>
			///<param name='terrain'>[in]A pointer to the terrain to draw</param>
			///<param name='sub'>[in]The submesh to draw</param>
			HRESULT initShaders(CrossHandlers::TextContainer* errorOut);
			HRESULT initBuffers(CrossHandlers::TextContainer* errorOut);
			HRESULT initStates(CrossHandlers::TextContainer* errorOut);
			HRESULT initTargets(GraphicQuality gQ, CrossHandlers::TextureResourceObject* backBuffer);
			HRESULT initDepth(GraphicQuality gQ);
			HRESULT initShadowmap(GraphicQuality gQ);
			void initTechniques();

			HRESULT releaseTargets();
			HRESULT releaseDepth();
			HRESULT releaseShadowMap();

			void transferRenderConstants();
			void transferFrameConstants();
			void transferLightConstants();
		
			///<summary>Wireframe-toogle</summary>
			bool wf;
			///<summary>Light-render-toogle</summary>
			bool light;
			///<summary>If the shader should draw global borders</summary>
			bool globalBorders;

			///<summary>If the shader should draw out spikes for normals</summary>
			bool normalSpikes;

			///<summary>The sum of the temp-array</summary>
			int bufferSize;
			///<summary>An array of Bufferpointers to use when setting vertex/instance-buffers</summary>
			ID3D10Buffer** buffers;
			///<summary>An array of ints to use as strides when setting buffers</summary>
			UINT* strides;
			///<summary>An array of ints to use as offsets when setting buffers</summary>
			UINT* offSets;

			///<summary>An array of shaderresourceviews to use when setting textures</summary> 
			ID3D10ShaderResourceView** srvs;

			///<summary>The device used to create the effects</summary>
			ID3D10Device1* card;
			///<summary>The Bufferhandler this shader uses</summary>
			CelestialBufferHandler* bH;
			///<summary>The ShadowMapConstants this shader uses</summary>
			ShadowMapConstants* sMC;

			///<summary>The stride for the buffer used when drawing the screen</summary>
			UINT screenStride;
			///<summary>The layout for the buffer used when drawing the screen</summary>
			ID3D10InputLayout* screenLayout;

			///<summary>The underlying effect this file uses</summary>
			ID3D10Blob* compiledShader;
		
			///<summary>A pointer to the camera to be used</summary>
			CrossHandlers::CameraFrame* camera;

			///<summary>Releases all the the mtrs</summary>
			///<returns>Errorcodes</returns>
			HRESULT releaseMTRs();

			///<summary>An array of pointers to the mtrs</summary>
			CrossHandlers::TextureResourceObject** mtrs;

			CrossHandlers::TextureResourceObject* dpthStnc;
			CrossHandlers::TextureResourceObject* shadowMap;
			CrossHandlers::TextureResourceObject* shadowInfo;

			CrossHandlers::TextureResourceObject* shadowFactor;
			CrossHandlers::TextureResourceObject* shadowFactor2;

			GraphicQuality quality;
			DrawingStyle dStyle;

			ID3D10BlendState** blendStates;
			ID3D10DepthStencilState** depthStates;
			ID3D10RasterizerState** rastStates;
			ID3D10SamplerState** sampleStates;

			ShaderContainer*** techs;

			ID3D10VertexShader** vertexShaders;
			ID3D10GeometryShader** geometryShaders;
			ID3D10PixelShader** pixelShaders;

			struct renderConstants
			{

				bool UseNormal[16];

			};

			struct frameConstants
			{
	

				Matrix View;
				Matrix Projection;
				Matrix ViewProjection;
				Matrix InvertViewProjection;
				Matrix LastViewProjection;
				
				Vector4 DebugCol;
				Vector4 GlobalAmbience;
				Vector4 CameraPos;
				Vector4 GlobalLight;
				Vector4 GlobalDirection;

				float BlurSamples;
				int FrameTime;
				float BorderThickness;
				float ZNear;

				float FOV;
				bool Volumes;
				float pad[2];
			};

			struct perLightConstants
			{
	
				float LightFalloff;
				float InvRenderWidth;
				float InvRenderHeight;	
				float BlockerSearch;
				float ShadowSearch;

				Vector3 Position;
				Vector3 Forward;
				float Radius;
				Vector3 Right;
				float Angle;
				Vector4 Diffuse;
				Vector4 Specular;
				float Size;
				float pad[3];

				Vector4 RenderCenter[6];

			};

			renderConstants rc;
			frameConstants fc;
			perLightConstants plc;

			ID3D10Buffer* rcb;
			ID3D10Buffer* fcb;
			ID3D10Buffer* plcb;

	};
}