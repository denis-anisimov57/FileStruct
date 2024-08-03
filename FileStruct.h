#pragma once
#include <string>
#include <vector>
#include <QString>
#include "rapidjson-master/include/rapidjson/document.h"
#include "ini_file.h"

using intId = unsigned long long;

class FileStruct {
    public:
        //! Конструктор по умолчанию
        FileStruct() = default;

        //! Конструктор с параметром
        //! \param pathToJson путь до существующего файла json с данными о файлах
        //! \brief Открывает  json и парсит его для получения данных о файлах
        FileStruct(std::string pathToIni);

        void openNewFolder(std::string path);

        void openData(std::string pathToJson);

        void updateFolder(std::string pathToJson);

        void addTag(std::string filePath, std::string tag);

        void addTag(std::vector<QString> filePaths, std::string tag);

        void removeTag(QString filePath, std::string tag);

        void removeTag(std::vector<QString> filePaths, std::string tag);

        bool isExistTag(QString filePath, QString tag);

        bool isExistFile(QString filePath);

        void uniteFiles(std::string tag); //generate folder with this tag

        void uniteFiles(std::vector<std::string> tags); //generate folder with this tags

        void saveToFolder(std::string path); // saves union from methods above to custom folder

        void saveChanges(std::string pathToIni); // save changed tags in json

        ~FileStruct();

    private:
//        void addFolder();
        void addFile();
        IniFile data;
        rapidjson::Document json;
};
