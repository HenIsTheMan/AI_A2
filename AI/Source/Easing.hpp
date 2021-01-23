#pragma once

#include <algorithm>

static float EaseInQuint(const float x){
	return x * x * x * x * x;
}

static float EaseInOutSine(const float x){
	return -(cosf(Math::PI * x) - 1.0f) * 0.5f;
}

static float EaseInOutCubic(const float x){
	return x < 0.5f ? 4.0f * x * x * x : 1.0f - powf(-2.0f * x + 2.0f, 3.0f) * 0.5f;
}

static float EaseOutBounce(float x){
	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if(x < 1.0f / d1){
		return n1 * x * x;
	} else if(x < 2.0f / d1){
		return n1 * (x -= 1.5f / d1) * x + 0.75f;
	} else if(x < 2.5f / d1){
		return n1 * (x -= 2.25f / d1) * x + 0.9375f;
	} else{
		return n1 * (x -= 2.625f / d1) * x + 0.984375f;
	}
}

static float EaseInOutBounce(const float x){
	return x < 0.5f ? (1.0f - EaseOutBounce(1.0f - 2.0f * x)) * 0.5f : (1.0f + EaseOutBounce(2.0f * x - 1.0f)) * 0.5f;
}