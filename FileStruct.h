#pragma once
#include <string>
#include <vector>
#include "ini_file.h"

class FileStruct {
    public:
        //! Конструктор по умолчанию
        FileStruct() = default;

        void openNewFolder(const std::string& path);

        void openData(const std::string& pathToIni);

        void addData(const std::string& pathToIni);

        std::vector<std::string> addFolder(const std::string& path);

        std::vector<std::string> getFiles();

        std::vector<std::string> getTags(const std::string& fileName);

        std::vector<std::string> getAllTags();

        std::vector<std::string> getUntagged();

        std::vector<std::string> getTagged();

        std::vector<std::string> getFiltered();

        bool isEmpty();

        void addTag(const std::string& filePath, const std::string& tag);

        void removeTag(const std::string& filePath, const std::string& tag);

        void removeTag(const std::vector<std::string>& filePaths, const std::string& tag);

        void removeUnusedTags();

        void groupFiles(const std::vector<std::string>& tags); //saves filenames with this tags

        void saveToFolder(const std::string& path, const bool isMoving); // saves union from methods above to custom folder

        void saveChanges(const std::string& pathToIni); // saves changed tags in ini

        ~FileStruct();

    private:
        std::vector<std::string> filteredFiles;
        IniFile data;
};
