#pragma once

#include <vector>
#include <SDL.h>
#include "Image.h"
#include "Camera.h"
#include "ObjectSphere.h"

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

		RT::ObjectSphere m_testSphere;
	};
}



