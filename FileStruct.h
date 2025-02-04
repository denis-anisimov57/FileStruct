#pragma once
#include <QDirIterator>
#include <QDebug>
#include "ini_file.h"

class FileStruct {
    public:
        //! Конструктор по умолчанию
        FileStruct() = default;

        void openNewFolder(const std::string& path);

        void openData(const std::string& pathToIni);

        void addData(const std::string& pathToIni);

        void addFolder(const std::string& path);

        QStringList getFiles();

        QStringList getTags(const std::string& fileName);

        QStringList getAllTags();

        QStringList getUntagged();

        QStringList getTagged();

        QStringList getFiltered();

        bool isEmpty();

        void addTag(const std::string& filePath, const std::string& tag);

        void removeTag(const std::string& filePath, const std::string& tag);

        void removeTag(const std::vector<std::string>& filePaths, const std::string& tag);

        void removeUnusedTags();

        void groupFiles(const QStringList& tags); //saves filenames with this tags

        void saveToFolder(const std::string& path, const bool isMoving); // saves union from methods above to custom folder

        void saveChanges(const std::string& pathToIni); // saves changed tags in ini

        ~FileStruct();

    private:
        QStringList filteredFiles;
        IniFile data;
};
