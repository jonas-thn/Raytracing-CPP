#pragma once

#include "ObjectBase.h"
#include "GTFM.h"

namespace RT
{
	class ObjectPlane : public ObjectBase
	{
	public:
		ObjectPlane();

		virtual ~ObjectPlane() override;

		virtual bool TestIntersection(const Ray& castRay, qbVector<double>& intPoint, qbVector<double>& localNormal, qbVector<double>& localColor) override;

	private:
	};
}


