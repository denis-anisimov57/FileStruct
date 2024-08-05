#pragma once
#include <string>
#include <vector>
#include <QString>
#include "ini_file.h"

class FileStruct {
    public:
        //! Конструктор по умолчанию
        FileStruct() = default;//

        void openNewFolder(const std::string& path);//

        void openData(const std::string& pathToIni);//

        void addData(const std::string& pathToIni);

        std::vector<std::string> addFolder(const std::string& path);//

        std::vector<std::string> getFiles();//

        std::vector<std::string> getTags(const std::string& fileName);//

        std::vector<std::string> getAllTags();//

        void addTag(const std::string& filePath, const std::string& tag);//

        void addTag(const std::vector<std::string>& filePaths, const std::string& tag);//

        void removeTag(const std::string& filePath, const std::string& tag);//

        void removeTag(const std::vector<std::string>& filePaths, const std::string& tag);//

        void removeUnusedTags(); //

        void groupFiles(const std::string& tag); //generates folder with this tag

        void groupFiles(const std::vector<std::string>& tags); //generates folder with this tags

        void saveToFolder(const std::string& path); // saves union from methods above to custom folder

        void saveChanges(const std::string& pathToIni); // saves changed tags in ini

        ~FileStruct();//

    private:
        IniFile data;
};
