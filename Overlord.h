#pragma once
#include "CelscriptHandler.h"
#include "GraphicHandler.h"
#include "ResourceHandler.h"
#include "InputHandler.h"
#include "PlayerHandler.h"
#include "GUIEntityHandler.h"
#include "Camera.h"
#include <condition_variable>

namespace Logic
{

	///<summary>This is the main class that handles all handlers as seperate underobjects</summary>
	class Overlord
	{

		public:

			Overlord(void);
			///<summary>Initializes the overlord as well as all underobjects</summary>
			///<param name='hwnd'>[in]The handle to the window that servers as main output</param>
			///<returns>Any error-codes</returns>
			HRESULT Init(HWND hwnd);
			///<summary>Updates the overlord along with every underhandler</summary>
			void Update(unsigned int time);
			void UpdateMessages(unsigned int time);
			void HandleDrawing(unsigned int time);
			///<summary>Send a message to be handled and forwarded to the right underobject</summary>
			///<param name='msg'>[in]The message chain to process</param>
			void SendMsg(CrossHandlers::Message* msg);

			void Kill();
			virtual ~Overlord();

		private:
			CrossHandlers::MessageQueue* inQueue;
			bool die;
			bool okToDraw;
			void processCommand(std::string* command, int nrParam);
			///<summary>The quality to use for graphics</summary>
			Graphics::GraphicQuality gQ;
			///<summary>The style to use for graphics</summary>
			Graphics::DrawingStyle dS;

			///<summary>The pointer to the camera to use</summary>
			CrossHandlers::Camera* cam;
			
			///<summary>The pointer to the GraphicHandler being used by this overlord</summary>
			Graphics::GraphicHandler* gH;
			///<summary>The pointer to the resourceHandler being used by this</summary>
			Resources::ResourceHandler* rH;
			///<summary>The pointer to the inputhandler being used by this</summary>
			Input::InputHandler* iH;
			///<summary>The pointer to handler handling any player-user interactions</summary>
			PlayerHandler* pH;

			CelscriptHandler* cH;
			Entities::GUIEntityHandler* guiH;
			Message firstMessage;
			Message lastMessage;

			///<summary>The number of basemeshes</summary>
			int meshes;
			///<summary>An array of ids to use as meshes when creating objects</summary>
			int* baseMeshes;

			///<summary>The scene to use</summary>
			LogicScene* scene;
		
			///<summary>The handle of the output window</summary>
			HWND hWnd;
			///<summary>The handle of the graphicthread</summary>
			HANDLE handleOfGraphics;
			///<summary>The handle of the logicthread</summary>
			HANDLE handleOfLogic;
		
			bool isDrawingBool;
			std::mutex isDrawing;
			std::condition_variable handleMessages;
			bool waitForMessages;
			Resources::GUITextBox* dbgIn;
			Resources::GUITextBox* dbgOut;
			Resources::GUITextBox* dbgPnl;

			IHandleMessages** messageHandlers;
	};
}