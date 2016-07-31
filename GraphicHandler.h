#pragma once

#include "CardHandler.h"
#include "IGraphicCamera.h"
#include "IGraphicDebug.h"
#include "LogicScene.h"
#include "GUILayout.h"
#include "IHandleMessages.h"
#include "GameBoard.h"

namespace Graphics
{

	///<summary>This class handles all graphic-related updating</summary>
	class GraphicHandler : public IGraphicDebug, public CrossHandlers::IHandleMessages
	{
		public:
			GraphicHandler(unsigned int flips);
			///<summary>Initilizes part of the graphichandler so we can output basic text</summary>
			///<param name='hwnd'>[in]The handle to the output window</param>
			///<param val='initQuality'>[in]The initial quality to use/</param>
			///<param val='dStyle'>[in]The initial style to use</param>
			HRESULT PreInit(HWND hwnd, GraphicQuality initQuality, DrawingStyle dStyle, CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* gameObjects);
			///<summary>Initilizes the rest of the graphichandler so we can output complicated drawings</summary>
			///<param val='errorOut'>[in]A pointer to a textcontainer to use for debugging</param>
			HRESULT FullInit(CrossHandlers::TextContainer* errorOut, unsigned int maxInstances);

			///<summary>Updates the handler and prepares to draw</summary>
			void Update(unsigned int time);
			///<summary>Draws out the scene</summary>
			void Draw(unsigned int time);

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

			void Toggle3D();
	
			virtual void ToggleWireFrame(bool enabled);
			virtual void ToggleGrid(bool enabled);
			virtual void ToggleDebug(bool enabled);
			virtual void ToggleNSpikes(bool enabled);
			virtual void ToggleGlobalBorders(bool val);
			virtual void SetCommandText(std::string text);

			unsigned int GetRenderFlip() const;
			///<summary>Releases the resources used by this handler and underobjects, must be called at the end of its lifetime</summary>
			void Release();
			///<summary>Gets the underlying cardhandler</summary>
			///<returns>A pointer to the cardhandler</returns>
			CardHandler* GetCardHandler();
			///<summary>Signals that the time has come to join the choir invisible</summary>
			void Kill();

			virtual ~GraphicHandler(void);

		private:
			void pauseRendering();
			void resumeRendering();

			bool stoppedDrawing;
			bool stopDrawing;

			std::mutex renderLock;

			Resources::GameBoard* gameBoard;
			CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* gameObjects;
			Resources::CameraObject* cameraObject;

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
			///<summary>If the handler is preinited</summary>
			bool isPreInited;
			///<summary>If the handler is inited</summary>
			bool isInited;

			Resources::GUILayout* guiLayout;

			///<summary>The number of renders</summary>
			int renderCount;

			///<summary>If the handler can or should draw out to the screen</summary>
			bool canDraw;
			///<summary>If the handler is drawing</summary>
			bool isDrawing;

			unsigned int renderFlip;
			unsigned int nextFlip;
			///<summary>If the handler has chosen which flip to write to</summary>
			bool hasChosenWriting;
			///<summary>The debugcommand to draw</summary>
			std::string command;
	};
}