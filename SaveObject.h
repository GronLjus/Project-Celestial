#pragma once
#include "Message.h"

namespace Resources
{

	class SaveObject
	{

		private:
			bool save;

		public:
			bool ShouldSave() const;

		protected:
			SaveObject();
			void setSaveState(bool save);
			virtual bool UpdateSaveObject(CrossHandlers::Message* mess);
			virtual ~SaveObject() {}
	};
}