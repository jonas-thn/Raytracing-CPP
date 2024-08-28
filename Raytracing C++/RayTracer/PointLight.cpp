#include "PointLight.h"

RT::PointLight::PointLight()
{
	m_color = qbVector<double>{ std::vector<double>{1.0, 1.0, 1.0} };
	m_intensity = 1.0;
}

RT::PointLight::~PointLight()
{
}

bool RT::PointLight::ComputeIllumination(const qbVector<double>& intPoint, const qbVector<double>& localNormal, const std::vector<std::shared_ptr<RT::ObjectBase>>& objectList, const std::shared_ptr<RT::ObjectBase>& currentObject, qbVector<double>& color, double& intensity)
{
	qbVector<double> lightDir = (m_location - intPoint).Normalized();

	qbVector<double> startPoint = intPoint;

	//angle between localNormal and lightRay (localNormal is unitVector)
	double angle = acos(qbVector<double>::dot(localNormal, lightDir));

	//if the angle pointing away, no illumination
	if (angle > 1.5708) //half pi
	{
		color = m_color;
		intensity = 0.0;
		return false;
	}
	else
	{
		color = m_color;
		intensity = m_intensity * (1.0 - (angle / 1.5708));
		return true;
	}
}
