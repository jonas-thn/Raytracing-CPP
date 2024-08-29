#include "MaterialBase.h"

RT::MaterialBase::MaterialBase()
{
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

bool RT::MaterialBase::CastRay(const RT::Ray& castRay, const std::vector<std::shared_ptr<RT::ObjectBase>>& objectList, const std::shared_ptr<RT::ObjectBase>& thisObject, std::shared_ptr<RT::ObjectBase>& closestObject, qbVector<double>& closestPoint, qbVector<double>& closestLocalNormal, qbVector<double>& closestColor)
{
	return false;
}
