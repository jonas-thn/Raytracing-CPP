#pragma once

#include "../qbLinAlg/qbVector.h"
#include "Ray.h"
#include "GTFM.h"

namespace RT
{
	class ObjectBase
	{
	public:
		ObjectBase();
		virtual ~ObjectBase();

		virtual bool TestIntersection(const Ray& castRay, qbVector<double>& intPoint, qbVector<double>& localNormal, qbVector<double>& localColor);

		void SetTransformMatrix(const RT::GTFM& transformMatrix);

		bool CloseEnough(const double f1, const double f2);

	public:
		qbVector<double> m_baseColor{ 3 };

		RT::GTFM m_transformMatrix;
	};
}


