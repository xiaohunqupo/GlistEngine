/*
 * gScreenSpaceReflections.cpp
 *
 *  Created on: 10 Nis 2026
 *      Author: Ruslan Akif Guven
 */

#include "gScreenSpaceReflections.h"

gScreenSpaceReflections::gScreenSpaceReflections(float reflectivity, float horizonline,
	float fresnelpower, float distortionscale, float distortionspeed,
	int blursamples, float blurspread) {
	this->reflectivity   = reflectivity;
	this->horizonline    = horizonline;
	this->fresnelpower   = fresnelpower;
	this->distortionscale = distortionscale;
	this->distortionspeed = distortionspeed;
	this->blursamples    = blursamples;
	this->blurspread     = blurspread;

	shader = new gShader();
	shader->loadProgram(getVertSrc(), getFragSrc());
	shader->use();
	shader->setFloat("reflectivity",    this->reflectivity);
	shader->setFloat("horizonline",     this->horizonline);
	shader->setFloat("fresnelpower",    this->fresnelpower);
	shader->setFloat("distortionscale", this->distortionscale);
	shader->setFloat("distortionspeed", this->distortionspeed);
	shader->setInt("blursamples",       this->blursamples);
	shader->setFloat("blurspread",      this->blurspread);
	shader->setFloat("time",            0.0f);
}

gScreenSpaceReflections::~gScreenSpaceReflections() {
	delete shader;
}

void gScreenSpaceReflections::use() {
	shader->use();
}

void gScreenSpaceReflections::setTime(float time) {
	shader->use();
	shader->setFloat("time", time);
}

void gScreenSpaceReflections::setReflectivity(float reflectivity) {
	this->reflectivity = reflectivity;
	shader->use();
	shader->setFloat("reflectivity", reflectivity);
}

void gScreenSpaceReflections::setHorizonLine(float horizonline) {
	this->horizonline = horizonline;
	shader->use();
	shader->setFloat("horizonline", horizonline);
}

void gScreenSpaceReflections::setFresnelPower(float fresnelpower) {
	this->fresnelpower = fresnelpower;
	shader->use();
	shader->setFloat("fresnelpower", fresnelpower);
}

void gScreenSpaceReflections::setDistortionScale(float distortionscale) {
	this->distortionscale = distortionscale;
	shader->use();
	shader->setFloat("distortionscale", distortionscale);
}

void gScreenSpaceReflections::setDistortionSpeed(float distortionspeed) {
	this->distortionspeed = distortionspeed;
	shader->use();
	shader->setFloat("distortionspeed", distortionspeed);
}

void gScreenSpaceReflections::setBlurSamples(int blursamples) {
	this->blursamples = blursamples;
	shader->use();
	shader->setInt("blursamples", blursamples);
}

void gScreenSpaceReflections::setBlurSpread(float blurspread) {
	this->blurspread = blurspread;
	shader->use();
	shader->setFloat("blurspread", blurspread);
}

float gScreenSpaceReflections::getReflectivity()    const { return reflectivity; }
float gScreenSpaceReflections::getHorizonLine()     const { return horizonline; }
float gScreenSpaceReflections::getFresnelPower()    const { return fresnelpower; }
float gScreenSpaceReflections::getDistortionScale() const { return distortionscale; }
float gScreenSpaceReflections::getDistortionSpeed() const { return distortionspeed; }
int   gScreenSpaceReflections::getBlurSamples()     const { return blursamples; }
float gScreenSpaceReflections::getBlurSpread()      const { return blurspread; }

const std::string gScreenSpaceReflections::getVertSrc() {
	const char* shadersource =
		"#version 330 core\n"
		"layout (location = 0) in vec2 aPos;"
		"layout (location = 1) in vec2 aTexCoords;"
		""
		"out vec2 TexCoords;"
		""
		"void main()"
		"{"
		"    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);"
		"    TexCoords = aTexCoords;"
		"}\n";
	return std::string(shadersource);
}

const std::string gScreenSpaceReflections::getFragSrc() {
	const char* shadersource =
		"#version 330 core\n"
		"out vec4 FragColor;"
		""
		"in vec2 TexCoords;"
		""
		"uniform sampler2D screenTexture;"
		""
		"uniform float reflectivity;"     /* overall blend strength              */
		"uniform float horizonline;"      /* Y where floor starts (NDC 0-1)      */
		"uniform float fresnelpower;"     /* Schlick exponent for angle falloff  */
		"uniform float distortionscale;"  /* ripple amplitude                    */
		"uniform float distortionspeed;"  /* ripple animation speed              */
		"uniform int   blursamples;"      /* soft-reflection sample count        */
		"uniform float blurspread;"       /* UV radius of blur kernel            */
		"uniform float time;"             /* elapsed time for ripple animation   */
		""
		"float hash(vec2 p)"
		"{"
		"    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453);"
		"}"
		""
		"float noise(vec2 p)"
		"{"
		"    vec2 i = floor(p);"
		"    vec2 f = fract(p);"
		"    vec2 u = f * f * (3.0 - 2.0 * f);"
		"    return mix(mix(hash(i + vec2(0.0,0.0)), hash(i + vec2(1.0,0.0)), u.x),"
		"               mix(hash(i + vec2(0.0,1.0)), hash(i + vec2(1.0,1.0)), u.x), u.y);"
		"}"
		""
		"void main()"
		"{"
		"    vec4 texcolor = texture(screenTexture, TexCoords);"
		""
		"    if(TexCoords.y < horizonline){"
		"        FragColor = texcolor;"
		"        return;"
		"    }"
		""
		"    vec2 mirrorUV = vec2(TexCoords.x, 2.0 * horizonline - TexCoords.y);"
		""
		"    vec2 noiseUV = TexCoords * 8.0 + vec2(0.0, time * distortionspeed);"
		"    float n  = noise(noiseUV)           * 2.0 - 1.0;"
		"    float n2 = noise(noiseUV * 1.7 + 5.3) * 2.0 - 1.0;"
		"    mirrorUV += vec2(n, n2) * distortionscale;"
		""
		"    vec4 reflectcolor = vec4(0.0);"
		"    vec2 offsets[8] = vec2[]("
		"        vec2(-1.0,  0.0), vec2( 1.0,  0.0),"
		"        vec2( 0.0, -1.0), vec2( 0.0,  1.0),"
		"        vec2(-0.7, -0.7), vec2( 0.7, -0.7),"
		"        vec2(-0.7,  0.7), vec2( 0.7,  0.7)"
		"    );"
		"    int clampedsamples = clamp(blursamples, 1, 8);"
		"    for(int i = 0; i < clampedsamples; i++){"
		"        vec2 sampleUV = mirrorUV + offsets[i] * blurspread;"
		"        sampleUV = clamp(sampleUV, 0.0, 1.0);"
		"        reflectcolor += texture(screenTexture, sampleUV);"
		"    }"
		"    reflectcolor /= float(clampedsamples);"
		""
		"    float distFromHorizon = (TexCoords.y - horizonline) / (1.0 - horizonline);"
		"    float costheta = clamp(distFromHorizon, 0.0, 1.0);"
		"    float f0 = 0.04;"
		"    float fresnel = f0 + (1.0 - f0) * pow(1.0 - costheta, fresnelpower);"
		""
		"    float blend = reflectivity * fresnel;"
		"    blend = clamp(blend, 0.0, 1.0);"
		"    FragColor = mix(texcolor, reflectcolor, blend);"
		"}";
	return std::string(shadersource);
}
