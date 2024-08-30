#pragma once

#include "../GTFM.h"
#include "ObjectBase.h"
#include <array>

namespace RT
{
	class Cone : public ObjectBase
	{
	public:
		// Default constructor.
		Cone();

		// Override the destructor.
		virtual ~Cone() override;

		// Override the function to test for intersections.
		virtual bool TestIntersection(const RT::Ray& castRay, qbVector<double>& intPoint,
			qbVector<double>& localNormal, qbVector<double>& localColor) override;
	};
}

