#pragma once
#include <stdint.h>
#include <iostream>
#include <string>
#include <fstream>
#include <map>

namespace My {
	struct GfxConfiguration {
		/// all-elements constructor.
		/// \param[in] r the red color depth in bits
		/// \param[in] g the green color depth in bits
		/// \param[in] b the blue color depth in bits
		/// \param[in] a the alpha color depth in bits
		/// \param[in] d the depth buffer depth in bits
		/// \param[in] s the stencil buffer depth in bits
		/// \param[in] msaa the msaa sample count
		/// \param[in] width the screen width in pixel
		/// \param[in] height the screen height in pixel
		GfxConfiguration(const char* file_path = nullptr ,uint32_t r = 8, uint32_t g = 8,
			uint32_t b = 8, uint32_t a = 8,
			uint32_t d = 32, uint32_t s = 0, uint32_t msaa = 0, uint32_t maxFPS = 60,
			uint32_t width = 600, uint32_t height = 800, const wchar_t* app_name=L"Final Project") :
			redBits(r), greenBits(g), blueBits(b), alphaBits(a),
			depthBits(d), stencilBits(s), msaaSamples(msaa), maxfps(maxFPS),
			screenWidth(width), screenHeight(height), appName(app_name)
		{
			if (file_path)
			{
				std::map<std::string, int> config_dict;
				std::ifstream input(file_path);
				if (!input.is_open())
				{
					exit(1);
				}
				else
				{
					for (std::string line; getline(input, line);)
					{
						char key[256] = { 0 };
						int value = 0;
						if (2 == sscanf_s(line.c_str(), "[%s %d", key, 256, &value))
						{
							config_dict[key] = value;
						}
					}
					if (config_dict.find("Screen_width") != config_dict.end())
					{
						screenWidth = config_dict["Screen_width"];
					}
					if (config_dict.find("Screen_height") != config_dict.end())
					{
						screenHeight = config_dict["Screen_height"];
					}
					if (config_dict.find("MAX_FPS") != config_dict.end())
					{
						maxfps = config_dict["MAX_FPS"];
					}
					input.close();
				}
			}
		}

		uint32_t redBits; ///< red color channel depth in bits
		uint32_t greenBits; ///< green color channel depth in bits
		uint32_t blueBits; ///< blue color channel depth in bits
		uint32_t alphaBits; ///< alpha color channel depth in bits
		uint32_t depthBits; ///< depth buffer depth in bits
		uint32_t stencilBits; ///< stencil buffer depth in bits
		uint32_t msaaSamples; ///< MSAA samples
		uint32_t maxfps; ///< maximum fps
		uint32_t screenWidth;
		uint32_t screenHeight;
        const wchar_t* appName;

        friend std::wostream& operator<<(std::wostream& out, const GfxConfiguration& conf)
        { 
            out << "App Name:" << conf.appName << std::endl;
            out << "GfxConfiguration:" << 
                " R:"  << conf.redBits << 
                " G:"  << conf.greenBits <<
                " B:"  << conf.blueBits <<
                " A:"  << conf.alphaBits <<
                " D:"  << conf.depthBits <<
                " S:"  << conf.stencilBits <<
                " M:"  << conf.msaaSamples <<
				" FPS: " << conf.maxfps <<
                " W:"  << conf.screenWidth <<
                " H:"  << conf.screenHeight <<
                std::endl;
            return out;
        }
	};
}

