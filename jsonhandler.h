#ifndef JSONHANDLER_H
#define JSONHANDLER_H
#include <string>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class JsonHandler
{
public:
    JsonHandler();
    json loadData(const std::string& file_path);
    bool saveData(const std::string& file_path, const json& data);

};

#endif // JSONHANDLER_H
