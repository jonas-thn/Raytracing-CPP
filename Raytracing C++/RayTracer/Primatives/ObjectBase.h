#pragma once

#include "../../qbLinAlg/qbVector.h"
#include "../Ray.h"
#include "../GTFM.h"
#include <memory>

namespace RT
{
	//forward declare material class (will be overridden later)
	class MaterialBase;

	class ObjectBase
	{
	public:
		ObjectBase();
		virtual ~ObjectBase();

		virtual bool TestIntersection(const Ray& castRay, qbVector<double>& intPoint, qbVector<double>& localNormal, qbVector<double>& localColor);

		void SetTransformMatrix(const RT::GTFM& transformMatrix);

		bool CloseEnough(const double f1, const double f2);

		bool AssignMaterial(const std::shared_ptr<RT::MaterialBase>& objectMaterial);

	public:
		qbVector<double> m_baseColor{ 3 };

		RT::GTFM m_transformMatrix;

		std::shared_ptr<RT::MaterialBase> m_pMaterial;

		bool m_hasMaterial = false;
	};
}


