#pragma once

#include <vector>
#include <SDL.h>
#include <memory>
#include "Image.h"
#include "Camera.h"
#include "./Primatives/ObjectSphere.h"
#include "./Lights/PointLight.h"
#include "./Primatives/ObjectPlane.h"
#include "./Primatives/Cylinder.h"
#include "./Primatives/Cone.h"

namespace RT
{
	class Scene
	{
	public:
		Scene();

		bool Render(Image& outputImage);

		bool CastRay(RT::Ray& castRay, std::shared_ptr<RT::ObjectBase>& closestObject, qbVector<double>& closestIntPoint, qbVector<double>& closestLocalNormal, qbVector<double>& closestLocalColor);

	private:

	private:
		RT::Camera m_camera;

		//list of objects
		std::vector<std::shared_ptr<RT::ObjectBase>> m_objectList;

		//list of lights
		std::vector<std::shared_ptr<RT::LightBase>> m_lightList;
	};
}



