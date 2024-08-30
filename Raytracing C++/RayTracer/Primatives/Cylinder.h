#pragma once

#include "ObjectBase.h"
#include "../GTFM.h"
#include "array"

namespace RT
{
	class Cylinder : public ObjectBase
	{
	public:
		Cylinder();

		virtual ~Cylinder() override;

		virtual bool TestIntersection(const Ray& castRay, qbVector<double>& intPoint, qbVector<double>& localNormal, qbVector<double>& localColor) override;

	};
}

