#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include "rapidjson-master/include/rapidjson/document.h"
#include "rapidjson-master/include/rapidjson/error/en.h"
#include "FileStruct.h"
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
//    QString json = "{\"some\": 1, \"other\": \"too\"}";
//    QFile file("1.json");
//    rapidjson::Document document;
//    if (!document.Parse(json.toStdString().c_str()).HasParseError()) {
//            if (document.IsObject()) {
//                if(document.HasMember("some")) {
//                    qDebug() << document["some"].GetInt();
//                }
//            }
//    }
//    FileStruct f(QString("D:/QtProjects/FileStructTestFolder"), QString("D:/QtProjects/build-FileStruct-Desktop_Qt_5_12_12_MinGW_64_bit-Debug/FSConf.ini"));
    FileStruct f("D:/QtProjects/build-FileStruct-Desktop_Qt_5_12_12_MinGW_64_bit-Debug/FSData.ini");
//    f.openNewFolder("D:/QtProjects/FileStructTestFolder");
//    f.addTag("D:/QtProjects/FileStructTestFolder/a/c/ccc.png", "Первый тэг");
    f.saveChanges("FSConf.ini");
    qDebug() << "Test";
    return a.exec();
}
