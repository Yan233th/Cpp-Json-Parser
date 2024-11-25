#pragma once

#include "Header.hpp"

bool IsNum(char Value);

std::optional<std::string> GetJsonStrData(std::string_view JsonPath);

std::vector<std::string> GetSplitJsonData(std::string_view JsonData);

int64_t ResolveInteger(std::string_view SpiltJsonData);

double ResolveFloat(std::string_view SpiltJsonData);

std::string ResolveString(std::string_view SpiltJsonData);

bool ResolveBool(std::string_view SpiltJsonData);

JsonObject ResloveObject(std::string_view JsonStrData);

JsonObject ResolveJson(std::string_view JsonPath);