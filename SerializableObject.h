#pragma once

namespace Resources
{

	enum SerializableType {
		SerializableType_NA,
		SerializableType_SCRIPTABLE,
		SerializableType_POSITIONABLE,
		SerializableType_GAMEOBJECTSCENERY,
		SerializableType_GAMEOBJECTROUTE,
		SerializableType_GAMEOBJECTTRAVEL,
		SerializableType_GAMEBOARD,
		
		SerializableType_ROUTEMANAGER,

		SerializableType_GUIOBJECT,
		SerializableType_GUILAYOUT,
		SerializableType_GUITEXTBOX,
		SerializableType_GUIIMAGE,

		SerializableType_RESOURCEDICTIONARY,
		SerializableType_ROUTE,
		SerializableType_TASK

	};

	class SerializableObject
	{

		public:
			SerializableObject()
			{}
			virtual char* Serialize(unsigned int &size) = 0;
			virtual char* Unserialize(char* data) = 0;


	};
}