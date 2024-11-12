#include <fstream>
#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <print>
#include <unordered_map>
#include <variant>
#include <vector>

class JsonObject
{
    public:
    std::variant<std::monostate, bool, int, double, std::string,std::vector<JsonObject>,std::unordered_map<std::string, JsonObject>> Data;
};

std::optional<std::string> GetJsonStrData(std::string_view JsonPath)
{
    std::ifstream TempFile(JsonPath.data());
    std::stringstream TempBuffer{};
    if (TempFile.is_open()) {
        TempBuffer << TempFile.rdbuf();
        TempFile.close();
        std::print("Read Successful");
        return TempBuffer.str();
    }else{
        std::print("Read Failed");
        return {};
    }
}

std::optional<JsonObject> JsonParser(std::string_view JsonPath)
{
    std::optional<std::string> JsonStrData = GetJsonStrData(JsonPath);
    if(!JsonStrData.has_value()){
        //获取json文件失败
        return {};
    }else {
        //获取json文件成功

    }
}

int main()
{
    return 0;
}