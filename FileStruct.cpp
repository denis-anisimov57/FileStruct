#include "FileStruct.h"
#include <QFile>
#include <QDir>
#include <QDirIterator>
#include <QDebug>
#include <fstream>

void FileStruct::openNewFolder(const std::string& path) {
    /*"*.png", "*.jpg", "*.jpeg", "*.webp"*/
    data.clear();
    QDirIterator it(QString::fromStdString(path), {"*"}, QDir::Files, QDirIterator::Subdirectories);
    qDebug() << "Reading files from directory\n";
    for(; it.hasNext(); it.next()) {
        if(!it.filePath().isEmpty()) {
            qDebug() << it.filePath();
            data.writeBool(it.filePath().toStdString(), "IsExist", true);
        }
    }
}

std::vector<std::string> FileStruct::addFolder(const std::string& path) {
    std::vector<std::string> newFiles;
    if(path.empty()) {
        return newFiles;
    }
    qDebug() << "Adding new files\n";
    QDirIterator it(QString::fromStdString(path), {"*"}, QDir::Files, QDirIterator::Subdirectories);
    for(; it.hasNext(); it.next()) {
        if(!it.filePath().isEmpty() && !data.isSectionExist(it.filePath().toStdString())) {
            qDebug() << it.filePath();
            data.writeBool(it.filePath().toStdString(), "IsExist", true);
            newFiles.push_back(it.filePath().toStdString());
        }
    }
    return newFiles;
}

void FileStruct::openData(const std::string& pathToIni) {
    data.open(pathToIni);
}

void FileStruct::addData(const std::string& pathToIni) {
    data.addNew(pathToIni);
}

std::vector<std::string> FileStruct::getFiles() {
    std::vector<std::string> files;
    SectionsMap sections = data.getSections();
    for(auto& file : sections) {
        if(file.first == "Tags") {
            continue;
        }
        files.push_back(file.first);
    }
    return files;
}

std::vector<std::string> FileStruct::getTags(const std::string& fileName) {
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
    if(!data.isSectionExist("Tags")) {
        return tags;
    }
    std::map<std::string, std::string> tagsMap = data.getSections().at("Tags");
    for(auto& tag : tagsMap) {
        if(tag.first == "IsExist") {
            continue;
        }
        tags.push_back(tag.first);
    }
    return tags;
}

std::vector<std::string> FileStruct::getUntagged() {
    std::vector<std::string> untaggedFiles;
    SectionsMap files = data.getSections();
    for(auto& file : files) {
        if(file.first == "Tags") {
            continue;
        }
        if(data.getKeysCount(file.first) == 1) {
            untaggedFiles.push_back(file.first);
        }
    }
    return untaggedFiles;
}

std::vector<std::string> FileStruct::getTagged() {
    std::vector<std::string> taggedFiles;
    SectionsMap files = data.getSections();
    for(auto& file : files) {
        if(file.first == "Tags") {
            continue;
        }
        if(data.getKeysCount(file.first) > 1) {
            taggedFiles.push_back(file.first);
        }
    }
    return taggedFiles;
}

std::vector<std::string> FileStruct::getFiltered() {
    return filteredFiles;
}

bool FileStruct::isEmpty() {
    return !data.getSectionsCount();
}

void FileStruct::addTag(const std::string& filePath, const std::string& tag) {
    if(data.isSectionExist(filePath)) {
        data.writeBool("Tags", tag, true);
        data.writeBool(filePath, tag, true);
    }
}

void FileStruct::removeTag(const std::string& filePath, const std::string& tag) {
    data.deleteKey(filePath, tag);
}

void FileStruct::removeTag(const std::vector<std::string>& filePaths, const std::string& tag) {
    for(auto& filePath : filePaths) {
        data.deleteKey(filePath, tag);
    }
}

void FileStruct::removeUnusedTags() {
    if(!data.isSectionExist("Tags")) {
        return;
    }
    std::map<std::string, std::string> tags = data.getSections().at("Tags");
    SectionsMap sections = data.getSections();
    for(auto& tag : tags) {
        bool isExist = false;
        for(auto& section : sections) {
            if(section.first == "Tags") {
                break;
            }
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

void FileStruct::groupFiles(const std::vector<std::string>& tags) {
    filteredFiles.clear();
    SectionsMap files = data.getSections();
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
                filteredFiles.push_back(filePath.first);
            }
        }
    }
}

void FileStruct::saveChanges(const std::string& pathToIni) {
    data.saveToFile(pathToIni);
}

void FileStruct::saveToFolder(const std::string& path, const bool isMoving) {
    qDebug() << "Copying files to custom folder: \n";
    std::vector<std::string> newFilteredFiles;
    for(auto& filePath : filteredFiles) {
        QString qFilePath = QString::fromStdString(filePath);
        QFileInfo file(qFilePath);
        QString old_name = qFilePath;
        QString new_name = QString::fromStdString(path) + "/" + file.fileName();
        if(QFile::exists(new_name)) {
            QFileInfo newFile(new_name);
            int renameCount = 1;
            while(QFile::exists(new_name =
                                newFile.absolutePath() + "/" +
                                newFile.baseName() +
                                "(" + QString::number(renameCount) + ")." +
                                newFile.suffix())) {
                renameCount++;
            }
        }
        bool ok;
        if(isMoving) {
            ok = QFile::rename(old_name, new_name);
            data.renameSection(old_name.toStdString(), new_name.toStdString());
            newFilteredFiles.push_back(new_name.toStdString());
            qDebug() << old_name << " " << new_name << "\nMove - " << ok << "\n";
        }
        else {
            ok = QFile::copy(old_name, new_name);
            qDebug() << old_name << " " << new_name << "\nCopy - " << ok << "\n";
        }
    }
    if(!newFilteredFiles.empty()) {
        filteredFiles = std::move(newFilteredFiles);
    }
}

FileStruct::~FileStruct() {}
