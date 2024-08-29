#include "SimpleMaterial.h"

RT::SimpleMaterial::SimpleMaterial()
{
}

RT::SimpleMaterial::~SimpleMaterial()
{
}

qbVector<double> RT::SimpleMaterial::ComputeColor(const std::vector<std::shared_ptr<RT::ObjectBase>>& objectList, const std::vector<std::shared_ptr<RT::LightBase>>& lightList, const std::shared_ptr<RT::ObjectBase>& currentObject, const qbVector<double>& intPoint, const qbVector<double>& localNormal, const RT::Ray& cameraRay)
{
	qbVector<double> matColor{ 3 };
	qbVector<double> refColor{ 3 };
	qbVector<double> difColor{ 3 };
	qbVector<double> spcColor{ 3 };

	//compute diffuse color
	difColor = ComputeDiffuseColor(objectList, lightList, currentObject, intPoint, localNormal, m_baseColor);

	//compute rflection component
	if (m_reflectivity > 0)
	{
		refColor = ComputeRfelectionColor(objectList, lightList, currentObject, intPoint, localNormal, cameraRay);

		//combine reflection and diffuse
		matColor = (refColor * m_reflectivity) + (difColor * (1 - m_reflectivity));
	}

	//compute specular color
	if (m_shininess > 0) 
	{
		spcColor = ComputeSpecular(objectList, lightList, intPoint, localNormal, cameraRay);
	}

	matColor = matColor + spcColor;

	return matColor;
}

qbVector<double> RT::SimpleMaterial::ComputeSpecular(const std::vector<std::shared_ptr<RT::ObjectBase>>& objectList, const std::vector<std::shared_ptr<RT::LightBase>>& lightList, const qbVector<double>& intPoint, const qbVector<double>& localNormal, const RT::Ray& cameraRay)
{
	qbVector<double> spcColor{ 3 };
	double red = 0.0;
	double green = 0.0;
	double blue = 0.0;

	//loop through all lights in scene
	for (auto currentLight : lightList)
	{
		//check intersections for all the objects in the scene
		double intensity = 0;

		qbVector<double> lightDir = (currentLight->m_location - intPoint).Normalized();

		qbVector<double> startPoint = intPoint + (lightDir * 0.001);

		RT::Ray lightRay(startPoint, startPoint + lightDir);

		//loop all objects in scene to check light obstruction
		qbVector<double> poi{ 3 };
		qbVector<double> poiNormal{ 3 };
		qbVector<double> poiColor{ 3 };
		bool validInt = false;

		for (auto sceneObject : objectList)
		{
			validInt = sceneObject->TestIntersection(lightRay, poi, poiNormal, poiColor);
			if (validInt)
			{
				break;
			}
		}

		if (!validInt)
		{
			qbVector<double> d = lightRay.m_lab;
			qbVector<double> r = d - (2 * qbVector<double>::dot(d, localNormal) * localNormal);
			r.Normalize();

			qbVector<double> v = cameraRay.m_lab;
			v.Normalize();
			double dotProduct = qbVector<double>::dot(r, v);

			if (dotProduct > 0.0)
			{
				intensity = m_reflectivity * std::pow(dotProduct, m_shininess);

			}
		}

		red += currentLight->m_color.GetElement(0) * intensity;
		green += currentLight->m_color.GetElement(1) * intensity;
		blue += currentLight->m_color.GetElement(2) * intensity;
	}

	spcColor.SetElement(0, red);
	spcColor.SetElement(1, green);
	spcColor.SetElement(2, blue);

	return spcColor;
}
