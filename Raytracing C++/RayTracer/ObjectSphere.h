#pragma once

#include "ObjectBase.h"
#include "GTFM.h"

#include "../qbLinAlg/qbVector.h"

namespace RT
{
	class ObjectSphere : public ObjectBase
	{
	public:
		//default shpere at origin

		ObjectSphere();

		virtual ~ObjectSphere() override;

		virtual bool TestIntersection(const Ray& castRay, qbVector<double>& intPoint, qbVector<double>& localNormal, qbVector<double>& localColor) override;

	};
}


