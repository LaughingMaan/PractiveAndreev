#pragma once
#include <Windows.h>
#include <string>
#include <json.hpp>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <list>
using nlohmann::json;
namespace ns {

	class Logs
	{
	public:
		std::time_t DateTime;
		std::string DateText; 
		struct tm*  Time;
		std::string FirstName;
		std::string Message;
		std::string SecondName;
		std::string user_id;


	private:
	};

	class Utils {
	public:

		// Convert an wide Unicode string to ANSI string
		static std::string unicode2ansi(const std::wstring& wstr)
		{
			int size_needed = WideCharToMultiByte(CP_ACP, 0, &wstr[0], -1, NULL, 0, NULL, NULL);
			std::string strTo(size_needed, 0);
			WideCharToMultiByte(CP_ACP, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
			return strTo;
		}

		// Convert an ANSI string to a wide Unicode String
		static std::wstring ansi2unicode(const std::string& str)
		{
			int size_needed = MultiByteToWideChar(CP_ACP, 0, &str[0], (int)str.size(), NULL, 0);
			std::wstring wstrTo(size_needed, 0);
			MultiByteToWideChar(CP_ACP, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
			return wstrTo;
		}
	};
	// Convert a wide Unicode string to an UTF8 string
	static std::string utf8_encode(const std::wstring& wstr)
	{
		int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
		std::string strTo(size_needed, 0);
		WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
		return strTo;
	}

	// Convert an UTF8 string to a wide Unicode String
	static std::wstring utf8_decode(const std::string& str)
	{
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
		std::wstring wstrTo(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
		return wstrTo;
	}


	// Converts UTC time string to a time_t value.
	static std::time_t getEpochTime(const std::wstring& dateTime)
	{
		// Let's consider we are getting all the input in
		// this format: '2014-07-25T20:17:22Z' (T denotes
		// start of Time part, Z denotes UTC zone).
		// A better approach would be to pass in the format as well.
		static const std::wstring dateTimeFormat{ L"%Y-%m-%dT%H:%M:%SZ" };

		// Create a stream which we will use to parse the string,
		// which we provide to constructor of stream to fill the buffer.
		std::wistringstream ss{ dateTime };

		// Create a tm object to store the parsed date and time.
		std::tm dt;

		// Now we read from buffer using get_time manipulator
		// and formatting the input appropriately.
		ss >> std::get_time(&dt, dateTimeFormat.c_str());

		// Convert the tm structure to time_t value and return.
		return std::mktime(&dt);
	}
	
	static void from_json(const json& j, Logs& logs) {
		j.at("user_id").get_to(logs.user_id);
		j.at("first_name").get_to(logs.FirstName);
		j.at("second_name").get_to(logs.SecondName);
		j.at("created_at").get_to(logs.DateText);
		j.at("message").get_to(logs.Message);
		logs.DateTime = getEpochTime(utf8_decode(logs.DateText));
		logs.Time = new tm;
		localtime_s(logs.Time ,&logs.DateTime);
	}
}