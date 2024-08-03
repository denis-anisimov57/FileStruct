#include "FileStruct.h"
#include <QFile>
#include <QDir>
#include <QDirIterator>
#include <QDebug>
#include <QDateTime>
#include <fstream>
#include "rapidjson-master/include/rapidjson/writer.h"
#include "rapidjson-master/include/rapidjson/ostreamwrapper.h"

// QFileInfo(lastModified), QFile, QDir, QDirIterator

//FileStruct::FileStruct(QString pathToFolder, QString pathToJson) {
//    if(!pathToJson.isEmpty()) {
//        QString jsonStr;
//        QFile file(pathToJson);
//        if(file.open(QIODevice::ReadOnly)) {
//            jsonStr = file.readAll();
//            file.close();
//        }
//        qDebug() << jsonStr;
//        if (json.Parse(jsonStr.toStdString().c_str()).HasParseError()) {
//            throw(std::runtime_error("Invalid json"));
//        }
//        return;
//    }

//    QDirIterator it(pathToFolder, {"*"}, QDir::Files, QDirIterator::Subdirectories);
//    json.SetObject();
//    rapidjson::Value files(rapidjson::kArrayType);

//    for(; it.hasNext(); it.next()) {
//        if(!it.filePath().isEmpty()) {
//            qDebug() << it.filePath() << " " << it.fileInfo().lastModified().toString();
//            rapidjson::Value file(rapidjson::kObjectType);

//            rapidjson::Value filePath;
//            char buffer[it.filePath().size()];
//            int len = sprintf(buffer, "%s", it.filePath().toStdString().c_str());
//            filePath.SetString(buffer, len, json.GetAllocator());

//            rapidjson::Value tags(rapidjson::kArrayType);

//            file.AddMember("FilePath", filePath, json.GetAllocator());
//            file.AddMember("Tags", tags, json.GetAllocator());
//            files.PushBack(file, json.GetAllocator());
//        }
//    }
//    json.AddMember("Files", files, json.GetAllocator());

//    std::ofstream ofs("output.json");
//    rapidjson::OStreamWrapper osw(ofs);
//    rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
//    json.Accept(writer);
//}

//void FileStruct::addTag(QString filePath, QString tag) {
//    if(json.IsObject() && json.HasMember("Files") && json["Files"].IsArray()) {
//        rapidjson::Value::Array files = json["Files"].GetArray();
//        for(auto it = files.Begin(); it != files.end(); ++it) {
//            rapidjson::Value& file = *it;
//            if(file.IsObject() && file.HasMember("FilePath") && file["FilePath"].IsString() && file["FilePath"].GetString() == filePath) {
//                if(file.HasMember("Tags") && file["Tags"].IsArray()) {
//                    rapidjson::Value::Array tags = file["Tags"].GetArray();
//                    for(auto tagIt = tags.Begin(); tagIt != tags.end(); ++tagIt) {
//                        if(tagIt->GetString() == tag) {
//                            return;
//                        }
//                    }
//                    rapidjson::Value tagValue;
//                    char buffer[tag.size()];
//                    int len = sprintf(buffer, "%s", tag.toStdString().c_str());
//                    tagValue.SetString(buffer, len, json.GetAllocator());
//                    tags.PushBack(tagValue, json.GetAllocator());
//                }
//            }
//        }
//    }
//}

FileStruct::FileStruct(std::string pathToIni): data(pathToIni) {}

void FileStruct::openNewFolder(std::string path) {
    data.clear();
    QDirIterator it(QString::fromStdString(path), {"*"}, QDir::Files, QDirIterator::Subdirectories);
    for(; it.hasNext(); it.next()) {
        if(!it.filePath().isEmpty()) {
            qDebug() << it.filePath() << " " << it.fileInfo().lastModified().toString();
            data.writeBool(it.filePath().toStdString(), "IsExist", true);
        }
    }
}

void FileStruct::addTag(std::string filePath, std::string tag) {
    if(data.isSectionExist(filePath)) {
        data.writeBool(filePath, tag, true);
    }
}



void FileStruct::saveChanges(std::string pathToIni) {
    data.saveToFile(pathToIni);
    //    std::ofstream ofs();
//    rapidjson::OStreamWrapper osw(ofs);
//    rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
//    json.Accept(writer);
}

//????
//bool FileStruct::isExistFile(QString filePath) {
//    if(json.IsObject() && json.HasMember("Files") && json["Files"].IsArray()) {
//        rapidjson::Value::Array files = json["Files"].GetArray();
//        for(auto it = files.Begin(); it != files.end(); ++it) {
//            rapidjson::Value& file = *it;
//            if(file.IsObject() && file.HasMember("FilePath") && file["FilePath"].IsString() && file["FilePath"].GetString() == filePath) {
//                return true;
//            }
//        }
//    }
//    return false;
//}


//bool FileStruct::isExistTag(QString filePath, QString tag) {
//    if(json.IsObject() && json.HasMember("Files") && json["Files"].IsArray()) {
//        rapidjson::Value::Array files = json["Files"].GetArray();
//        for(auto it = files.Begin(); it != files.end(); ++it) {
//            rapidjson::Value& file = *it;
//            if(file.IsObject() && file.HasMember("FilePath") && file["FilePath"].IsString() && file["FilePath"].GetString() == filePath) {
//                if(file.HasMember("Tags") && file["Tags"].IsArray()) {
//                    rapidjson::Value::Array tags = file["Tags"].GetArray();
//                    for(auto tagIt = tags.Begin(); tagIt != tags.end(); ++tagIt) {
//                        if(tagIt->GetString() == tag) {
//                            return true;
//                        }
//                    }
//                }
//            }
//        }
//    }
//    return false;
//}

FileStruct::~FileStruct() {}


