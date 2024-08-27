#include "Scene.h"

RT::Scene::Scene()
{
	m_camera.SetPosition(qbVector<double>(std::vector<double>{0.0, -10.0, 0.0}));
	m_camera.SetLookAt(qbVector<double>(std::vector<double>{0.0, 0.0, 0.0}));
	m_camera.SetUp(qbVector<double>(std::vector<double>{0.0, 0.0, 1.0}));
	m_camera.SetHorzSize(0.25);
	m_camera.SetAspect(16.0 / 9.0);
	m_camera.UpdateCameraGeometry();
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

			//test for valid intersection
			bool validInt = m_testSphere.TestIntersection(cameraRay, intPoint, localNormal, localColor);

			//change color to red if valid intersecion
			if (validInt)
			{
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

				outputImage.SetPixel(x, y, 255.0 - ((dist - 9.0) / 0.94605) * 255.0, 0.0, 0.0);
			}
			else
			{
				outputImage.SetPixel(x, y, 0.0, 0.0, 0.0);
			}
		}
	}

	std::cout << "Minimum distance: " << minDist << std::endl;
	std::cout << "Maximum distance: " << maxDist << std::endl;

	return true;
}
