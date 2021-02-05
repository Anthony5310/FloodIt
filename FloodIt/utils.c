#include "utils.h"

color CreateRGBColor(unsigned char p_red, unsigned char p_green, unsigned char p_blue)
{
	color result;

	result.r = p_red;
	result.g = p_green;
	result.b = p_blue;

	return result;
}

color CreateHSVColor(float p_hue, float p_saturation, float p_value)
{
	color result;

	result.h = p_hue;
	result.s = p_saturation;
	result.v = p_value;

	return result;
}

color RGB2HSV(color p_colorRGB)
{
	return p_colorRGB;
}

color HSV2RGB(color p_colorHSV)
{
	float c = p_colorHSV.v * p_colorHSV.s;

	float h2 = p_colorHSV.h / 60.0f;
	float x = c * (1.0f - absolute(mod(h2, 2.0f) - 1.0f));
	float m = p_colorHSV.v - c;

	switch ((int)h2)
	{
		case 0:
			p_colorHSV.r = c;
			p_colorHSV.g = x;
			p_colorHSV.b = 0.0f;
			break;

		case 1:
			p_colorHSV.r = x;
			p_colorHSV.g = c;
			p_colorHSV.b = 0.0f;
			break;

		case 2:
			p_colorHSV.r = 0.0f;
			p_colorHSV.g = c;
			p_colorHSV.b = x;
			break;

		case 3:
			p_colorHSV.r = 0.0f;
			p_colorHSV.g = x;
			p_colorHSV.b = c;
			break;

		case 4:
			p_colorHSV.r = x;
			p_colorHSV.g = 0.0f;
			p_colorHSV.b = c;
			break;

		case 5:
			p_colorHSV.r = c;
			p_colorHSV.g = 0.0f;
			p_colorHSV.b = x;
			break;

		default:
			p_colorHSV.r = 0.0f;
			p_colorHSV.g = 0.0f;
			p_colorHSV.b = 0.0f;
			break;
	}

	p_colorHSV.r += m;
	p_colorHSV.g += m;
	p_colorHSV.b += m;

	p_colorHSV.r = (unsigned char)(p_colorHSV.r * 255);
	p_colorHSV.g = (unsigned char)(p_colorHSV.g * 255);
	p_colorHSV.b = (unsigned char)(p_colorHSV.b * 255);

	return p_colorHSV;
}

float mod(float p_value, float p_n)
{
	return p_value - (int)(p_value / p_n) * p_n;
}

float absolute(float p_value)
{
	if (p_value > 0.0f)
		return p_value;

	return -p_value;
}