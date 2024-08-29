#pragma once

#include <memory>
#include "../Primatives/ObjectBase.h"
#include "../Lights/LightBase.h"
#include "../../qbLinAlg/qbVector.h"
#include "../Ray.h"

namespace RT
{
	class MaterialBase
	{
	public:
		MaterialBase();
		virtual ~MaterialBase();

		virtual qbVector<double> ComputeColor(const std::vector<std::shared_ptr<RT::ObjectBase>>& objectList,
			const std::vector<std::shared_ptr<RT::LightBase>>& lightList, const std::shared_ptr<RT::ObjectBase>& currentObject,
			const qbVector<double>& intPoint, const qbVector<double>& localNormal, const RT::Ray& cameraRay);

		static qbVector<double> ComputeDiffuseColor(const std::vector<std::shared_ptr<RT::ObjectBase>>& objectList,
			const std::vector<std::shared_ptr<RT::LightBase>>& lightList, const std::shared_ptr<RT::ObjectBase>& currentObject,
			const qbVector<double>& intPoint, const qbVector<double>& localNormal, qbVector<double>& baseColor);

		bool CastRay(const RT::Ray& castRay, const std::vector < std::shared_ptr <RT::ObjectBase>>& objectList,
			const std::shared_ptr<RT::ObjectBase>& thisObject, std::shared_ptr<RT::ObjectBase>& closestObject, qbVector<double>& closestPoint, 
			qbVector<double>& closestLocalNormal, qbVector<double>& closestColor);

	public:

	private:

	};
}

