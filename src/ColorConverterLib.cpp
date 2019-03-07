/***************************************************
Copyright (c) 2017 Luis Llamas
(www.luisllamas.es)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/
 
#include "RGBConverterLib.h"

void RGBConverter::RgbToHsv(uint8_t red, uint8_t green, uint8_t blue, double& hue, double& saturation, double& value)
{
	auto rd = static_cast<double>(red) / 255;
	auto gd = static_cast<double>(green) / 255;
	auto bd = static_cast<double>(blue) / 255;
	auto max = threeway_max(rd, gd, bd), min = threeway_min(rd, gd, bd);
	 
	value = max;

	auto d = max - min;
	saturation = max == 0 ? 0 : d / max;

	hue = 0;
	if (max != min)
	{
		if (max == rd)
		{
			hue = (gd - bd) / d + (gd < bd ? 6 : 0);
		}
		else if (max == gd)
		{
			hue = (bd - rd) / d + 2;
		}
		else if (max == bd)
		{
			hue = (rd - gd) / d + 4;
		}
		hue /= 6;
	}
}


void RGBConverter::RgbToHsl(uint8_t red, uint8_t green, uint8_t blue, double& hue, double& saturation, double& lighting)
{
	auto rd = static_cast<double>(red) / 255;
	auto gd = static_cast<double>(green) / 255;
	auto bd = static_cast<double>(blue) / 255;

	auto max = threeway_max(rd, gd, bd);
	auto min = threeway_min(rd, gd, bd);

	double h, s, l = (max + min) / 2;

	if (max == min)
	{
		h = s = 0; // achromatic
	}
	else
	{
		auto d = max - min;
		s = l > 0.5 ? d / (2 - max - min) : d / (max + min);
		if (max == rd)
		{
			h = (gd - bd) / d + (gd < bd ? 6 : 0);
		}
		else if (max == gd)
		{
			h = (bd - rd) / d + 2;
		}
		else if (max == bd)
		{
			h = (rd - gd) / d + 4;
		}
		h /= 6;
	}

	hue = h;
	saturation = s;
	lighting = l;
}

void RGBConverter::HsvToRgb(double hue, double saturation, double value, uint8_t& red, uint8_t& green, uint8_t& blue)
{
	double r, g, b;

	auto i = static_cast<int>(hue * 6);
	auto f = hue * 6 - i;
	auto p = value * (1 - saturation);
	auto q = value * (1 - f * saturation);
	auto t = value * (1 - (1 - f) * saturation);

	switch (i % 6)
	{
	case 0: r = value , g = t , b = p;
		break;
	case 1: r = q , g = value , b = p;
		break;
	case 2: r = p , g = value , b = t;
		break;
	case 3: r = p , g = q , b = value;
		break;
	case 4: r = t , g = p , b = value;
		break;
	case 5: r = value , g = p , b = q;
		break;
	}

	red = static_cast<uint8_t>(r * 255);
	green = static_cast<uint8_t>(g * 255);
	blue = static_cast<uint8_t>(b * 255);
}


void RGBConverter::HslToRgb(double hue, double saturation, double lightness, uint8_t& red, uint8_t& green, uint8_t& blue)
{
	double r, g, b;

	if (saturation == 0)
	{
		r = g = b = lightness; // achromatic
	}
	else
	{
		auto q = lightness < 0.5 ? lightness * (1 + saturation) : lightness + saturation - lightness * saturation;
		auto p = 2 * lightness - q;
		r = hue2rgb(p, q, hue + 1 / 3.0);
		g = hue2rgb(p, q, hue);
		b = hue2rgb(p, q, hue - 1 / 3.0);
	}

	red = static_cast<uint8_t>(r * 255);
	green = static_cast<uint8_t>(g * 255);
	blue = static_cast<uint8_t>(b * 255);
}

void RGBConverter::TemperatureToRgb(int kelvin, uint8_t& red, uint8_t& green, uint8_t& blue)
{
	auto temp = kelvin / 100;

	if (temp <= 66)
	{
		red = 255;
		green = 99.4708025861 * log(temp) - 161.1195681661;

		if (temp <= 19)
		{
			blue = 0;
		}
		else
		{
			blue = 138.5177312231 * log(temp - 10) - 305.0447927307;
		}
	}
	else
	{
		red = 329.698727446 * pow(temp - 60, -0.1332047592);
		green = 288.1221695283 * pow(temp - 60, -0.0755148492);
		blue = 255;
	}
}

void RGBConverter::HexToRgb(String hex, uint8_t& red, uint8_t& green, uint8_t& blue)
{
	long number = strtoll(&hex[0], NULL, 16);
	red = number >> 16;
	green = number >> 8 & 0xFF;
	blue = number & 0xFF;
}

void RGBConverter::RgbToHex(uint8_t red, uint8_t green, uint8_t blue, String &hex)
{
	char hexArray[6] = { 0 };
	sprintf(hexArray, "%02X%02X%02X", red, green, blue);
	hex = hexArray;
}


double inline RGBConverter::threeway_max(double a, double b, double c)
{
	return max(a, max(b, c));
}

double inline RGBConverter::threeway_min(double a, double b, double c)
{
	return min(a, min(b, c));
}

double RGBConverter::hue2rgb(double p, double q, double t)
{
	if (t < 0) t += 1;
	if (t > 1) t -= 1;
	if (t < 1 / 6.0) return p + (q - p) * 6 * t;
	if (t < 1 / 2.0) return q;
	if (t < 2 / 3.0) return p + (q - p) * (2 / 3.0 - t) * 6;
	return p;
}

