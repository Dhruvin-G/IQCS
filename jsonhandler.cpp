#include "jsonhandler.h"
#include"fstream"


JsonHandler::JsonHandler()
{
}
json JsonHandler::loadData(const std::string& file_path){
    std::ifstream file(file_path);
        if (!file.is_open()) {
            return nullptr;
        }
        try {
            json data;
            file >> data;
            return data;
        } catch (json::parse_error& e) {
            return nullptr;
        }
}
bool JsonHandler::saveData(const std::string &file_path, const json &data){
    std::ofstream file(file_path);
        if (!file.is_open()) {
            return false;
        }
        try {
            file << data.dump(4); // Pretty print with 4 spaces
            return true;
        } catch (const json::exception& e) {
            return false;
        }
}
