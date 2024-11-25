#pragma once

#include <cctype>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <ostream>
#include <span>
#include <sstream>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

class JsonObject {
  public:
    std::variant<std::monostate, bool, int64_t, double, std::string, std::vector<JsonObject>,std::unordered_map<std::string, JsonObject>> Data;
 
    JsonObject() = default;

    JsonObject(bool Value) : Data(Value){};
    JsonObject(int64_t Value) : Data(Value){};
    JsonObject(double Value) : Data(Value){};
    JsonObject(std::string Str) : Data(Str){};
};