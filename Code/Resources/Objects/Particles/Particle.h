#pragma once
#include "../../../CelestialMath/CelestialVector.h"

namespace Resources
{
	///<summary>This class encapsulates the data around each particle</summary>
	class Particle
	{

		public:
			///<param name='pos'>The position of this particle</param>
			///<param name='vel'>The velocity of his particle, per millisecond</param>
			///<param name='size'>The size of this particle</param>
			///<param name='ft'>How long before this particle starts fading, it will fade until it reaches lt</param>
			///<param name='lt'>How long this particle will travel before being reset to its original position</param>
			///<param name='tll'>How long this particle will be drawn</param>
			///<param name='st'>When the particle will get a life</param>
			Particle(CelestialMath::Vector3 pos, CelestialMath::Vector3 vel, CelestialMath::Vector3 size, float ft, float lt, float ttl, int st);
			///<summary>Gets the position of this particle</summary>
			///<returns>The particles position in objectspace around the emitter</returns>
			CelestialMath::Vector3 GetPosition() const;
			///<summary>Gets the velocity of this particle</summary>
			///<returns>The velocity this particle travels</returns>
			CelestialMath::Vector3 GetVelocity() const;
			///<summary>Gets the size of the particle</summary>
			///<returns>The particles size in worldspace</returns>
			CelestialMath::Vector3 GetSize() const;
			///<summary>Gets the fadetime of this perticle</summary>
			///<returns>How long this particle travels before fading</returns>
			float GetFadeTime() const;
			///<summary>Gets the looptime of this particle</summary>
			///<returns>How long this particle travels before going back to its startingposition</returns>
			float GetLoopTime() const;
			///<summary>Gets the time this particle will live</summary>
			///<returns>The time this particle will spend alive</returns>
			float GetTimeToLive() const;
			///<summary>Gets the time this particle will start living</summary>
			///<returns>At what time this particle will LIVE</returns>
			int GetStartTime() const;

		private:
			
			///<summary>The position of this particle</summary>
			CelestialMath::Vector3 position;
			///<summary>The velocity of his particle per millisecond</summary>
			CelestialMath::Vector3 velocity;
			///<summary>The size of this particle</summary>
			CelestialMath::Vector3 size;
			///<summary>How long before this particle starts fading, it will fade until it reaches lt</summary>
			float fadeTime;
			///<summary>How long this particle will travel before being reset to its original position</summary>
			float loopTime;
			///<summary>How long this particle will be drawn</summary>
			float timeToLive;
			///<summary>When the particle will get a life</summary>
			int startTime;

	};
}