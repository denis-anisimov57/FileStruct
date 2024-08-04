#pragma once
#include <string>
#include <vector>
#include <QString>
#include "ini_file.h"

using intId = unsigned long long;

class FileStruct {
    public:
        //! Конструктор по умолчанию
        FileStruct() = default;//

        //! Конструктор с параметром
        //! \param pathToJson путь до существующего файла json с данными о файлах
        //! \brief Открывает  json и парсит его для получения данных о файлах
        FileStruct(std::string pathToIni);//

        void openNewFolder(std::string path);//

        void openData(std::string pathToJson);

        void updateFolder(std::string pathToJson);

        std::vector<std::string> getFiles();//

        std::vector<std::string> getTags(std::string fileName);//

        std::vector<std::string> getAllTags();//

        void addTag(std::string filePath, std::string tag);//

        void addTag(std::vector<std::string> filePaths, std::string tag);//

        void removeTag(std::string filePath, std::string tag);//

        void removeTag(std::vector<std::string> filePaths, std::string tag);//

        void removeUnusedTags(); //

        void uniteFiles(std::string tag); //generate folder with this tag

        void uniteFiles(std::vector<std::string> tags); //generate folder with this tags

        void saveToFolder(std::string path); // saves union from methods above to custom folder(not done yet)

        void saveChanges(std::string pathToIni); // save changed tags in json

        ~FileStruct();

    private:
        IniFile data;
};
