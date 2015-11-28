#pragma once

#include "CardHandler.h"
#include "IGraphicCamera.h"
#include "IGraphicDebug.h"
#include "LogicScene.h"
#include "GUILayout.h"
#include "IHandleMessages.h"

namespace Graphics
{

	///<summary>This class handles all graphic-related updating</summary>
	class GraphicHandler : public IGraphicDebug, public CrossHandlers::IHandleMessages
	{
		public:
			GraphicHandler(void);
			///<summary>Initilizes part of the graphichandler so we can output basic text</summary>
			///<param name='hwnd'>[in]The handle to the output window</param>
			///<param val='initQuality'>[in]The initial quality to use/</param>
			///<param val='dStyle'>[in]The initial style to use</param>
			HRESULT PreInit(HWND hwnd, GraphicQuality initQuality, DrawingStyle dStyle);
			///<summary>Initilizes the rest of the graphichandler so we can output complicated drawings</summary>
			///<param name='camera'>[in]A pointer to the camera used by this and its underobject</param>
			///<param val='errorOut'>[in]A pointer to a textcontainer to use for debugging</param>
			HRESULT FullInit(IGraphicCamera* camera, CrossHandlers::TextContainer* errorOut);

			///<summary>Updates the handler and prepares to draw</summary>
			void Update(unsigned int time);
			///<summary>Draws out the scene</summary>
			void Draw();
			///<summary>Sets the camera the handler as well as underobjects ues</summary>
			///<param name='camera'>[in]A pointer to the new camera to be used</param>
			void SetCamera(IGraphicCamera* camera);

			///<summary>Sets the scene</summary>
			///<param val='scene'>[in]scene</param>
			void SetScene(Logic::LogicScene* scene);
			///<summary>Adds a layout to draw</summary>
			///<param val='layout'>[in]A pointer to the layout to draw</param>
			void AddLayout(Resources::GUILayout* layout);
			///<summary>Clears the layouts</summary>
			void ClearLayouts();

			///<summary>Check if the handler is initilized</summary>
			///<returns>If the handler is initilized</returns>
			bool GetIsInited() const;

			///<summary>Updates the quality</summary>
			///<param val='gQ'>The quality to use/</param>
			void UpdateQuality(GraphicQuality gQ);
			///<summary>Updates the style</summary>
			///<param val='dS'>The new style to use/</param>
			void UpdateStyle(DrawingStyle dS);
			///<summary>Gets the quality in use</summary>
			///<returns>The quality in use</returns>
			GraphicQuality GetQuality();
			///<summary>Gets the style in use</summary>
			///<returns>The style in use</returns>
			DrawingStyle GetStyle();
	
			virtual void ToggleWireFrame(bool enabled);
			virtual void ToggleGrid(bool enabled);
			virtual void ToggleDebug(bool enabled);
			virtual void ToggleNSpikes(bool enabled);
			virtual void ToggleGlobalBorders(bool val);
			virtual void SetCommandText(std::string text);

			///<summary>Releases the resources used by this handler and underobjects, must be called at the end of its lifetime</summary>
			void Release();
			///<summary>Gets the underlying cardhandler</summary>
			///<returns>A pointer to the cardhandler</returns>
			CardHandler* GetCardHandler();
			///<summary>Signals that the time has come to join the choir invisible</summary>
			void Kill();

			virtual ~GraphicHandler(void);

		private:
			///<summary>What to draw</summary>
			DrawingStyle dStyle;
			///<summary>The quality to use when drawing</summary>
			GraphicQuality quality;
			///<summary>The drawing of wireframes</summary>
			bool wf;
			///<summary>If debugtext should be drawn</summary>
			bool debug;

			///<summary>The internal cardhandler</summary>
			CardHandler* cardHandler;
			///<summary>The interface to use when debugging the card</summary>
			ICardDebug* debugCard;
			///<summary>The intermediator to use when adding instances</summary>
			Intermediator* inter;
			///<summary>A pointer to the camera being used</summary>
			IGraphicCamera* camera;
			///<summary>If the handler is preinited</summary>
			bool isPreInited;
			///<summary>If the handler is inited</summary>
			bool isInited;
			///<summary>How the handler is reading objects</summary>
			int isReadingObjects;

			///<summary>The logic information of the scene</summary>
			Logic::LogicScene* logicScene;
			///<summary>The graphic information of the scene</summary>
			DrawScene* graphicScene;
			///<summary>A list with the guis to draw</summary>
			CrossHandlers::CelestialList<Resources::GUILayout*>* guiLayouts;

			///<summary>An array of cameraframes</summary>
			CrossHandlers::CameraFrame** cameras;

			///<summary>The number of renders</summary>
			int renderCount;

			///<summary>If the handler can or should draw out to the screen</summary>
			bool canDraw;
			///<summary>If the handler is drawing</summary>
			bool isDrawing;

			///<summary>The current renderdata to read from</summary>
			int renderFlip;
			///<summary>The total amount of objects in renderdata</summary>
			int renderFlips;
			///<summary>The flip that is being read right now</summary>
			int readingFlip;
			///<summary>The flip that is being written to</summary>
			int writeFlip;
			///<summary>If the handler has chosen which flip to write to</summary>
			bool hasChosenWriting;
			///<summary>The debugcommand to draw</summary>
			std::string command;
	};
}