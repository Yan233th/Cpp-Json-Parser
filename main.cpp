#include <fstream>
#include <map>
#include <unordered_map>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <iostream>
#include <variant>
#include <vector>

#define endl '\n'

class JsonObject
{
    public:
    std::variant<std::monostate, bool, int, double, std::string, std::vector<JsonObject>, std::unordered_map<std::string, JsonObject>> Data;
};

std::optional<std::string> GetJsonStrData (std::string_view JsonPath)
{
    std::ifstream TempFile (JsonPath.data ());
    std::stringstream TempBuffer{};
    if (TempFile.is_open ())
    {
        TempBuffer << TempFile.rdbuf ();
        TempFile.close ();
        std::cout << "Read Successful" << endl;
        return TempBuffer.str ();
    }
    else
    {
        std::cout << "Read Failed" << endl;
        return {};
    }
}

JsonObject JsonParser (std::string_view JsonStrData)
{
    JsonObject current = JsonParser(JsonStrData);
    return current;
}

int main ()
{
    std::string JsonPath;
    std::cin >> JsonPath;
    std::optional<std::string> JsonStrData = GetJsonStrData(JsonPath);
    if (JsonStrData.has_value())
    {
        JsonObject main = JsonParser (JsonStrData.value());
    }
    else return 1;
    return 0;
}
