#pragma once
#include "../../../CelestialMath/CelestialMatrix.h"
#include "IParticleEmitter.h"

namespace Resources
{

	///<summary>This interface governs each instance of a particleemitter</summary>
	class ParticleEmitterInstance
	{

		public:
			///<param name='position'>[in]The position of the emitter in worldspace</param>
			///<param name='rotation'>[in]The rotation of the emitter</param>
			///<param name='scale'>[in]The scale of the emitter</param>
			///<param name='sizeFactor'>[in]The factor to scale the size of particles for this instance</param>
			///<param name='velocityFactor'>[in]The factor to scale the velocity by</param>
			///<param name='emitter'>[in]A pointer to the emitter this instance uses</param>
			///<param name='startTime'>[in]When the instance starts</param>
			///<param name='endTime'>[in]When the instance ends</param>
			///<param name='color'>[in]The color of this instance</param>
			///<param name='glow'>[in]The amount of the particle that will have a borderglow</param>
			///<param name='fade'>[in]Gets the glowfadefactor of this particle</param>
			ParticleEmitterInstance(CelestialMath::Vector3 &position, CelestialMath::Vector3 &rotation, CelestialMath::Vector3 &scale, float sizefactor, float velocityFactor, IParticleEmitter* emitter, float startTime, float endTime, CelestialMath::Vector4 colour, float glow, float fade);
			///<summary>Gets the transformation of the emitter in worldspace</summary>
			///<returns>The emitters position</returns>
			CelestialMath::Matrix GetTransformation() const;
			///<summary>Gets the % factor of the particles velocity in the system</summary>
			///<returns>A float representing the % of difference in each particles velocity</returns>
			float GetVelocityFactor() const;
			///<summary>Gets the % factor of the particles size in the system</summary>
			///<returns>A float representing the % of difference in each particles size</returns>
			float GetSizeFactor() const;
			///<summary>Gets the emitter this instance uses</summary>
			///<returns>A pointer to the emitter used by this instance</returns>
			IParticleEmitter* GetEmitter() const;
			///<summary>Gets when this instance started</summary>
			///<returns>The time when this instance started</returns>
			float GetStartTime() const;
			///<summary>Gets when this instance will end</summary>
			///<returns>The time when this instance ends</returns>
			float GetEndTime() const;
			///<summary>Gets the colour of this instance</summary>
			///<returns>The colour this instance uses</returns>
			CelestialMath::Vector4 GetColour() const;
			///<summary>Gets the glowfactor of this particle</summary>
			///<returns>The amount of the particle that will consist of a glow</returns>
			float GetGlow() const;
			///<summary>Gets the glowfadefactor of this particle</summary>
			///<returns>The amount of the particle that will consist of a fade in the glow</returns>
			float GetFade() const;

			///<summary>Sets the position of the instance</summary>
			///<param name='position'>[in]The position of the emitter in worldspace</param>
			void SetPosition(CelestialMath::Vector3 &position);
			///<summary>Sets the scale of the instance</summary>
			///<param name='scale'>[in]The scale of the emitter</param>
			void SetScale(CelestialMath::Vector3 &scale);
			///<summary>Sets the rotation of the emitter</summary>
			///<param name='rotation'>[in]The rotation of the emitter</param>
			void SetRotation(CelestialMath::Vector3 &rotation);

		private:
			///<summary>The emitter this instance uses</summary>
			IParticleEmitter* emitter;
			///<summary>The factor to scale the size of particles for this instance</summary>
			float sizefactor;
			///<summary>The factor to scale the velocity by</summary>
			float velocityFactor;
			///<summary>When the instance starts</summary>
			float startTime;
			///<summary>When the instance ends</summary>
			float endTime;
			///<summary>The color of this instance</summary>
			CelestialMath::Vector4 colour;
			///<summary>The amount of the particle that will have a borderglow</summary>
			float glowFactor;
			///<summary>The amount of the particles glow that will fade from to glowcolour</summary>
			float fade;

			///<summary>The position of the emitter in worldspace</summary>
			CelestialMath::Vector3 position;
			///<summary>The scale of the emitter</summary>
			CelestialMath::Vector3 scale;
			///<summary>The rotation of the emitter</summary>
			CelestialMath::Vector3 rotation;

	};
}