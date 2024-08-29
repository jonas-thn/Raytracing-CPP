#include "ObjectPlane.h"
#include <cmath>

RT::ObjectPlane::ObjectPlane()
{
}

RT::ObjectPlane::~ObjectPlane()
{
}

bool RT::ObjectPlane::TestIntersection(const Ray& castRay, qbVector<double>& intPoint, qbVector<double>& localNormal, qbVector<double>& localColor)
{
	//(perpendicular standing plane at origin)

	RT::Ray bckRay = m_transformMatrix.Apply(castRay, RT::BCKTFORM);

	qbVector<double> k = bckRay.m_lab;
	k.Normalize();

	//check if cast ray is not parallel to the plane
	if (!CloseEnough(k.GetElement(2), 0.0))
	{
		double t = bckRay.m_point1.GetElement(2) / -k.GetElement(2);

		//if t negative, interesection behind camera can be ignored

		if (t > 0.0)
		{
			double u = bckRay.m_point1.GetElement(0) + (k.GetElement(0) * t);
			double v = bckRay.m_point1.GetElement(1) + (k.GetElement(1) * t);

			//if magnitude of u and v is less then one, when we are on the (unit) plane 
			if (abs(u) < 1.0 && (abs(v) < 1.0))
			{
				//point of intersection
				qbVector<double> poi = bckRay.m_point1 + t * k;

				//transform back to world coords
				intPoint = m_transformMatrix.Apply(poi, RT::FWDTFORM);

				qbVector<double> localOrigin{ std::vector<double>{0.0, 0.0, 0.0} };
				qbVector<double> normalVector{ std::vector<double>{0.0, 0.0, -1.0} };
				qbVector<double> globalOrigin = m_transformMatrix.Apply(localOrigin, RT::FWDTFORM);
				localNormal = m_transformMatrix.Apply(normalVector, RT::FWDTFORM) - globalOrigin;

				localColor = m_baseColor;

				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false; 
		}
	}

	return false;

}
