#pragma once

struct Configuration
{
	Configuration(int framerate, float width, float height, float gravity)
		: framerate(framerate), width(width), height(height), gravity(gravity), current_time{0} {}

	int framerate;
	float width;
	float height;
	float gravity;
	float current_time;
};
