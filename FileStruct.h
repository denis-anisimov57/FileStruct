#pragma once
#include <string>
#include <vector>

using intId = unsigned long long;

class FileStruct {
    public:
        //! Конструктор по умолчанию
        FileStruct();

        //! Конструктор с параметром
        //! \param pathToJson путь до существующего файла json с данными о файлах
        //! \brief Открывает  json и парсит его для получения данных о файлах
        FileStruct(std::string pathToJson);

        void openNewFolder(std::string path);

        void openExistingFolder(std::string pathToJson);

        void updateFolder(std::string pathToJson);

        void addTag(intId fileId, std::string tag);

        void addTag(std::vector<intId> filesId, std::string tag);

        void removeTag(intId fileId, std::string tag);

        void removeTag(std::vector<intId> filesId, std::string tag);

        bool isExistTag(intId fileId, std::string tag);

        bool isExistFile(intId fileId);

        void uniteFiles(std::string tag);

        void uniteFiles(std::vector<std::string> tags);

        void saveToFolder(std::string path);

        void saveChanges();

        ~FileStruct();

    private:
        //json data;
};
