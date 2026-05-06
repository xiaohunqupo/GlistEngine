/*
 * gScreenSpaceReflections.h
 *
 *  Created on: 10 Nis 2026
 *      Author: Ruslan Akif Guven
 */

#ifndef GRAPHICS_POSTEFFECTS_GSCREENSPACEREFLECTIONS_H_
#define GRAPHICS_POSTEFFECTS_GSCREENSPACEREFLECTIONS_H_

#include "gBasePostProcess.h"

class gScreenSpaceReflections: public gBasePostProcess {
public:
	gScreenSpaceReflections(float reflectivity = 0.4f, float horizonline = 0.5f,
	float fresnelpower = 3.0f, float distortionscale = 0.02f,
	float distortionspeed = 1.0f, int blursamples = 4,
	float blurspread = 0.004f);
	virtual ~gScreenSpaceReflections();

	void use();

	void setTime(float time);

	void setReflectivity(float reflectivity);
	void setHorizonLine(float horizonline);
	void setFresnelPower(float fresnelpower);
	void setDistortionScale(float distortionscale);
	void setDistortionSpeed(float distortionspeed);
	void setBlurSamples(int blursamples);
	void setBlurSpread(float blurspread);

	float getReflectivity() const;
	float getHorizonLine() const;
	float getFresnelPower() const;
	float getDistortionScale() const;
	float getDistortionSpeed() const;
	int   getBlurSamples() const;
	float getBlurSpread() const;

protected:
	const std::string getVertSrc();
	const std::string getFragSrc();

private:
	float reflectivity;
	float horizonline;
	float fresnelpower;
	float distortionscale;
	float distortionspeed;
	int   blursamples;
	float blurspread;
};

#endif
