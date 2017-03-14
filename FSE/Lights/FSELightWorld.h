#pragma once

#include "../FSEObject/FSEObject.h"

#include "../../LTBL2/src/LightSystem.hpp"

namespace fse
{
	/*!
	 * \brief Class holds and manages LTBL2 Light System \n 
	 * Default z-order = 255
	 */
	class FSELightWorld : public FSEObject
	{
	public:
		FSELightWorld(Scene* scene);
		FSELightWorld(Scene* scene, const sf::Vector2f& spawnPos);
		~FSELightWorld();

		void update(float deltaTime) override;
		void draw(sf::RenderTarget& target) override;
		void spawned() override;

		bool destroy() override;

		/*!
		 * \brief initializes Lighting system
		 * \param target Ptr to RenderTarget 
		 */
		void init(sf::RenderTarget* target) const;

		/*!
		 * \return lighting active
		 */
		bool getLighting() const;
		/*!
		 * \brief Enable / disable lighting
		 */
		void setLighting(bool lighting);


		/*!
		 * \brief Update view for light system
		 */
		void updateView();

		
		bool getBloom() const;
		void setBloom(bool bloom);


		/*!
		 * \brief Get ambient (==unlit) shadow color
		 * \return ambient color
		 */
		sf::Color getAmbientColor() const;
		/*!
		 * \brief Set ambient (==unlit) shadow color
		 * \param color color
		 */
		void setAmbientColor(const sf::Color color) const;

		/*!
		 * \brief get directional sunlight
		 * Returns a ptr to or creates and returns a ptr to directional sunlight \n 
		 * Is very performance heavy when using lots of light shapes
		 * \return sunlight
		 */
		ltbl::LightDirectionEmission* getSun();

		/*!
		 * \brief Get LTBL2 Light system
		 * \return Ptr to LTBL2 light system
		 */
		ltbl::LightSystem* getLightSystem() const;

		sf::RenderTarget* getNormalTarget();
		sf::RenderTarget* getSpecularTarget();


	private:

		sf::RenderTexture normal_texture_;
		sf::RenderTexture specular_texture_;

		sf::RenderTexture bloom_texture_;

		std::unique_ptr<ltbl::LightSystem> light_system_;
		bool lighting_ = true;

		bool bloom_ = false;

		ltbl::LightDirectionEmission* sun_ = nullptr;

		sf::Shader bloom_shader_;
		sf::Shader gauss_blur_shader_;

		RTTR_ENABLE(fse::FSEObject)
		RTTR_REGISTRATION_FRIEND


		const std::string bloom_shader_str_ = "" \
		"uniform sampler2D currTex;" \
		"uniform sampler2D lightCompTex;" \
		"uniform sampler2D specCompTex;" \
		"" \
		"void main()" \
		"{" \
		"	vec2 coord = gl_TexCoord[0].xy;" \
		"	vec4 frColor = texture2D(currTex, coord);" \
		"	vec4 lColor = texture2D(lightCompTex, coord);" \
		"	lColor += texture2D(specCompTex, coord);" \
		"	float lstrength = clamp(dot(lColor.rgb, vec3(0.2126, 0.7152, 0.0722)), 0.0, 1.0);" \
		"	float brightness = dot(frColor.rgb, vec3(0.2126, 0.7152, 0.0722));" \
		"	if (lstrength > 0.1)" \
		"	{" \
		"		gl_FragColor = vec4(lColor.rgb, lstrength * brightness);" \
		"	}" \
		"}";
		const std::string gauss_blur_shader_str_ = "" \
		"uniform sampler2D currTex;" \
		"uniform bool horizontal;" \
		"uniform vec2 texSize; " \
		"const float weight[5] = float[5](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);" \
		"" \
		"void main() {" \
		"	vec2 coord = gl_TexCoord[0].xy;" \
		"	vec2 tex_offset = 1.0 / texSize;" \
		"	vec3 result = texture(currTex, coord).rgb * weight[0];" \
		"	if (horizontal)" \
		"	{" \
		"		for(int i = 1; i < 5; ++i)" \
		"		{" \
		"		   result += texture(currTex, coord + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];" \
		"		   result += texture(currTex, coord - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];" \
		"		}" \
		"	}" \
		"	else" \
		"	{" \
		"		for(int i = 1; i < 5; ++i)" \
		"		{" \
		"		    result += texture(currTex, coord + vec2(0.0, tex_offset.y * i)).rgb * weight[i];" \
		"		    result += texture(currTex, coord - vec2(0.0, tex_offset.y * i)).rgb * weight[i];" \
		"		}" \
		"	}" \
		"	gl_FragColor = vec4(result, 1.0);" \
		"}";

	};
}
