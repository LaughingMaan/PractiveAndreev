// PractiveAndreev.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include <codecvt>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <filesystem>
#include <algorithm>
#include <json.hpp>
#include <tchar.h>
#include "HttpRequest.h"
#include "MyJsonData.h"
#include "sqlite-amalgamation-3350500/sqlite3.h"
using namespace nlohmann;
using namespace ns;
typedef   std::basic_string<TCHAR>  T_string;

void swap(Logs* a, Logs* b)
{
	Logs t = *a;
	*a = *b;
	*b = t;
}

int partition(MyJsonData* arr, int low, int high) {
	Logs pivot = arr->myLogs[high];
	int i = (low - 1);
	for (int j = low; j <= high - 1; j++)
	{
		// If current element is smaller than the pivot
		if (arr->myLogs[j].DateTime < pivot.DateTime)
		{
			i++; // increment index of smaller element
			swap(&arr->myLogs[i], &arr->myLogs[j]);
		}
	}
	swap(&arr->myLogs[i + 1], &arr->myLogs[high]);
	return (i + 1);
}

void quickSort(MyJsonData* arr, int low, int high) {
	if (low < high) {
		int pi = partition(arr, low, high);
		quickSort(arr, low, pi - 1);
		quickSort(arr, pi + 1, high);
	}
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");
	if (argc > 1) {
		printf("%s\n", argv[1]);
		HttpRequest test = HttpRequest(argv[1]);
		json j = json::parse(test.responseData);
		MyJsonData data = MyJsonData();
		data.Error = j["error"].get<std::string>();
		if (!strcmp(data.Error.c_str(), "")) {
			for (const auto testat : j["logs"]) {
				data.myLogs.push_back(testat.get<Logs>());
			}
			quickSort(&data, 0, static_cast<int>(data.myLogs.size() - 1));
			sqlite3* db = 0;
			std::string path = std::filesystem::current_path().string();
			path.append("\\Logs.db");
			if (!sqlite3_open_v2(path.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_URI, NULL)) {
				char* err = 0;
				const char* SQL = u8"INSERT INTO Logs VALUES(?,?,?,?,?,?)";
				const char* tail = 0;
				sqlite3_stmt* ppStmt = 0;
				const int TIME_STRING_LENGTH = 20;
				char buffer[TIME_STRING_LENGTH];
				for (auto temp : data.myLogs) {
					if (SQLITE_OK == sqlite3_prepare_v2(db, SQL, -1, &ppStmt, &tail)) {
						sqlite3_bind_null(ppStmt, 1);
						sqlite3_bind_int(ppStmt, 2, std::stoi(temp.user_id));
						sqlite3_bind_text(ppStmt, 3, temp.FirstName.c_str(), static_cast<int>(temp.FirstName.size() * sizeof(T_string::value_type)), SQLITE_STATIC);
						sqlite3_bind_text(ppStmt, 4, temp.SecondName.c_str(), static_cast<int>(temp.SecondName.size() * sizeof(T_string::value_type)), SQLITE_STATIC);
						strftime(buffer, TIME_STRING_LENGTH, "%Y-%m-%d %H:%M:%S", temp.Time);
						sqlite3_bind_text(ppStmt, 5, buffer, -1, SQLITE_TRANSIENT);
						sqlite3_bind_text(ppStmt, 6, temp.Message.c_str(), static_cast<int>(temp.Message.size() * sizeof(T_string::value_type)), SQLITE_STATIC);
						sqlite3_step(ppStmt);
						sqlite3_clear_bindings(ppStmt);
						sqlite3_reset(ppStmt);
					}
					else fprintf(stderr, "не могу подготовить выражение: %s\n", sqlite3_errmsg(db));
				}
				sqlite3_close(db);
			}
			else fprintf(stderr, "Не могу открыть бд: %s\n", sqlite3_errmsg(db));
		}
	}
	else fprintf_s(stdout, "Неверная дата, не могу получить данные");
	return 0;
}