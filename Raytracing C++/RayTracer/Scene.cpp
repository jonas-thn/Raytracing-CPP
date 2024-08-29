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
	m_objectList.at(3)->m_baseColor = qbVector<double>{ std::vector<double>{128.0, 128.0, 128.0} };

	//modify spheres
	RT::GTFM testMatrix1, testMatrix2, testMatrix3;
	testMatrix1.SetTransform(qbVector<double>{std::vector<double>{-1.5, 0.0, 0.0}}, qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}}, qbVector<double>{std::vector<double>{0.5, 0.5, 0.75}});
	testMatrix2.SetTransform(qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}}, qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}}, qbVector<double>{std::vector<double>{0.75, 0.5, 0.5}});
	testMatrix3.SetTransform(qbVector<double>{std::vector<double>{1.5, 0.0, 0.0}}, qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}}, qbVector<double>{std::vector<double>{0.75, 0.75, 0.75}});

	m_objectList.at(0)->SetTransformMatrix(testMatrix1);
	m_objectList.at(1)->SetTransformMatrix(testMatrix2);
	m_objectList.at(2)->SetTransformMatrix(testMatrix3);

	m_objectList.at(0)->m_baseColor = qbVector<double>{ std::vector<double>{64.0, 128.0, 200.0} };
	m_objectList.at(1)->m_baseColor = qbVector<double>{ std::vector<double>{255.0, 128.0, 0.0} };
	m_objectList.at(2)->m_baseColor = qbVector<double>{ std::vector<double>{255.0, 200.0, 0.0} };


	//construct a test light
	m_lightList.push_back(std::make_shared<RT::PointLight>(RT::PointLight()));
	m_lightList.at(0)->m_location = qbVector<double>{ std::vector<double>{5.0, -10.0, -5.0} };
	m_lightList.at(0)->m_color = qbVector<double>{ std::vector<double>{255.0, 255.0, 255.0} };
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
		for (int y = 0; y < ySize; ++y)
		{
			//normalize x and y coords
			double normX = (static_cast<double>(x) * xFact) - 1.0;
			double normY = (static_cast<double>(y) * yFact) - 1.0;

			//generate ray for this pixel
			m_camera.GenerateRay(normX, normY, cameraRay);

			//test for intersections with all objects in scene
			for (auto currentObject : m_objectList)
			{
				bool validInt = currentObject->TestIntersection(cameraRay, intPoint, localNormal, localColor);

				//change color to red if valid intersecion
				if (validInt)
				{
					//compute intensity of illumination
					double intensity;
					qbVector<double> color{ 3 };
					bool validIllum = false;
					for (auto currentLight : m_lightList)
					{
						validIllum = currentLight->ComputeIllumination(intPoint, localNormal, m_objectList, currentObject, color, intensity);
					}

					//compute distance to camera and the point of intersecition
					double dist = (intPoint - cameraRay.m_point1).norm();
					if (dist > maxDist)
					{
						maxDist = dist;
					}
					if (dist < minDist)
					{
						minDist = dist;
					}

					//outputImage.SetPixel(x, y, 255.0 - ((dist - 9.0) / 0.94605) * 255.0, 0.0, 0.0);
					if (validIllum)
					{
						//outputImage.SetPixel(x, y, 255.0 * intensity, 0.0, 0.0);
						outputImage.SetPixel(x, y, localColor.GetElement(0) * intensity, localColor.GetElement(1) * intensity, localColor.GetElement(2) * intensity);
					}
					else
					{
						//outputImage.SetPixel(x, y, 0.0, 0.0, 0.0);
					}
				}
				else
				{
					//outputImage.SetPixel(x, y, 0.0, 0.0, 0.0);
				}
			}			
		}
	}

	std::cout << "Minimum distance: " << minDist << std::endl;
	std::cout << "Maximum distance: " << maxDist << std::endl;

	return true;
}
