#pragma once

#include <string>
#include <vector>
#include <SDL.h>

class Image
{
public:
	Image();
	~Image();

	void Initialize(const int xSize, const int ySize, SDL_Renderer* pRenderer);
	void SetPixel(const int x, const int y, const double red, const double green, const double blue);
	void Display();

	int GetXSize();
	int GetYSize();

private:
	Uint32 ConvertColor(const double red, const double green, const double blue);
	void InitTexture();
	void ComputeMaxValues();

private:
	std::vector<std::vector<double>> m_rChannel;
	std::vector<std::vector<double>> m_gChannel;
	std::vector<std::vector<double>> m_bChannel;

	int m_xSize, m_ySize;

	//max values
	double m_maxRed, m_maxGreen, m_maxBlue, m_overallMax;

	SDL_Renderer* m_pRenderer;
	SDL_Texture* m_pTexture;
};

