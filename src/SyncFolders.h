#ifndef SYNC_FOLDERS_H
#define SYNC_FOLDERS_H


#include <string>
#include <queue>

class SyncFolders {
private:
    /** 配置文件路径 */
    const std::string CONF_PATH;
    /** robocopy 参数 */
    const std::string ROBOCOPY_ARGS;

    /** 读取的目录对总数 */
    std::size_t total;
    /** 读取的目录对中有效的数量 */
    std::size_t valid;

    /** 目录对 */
    struct DirPair {
        /** 元文件目录 */
        std::string src;
        /** 目标目录 */
        std::string dest;
    };

    /** 有效的目录对 */
    std::queue<DirPair> dirPairs;

    /** 生成的命令 */
    std::queue<std::string> commands;

public:
    /**
     * @param confPath 配置文件路径
     * @param robocopyArgs robocopy 参数
     * @param logPath 脚本路径
     */
    SyncFolders(std::string confPath, std::string robocopyArgs);

    /**
     * 读取配置
     * @return 读取的目录对中有效的数量
     */
    size_t readConfig();

    /** 生成命令. */
    void generateCommands();

    /**
     * 执行命令.
     * @return 最后一次执行的返回值
     */
    int executeCommands();
};


#endif //SYNC_FOLDERS_H
