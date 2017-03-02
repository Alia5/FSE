#pragma once

#include "../FSEObject/FSEObject.h"

#include "../../LTBL2/src/LightSystem.hpp"

namespace fse
{
	class FSELightWorld : public FSEObject
	{
	public:
		FSELightWorld(Scene* scene);
		FSELightWorld(Scene* scene, const sf::Vector2f& spawnPos);
		~FSELightWorld();

		void update(float deltaTime) override;
		void draw(sf::RenderTarget& target) override;
		void spawned() override;

		void init(sf::RenderTarget* target) const;

		bool getLighting() const;
		void setLighting(bool lighting);

		bool getBloom() const;
		void setBloom(bool bloom);

		void updateView();

		sf::Color getAmbientColor() const;
		void setAmbientColor(const sf::Color color) const;

		ltbl::LightDirectionEmission* getSun();

		ltbl::LightSystem* getLightSystem() const;

		sf::RenderTarget* getNormalTarget();
		sf::RenderTarget* getSpecularTarget();


	private:

		sf::RenderTexture normal_texture_;
		sf::RenderTexture specular_texture_;

		sf::RenderTexture bloom_texture_;
		sf::Texture b_texture_;

		std::unique_ptr<ltbl::LightSystem> light_system_;
		bool lighting_ = true;

		bool bloom_ = true;

		ltbl::LightDirectionEmission* sun_ = nullptr;

		sf::Shader bloom_shader_;
		sf::Shader gauss_blur_shader_;

		RTTR_ENABLE(fse::FSEObject)
		RTTR_REGISTRATION_FRIEND


		const std::string bloom_shader_str_ = "" \
		"uniform sampler2D currTex;" \
		"" \
		"void main()" \
		"{" \
		"	vec2 coord = gl_TexCoord[0].xy;" \
		"	vec4 frColor = texture2D(currTex, coord);" \
		"	float brightness = dot(frColor.rgb, vec3(0.2126, 0.7152, 0.0722));" \
		"	if (brightness > 0.49)" \
		"	{" \
		"		gl_FragColor = vec4(pow(frColor.rgb, vec3(3.5, 3.5, 3.5)), 1.0);" \
		"	}" \
		"	else" \
		"	{" \
		"		gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);" \
		"	}" \
		"}";
		const std::string gauss_blur_shader_str_ = "" \
		"uniform sampler2D currTex;" \
		"uniform bool horizontal;" \
		"uniform vec2 texSize; " \
		"const float pi = 3.14159265;" \
		"" \
		"void main() {" \
		"	const float numBlurPixelsPerSide = 12.0;" \
		"	vec2  blurMultiplyVec = vec2(0.0, 1.0);" \
		"	float sigma = 9.243;" \
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
		"	incrementalGaussian.x = 1.0f / (sqrt(2.0 * pi) * sigma);" \
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
		"		coefficientSum += 2 * incrementalGaussian.x;" \
		"		incrementalGaussian.xy *= incrementalGaussian.yz;" \
		"	}" \
		"	gl_FragColor = avgValue / coefficientSum;" \
		"}";

	};
}
