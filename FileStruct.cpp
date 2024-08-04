#include "FileStruct.h"
#include <QFile>
#include <QDir>
#include <QDirIterator>
#include <QDebug>
#include <QDateTime>
#include <fstream>

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

void FileStruct::openData(std::string pathToIni) {
    data.open(pathToIni);
}

std::vector<std::string> FileStruct::getFiles() {
    std::vector<std::string> files;
    SectionsMap sections = data.getSections();
    for(auto& file : sections) {
        files.push_back(file.first);
    }
    return files;
}

std::vector<std::string> FileStruct::getTags(std::string fileName) {
    std::vector<std::string> tags;
    std::map<std::string, std::string> tagsMap = data.getSections().at(fileName);
    for(auto& tag : tagsMap) {
        if(tag.first == "IsExist") {
            continue;
        }
        tags.push_back(tag.first);
    }
    return tags;
}

std::vector<std::string> FileStruct::getAllTags() {
    std::vector<std::string> tags;
    std::map<std::string, std::string> tagsMap = data.getSections().at("Tags");
    for(auto& tag : tagsMap) {
        if(tag.first == "IsExist") {
            continue;
        }
        tags.push_back(tag.first);
    }
    return tags;
}

void FileStruct::addTag(std::string filePath, std::string tag) {
    if(data.isSectionExist(filePath)) {
        data.writeBool("Tags", tag, true);
        data.writeBool(filePath, tag, true);
    }
}

void FileStruct::addTag(std::vector<std::string> filePaths, std::string tag) {
    for(auto& filePath : filePaths) {
        if(data.isSectionExist(filePath)) {
            data.writeBool("Tags", tag, true);
            data.writeBool(filePath, tag, true);
        }
    }
}

void FileStruct::removeTag(std::string filePath, std::string tag) {
    data.deleteKey(filePath, tag);
}

void FileStruct::removeTag(std::vector<std::string> filePaths, std::string tag) {
    for(auto& filePath : filePaths) {
        data.deleteKey(filePath, tag);
    }
}

void FileStruct::removeUnusedTags() {
    std::map<std::string, std::string> tags = data.getSections().at("Tags");
    SectionsMap sections = data.getSections();
    for(auto& tag : tags) {
        bool isExist = false;
        for(auto& section : sections) {
            isExist = data.isKeysExist(section.first, tag.first);
            if(isExist) {
                break;
            }
        }
        if(!isExist) {
            data.deleteKey("Tags", tag.first);
        }
    }
}

void FileStruct::groupFiles(std::string tag) {
    QDir* combinedFiles = new QDir(".");
    combinedFiles->mkdir("relatedFiles");
    combinedFiles->cd("relatedFiles");
    combinedFiles->setFilter(QDir::Files);
    qDebug() << "Deleting previous group: \n";
    for(auto& dirFile : combinedFiles->entryList()) {
        combinedFiles->remove(dirFile);
        qDebug() << dirFile;
    }
    delete combinedFiles;
    SectionsMap files = data.getSections();
    qDebug() << "Copying files: \n";
    for(auto& filePath : files) {
        if(filePath.first != "Tags" && data.isKeysExist(filePath.first, tag)) {
            QFileInfo file(QString::fromStdString(filePath.first));
            QString old_name = QString::fromStdString(filePath.first);
            QString new_name = "relatedFiles/" + file.fileName();
            bool ok = QFile::copy(old_name, new_name);
            qDebug() << old_name << " " << new_name << "\nCopy - " << ok << "\n";
        }
    }
}

void FileStruct::groupFiles(std::vector<std::string> tags) {
    QDir* combinedFiles = new QDir(".");
    combinedFiles->mkdir("relatedFiles");
    combinedFiles->cd("relatedFiles");
    combinedFiles->setFilter(QDir::Files);
    qDebug() << "Deleting previous group: \n";
    for(auto& dirFile : combinedFiles->entryList()) {
        combinedFiles->remove(dirFile);
        qDebug() << dirFile;
    }
    delete combinedFiles;
    SectionsMap files = data.getSections();
    qDebug() << "Copying files: \n";
    for(auto& filePath : files) {
        bool containTags = true;
        if(filePath.first != "Tags") {
            for(auto& tag : tags) {
                if(!data.isKeysExist(filePath.first, tag)) {
                    containTags = false;
                    break;
                }
            }
            if(containTags) {
                QFileInfo file(QString::fromStdString(filePath.first));
                QString old_name = QString::fromStdString(filePath.first);
                QString new_name = "relatedFiles/" + file.fileName();
                bool ok = QFile::copy(old_name, new_name);
                qDebug() << old_name << " " << new_name << "\nCopy - " << ok << "\n";
            }
        }
    }
}

void FileStruct::saveChanges(std::string pathToIni) {
    data.saveToFile(pathToIni);
}

FileStruct::~FileStruct() {}
