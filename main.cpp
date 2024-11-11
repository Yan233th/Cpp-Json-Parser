#include <fstream>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <print>

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

int main()
{
    return 0;
}