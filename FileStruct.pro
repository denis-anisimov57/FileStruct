QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    FileStruct.cpp \
    ini_file.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    FileStruct.h \
    ini_file.h \
    mainwindow.h \
    rapidjson-master/include/rapidjson/allocators.h \
    rapidjson-master/include/rapidjson/cursorstreamwrapper.h \
    rapidjson-master/include/rapidjson/document.h \
    rapidjson-master/include/rapidjson/encodedstream.h \
    rapidjson-master/include/rapidjson/encodings.h \
    rapidjson-master/include/rapidjson/error/en.h \
    rapidjson-master/include/rapidjson/error/error.h \
    rapidjson-master/include/rapidjson/filereadstream.h \
    rapidjson-master/include/rapidjson/filewritestream.h \
    rapidjson-master/include/rapidjson/fwd.h \
    rapidjson-master/include/rapidjson/internal/biginteger.h \
    rapidjson-master/include/rapidjson/internal/clzll.h \
    rapidjson-master/include/rapidjson/internal/diyfp.h \
    rapidjson-master/include/rapidjson/internal/dtoa.h \
    rapidjson-master/include/rapidjson/internal/ieee754.h \
    rapidjson-master/include/rapidjson/internal/itoa.h \
    rapidjson-master/include/rapidjson/internal/meta.h \
    rapidjson-master/include/rapidjson/internal/pow10.h \
    rapidjson-master/include/rapidjson/internal/regex.h \
    rapidjson-master/include/rapidjson/internal/stack.h \
    rapidjson-master/include/rapidjson/internal/strfunc.h \
    rapidjson-master/include/rapidjson/internal/strtod.h \
    rapidjson-master/include/rapidjson/internal/swap.h \
    rapidjson-master/include/rapidjson/istreamwrapper.h \
    rapidjson-master/include/rapidjson/memorybuffer.h \
    rapidjson-master/include/rapidjson/memorystream.h \
    rapidjson-master/include/rapidjson/msinttypes/inttypes.h \
    rapidjson-master/include/rapidjson/msinttypes/stdint.h \
    rapidjson-master/include/rapidjson/ostreamwrapper.h \
    rapidjson-master/include/rapidjson/pointer.h \
    rapidjson-master/include/rapidjson/prettywriter.h \
    rapidjson-master/include/rapidjson/rapidjson.h \
    rapidjson-master/include/rapidjson/reader.h \
    rapidjson-master/include/rapidjson/schema.h \
    rapidjson-master/include/rapidjson/stream.h \
    rapidjson-master/include/rapidjson/stringbuffer.h \
    rapidjson-master/include/rapidjson/uri.h \
    rapidjson-master/include/rapidjson/writer.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
