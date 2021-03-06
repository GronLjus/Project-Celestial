#pragma once
#include "KubLingHandler.h"
#include "GameBoardHandler.h"
#include "GraphicHandler.h"
#include "ResourceHandler.h"
#include "InputHandler.h"
#include "PlayerHandler.h"
#include "GUIEntityHandler.h"
#include "TaskHandler.h"
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
			void HandleDrawing(unsigned int time);
			///<summary>Send a message to be handled and forwarded to the right underobject</summary>
			///<param name='msg'>[in]The message chain to process</param>
			void SendMsg(CrossHandlers::Message* msg);
			CrossHandlers::Message* GetNextSystemMessage();
			void Kill();
			virtual ~Overlord();

		private:

			void updateSystemVars(unsigned int time);

			void updateMessages(MessageSource handler);
			CrossHandlers::MessageQueue* inQueue;
			CrossHandlers::MessageQueue* outQueue;
			bool die;
			bool okToDraw;
			void processCommand(std::string* command, int nrParam);
			///<summary>The quality to use for graphics</summary>
			Graphics::GraphicQuality gQ;
			///<summary>The style to use for graphics</summary>
			Graphics::DrawingStyle dS;

			Tasking::TaskHandler* tH;
			///<summary>The pointer to the GraphicHandler being used by this overlord</summary>
			Graphics::GraphicHandler* gH;
			///<summary>The pointer to the resourceHandler being used by this</summary>
			Resources::ResourceHandler* rH;
			///<summary>The pointer to the inputhandler being used by this</summary>
			Input::InputHandler* iH;
			Entities::GameBoardHandler* gBH;
			KubLingHandler* klH;
			Entities::GUIEntityHandler* guiH;
			Message firstMessage;

			///<summary>The number of basemeshes</summary>
			int meshes;
			///<summary>An array of ids to use as meshes when creating objects</summary>
			int* baseMeshes;

			///<summary>The handle of the output window</summary>
			HWND hWnd;
			///<summary>The handle of the graphicthread</summary>
			HANDLE handleOfGraphics;
			///<summary>The handle of the logicthread</summary>
			HANDLE handleOfLogic;
		
			bool killdbg;
			CrossHandlers::TextContainer* dbgOut;

			IHandleMessages** messageHandlers;
	};
}