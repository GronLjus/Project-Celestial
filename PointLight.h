#pragma once
#include "ILight.h"

namespace Resources
{

	///<summary>This class represents a point light</summary>
	class PointLight:public ILight
	{
		public:

			PointLight(BaseLightDesc);
			virtual unsigned char* Update(CrossHandlers::Message* mess) { return nullptr; }
			virtual CelestialMath::Vector4 GetSpec() const;
			virtual CelestialMath::Vector4 GetDiff() const;
			
			virtual CelestialMath::Vector3 GetPos() const;
			virtual CelestialMath::Vector3 GetDirection() const;
			virtual CelestialMath::Vector3 GetSide() const;
			virtual float GetSize() const;

			virtual float GetAngle() const;
			virtual float GetIntensity() const;

			virtual LightType GetType() const;

			virtual int GetId() const;
			virtual void SetId(int id);

			virtual void SetSpec(CelestialMath::Vector4 col);
			virtual void SetDiff(CelestialMath::Vector4 col);

			virtual void SetPosIntensity(CelestialMath::Vector3 &pos, float intens);
			virtual void SetPos(CelestialMath::Vector3 &pos);

			virtual void Point(CelestialMath::Vector3 forward, float roll);

			virtual CrossHandlers::IBounding* GetBoundingVolume();

			virtual ~PointLight();

		private:
			///<summary>The specular colour of the pointlight</summary>
			CelestialMath::Vector4 specCol;
			///<summary>The diffuse colour of the pointlight</summary>
			CelestialMath::Vector4 diffCol;
			///<summary>The ambiant colour of the pointlight</summary>
			CelestialMath::Vector4 ambCol;
			///<summary>The position of the pointlight</summary>
			CelestialMath::Vector3 pos;

			///<summary>The direction of where the light is pointing</summary>
			CelestialMath::Vector3 forwards;
			///<summary>The direction of where the lights side is</summary>
			CelestialMath::Vector3 sidewards;

			///<summary>What type of light this is</summary>
			LightType type;

			///<summary>The lights intensity</summary>
			float intensity;
			///<summary>The lights basic intensity</summary>
			float baseIntensity;
			///<summary>The size of the point</summary>
			float size;
			///<summary>The lights id</summary>
			int id;

			///<summary>The volume the point light takes up</summary>
			CrossHandlers::BoundingSphere* bound;

	};
}