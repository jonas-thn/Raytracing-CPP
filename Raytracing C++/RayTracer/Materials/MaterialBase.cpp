#include "MaterialBase.h"

RT::MaterialBase::MaterialBase()
{
	m_maxReflectionRays = 3;
	m_reflectionRayCount = 0;
}

RT::MaterialBase::~MaterialBase()
{
}

qbVector<double> RT::MaterialBase::ComputeColor(const std::vector<std::shared_ptr<RT::ObjectBase>>& objectList, const std::vector<std::shared_ptr<RT::LightBase>>& lightList, const std::shared_ptr<RT::ObjectBase>& currentObject, const qbVector<double>& intPoint, const qbVector<double>& localNormal, const RT::Ray& cameraRay)
{
	qbVector<double> matColor{ 3 };

	return matColor;
}

qbVector<double> RT::MaterialBase::ComputeDiffuseColor(const std::vector<std::shared_ptr<RT::ObjectBase>>& objectList, const std::vector<std::shared_ptr<RT::LightBase>>& lightList, const std::shared_ptr<RT::ObjectBase>& currentObject, const qbVector<double>& intPoint, const qbVector<double>& localNormal, qbVector<double>& baseColor)
{
	qbVector<double> diffuseColor{ 3 };
	double intensity;
	qbVector<double> color{ 3 };
	double red = 0.0;
	double green = 0.0;
	double blue = 0.0;
	bool validIllum = false;
	bool illumFound = false;
	for (auto currentLight : lightList) 
	{
		validIllum = currentLight->ComputeIllumination(intPoint, localNormal, objectList, currentObject, color, intensity);

		if (validIllum) 
		{
			illumFound = true;
			red += color.GetElement(0) * intensity;
			green += color.GetElement(1) * intensity;
			blue += color.GetElement(2) * intensity;

		}
	}

	if (illumFound)
	{
		diffuseColor.SetElement(0, red * baseColor.GetElement(0));
		diffuseColor.SetElement(1, green * baseColor.GetElement(1));
		diffuseColor.SetElement(2, blue * baseColor.GetElement(2));
	}

	return diffuseColor;
}

qbVector<double> RT::MaterialBase::ComputeRfelectionColor(const std::vector<std::shared_ptr<RT::ObjectBase>>& objectList, const std::vector<std::shared_ptr<RT::LightBase>>& lightList, const std::shared_ptr<RT::ObjectBase>& currentObject, const qbVector<double>& intPoint, const qbVector<double>& localNormal, const RT::Ray& incidentRay)
{
	qbVector<double> reflectionColor{ 3 };
	qbVector<double> d = incidentRay.m_lab;
	qbVector<double> reflectionVector = d - (2 * qbVector<double>::dot(d, localNormal) * localNormal);

	//reflection ray
	RT::Ray reflectionRay(intPoint, intPoint + reflectionVector);

	//shoot into scene 
	std::shared_ptr<RT::ObjectBase> closestObject;
	qbVector<double> closestIntPoint{ 3 };
	qbVector<double> closestLocalNormal{ 3 };
	qbVector<double> closestLocalColor{ 3 };

	bool intersectionFound = CastRay(reflectionRay, objectList, currentObject, closestObject, closestIntPoint, closestLocalNormal, closestLocalColor);

	//Illumination for closest Object
	qbVector<double> matColor{ 3 };
	if ((intersectionFound) && (m_reflectionRayCount < m_maxReflectionRays))
	{
		m_reflectionRayCount++;

		if (closestObject->m_hasMaterial)
		{
			matColor = closestObject->m_pMaterial->ComputeColor(objectList, lightList, closestObject, closestIntPoint, closestLocalNormal, reflectionRay);
		}
		else
		{
			matColor = RT::MaterialBase::ComputeDiffuseColor(objectList, lightList, closestObject, closestIntPoint, closestLocalNormal, closestObject->m_baseColor);
		}
	}
	else
	{
		//leave color as it is

	}

	reflectionColor = matColor;
	return reflectionColor;

}

bool RT::MaterialBase::CastRay(const RT::Ray& castRay, const std::vector<std::shared_ptr<RT::ObjectBase>>& objectList, const std::shared_ptr<RT::ObjectBase>& thisObject, std::shared_ptr<RT::ObjectBase>& closestObject, qbVector<double>& closestIntPoint, qbVector<double>& closestLocalNormal, qbVector<double>& closestLocalColor)
{
	//test for intersections with all objects in scene
	qbVector<double> intPoint{ 3 };
	qbVector<double> localNormal{ 3 };
	qbVector<double> localColor{ 3 };

	double minDist = 1e6;
	bool intersectionFound = false;

	for (auto currentObject : objectList)
	{
		if (currentObject != thisObject)
		{
			bool validInt = currentObject->TestIntersection(castRay, intPoint, localNormal, localColor);

			if (validInt)
			{
				intersectionFound = true;

				double dist = (intPoint - castRay.m_point1).norm();

				if (dist < minDist)
				{
					minDist = dist;
					closestObject = currentObject; 
					closestIntPoint = intPoint;
					closestLocalNormal = localNormal;
					closestLocalColor = localColor;
				}
			}
		}
	}

	return intersectionFound;
}
