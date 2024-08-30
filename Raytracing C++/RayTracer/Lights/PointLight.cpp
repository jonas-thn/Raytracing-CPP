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
	double lightDist = (m_location - intPoint).norm();

	qbVector<double> startPoint = intPoint;

	//construct ray from point of intersection to the light
	RT::Ray lightRay(startPoint, startPoint + lightDir);

	//check intersections
	qbVector<double> poi{ 3 };
	qbVector<double> poiNormal{ 3 };
	qbVector<double> poiColor{ 3 };

	bool validInt = false;

	for (auto sceneObject : objectList)
	{
		if (sceneObject != currentObject)
		{
			validInt = sceneObject->TestIntersection(lightRay, poi, poiNormal, poiColor);
			if (validInt)
			{
				double dist = (poi - startPoint).norm();
				if (dist > lightDist)
				{
					validInt = false;
				}
			}
		}

		//if intersection, then there is no point checking further
		if (validInt)
		{
			break;
		}
	}

	//only continue if ligth ray didnt intersect with any objects
	if (!validInt)
	{
		//compute angle between localNormal and lightRay (localNormal is unit vector)

		double angle = acos(qbVector<double>::dot(localNormal, lightDir));

		//if pointing away, then no illumination
		if (angle > 1.5708)
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
	else
	{
		//shadow, so no illumination
		color = m_color;
		intensity = 0.0;
		return false;
	}
}
