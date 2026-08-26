/*
 *	Author: JeongHak Kim
 *	File_name: GPUTimer.h
 *
 *	Measure GPU side frame time
 *
 *	2026. 08. 02
 */

#pragma once
#include <glew.h>

class GPUTimer
{
public:
	GPUTimer() { glGenQueries(2, queryIDs); }
	~GPUTimer() { glDeleteQueries(2, queryIDs); }

	void Begin() { glBeginQuery(GL_TIME_ELAPSED, queryIDs[currentFrame]); }
	void End() { glEndQuery(GL_TIME_ELAPSED); }

	float GetElapsedTime()
	{
		// gputime unit == nanoseconds
		GLuint64 gpuTime = 0;
		glGetQueryObjectui64v(queryIDs[1 - currentFrame], GL_QUERY_RESULT, &gpuTime);
		currentFrame = 1 - currentFrame;
		return gpuTime / 1'000'000.f;
	}

private:
	int currentFrame = 0;
	GLuint queryIDs[2];

};
