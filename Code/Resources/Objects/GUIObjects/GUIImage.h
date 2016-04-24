#include "GUIObject.h"
#include "../../../CrossHandler-Tools/Image/ImageResourceObject.h"

namespace Resources
{

	class GUIImage : public GUIObject
	{

		private:
			ImageResourceObject* drawFrame;
			ImageResourceObject** frames;
			unsigned int totalFrames;
			unsigned int currentFrame;
			unsigned int lastTime;

		public:
			GUIImage(GUISnap h, GUISnap v);
			GUIImage(ImageResourceObject** frames, unsigned int totalFrames, GUISnap h, GUISnap v);
			virtual void Update(CrossHandlers::Message* mess);
			ImageResourceObject* GetCurrentFrame() const;
			ImageResourceObject* GetDrawFrame();
			void SetDrawFrame(ImageResourceObject* frame);
			~GUIImage();
	};
};