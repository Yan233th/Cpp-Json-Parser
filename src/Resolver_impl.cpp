#include "JsonObject.hpp"

bool IsNum(char Value)
{
    return (((uint64_t)Value - (uint64_t)'0') <= ((uint64_t)'9' - (uint64_t)'0'));
}

std::optional<std::string> GetJsonStrData(std::string_view JsonPath)
{
    std::ifstream TempFile(JsonPath.data());
    std::stringstream TempBuffer{};
    if (TempFile.is_open()) {
        TempBuffer << TempFile.rdbuf();
        TempFile.close();
        std::cout << "Read Successful" << '\n';
        return TempBuffer.str();
    } else {
        std::cout << "Read Failed" << '\n';
        return {};
    }
}

std::vector<std::string> GetSplitJsonData(std::string_view JsonData)
{
    std::vector<std::string> JsonChunkList;
    std::stack<char> Symbol;
    std::string TempStr{};
    //update 
    for (std::string_view::const_iterator cIter = JsonData.cbegin(); cIter != JsonData.cend(); cIter++) {
        char TempChar = *cIter;
        if (TempChar == '[' or TempChar == '{' or (TempChar == '"' and Symbol.top() != '"')) {
            Symbol.push(TempChar);
        } else if (TempChar == Symbol.top() + 2 or (TempChar == '"' and Symbol.top() == '"')) {
            Symbol.pop();
        }
        if ((!Symbol.empty() and Symbol.top() != '"') and (TempChar == ',' and Symbol.size() == 1)) {
            JsonChunkList.push_back(TempStr);
            TempStr.clear();
        } else {
            if ((!Symbol.empty() and Symbol.top() != '"') and (*cIter != ' ' and *cIter != '\n' and !(cIter == JsonData.cbegin() or cIter == JsonData.cend() - 1)))
                TempStr.push_back(*cIter);
            else if((!Symbol.empty() and Symbol.top() == '"'))
                TempStr.push_back(*cIter);
        }
    }
    JsonChunkList.push_back(TempStr);
    TempStr.clear();
    return JsonChunkList;
}

int64_t ResolveInteger(std::string_view SpiltJsonData)
{
    size_t Length = SpiltJsonData.length();
    int64_t RetNum = 0;
    size_t Exp = 0;
    int NegativeFlag = 1;
    for (std::string_view::const_reverse_iterator cIter = SpiltJsonData.crbegin(); cIter != SpiltJsonData.crend(); cIter++) {
        if (*cIter == '-') {
            NegativeFlag = -1;
        } else {
            RetNum += (*cIter - '0') * std::pow(10, Exp);
            Exp++;
        }
    }
    return RetNum * NegativeFlag;
}

double ResolveFloat(std::string_view SpiltJsonData)
{
    size_t Length = SpiltJsonData.length();
    size_t Exp = 0;
    double RetNum = 0;
    int NegativeFlag = 1;

    size_t eIndex = SpiltJsonData.find('e');
    if (SpiltJsonData.npos == eIndex) {
        //.
        int Index = 0;
        for (std::string_view::const_reverse_iterator cIter = SpiltJsonData.crbegin(); cIter != SpiltJsonData.crend(); cIter++) {
            if (*cIter != '.' and *cIter != '-') {
                RetNum += (*cIter - '0') * std::pow(10, Exp);
                Exp++;
            } else if (*cIter == '-') {
                NegativeFlag = -1;
            } else {
                Index = Exp;
            }
        }
        RetNum /= std::pow(10, Index) * NegativeFlag;
    } else {
        // e
        // resolve the shit in front of e
        for (int i = eIndex - 1; i >= 0; i--) {
            if (i == 0 and SpiltJsonData[i] == '-') {
                NegativeFlag = -1;
            } else {
                RetNum += (SpiltJsonData[i] - '0') * std::pow(10, Exp);
                Exp++;
            }
        }
        RetNum *= NegativeFlag;

        // resolve the shit behind e
        Exp = 0;
        NegativeFlag = 1;
        int RetExp = 0;
        for (int i = Length - 1; i > eIndex; i--) {
            if (SpiltJsonData[i] == '-') {
                NegativeFlag = -1;
            } else {
                RetExp += (SpiltJsonData[i] - '0') * std::pow(10, Exp);
                Exp++;
            }
        }
        RetExp *= NegativeFlag;
        return RetNum * std::pow(10, RetExp);
    }
    //需要支持同时有e和.的eg. 1.1e-10
    return RetNum;
}

