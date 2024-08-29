#include "Scene.h"

RT::Scene::Scene()
{
	m_camera.SetPosition(qbVector<double>(std::vector<double>{0.0, -10.0, -1.0}));
	m_camera.SetLookAt(qbVector<double>(std::vector<double>{0.0, 0.0, 0.0}));
	m_camera.SetUp(qbVector<double>(std::vector<double>{0.0, 0.0, 1.0}));
	m_camera.SetHorzSize(0.25);
	m_camera.SetAspect(16.0 / 9.0);
	m_camera.UpdateCameraGeometry();

	//construct a test sphere
	m_objectList.push_back(std::make_shared<RT::ObjectSphere>(RT::ObjectSphere()));
	m_objectList.push_back(std::make_shared<RT::ObjectSphere>(RT::ObjectSphere()));
	m_objectList.push_back(std::make_shared<RT::ObjectSphere>(RT::ObjectSphere()));

	//construct test plane
	m_objectList.push_back(std::make_shared <RT::ObjectPlane>(RT::ObjectPlane()));
	m_objectList.at(3)->m_baseColor = qbVector<double>{ std::vector<double>{0.5, 0.5, 0.5} };

	//transform for plane
	RT::GTFM planeMatrix;
	planeMatrix.SetTransform(qbVector<double>{std::vector<double>{0.0, 0.0, 0.75}}, qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}}, qbVector<double>{std::vector<double>{4.0, 4.0, 1.0}});
	m_objectList.at(3)->SetTransformMatrix(planeMatrix);

	//modify spheres
	RT::GTFM testMatrix1, testMatrix2, testMatrix3;
	testMatrix1.SetTransform(qbVector<double>{std::vector<double>{-1.5, 0.0, 0.0}}, qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}}, qbVector<double>{std::vector<double>{0.5, 0.5, 0.75}});
	testMatrix2.SetTransform(qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}}, qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}}, qbVector<double>{std::vector<double>{0.75, 0.5, 0.5}});
	testMatrix3.SetTransform(qbVector<double>{std::vector<double>{1.5, 0.0, 0.0}}, qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}}, qbVector<double>{std::vector<double>{0.75, 0.75, 0.75}});

	m_objectList.at(0)->SetTransformMatrix(testMatrix1);
	m_objectList.at(1)->SetTransformMatrix(testMatrix2);
	m_objectList.at(2)->SetTransformMatrix(testMatrix3);

	m_objectList.at(0)->m_baseColor = qbVector<double>{ std::vector<double>{0.25, 0.5, 0.8} };
	m_objectList.at(1)->m_baseColor = qbVector<double>{ std::vector<double>{1, 0.5, 0.0} };
	m_objectList.at(2)->m_baseColor = qbVector<double>{ std::vector<double>{1, 0.8, 0.0} };


	//construct a test light
	m_lightList.push_back(std::make_shared<RT::PointLight>(RT::PointLight()));
	m_lightList.at(0)->m_location = qbVector<double>{ std::vector<double>{5.0, -10.0, -5.0} };
	m_lightList.at(0)->m_color = qbVector<double>{ std::vector<double>{1, 1, 1} };
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
		std::cout << "Iteration: " << x << " / " << xSize << std::endl;

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
			double minDist = 1e6;
			bool intersectionFound = false;

			//test for intersections with all objects in scene
			for (auto currentObject : m_objectList)
			{
				bool validInt = currentObject->TestIntersection(cameraRay, intPoint, localNormal, localColor);

				//check valid intersection
				if (validInt)
				{
					intersectionFound = true;

					//distance between camera and intersection
					double dist = (intPoint - cameraRay.m_point1).norm();

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

			//compute illumination for closest object
			if (intersectionFound)
			{
				double intensity;
				qbVector<double> color{ 3 };
				double red = 0.0;
				double green = 0.0;
				double blue = 0.0;
				bool validIllum = false;
				bool illumFound = false;

				for (auto currentLight : m_lightList)
				{
					validIllum = currentLight->ComputeIllumination(closestIntPoint, closestLocalNormal, m_objectList, closestObject, color, intensity);

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
					red *= closestLocalColor.GetElement(0);
					green *= closestLocalColor.GetElement(1);
					blue *= closestLocalColor.GetElement(2);
					outputImage.SetPixel(x, y, red, green, blue);
				}
			}
		}
	}
	return true;
}
