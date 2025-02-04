#include "FileStruct.h"
#include <QDebug>

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

void FileStruct::addFolder(const std::string& path) {
    qDebug() << "Adding new files\n";
    QDirIterator it(QString::fromStdString(path), {"*"}, QDir::Files, QDirIterator::Subdirectories);
    for(; it.hasNext(); it.next()) {
        if(!it.filePath().isEmpty() && !data.isSectionExist(it.filePath().toStdString())) {
            qDebug() << it.filePath();
            data.writeBool(it.filePath().toStdString(), "IsExist", true);
        }
    }
}

void FileStruct::openData(const std::string& pathToIni) {
    data.open(pathToIni);
}

void FileStruct::addData(const std::string& pathToIni) {
    data.addNew(pathToIni);
}

QStringList FileStruct::getFiles() {
    QStringList files;
    SectionsMap sections = data.getSections();
    for(auto& file : sections) {
        if(file.first == "Tags") {
            continue;
        }
        files.push_back(QString::fromStdString(file.first));
    }
    return files;
}

QStringList FileStruct::getTags(const std::string& fileName) {
    QStringList tags;
    std::map<std::string, std::string> tagsMap = data.getSections().at(fileName);
    for(auto& tag : tagsMap) {
        if(tag.first == "IsExist") {
            continue;
        }
        tags.push_back(QString::fromStdString(tag.first));
    }
    return tags;
}

QStringList FileStruct::getAllTags() {
    QStringList tags;
    if(!data.isSectionExist("Tags")) {
        return tags;
    }
    std::map<std::string, std::string> tagsMap = data.getSections().at("Tags");
    for(auto& tag : tagsMap) {
        if(tag.first == "IsExist") {
            continue;
        }
        tags.push_back(QString::fromStdString(tag.first));
    }
    return tags;
}

QStringList FileStruct::getUntagged() {
    QStringList untaggedFiles;
    SectionsMap files = data.getSections();
    for(auto& file : files) {
        if(file.first == "Tags") {
            continue;
        }
        if(data.getKeysCount(file.first) == 1) {
            untaggedFiles.push_back(QString::fromStdString(file.first));
        }
    }
    return untaggedFiles;
}

QStringList FileStruct::getTagged() {
    QStringList taggedFiles;
    SectionsMap files = data.getSections();
    for(auto& file : files) {
        if(file.first == "Tags") {
            continue;
        }
        if(data.getKeysCount(file.first) > 1) {
            taggedFiles.push_back(QString::fromStdString(file.first));
        }
    }
    return taggedFiles;
}

QStringList FileStruct::getFiltered() {
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

void FileStruct::groupFiles(const QStringList& tags) {
    filteredFiles.clear();
    SectionsMap files = data.getSections();
    for(auto& filePath : files) {
        bool containTags = true;
        if(filePath.first != "Tags") {
            for(auto& tag : tags) {
                if(!data.isKeysExist(filePath.first, tag.toStdString())) {
                    containTags = false;
                    break;
                }
            }
            if(containTags) {
                filteredFiles.push_back(QString::fromStdString(filePath.first));
            }
        }
    }
}

void FileStruct::saveChanges(const std::string& pathToIni) {
    data.saveToFile(pathToIni);
}

void FileStruct::saveToFolder(const std::string& path, const bool isMoving) {
    qDebug() << "Copying files to custom folder: \n";
    QStringList newFilteredFiles;
    for(auto& filePath : filteredFiles) {
        QFileInfo file(filePath);
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
            ok = QFile::rename(filePath, new_name);
            data.renameSection(filePath.toStdString(), new_name.toStdString());
            newFilteredFiles.push_back(new_name);
            qDebug() << filePath << " " << new_name << "\nMove - " << ok << "\n";
        }
        else {
            ok = QFile::copy(filePath, new_name);
            qDebug() << filePath << " " << new_name << "\nCopy - " << ok << "\n";
        }
    }
    if(!newFilteredFiles.empty()) {
        filteredFiles = std::move(newFilteredFiles);
    }
}

FileStruct::~FileStruct() {}
