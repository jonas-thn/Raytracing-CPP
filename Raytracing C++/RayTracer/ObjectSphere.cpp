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
	
		at^2 + bt + c = 0 (quadratic formular)

		a = v * v
		b = 2(p1 * v)
		c = (p1 * p1) - r^2
		
		b^2 - 4ac > 0 
	*/

	qbVector<double> vhat = castRay.m_lab;
	vhat.Normalize();

	// a == squared magnitude of direction of cast ray (value of a always 1 -> unit vector)

	double b = 2.0 * qbVector<double>::dot(castRay.m_point1, vhat);

	double c = qbVector<double>::dot(castRay.m_point1, castRay.m_point1) - 1.0;

	double intTest = (b * b) - 4.0 * c;

	if (intTest > 0.0)
	{
		return true;
	}
	else
	{
		return false;
	}
}




