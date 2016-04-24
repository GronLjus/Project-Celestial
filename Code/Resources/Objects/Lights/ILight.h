#pragma once
#include "../../../CrossHandler-Tools/Frustum.h"
#include "../BaseObject.h"

namespace Resources
{

	enum LightType{LightType_POINT,LightType_NA};

	struct BaseLightDesc
	{

		public:
			///<summary>The specularity of this light</summary>
			CelestialMath::Vector4 spec;
			///<summary>The diffuse of this light</summary>
			CelestialMath::Vector4 diff;
			///<summary>The lights intensity</summary>
			float intensity;
			///<summary>The size of the point</summary>
			float size;
	
	};

	///<summary>This interface contains all neceseary functions to abstract the usage of lights. Every light has a number of subs denonting how many shadowmaps/views this light has</summary>
	class ILight : public BaseObject
	{

		public:
		
			///<summary>Gets the Specular colour of the light</summary>
			///<returns>A colour representing the specularity of this light</returns>
			virtual CelestialMath::Vector4 GetSpec() const = 0;
			///<summary>Gets the Diffuse colour of the light</summary>
			///<returns>A colour representing the diffuse of this light</returns>
			virtual CelestialMath::Vector4 GetDiff() const = 0;
			///<summary>Gets the position of the light</summary>
			///<returns>Where in worldspace the light is</returns>
			virtual CelestialMath::Vector3 GetPos() const = 0;
			///<summary>Gets the forward-direction of where the light is pointing</summary>
			///<returns>The direction of the light</returns>
			virtual CelestialMath::Vector3 GetDirection() const = 0;
			///<summary>Gets the sideway-direction of where the light is pointing</summary>
			///<returns>The direction of the light</returns>
			virtual CelestialMath::Vector3 GetSide() const = 0;
			///<summary>Gets the size of the lights emitter</summary>
			///<returns>The size of the light in worldspace</returns>
			virtual float GetSize() const = 0;

			///<summary>Gets the angle between the midpoint and the lightcone</summary>
			virtual float GetAngle() const = 0;
			///<summary>Gets the lights intensity</summary>
			///<returns>A value meaning the lights intensity</returns>
			virtual float GetIntensity() const = 0;
			///<summary>Gets what type of light this light is</summary>
			///<returns>What sort of light this is meaning which shader-pass should be use to light</returns>
			virtual LightType GetType() const = 0;

			///<summary>Sets the lights specularity</summary>
			///<param name='col'>[in]The new colour</param>
			virtual void SetSpec(CelestialMath::Vector4 col) = 0;
			///<summary>Sets the lights diffuse</summary>
			///<param name='col'>[in]The new colour</param>
			virtual void SetDiff(CelestialMath::Vector4 col) = 0;

			///<summary>sets the lights new position and intensity</summary>
			///<param name='pos'>[in]The new position of the light</param>
			///<param name='intens'>[in]The new intensity</param>
			virtual void SetPosIntensity(CelestialMath::Vector3 &pos, float intens) = 0;
			///<summary>sets the lights new position</summary>
			///<param name='pos'>The new position of the light</param>
			virtual void SetPos(CelestialMath::Vector3 &pos) = 0;

			///<summary>Points the light in a direction</summary>
			///<param name='forward'>[in]The normalized direction for the light to point in</param>
			///<param name='roll'>[in]The degrees to rotate around the z-axis of the light</param>
			virtual void Point(CelestialMath::Vector3 forward, float roll) = 0;

			///<summary>Gets the volume this light fills</summary>
			///<returns>A pointer to the bounding object associated with this light</returns>
			virtual CrossHandlers::IBounding* GetBoundingVolume() = 0;

			virtual ~ILight(){}

	};

}