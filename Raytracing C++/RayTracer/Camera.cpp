#include "Camera.h"
#include "Ray.h"
#include "math.h"

RT::Camera::Camera()
{
	m_cameraPosition = qbVector<double>{ std::vector<double> {0.0, -10.0, 0.0} };
	m_cameraLookAt = qbVector<double>{ std::vector<double> {0.0, 0.0, 0.0} };
	m_cameraUp = qbVector<double>{ std::vector<double> {0.0, 0.0, 1.0} };
	m_cameraLength = 1;
	m_cameraAspectRatio = 1;
	m_cameraHorzSize = 1;
}

void RT::Camera::SetPosition(const qbVector<double>& newPosition)
{
	m_cameraPosition = newPosition;
}

void RT::Camera::SetLookAt(const qbVector<double>& newLookAt)
{
	m_cameraLookAt = newLookAt;
}

void RT::Camera::SetUp(const qbVector<double>& upVector)
{
	m_cameraUp = upVector;
}

void RT::Camera::SetLength(double newLength)
{
	m_cameraLength = newLength;
}

void RT::Camera::SetHorzSize(double newSize)
{
	m_cameraHorzSize = newSize;
}

void RT::Camera::SetAspect(double newAspect)
{
	m_cameraAspectRatio = newAspect;
}

qbVector<double> RT::Camera::GetPosition()
{
	return m_cameraPosition;
}

qbVector<double> RT::Camera::GetLookAt()
{
	return m_cameraLookAt;
}

qbVector<double> RT::Camera::GetUp()
{
	return m_cameraUp;
}

qbVector<double> RT::Camera::GetU()
{
	return m_projectionScreenU;
}

qbVector<double> RT::Camera::GetV()
{
	return m_projectionScreenV;
}

qbVector<double> RT::Camera::GetScreenCenter()
{
	return m_projectionScreenCenter;
}

double RT::Camera::GetLength()
{
	return m_cameraLength;
}

double RT::Camera::GetHorzSize()
{
	return m_cameraHorzSize;
}

double RT::Camera::GetAspect()
{
	return m_cameraAspectRatio;
}

void RT::Camera::UpdateCameraGeometry()
{
	//vector betreen cam and lookat
	m_alignmentVector = m_cameraLookAt - m_cameraPosition;
	m_alignmentVector.Normalize();

	//U and V
	m_projectionScreenU = qbVector<double>::cross(m_alignmentVector, m_cameraUp);
	m_projectionScreenU.Normalize();
	m_projectionScreenV = qbVector<double>::cross(m_projectionScreenU, m_alignmentVector);

	//center of screen
	m_projectionScreenCenter = m_cameraPosition + (m_cameraLength * m_alignmentVector);

	//modify u and v to match size and aspect
	m_projectionScreenU = m_projectionScreenU * m_cameraHorzSize;
	m_projectionScreenV = m_projectionScreenV * (m_cameraHorzSize / m_cameraAspectRatio);
}

RT::Ray RT::Camera::GenerateRay(float proScreenX, float proScreenY)
{
	//screen point in world coords
	qbVector<double> screenWorldPart1 = m_projectionScreenCenter + (m_projectionScreenU * proScreenX);
	qbVector<double> screenWorldCoordinate = screenWorldPart1 + (m_projectionScreenV * proScreenY);

	//use this point with camera pos to compute ray
	return RT::Ray(m_cameraPosition, screenWorldCoordinate);
}