std::string ResolveString(std::string_view SpiltJsonData)
{
    //转译符号需要增加支持
    return std::string(SpiltJsonData.cbegin() + 1,SpiltJsonData.cend() - 1);
}

bool ResolveBool(std::string_view SpiltJsonData)
{
    if(SpiltJsonData[0] == 't')
        return true;
    else
        return false;
}

JsonObject ResloveObject(std::string_view JsonStrData)
{
    std::vector<std::string> SplitJsonData = GetSplitJsonData(JsonStrData);
    JsonObject JsonData;
    std::unordered_map<std::string, JsonObject> TempMap;
    JsonData.Data = TempMap;
    for (std::vector<std::string>::const_iterator cVecIter = SplitJsonData.cbegin(); cVecIter != SplitJsonData.cend(); cVecIter++) {
        size_t Index = cVecIter->find(":");
        char NextChar = cVecIter->at(Index + 1);
        if (IsNum(NextChar) or NextChar == '-') {
            // num
            bool IsFloat = true;
            if (cVecIter->npos == cVecIter->find('.') and cVecIter->npos == cVecIter->find('e'))
                IsFloat = false;
            if (IsFloat) {
                TempMap.insert(
                    {std::string(cVecIter->cbegin() + 1, cVecIter->cbegin() + Index - 1),
                     JsonObject(ResolveFloat(std::string_view(cVecIter->cbegin() + Index + 1, cVecIter->cend())))});
            } else {
                TempMap.insert(
                    {std::string(cVecIter->cbegin() + 1, cVecIter->cbegin() + Index - 1),
                     JsonObject(ResolveInteger(std::string_view(cVecIter->cbegin() + Index + 1, cVecIter->cend())))});
            }
        } else if (NextChar == '"') {
            // string
            TempMap.insert(
                {std::string(cVecIter->cbegin() + 1, cVecIter->cbegin() + Index - 1),
                 JsonObject(ResolveString(std::string_view(cVecIter->cbegin() + Index + 1, cVecIter->cend())))});
        } else if (NextChar == 't' or NextChar == 'f') {
            // bool
            TempMap.insert({std::string(cVecIter->cbegin() + 1, cVecIter->cbegin() + Index - 1),
                 JsonObject(ResolveBool(std::string_view(cVecIter->cbegin() + Index + 1, cVecIter->cend())))});
        } else if (NextChar == '[') {
            // array
            // TempMap.insert({std::string(cVecIter->cbegin() + 1, cVecIter->cbegin() + Index - 1),
            //      JsonObject(ResolveArray(std::string_view(cVecIter->cbegin() + Index + 1, cVecIter->cend())))});
        } else if (NextChar == '{') {
            // object
            TempMap.insert({std::string(cVecIter->cbegin() + 1, cVecIter->cbegin() + Index - 1),
                 JsonObject(ResloveObject(std::string_view(cVecIter->cbegin() + Index + 1, cVecIter->cend())))});
        }
    }
    JsonData.Data = TempMap;
    return JsonData;
}

JsonObject ResolveJson(std::string_view JsonPath)
{
    std::optional<std::string> JsonStrData = GetJsonStrData(JsonPath);
    if (!JsonStrData.has_value()) {
        std::cout << "Resolve Failed" << '\n';
        return {};
    } else {
        std::cout << "Resolve Succeed" << '\n';
        return ResloveObject(JsonStrData.value());
    }
}