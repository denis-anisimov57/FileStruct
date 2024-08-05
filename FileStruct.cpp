#include "FileStruct.h"
#include <QFile>
#include <QDir>
#include <QDirIterator>
#include <QDebug>
#include <QDateTime>
#include <fstream>

void FileStruct::openNewFolder(const std::string& path) {
    data.clear();
    QDirIterator it(QString::fromStdString(path), {"*"}, QDir::Files, QDirIterator::Subdirectories);
    qDebug() << "Reading files from directory\n";
    for(; it.hasNext(); it.next()) {
        if(!it.filePath().isEmpty()) {
            qDebug() << it.filePath() << " " << it.fileInfo().lastModified().toString();
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
            qDebug() << it.filePath() << " " << it.fileInfo().lastModified().toString();
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
    std::map<std::string, std::string> tagsMap = data.getSections().at("Tags");
    for(auto& tag : tagsMap) {
        if(tag.first == "IsExist") {
            continue;
        }
        tags.push_back(tag.first);
    }
    return tags;
}

void FileStruct::addTag(const std::string& filePath, const std::string& tag) {
    if(data.isSectionExist(filePath)) {
        data.writeBool("Tags", tag, true);
        data.writeBool(filePath, tag, true);
    }
}

void FileStruct::addTag(const std::vector<std::string>& filePaths, const std::string& tag) {
    for(auto& filePath : filePaths) {
        if(data.isSectionExist(filePath)) {
            data.writeBool("Tags", tag, true);
            data.writeBool(filePath, tag, true);
        }
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

void FileStruct::groupFiles(const std::string& tag) {
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

void FileStruct::groupFiles(const std::vector<std::string>& tags) {
    QDir* groupedFiles = new QDir(".");
    groupedFiles->mkdir("relatedFiles");
    groupedFiles->cd("relatedFiles");
    groupedFiles->setFilter(QDir::Files);
    qDebug() << "Deleting previous group: \n";
    for(auto& dirFile : groupedFiles->entryList()) {
        groupedFiles->remove(dirFile);
        qDebug() << dirFile;
    }
    delete groupedFiles;
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

void FileStruct::saveChanges(const std::string& pathToIni) {
    data.saveToFile(pathToIni);
}

void FileStruct::saveToFolder(const std::string& path) {
    QDir* groupedFiles = new QDir("relatedFiles");
    groupedFiles->setFilter(QDir::Files);
    qDebug() << "Copying files to custom folder: \n";
    for(auto& filePath : groupedFiles->entryList()) {
        QFileInfo file(filePath);
        QString old_name = "relatedFiles/" + filePath;
        QString new_name = QString::fromStdString(path) + "/" + file.fileName();
        bool ok = QFile::copy(old_name, new_name);
        qDebug() << old_name << " " << new_name << "\nCopy - " << ok << "\n";
    }
}

FileStruct::~FileStruct() {}
