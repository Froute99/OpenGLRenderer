/*
 *	Author: JeongHak Kim
 *	File_name: Log.h
 *
 *	Manage log messages and print them.
 *
 *	2026. 07. 31
 */

#pragma once
#include <string>
#include <vector>
#include <iostream>

class Log
{
public:
	static void Add(const std::string& s)
	{
		logs.push_back(s);
	}

	static void Flush()
	{
		// \033[NA		N = number
		if (lineCount > 0)
		{
			std::cout << "\033[" << lineCount << "A";
		}

		lineCount = 0;
		for (const std::string& s : logs)
		{
			std::cout << "\033[2K" << s << "\n";
			++lineCount;
		}
		logs.clear();
	}

private:
	inline static std::vector<std::string> logs;
	inline static int					   lineCount = 0;

};
