#pragma once

#include "LightBase.h"

namespace RT
{
	class PointLight : public LightBase
	{
	public:
		PointLight();

		virtual ~PointLight() override;

		virtual bool ComputeIllumination(const qbVector<double>& intPoint, const qbVector<double>& localNormal,
			const std::vector<std::shared_ptr<RT::ObjectBase>>& objectList,
			const std::shared_ptr<RT::ObjectBase>& currentObject, qbVector<double>& color, double& intensity) override;
	};
}


