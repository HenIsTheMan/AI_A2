#pragma once

static void RoundedTo2dp(float& var){
	var = (float)(int)(var * 100.0f + 0.5f) / 100.0f; 
}