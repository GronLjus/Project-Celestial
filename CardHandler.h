#pragma once
#include "ResourceObject.h"
#include "CelestialShader.h"
#include "I2DDrawer.h"
#include "ICardDebug.h"
#include "Intermediator.h"
#include "CelestialBufferHandler.h"

namespace Graphics
{

	///<summary>This class encapsulates all functions required to handle DirectX</summary>
	class CardHandler : public ICardDebug
	{
		public:

			///<param name='flips'>[in]The number of flips used by the graphichandler</param>
			///<param name='useText'>[in]If the handler should be able te draw text</param>
			CardHandler(int flips, bool useText);
			///<summary>Initilizes the Card with textrendering and textureloading, must be called before any attempts are made to draw or clear</summary>
			///<param name='hwnd'>[in]The handle to the window to use as output</param>
			///<param val='gQ'>[in]The quality to use/</param>
			///<param val='dS'>[in]The drawingstyle to use</param>
			///<returns>Any errors</returns>
			HRESULT Init(HWND hwnd, GraphicQuality gQ, DrawingStyle dS);
			///<summary>Initilizes the cards underobject enabling advanced drawing</summary>
			///<param val='errorOut'>[in]A pointer to a textcontainer to use for debugging</param>
			///<returns>Any errors</returns>
			HRESULT InitShader(CrossHandlers::TextContainer* errorOut);

			///<summary>Gets the intermediator of the cardhandler</summary>
			///<returns>A pointer to the intermediato this cardhandler uses</returns>
			Intermediator* GetIntermediator();

			void UpdateMeshBuffers(Entities::DrawingBoard* db);
			void UpdateInstanceBuffers(Entities::DrawingBoard* db,unsigned int flip);

			///<summary>Draws the meshes</summary>
			///<param name='scene'>[in]A pointer to the scene to draw</param>
			void Draw(DrawScene* scene,int flip);
			///<summary>Draws a ui</summary>
			///<param val='ui'>[in]A pointer to the objects to draw</param>
			void Draw(CrossHandlers::CelestialList< Resources::GUIObject*>* objects);

			///<summary>Draws text on screen</summary>
			///<param name='text'>[in]The text to draw</param>
			///<param name='x'>[in]The coordinate of the upper left corner[0,1]</param>
			///<param name='y'>[in]The coordinate of the upper left corner[0,1]</param>
			void DrawTextOnScreen(std::string text,float x,float y);
			///<summary>Presents everything the card has drawn</summary>
			void Present();

			///<summary>Loads a texture from a file with the underlying card</summary>
			///<param name='values'>[in]The values of the image</param>
			///<param name='channelOrder'>[in]The order of the channels</param>
			///<param name='channels'>[in]The number of channels</param>
			///<param name='bPP'>[in]The number of bits per channel</param>
			///<param name='width'>[in]The width of the texture</param>
			///<param name='height'>[in]The height of the texture</param>
			///<returns>A pointer to the loaded resource///</returns>
			CrossHandlers::TextureResourceObject* LoadTexture(byte* values, UINT bPC, UINT channels, char* channelOrder, UINT width, UINT height);
			///<summary>Creates a 2x2texture from the underlying card</summary>
			///<param name='resource'>[out]</param>
			///<param name='r'>[in]The red of the colour</param> 
			///<param name='g'>[in]The green of the colour</param> 
			///<param name='b'>[in]The blue of the colour</param> 
			///<returns>A pointer to the loaded resource</returns>
			CrossHandlers::TextureResourceObject* CreateTexture(UCHAR r, UCHAR g, UCHAR b);
	
			///<summary>Sets the camera used y this card and underlying objects</summary>
			///<param name='cam'>[in]A pointer to the camera that should be used</param>
			void SetCamera(CrossHandlers::CameraFrame* cam);

			///<summary>Updates the quality</summary>
			///<param val='gQ'>The quality to use/</param>
			void UpdateQuality(GraphicQuality gQ);
			///<summary>Updates the drawingstyle</summary>
			///<param val='dS'>The style to use/</param>
			void UpdateStyle(DrawingStyle dS);

			virtual void ToggleWireFrame(bool val);
			virtual void ToggleGridding(bool enabled);
			virtual void ToggleNormalSpikes(bool val);
			virtual void ToggleGlobalBorders(bool val);

			///<summary>Releases all resources hold by this object and it's underobjects, must be called at the end of the objects' lifetime</summary>
			void Release();

			virtual void SetOut(RT target);
			~CardHandler(void);

		private:
			///<summary>If the underlying objects ara initiated</summary>
			bool underInitiated;

			///<summary>The total amount of flips for the card to use</summary>
			int totalFlips;

			///<summary>If this object should draw a grid along the floor</summary>
			bool shouldGrid;
			///<summary>If this object should draw meshes wireframe</summary>
			bool wireFrame;

			///<summary>The quality to use</summary>
			GraphicQuality quality;
			///<summary>The drawstyle to use</summary>
			DrawingStyle dStyle;

			///<summary>The number of flips used</summary>
			int flip;

			///<summary>initiate the backbuffer of the card</summary>
			HRESULT initBackBuffer();
			///<summary>Releases the backbuffer</summary>
			HRESULT releaseBackBuffer();

			///<summary>The backbuffer of the card</summary>
			CrossHandlers::TextureResourceObject* backBuffer;

			///<summary>A pointer to the bufferhandler for this object</summary>
			CelestialBufferHandler* bH;
			///<summary>The internal shader used by this object to draw out</summary>
			CelestialShader* shader;

			///<summary>The underlying card</summary>
			ID3D10Device1* card;
			///<summary>The drivertype the device uses</summary>
			D3D10_DRIVER_TYPE driverType;

			///<summary>The intermediator to the card</summary>
			Intermediator* inter;

			///<summary>The swapchain used by this card</summary>
			IDXGISwapChain* swapChain;

			///<summary>A pointer to the camera used by this objects underobjects</summary>
			CrossHandlers::CameraFrame* camera;
	
			///<summary>The current frame</summary>
			ID3D10Resource* currentFrame;
			ID3D10ShaderResourceView* currentFramwView;

			///<summary>The internal texthandler</summary>
			I2DDrawer* basic2DHandler;
			///<summary>The index of the font used for debugtext</summary>
			int debugText;
			///<summary>The index of the brush used for debugtext</summary>
			int debugBrush;
			///<summary>The number of times debugtext has been drawn</summary>
			int dtd;

			///<summary>Which rendertarget to draw out to screen</summary>
			RT out;

	};
}