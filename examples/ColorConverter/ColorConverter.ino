/***************************************************
Copyright (c) 2017 Luis Llamas
(www.luisllamas.es)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/
 
#include "RGBConverterLib.h"

void setup() 
{
	uint8_t red = 50;
	uint8_t green = 100;
	uint8_t blue = 150;
	double hue, saturation, lighting, value;
	
	RGBConverter::RgbToHsl(red, green, blue, hue, saturation, lighting);
	RGBConverter::RgbToHsv(red, green, blue, hue, saturation, value);
	RGBConverter::HslToRgb(hue, saturation, lighting, red, green, blue);
	RGBConverter::HsvToRgb(hue, saturation, lighting, red, green, blue);

	RGBConverter::TemperatureToRgb(15000, red, green, blue);

	String hex = "010509";
	RGBConverter::HexToRgb(hex, red, green, blue);
	RGBConverter::RgbToHex(red, green, blue, hex);
}

void loop() 
{
  
}
