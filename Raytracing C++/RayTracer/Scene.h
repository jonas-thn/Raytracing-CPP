#pragma once

#include <vector>
#include <SDL.h>
#include "Image.h"
#include "Camera.h"
#include "ObjectSphere.h"
#include <memory>
#include "PointLight.h"

namespace RT
{
	class Scene
	{
	public:
		Scene();

		bool Render(Image& outputImage);

	private:

	private:
		RT::Camera m_camera;

		//list of objects
		std::vector<std::shared_ptr<RT::ObjectBase>> m_objectList;

		//list of lights
		std::vector<std::shared_ptr<RT::LightBase>> m_lightList;
	};
}



