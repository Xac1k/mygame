#pragma once
#include <functional>
#include <string>
#include <map>
#include <variant>
#include <set>
#include <fstream>
#include <regex>
#include <Common/buildPath.hpp>
#include <Common/Vect.hpp>

struct DialogNode {
    std::string id;
    std::string text;
    std::string init_func;
    
    struct Answer {
        std::string text;
        std::string target_node_id;
    };
    
    std::vector<Answer> answers;
};

using var_env_type = std::unordered_map<std::string, std::shared_ptr<void>>;
enum class ComandType {exit, redrig, setAvatar, setName};
struct Comand {
    ComandType type;
    var_env_type var_env;
};

struct DialogTreeComponent {
    std::unordered_map<std::string, std::shared_ptr<DialogNode>> nodes;
    std::shared_ptr<DialogNode> current_node;

    private:
        bool isSide(std::string parameter) {
            return parameter == "left" || parameter == "right";
        }

        bool isPath(std::string parameter) {
            std::regex pattern("\\S(?:.*\\S)?");
            return std::regex_match(parameter, pattern);
        }

        bool isInt(std::string parameter) {
            std::regex pattern("[0-9]+");
            return std::regex_match(parameter, pattern);
        }

        bool isFloat(std::string parameter) {
            std::regex pattern("[0-9]+.[0-9]*");
            return std::regex_match(parameter, pattern);
        }

        bool isVect2D(std::string parameter) {
            std::regex pattern("\\s*\\{\\s*([0-9]+)\\s*,\\s*([0-9]+)\\s*\\}\\s*");
            return std::regex_match(parameter, pattern);
        }

        Vect2D getVect2D(std::string parameter) {
            std::regex pattern("\\s*\\{\\s*([0-9]+)\\s*,\\s*([0-9]+)\\s*\\}\\s*");
            std::sregex_iterator begin(parameter.begin(), parameter.end(), pattern);
            std::sregex_iterator end;

            Vect2D vect;
            for (auto it = begin; it != end; ++it) {
                std::smatch match = *it; 

                vect.x = std::stoi(match[1]);
                vect.y = std::stoi(match[2]);
            }
            return vect;
        }

        void loadAnswers(std::string answers, DialogNode& node) {
            std::regex pattern("\\s*\"([^\"]+)\"\\s*:\\s*\"([^\"]+)\"\\s*");

            std::sregex_iterator begin(answers.begin(), answers.end(), pattern);
            std::sregex_iterator end;

            for (auto it = begin; it != end; ++it) {
                std::smatch match = *it;

                DialogNode::Answer answer;
                answer.text = match[1];
                answer.target_node_id = match[2];
                node.answers.push_back(answer);
            }
        }

        std::unordered_map<std::string, std::string> getMapOfParams(std::string parameters) {
            std::unordered_map<std::string, std::string> var_env_temp;

            std::regex pattern("\\s*([^,]+?(?:\\{[^}]*\\})?)\\s*(?=,\\s*[^,]|$)");
            std::sregex_iterator begin(parameters.begin(), parameters.end(), pattern);
            std::sregex_iterator end;
            int paramID = 0;

            for (auto it = begin; it != end; ++it) {
                std::smatch match = *it; 
                std::regex patternAdditionlParam("\\s*([\\w\\-_]+)\\s*:\\s*([^\n]+)\\s*");

                std::string param = match[1]; // parameter with tag || single param(required)
                std::sregex_iterator begin(param.begin(), param.end(), patternAdditionlParam);
                std::sregex_iterator end;

                if(begin == end) { // single param
                    var_env_temp.insert(std::make_pair(std::to_string(paramID), param));
                    paramID++;
                }
                else { // parameter with tag
                    for (auto it = begin; it != end; ++it) {
                        std::smatch match = *it;
                        var_env_temp.insert(std::make_pair(match[1], match[2]));
                    }
                }
            }

            return var_env_temp;
        }

