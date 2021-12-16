#include <SyncFolders.h>

#include <regex>
#include <iostream>
#include <yaml-cpp/yaml.h>

int main() {
    /** 配置文件路径 */
    const std::string CONF_PATH("config.yml");
    /** robocopy 参数 */
    const std::string ROBOCOPY_ARGS("/mir /unicode");

    SyncFolders syncFolders(CONF_PATH, ROBOCOPY_ARGS);
    if (syncFolders.readConfig()) {
        syncFolders.generateCommands();

        char choice = 0;
        while (choice != 'y' && choice != 'n') {
            std::cout << "Execute? (Y/N):";
            std::cin >> choice;
            choice = (char) std::tolower(choice);
        }
        if (choice == 'y') {
            auto code = syncFolders.executeCommands();
            std::cout << "Finished with exit code " << code << std::endl
                      << std::endl;
        }
    }

    return 0;
}

SyncFolders::SyncFolders(std::string confPath, std::string robocopyArgs) :
        CONF_PATH(std::move(confPath)),
        ROBOCOPY_ARGS(std::move(robocopyArgs)),
        total(0),
        valid(0),
        dirPairs(),
        commands() {}

std::size_t SyncFolders::readConfig() {
    std::cout << "Reading config file \"" << this->CONF_PATH << "\"..." << std::endl;

    /** 路径的正则表达式 */
    const std::regex PATH_PATTERN(R"([a-zA-Z]:\\.*)");

    YAML::Node conf = YAML::LoadFile(this->CONF_PATH);
    for (const auto &group: conf) {
        for (const auto &item: group["dirs"]) {
            const DirPair dirPair{
                    std::regex_replace(
                            group["root"]["src"].as<std::string>()  // 盘符
                            + (group["root"]["src"].as<std::string>().length() ? "\\" : "")  // '\'
                            + item["src"].as<std::string>(),  // 路径
                            std::regex(R"(\\\\)"),
                            "\\"
                    ),
                    std::regex_replace(
                            group["root"]["dest"].as<std::string>()  // 盘符
                            + (group["root"]["dest"].as<std::string>().length() ? "\\" : "")  // '\'
                            + item["dest"].as<std::string>(),  // 路径
                            std::regex(R"(\\\\)"),
                            "\\"
                    )
            };
            ++this->total;
            std::cout << "src:\t" << dirPair.src << std::endl
                      << "dest:\t" << dirPair.dest << std::endl;

            /* 仅将合法的目录对加入队列. */
            if (std::regex_match(dirPair.src, PATH_PATTERN) && std::regex_match(dirPair.dest, PATH_PATTERN)) {
                this->dirPairs.push(dirPair);
            } else {
                std::cout << "(INVALID!)" << std::endl;
            }

            std::cout << std::endl;
        }
    }
    this->valid = this->dirPairs.size();
    std::cout << "(" << this->valid << "/" << this->total << " valid, "
              << this->total - this->valid << " invalid.)" << std::endl
              << std::endl;

    return this->valid;
}

void SyncFolders::generateCommands() {
    std::cout << "Generating commands..." << std::endl;

    /* 清空生成的命令. */
    while (!this->commands.empty()) {
        this->commands.pop();
    }

    while (!this->dirPairs.empty()) {
        auto dirPair = this->dirPairs.front();
        const std::string cmd("robocopy \"" + dirPair.src + "\" \"" + dirPair.dest + "\" " + this->ROBOCOPY_ARGS);
        std::cout << cmd << std::endl;
        this->commands.push(cmd);
        this->dirPairs.pop();
    }

    std::cout << std::endl;
}

int SyncFolders::executeCommands() {
    std::cout << "Executing..." << std::endl;
    int code = 0;
    while (!this->commands.empty()) {
        auto result = std::system(this->commands.front().c_str());
        if (result) {
            code = result;
        }
        this->commands.pop();
    }
    std::cout << std::endl;
    return code;
}
