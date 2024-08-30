#include "Scene.h"
#include "./Materials/MaterialBase.h"
#include "./Materials/SimpleMaterial.h"

RT::Scene::Scene()
{
	auto floorMaterial = std::make_shared<RT::SimpleMaterial>(RT::SimpleMaterial());
	floorMaterial->m_baseColor = qbVector<double>{ std::vector<double>{1, 1, 1} };
	floorMaterial->m_reflectivity = 0.2;
	floorMaterial->m_shininess = 0.0;

	auto blueDiffuse = std::make_shared<RT::SimpleMaterial>(RT::SimpleMaterial());
	blueDiffuse->m_baseColor = qbVector<double>{ std::vector<double>{0.2, 0.2, 0.8} };
	blueDiffuse->m_reflectivity = 0.05;
	blueDiffuse->m_shininess = 5.0;

	auto yellowDiffuse = std::make_shared<RT::SimpleMaterial>(RT::SimpleMaterial());
	yellowDiffuse->m_baseColor = qbVector<double>{ std::vector<double>{0.8, 0.8, 0.4} };
	yellowDiffuse->m_reflectivity = 0.4;
	yellowDiffuse->m_shininess = 5.0;

	//create floor
	auto floor = std::make_shared <RT::ObjectPlane>(RT::ObjectPlane());
	//RT::GTFM planeMatrix;
	floor->SetTransformMatrix(RT::GTFM{ qbVector<double>{std::vector<double>{0.0, 0.0, 1.0}}, qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}}, qbVector<double>{std::vector<double>{4.0, 4.0, 1.0}} });
	floor->AssignMaterial(floorMaterial);

	//create objects
	auto cylinder1 = std::make_shared<RT::Cylinder>(RT::Cylinder());
	cylinder1->SetTransformMatrix(RT::GTFM{ qbVector<double>{std::vector<double>{-1.0, 0.0, 0.0}}, qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}}, qbVector<double>{std::vector<double>{0.5, 0.5, 1.0}} });
	cylinder1->AssignMaterial(blueDiffuse);

	auto cone1 = std::make_shared<RT::Cone>(RT::Cone());
	cone1->SetTransformMatrix(RT::GTFM{ qbVector<double>{std::vector<double>{1.0, 0.0, 0.0}}, qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}}, qbVector<double>{std::vector<double>{0.5, 0.5, 1.0}} });
	cone1->AssignMaterial(yellowDiffuse);

	m_objectList.push_back(floor);
	m_objectList.push_back(cylinder1);
	m_objectList.push_back(cone1);

	//camera
	m_camera.SetPosition(qbVector<double>(std::vector<double>{0.0, -10.0, -1.0}));
	m_camera.SetLookAt(qbVector<double>(std::vector<double>{0.0, 0.0, 0.0}));
	m_camera.SetUp(qbVector<double>(std::vector<double>{0.0, 0.0, 1.0}));
	m_camera.SetHorzSize(0.25);
	m_camera.SetAspect(16.0 / 9.0);
	m_camera.UpdateCameraGeometry();


	//transform for plane

	//construct a test light
	m_lightList.push_back(std::make_shared<RT::PointLight>(RT::PointLight()));
	m_lightList.at(0)->m_location = qbVector<double>{ std::vector<double>{5.0, -10.0, -5.0} };
	m_lightList.at(0)->m_color = qbVector<double>{ std::vector<double>{0.7, 0.7, 1} };

	m_lightList.push_back(std::make_shared<RT::PointLight>(RT::PointLight()));
	m_lightList.at(1)->m_location = qbVector<double>{ std::vector<double>{-5.0, -10.0, -5.0} };
	m_lightList.at(1)->m_color = qbVector<double>{ std::vector<double>{1, 0.3, 0.3} };

	m_lightList.push_back(std::make_shared<RT::PointLight>(RT::PointLight()));
	m_lightList.at(2)->m_location = qbVector<double>{ std::vector<double>{0.0, -10.0, -5.0} };
	m_lightList.at(2)->m_color = qbVector<double>{ std::vector<double>{0.1, 1, 0.1} };
}

bool RT::Scene::Render(Image& outputImage)
{
	int xSize = outputImage.GetXSize();
	int ySize = outputImage.GetYSize();

	//loop over each pixel in our image
	RT::Ray cameraRay;
	qbVector<double> intPoint(3);
	qbVector<double> localNormal(3);
	qbVector<double> localColor(3);

	double xFact = 1.0 / (static_cast<double>(xSize) / 2.0);
	double yFact = 1.0 / (static_cast<double>(ySize) / 2.0);

	double minDist = 1e6;
	double maxDist = 0.0;

	for (int x = 0; x < xSize; ++x)
	{
		std::cout << "Processing line: " << x << " / " << xSize << std::endl;

		for (int y = 0; y < ySize; ++y)
		{
			//normalize x and y coords
			double normX = (static_cast<double>(x) * xFact) - 1.0;
			double normY = (static_cast<double>(y) * yFact) - 1.0;

			//generate ray for this pixel
			m_camera.GenerateRay(normX, normY, cameraRay);

			std::shared_ptr<RT::ObjectBase> closestObject;
			qbVector<double> closestIntPoint{ 3 };
			qbVector<double> closestLocalNormal{ 3 };
			qbVector<double> closestLocalColor{ 3 };

			bool intersectionFound = CastRay(cameraRay, closestObject, closestIntPoint, closestLocalNormal, closestLocalColor);

			//compute illumination for closest object
			if (intersectionFound)
			{
				//check obj material
				if (closestObject->m_hasMaterial)
				{
					RT::MaterialBase::m_reflectionRayCount = 0;
					qbVector<double> color = closestObject->m_pMaterial->ComputeColor(m_objectList, m_lightList, closestObject, closestIntPoint, closestLocalNormal, cameraRay);

					outputImage.SetPixel(x, y, color.GetElement(0), color.GetElement(1), color.GetElement(2));
				}
				else
				{

					qbVector<double> matColor = RT::MaterialBase::ComputeDiffuseColor(m_objectList, m_lightList, closestObject, closestIntPoint, closestLocalNormal, closestObject->m_baseColor);

					outputImage.SetPixel(x, y, matColor.GetElement(0), matColor.GetElement(1), matColor.GetElement(2));
				}
			}
		}
	}
	return true;
}

bool RT::Scene::CastRay(RT::Ray& castRay, std::shared_ptr<RT::ObjectBase>& closestObject, qbVector<double>& closestIntPoint, qbVector<double>& closestLocalNormal, qbVector<double>& closestLocalColor)
{
	qbVector<double> intPoint{ 3 };
	qbVector<double> localNormal{ 3 };
	qbVector<double> localColor{ 3 };
	double minDist = 1e6;
	bool intersectionFound = false;

	//test for intersections with all objects in scene
	for (auto currentObject : m_objectList)
	{
		bool validInt = currentObject->TestIntersection(castRay, intPoint, localNormal, localColor);

		//check valid intersection
		if (validInt)
		{
			intersectionFound = true;

			//distance between camera and intersection
			double dist = (intPoint - castRay.m_point1).norm();

			//if closer than store reference
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

	return intersectionFound;
}