        std::unordered_map<std::string, std::shared_ptr<void>> getVarEnv(ComandType type, std::string parameters) {
            auto paramTokenMap = getMapOfParams(parameters);

            std::unordered_map<std::string, std::shared_ptr<void>> var_env;
            switch (type)
            {
            case ComandType::exit: {
                if(!paramTokenMap.empty()) 
                    std::cerr << "Ошибка в программе. Команда exit не должна иметь каких либо параметров\n";
                return {};
            }
            case ComandType::redrig: {
                if(paramTokenMap.size() != 2) 
                    std::cerr << "Ошибка в программе. Команда redrig имеет ровно два параметра [side, strong]\n";
                else {
                    if(isSide(paramTokenMap["0"])) 
                        var_env.insert(std::make_pair("side", std::make_shared<std::string>(paramTokenMap["0"])));
                    else 
                        std::cerr << "Ошибка в программе. Неправельное значение параметра side. side может быть либо left, либо right\n";
                    

                    if(isInt(paramTokenMap["1"])) 
                        var_env.insert(std::make_pair("strong", std::make_shared<int>(std::stoi(paramTokenMap["1"]))));
                    else 
                        std::cerr << "Ошибка в программе. Неправельный тип параметра strong. strong может быть типа int\n";
                }
                return var_env;
            }
            case ComandType::setAvatar: {
                if(paramTokenMap.size() < 2 || paramTokenMap.size() > 5) 
                    std::cerr << "Ошибка в программе. Команда setAvatar может иметь от 2 до 5 параметров."
                    << "[side, path, ?tileSize, ?tileID, ?shift]. ? - не обязателен, но при указывании требует тега.\n";
                else {
                    if(isSide(paramTokenMap["0"])) 
                        var_env.insert(std::make_pair("side", std::make_shared<std::string>(paramTokenMap["0"])));
                    else 
                        std::cerr << "Ошибка в программе. Неправельное значение параметра side. side может быть либо left, либо right\n";

                    if(isPath(paramTokenMap["1"])) 
                        var_env.insert(std::make_pair("path", std::make_shared<std::string>(paramTokenMap["1"])));
                    else 
                        std::cerr << "Ошибка в программе. Неправельное значение параметра path. path может начинаться и заканчиваться только любым не пробельным символом.\n";

                    auto itTileSize = paramTokenMap.find("tileSize");
                    auto itTileID = paramTokenMap.find("tileID");
                    auto itShift = paramTokenMap.find("shift");

                    if(itTileSize != paramTokenMap.end()) {
                        if(isVect2D(itTileSize->second)) {
                            Vect2D vect = getVect2D(itTileSize->second);
                            var_env.insert(std::make_pair("tileSize", std::make_shared<Vect2D>(vect)));
                        }
                        else
                            std::cerr << "Ошибка в программе. Неправельное значение параметра tileSize. tileSize может быть только структурой типа {int, int}\n";
                    }
                    if(itTileID != paramTokenMap.end()) {
                        if(isVect2D(itTileID->second)) {
                            Vect2D vect = getVect2D(itTileID->second);
                            var_env.insert(std::make_pair("tileID", std::make_shared<Vect2D>(vect)));
                        }
                        else
                            std::cerr << "Ошибка в программе. Неправельное значение параметра tileID. tileID может быть только структурой типа {int, int}\n";
                    }
                    if(itShift != paramTokenMap.end()) {
                        if(isVect2D(itShift->second)) {
                            Vect2D vect = getVect2D(itShift->second);
                            var_env.insert(std::make_pair("shift", std::make_shared<Vect2D>(vect)));
                        }
                        else
                            std::cerr << "Ошибка в программе. Неправельное значение параметра shift. tileID может быть только структурой типа {int, int}\n";
                    }
                }
                return var_env;
            }
            case ComandType::setName: {
                if(paramTokenMap.size() != 1) 
                    std::cerr << "Ошибка в программе. Команда setName имеет ровно один параметра name\n";
                else 
                    if(isPath(paramTokenMap["0"])) 
                        var_env.insert(std::make_pair("name", std::make_shared<std::string>(paramTokenMap["0"])));
                    else 
                        std::cerr << "Ошибка в программе. Неправельное значение параметра name. name может начинаться и заканчиваться только любым не пробельным символом.\n";
                return var_env;
            }
            default:
                return var_env;
            }
        }

    public: 
        bool loadFromTXT(const std::string& filename) {
            std::ifstream dialogJson(buildFullPath(filename, 1));

            if (!dialogJson.is_open()) {
                std::cerr << "Не удалось открыть файл для чтения! Путь: " << buildFullPath(filename, 1) << std::endl;
                return 0;
            }

            std::string json_data(
                (std::istreambuf_iterator<char>(dialogJson)),
                std::istreambuf_iterator<char>()
            );

            std::regex pattern(
                "\"([^\"]+)\"\\s*:\\s*\\{\\s*"
                "\"text\"\\s*:\\s*\"([^\"]*)\"\\s*,\\s*"
                "\"init_func\"\\s*:\\s*\"([^\"]*)\"\\s*,\\s*"
                "\"answers\"\\s*:\\s*\\{([^}]*)\\}\\s*\\}"
            );
            std::sregex_iterator begin(json_data.begin(), json_data.end(), pattern);
            std::sregex_iterator end;

            if(begin->size() == 0) {
                std::cerr << "В файле диалога не найдено не одной ноды\n"; 
                return 0;
            }

            begin = std::sregex_iterator(json_data.begin(), json_data.end(), pattern);
            for (auto it = begin; it != end; ++it) {
                std::smatch match = *it;

                DialogNode node;
                node.id = match[1];
                node.text = match[2];
                node.init_func = match[3];
                loadAnswers(match[4], node);
                auto ptr = std::make_shared<DialogNode>(node);

                nodes.insert(std::make_pair(match[1], ptr));
            }
            
            if(nodes.find("root") == nodes.end()) {
                std::cerr << "Неправильный формат json файла диалога. Путь до файла: " << buildFullPath(filename, 1) << " Отсутствует root нода.\n";
                return 0;
            }
            current_node = nodes["root"];
            return 1;
        };

        std::vector<Comand> getProgramm() {
            std::vector<Comand> programm;

            std::regex pattern(
                "\\s*([a-zA-Z][\\w\\-_]+?)\\(\\s*([^]*?)\\s*\\)\\s*;\\s*"
            );
            std::sregex_iterator begin(current_node->init_func.begin(), current_node->init_func.end(), pattern);
            std::sregex_iterator end;

            for (auto it = begin; it != end; ++it) {
                std::smatch match = *it;

                Comand node;
                auto comandName = static_cast<std::string>(match[1]);
                auto comandEnv = static_cast<std::string>(match[2]);

                if(comandName == "exit") 
                    node.type = ComandType::exit;
                else if(comandName == "redrig") 
                    node.type = ComandType::redrig;
                else if(comandName == "setAvatar")
                    node.type = ComandType::setAvatar;
                else if(comandName == "setName")
                    node.type = ComandType::setName;
                else {
                    std::cerr << "[parse] Ошибка внутри init_func. Неизвестный токен: " << comandName << std::endl;
                }
                node.var_env = getVarEnv(node.type, comandEnv);

                programm.push_back(node);
            }

            return programm;
        };
};


