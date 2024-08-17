QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    account.cpp \
    admin.cpp \
    adminwindow.cpp \
    date.cpp \
    main.cpp \
    mainwindow.cpp \
    news.cpp \
    newsmanagementsystem.cpp \
    newstime.cpp \
    review.cpp \
    user.cpp \
    userwindow.cpp \
    validation.cpp

HEADERS += \
    account.h \
    admin.h \
    adminwindow.h \
    date.h \
    mainwindow.h \
    news.h \
    newsmanagementsystem.h \
    newstime.h \
    review.h \
    user.h \
    userwindow.h \
    validation.h

FORMS += \
    adminwindow.ui \
    mainwindow.ui \
    userwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    news_data.qrc \
    rees.qrc

DISTFILES += \
    AdminData.txt \
    CategoriesData.txt \
    NewsData.txt \
    NewsReview.txt \
    UserData.txt
