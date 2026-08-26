/*
 *	Author: JeongHak Kim
 *	File_name: FrameTimeGraph.h
 *
 *	For ImGui Plotline graph
 *  Manage frame times in millisecond
 * 
 *  Help profiling frame times by render the imgui graph
 *
 *	2026. 08. 01
 */

#pragma once
#include <vector>
#include <algorithm>
#include <numeric>

// imguis
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

class FrameTimeGraph
{
public:
	FrameTimeGraph(int graphSize, int avgSize)
		: graphBuffer(graphSize, 0.f)
		, graphSize(graphSize)
		, avgBuffer(avgSize, 0.f)
		, avgSize(avgSize)
	{}

	void Push(float frameTime)
	{
		avgBuffer[avgIndex] = frameTime;
		avgIndex = (avgIndex + 1) % avgSize;
		avgCount = std::min(avgCount + 1, avgSize);

		float avg = std::accumulate(avgBuffer.begin(), avgBuffer.end(), 0.f) / avgCount;
		graphBuffer[graphIndex] = avg;
		graphIndex = (graphIndex + 1) % graphSize;
		graphCount = std::min(graphCount + 1, graphSize);
	}

	void Render()
	{
		float min = *std::min_element(graphBuffer.begin(), graphBuffer.begin() + graphCount);
		float max = *std::max_element(graphBuffer.begin(), graphBuffer.begin() + graphCount);
		float avg = std::accumulate(graphBuffer.begin(), graphBuffer.begin() + graphCount, 0.f) / graphCount;
		
		char overlay[64];
		snprintf(overlay, sizeof(overlay), "avg %.2f ms", avg);

		ImGui::SetNextWindowPos(ImVec2(10, 15), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(280, 180), ImGuiCond_Once);

		ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		ImGui::Begin("Frame Time", nullptr, flags);
		ImGui::Text("CPU: %.2f ms        GPU: %.2f ms", cpuMs, gpuMs);		// cpu, gpu ms
		ImGui::Text("CPU Min: %.2f ms    CPU Max: %.2f ms", min, max);
		ImGui::Text("FPS: %.1f", 1000.f / avg); // frame time -> FPS
		ImGui::Spacing();
		ImGui::PlotLines("##frametime",
			graphBuffer.data(),
			graphCount,
			graphIndex,
			overlay,
			0.f,
			max * 1.5f,
			ImVec2(260.f, 80.f));

		ImGui::End();
	}

	float GetAvgFrameTime()
	{
		return avgBuffer[avgIndex];
	}

public:
	float cpuMs = 0.f;
	float gpuMs = 0.f;
	int	  drawCallCount = 0;

private:
	std::vector<float> graphBuffer;
	int				   graphSize, graphIndex = 0, graphCount = 0;

	std::vector<float> avgBuffer;
	int				   avgSize, avgIndex = 0, avgCount = 0;
	
};
