/*
 *	Author: JeongHak Kim
 *
 *	File_name: ShaderPaths.h
 *
 *	Contains shader file paths
 *
 *	2025. 12. 08
 */

#pragma once
#include <string>

namespace ShaderPaths
{
	const std::string commonVS = "../assets/shaders/CommonGeometry.vs";
	const std::string textureVS = "../assets/shaders/Texture.vs";
	const std::string cubemapVS = "../assets/shaders/Cubemap.vs";
	const std::string transformVS = "../assets/shaders/SimpleTransform.vs";

	// Fragments shader for demos
	const std::string pbrFS = "../assets/shaders/PBR.fs";
	const std::string texturePbrFS = "../assets/shaders/PBR_Textured.fs";
	const std::string phongFS = "../assets/shaders/Phong.fs";
	const std::string skyboxFS = "../assets/shaders/Skybox.fs";

	// IBL
	const std::string equirectangular2cubemapFS = "../assets/shaders/Equirectangular.fs";
	const std::string convolutionFS = "../assets/shaders/Convolution.fs";
	const std::string prefilterFS = "../assets/shaders/Prefilter.fs";
	const std::string brdfLutFS = "../assets/shaders/BRDF_LUT.fs";


} // namespace ShaderPaths