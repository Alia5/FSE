#pragma once

#include "../FSEObject/FSEObject.h"

#include "../../LTBL2/src/LightSystem.hpp"

namespace fse
{
	class Light;

	/*!
	 * \brief Class holds and manages LTBL2 Light System \n 
	 * Default z-order = 255
	 */
	class FSELightWorld : public FSEObject
	{
	public:
		FSELightWorld();
		explicit FSELightWorld(const sf::Vector2f& spawnPos);
		~FSELightWorld();

		void update(float deltaTime) override;
		void draw(sf::RenderTarget& target) override;
		void spawned() override;


		void onDespawn() override;
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

		void registerLight(fse::Light* light);
		void unregisterLight(const fse::Light* light);


		sf::RenderTexture normal_texture_;
		sf::RenderTexture specular_texture_;

		sf::RenderTexture bloom_texture_;

		std::unique_ptr<ltbl::LightSystem> light_system_;
		bool lighting_ = true;

		bool bloom_ = false;

		ltbl::LightDirectionEmission* sun_ = nullptr;

		sf::Shader bloom_shader_;
		sf::Shader gauss_blur_shader_;

		std::vector<fse::Light*> lights_;

		RTTR_ENABLE(fse::FSEObject)
		RTTR_REGISTRATION_FRIEND

		friend class fse::priv::FSEChaiLib;


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
		"const float pi = 3.14159265;" \
		"" \
		"void main() {" \
		"	const float numBlurPixelsPerSide = 4.0;" \
		"	vec2  blurMultiplyVec = vec2(0.0, 1.0);" \
		"	float sigma = 5.243;" \
		"	if (horizontal)" \
		"	{" \
		"		blurMultiplyVec = vec2(1.0, 0.0);" \
		"	}" \
		"	float blurSize = 1.0 / texSize.y;" \
		"	if (horizontal)" \
		"	{" \
		"		blurSize = 1.0 / texSize.x;" \
		"	}" \
		"	vec3 incrementalGaussian;" \
		"	incrementalGaussian.x = 1.0 / (sqrt(2.0 * pi) * sigma);" \
		"	incrementalGaussian.y = exp(-0.5 / (sigma * sigma));" \
		"	incrementalGaussian.z = incrementalGaussian.y * incrementalGaussian.y;" \
		"	vec4 avgValue = vec4(0.0, 0.0, 0.0, 0.0);" \
		"	float coefficientSum = 0.0;" \
		"	avgValue += texture2D(currTex, gl_TexCoord[0].xy) * incrementalGaussian.x;" \
		"	coefficientSum += incrementalGaussian.x;" \
		"	incrementalGaussian.xy *= incrementalGaussian.yz;" \
		"	for (float i = 1.0; i <= numBlurPixelsPerSide; i++) {" \
		"		avgValue += texture2D(currTex, gl_TexCoord[0].xy - i * blurSize *" \
		"			blurMultiplyVec) * incrementalGaussian.x;" \
		"		avgValue += texture2D(currTex, gl_TexCoord[0].xy + i * blurSize *" \
		"			blurMultiplyVec) * incrementalGaussian.x;" \
		"		coefficientSum += 2.0 * incrementalGaussian.x;" \
		"		incrementalGaussian.xy *= incrementalGaussian.yz;" \
		"	}" \
		"	gl_FragColor = avgValue / coefficientSum;" \
		"}";

	};
}
