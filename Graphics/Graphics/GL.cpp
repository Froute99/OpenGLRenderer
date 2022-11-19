/*
 *	Jeonghak Kim	junghak.kim@digipen.edu
 *	CS230 Text Assignment
 *	2019 Spring
 */

#include <Graphics/Color4f.hpp>
#include <Graphics/GL.hpp>
#include <Graphics/Material.hpp>
#include <Graphics/Shader.h>
#include <Graphics/Texture.hpp>
#include <Graphics/Vertices.h>
#include <GL/glew.h>
#include <iostream>



void GL::setup() noexcept
{
	set_clear_color({ 0,0,0,1 });
	(glEnable(GL_DEPTH_TEST));
	(glEnable(GL_BLEND));
	(glBlendEquation(GL_FUNC_ADD));
	(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	enable_multiple_sampling(true);
}

void GL::begin_drawing() noexcept
{
	(glClear(GL_COLOR_BUFFER_BIT));
	(glClear(GL_DEPTH_BUFFER_BIT));
}

void GL::draw(const Vertices& vertices, const material& material) noexcept
{
	Vertices::SelectVAO(vertices);
	Shader::UseShader(*material.shader);
	for (std::pair<std::string, mat3<float>> matrix3 : material.matrix3Uniforms)
	{
		material.shader->SendUniformVariable(matrix3.first, matrix3.second);
	}
	for (std::pair<std::string, Color4f> color : material.color4fUniforms)
	{
		material.shader->SendUniformVariable(color.first, color.second);
	}
	for (std::pair<std::string, float> floatNumber : material.floatUniforms)
	{
		material.shader->SendUniformVariable(floatNumber.first, floatNumber.second);
	}
	for (std::pair<std::string, texture_uniform> textureUniform : material.textureUniforms)
	{
		Texture::SelectTextureForSlot(*textureUniform.second.texture, textureUniform.second.textureSlot);
	}

	(glDrawArrays(vertices.GetVerticesListPattern(), 0, vertices.GetVerticesCount()));
}

void GL::end_drawing() noexcept
{
	(glFinish());
}

void GL::set_clear_color(Color4f color) noexcept
{
	(glClearColor(color.r, color.g, color.b, color.a));
}

void GL::set_display_area(int pixels_width, int pixels_height, int left_x, int bottom_y) noexcept
{
	(glViewport(left_x, bottom_y, pixels_width, pixels_height));
}

void GL::set_line_width(float line_width) noexcept
{
	(glLineWidth(line_width));
}

void GL::enable_multiple_sampling(bool enable_it) noexcept
{
	if (enable_it)
	{
		(glEnable(GL_MULTISAMPLE));
	}
	else
	{
		(glDisable(GL_MULTISAMPLE));
	}
}

void GL::print_context_information()
{
	// print GL context parameters ...
	GLenum param_enums[] = {
		GL_VENDOR, // 0
		GL_RENDERER,
		GL_VERSION,
		GL_SHADING_LANGUAGE_VERSION, // 3

		GL_MAJOR_VERSION, // 4
		GL_MINOR_VERSION,
		GL_MAX_ELEMENTS_VERTICES,
		GL_MAX_ELEMENTS_INDICES,
		GL_MAX_GEOMETRY_OUTPUT_VERTICES,
		GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
		GL_MAX_CUBE_MAP_TEXTURE_SIZE,
		GL_MAX_DRAW_BUFFERS,
		GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
		GL_MAX_TEXTURE_IMAGE_UNITS,
		GL_MAX_TEXTURE_SIZE,
		GL_MAX_VARYING_FLOATS,
		GL_MAX_VERTEX_ATTRIBS,
		GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
		GL_MAX_VERTEX_UNIFORM_COMPONENTS, // 18

		GL_MAX_VIEWPORT_DIMS, // 19

		GL_STEREO // 20
	};

	char const* param_names[] = {
		"GL_VENDOR", // 0
		"GL_RENDERER",
		"GL_VERSION",
		"GL_SHADING_LANGUAGE_VERSION", // 3

		"GL_MAJOR_VERSION", // 4
		"GL_MINOR_VERSION",
		"GL_MAX_ELEMENTS_VERTICES",
		"GL_MAX_ELEMENTS_INDICES",
		"GL_MAX_GEOMETRY_OUTPUT_VERTICES",
		"GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
		"GL_MAX_CUBE_MAP_TEXTURE_SIZE",
		"GL_MAX_DRAW_BUFFERS",
		"GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
		"GL_MAX_TEXTURE_IMAGE_UNITS",
		"GL_MAX_TEXTURE_SIZE",
		"GL_MAX_VARYING_FLOATS",
		"GL_MAX_VERTEX_ATTRIBS",
		"GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
		"GL_MAX_VERTEX_UNIFORM_COMPONENTS", // 18

		"GL_MAX_VIEWPORT_DIMS", // 19

		"GL_STEREO" // 20
	};

	// C-strings for 1st four parameters
	std::cout << "GL version information and context parameters: " << std::endl;
	int i = 0;
	for (i = 0; i < 4; ++i)
	{
		GLubyte const* param_str = glGetString(param_enums[i]);
		std::cout << param_names[i] << ": " << param_str << std::endl;
	}

	// one integer for next set of fifteen parameters
	for (; i < 19; ++i)
	{
		GLint val;
		(glGetIntegerv(param_enums[i], &val));
		std::cout << param_names[i] << ": " << val << std::endl;
	}

	// two integers for next parameter
	GLint dim[2];
	(glGetIntegerv(param_enums[19], dim));
	std::cout << param_names[19] << ": " << dim[0] << " x " << dim[1] << std::endl;

	// bool for next parameter
	GLboolean flag;
	(glGetBooleanv(param_enums[20], &flag));
	std::cout << param_names[20] << ": " << static_cast<GLint>(flag) << std::endl;
}
