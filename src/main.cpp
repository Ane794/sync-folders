#include <iostream>
#include <sync-folders/SyncFolders.h>
#include <regex>

int main() {
    /** 配置文件路径 */
    const std::string CONF_PATH("config.yml");
    /** robocopy 参数 */
    const std::string ROBOCOPY_ARGS("/mir /unicode");

    SyncFolders syncFolders(CONF_PATH, ROBOCOPY_ARGS);
    if (syncFolders.readConf() && syncFolders.generateCommands()) {
        char choice = 0;
        while (choice != 'Y' && choice != 'y' && choice != 'N' && choice != 'n') {
            std::cout << "Execute? (Y/N):";
            std::cin >> choice;
        }
        if (choice == 'Y' or choice == 'y') {
            auto code = syncFolders.execute();
            std::cout << "Finished with exit code " << code << std::endl
                      << std::endl;
        }
    }

    return 0;
}
