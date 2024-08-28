#include "ObjectSphere.h"

RT::ObjectSphere::ObjectSphere()
{

}

RT::ObjectSphere::~ObjectSphere()
{

}

bool RT::ObjectSphere::TestIntersection(const Ray& castRay, qbVector<double>& intPoint, qbVector<double>& localNormal, qbVector<double>& localColor)
{
	/* Sphere Intersection Test Formula:
	
		at^2 + bt + c = 0 (quadratic formular -> front face and backface solution)

		a = v * v
		b = 2(p1 * v)
		c = (p1 * p1) - r^2
		
		b^2 - 4ac > 0 
	*/

	//from world to local
	RT::Ray bckRay = m_transformMatrix.Apply(castRay, RT::BCKTFORM);

	qbVector<double> vhat = bckRay.m_lab;
	vhat.Normalize();

	// a == squared magnitude of direction of cast ray (value of a always 1 -> unit vector)

	double b = 2.0 * qbVector<double>::dot(bckRay.m_point1, vhat);

	double c = qbVector<double>::dot(bckRay.m_point1, bckRay.m_point1) - 1.0;

	double intTest = (b * b) - 4.0 * c;

	qbVector<double> poi;

	if (intTest > 0.0)
	{
		double numSQRT = sqrtf(intTest);
		double t1 = (-b + numSQRT) / 2.0;
		double t2 = (-b - numSQRT) / 2.0;

		if ((t1 < 0) || (t2 < 0))
		{
			return false;
		}
		else
		{
			if (t1 < t2)
			{
				poi = bckRay.m_point1 + (vhat * t1);
			}
			else
			{
				poi = bckRay.m_point1 + (vhat * t2);
			}

			//transform intersection backto world coords
			intPoint = m_transformMatrix.Apply(poi, RT::FWDTFORM);

			//compute local normal
			qbVector<double> objOrigin = qbVector<double>{ std::vector<double>{0.0, 0.0, 0.0} };
			qbVector<double> newObjOrigin = m_transformMatrix.Apply(objOrigin, RT::FWDTFORM);
			localNormal = intPoint - newObjOrigin; //from origin to intersecion
			localNormal.Normalize();

			//return base color
			localColor = m_baseColor;
		}

		return true;
	}
	else
	{
		return false;
	}
}




