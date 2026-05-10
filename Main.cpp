#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>

namespace fs = std::filesystem;

void deleteCompletelyEmptyFoldersRecursive(const fs::path& startPath, bool dryRun = false) {
    bool deleted = true;

    while (deleted) {
        deleted = false;
        std::vector<fs::path> emptyFolders;

        // Сначала собираем все пустые папки
        for (const auto& entry : fs::recursive_directory_iterator(startPath)) {
            if (fs::is_directory(entry.path())) {
                try {
                    if (fs::is_empty(entry.path())) {
                        emptyFolders.push_back(entry.path());
                    }
                }
                catch (const fs::filesystem_error& e) {
                    // Игнорируем ошибки доступа
                }
            }
        }

        // Удаляем все найденные пустые папки
        for (const auto& folder : emptyFolders) {
            std::cout << "Пустая папка: " << folder << std::endl;
            if (!dryRun) {
                try {
                    fs::remove(folder);
                    deleted = true;
                }
                catch (const fs::filesystem_error& e) {
                    std::cerr << "Ошибка удаления: " << folder << " - " << e.what() << std::endl;
                }
            }
        }

        if (dryRun) break;
    }
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");

    if (argc < 2) {
        std::cout << "Использование: " << argv[0] << " <путь_к_папке>" << std::endl;
        std::cout << "Пример: " << argv[0] << " C:\\Temp" << std::endl;
        std::cout << "Для просмотра: " << argv[0] << " C:\\Temp --dry-run" << std::endl;
        return 1;
    }

    if (!fs::exists(argv[1])) {
        std::cout << "Ошибка: путь не существует!" << std::endl;
        return 1;
    }

    bool dryRun = false;
    if (argc > 2 && std::string(argv[2]) == "--dry-run") {
        dryRun = true;
    }

    std::cout << "Режим: " << (dryRun ? "ПРОСМОТР" : "УДАЛЕНИЕ") << std::endl;
    std::cout << "Путь: " << argv[1] << std::endl;
    std::cout << "------------------------" << std::endl;

    deleteCompletelyEmptyFoldersRecursive(argv[1], dryRun);

    std::cout << "------------------------" << std::endl;
    std::cout << "Готово!" << std::endl;

    return 0;
}