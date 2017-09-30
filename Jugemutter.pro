#-------------------------------------------------
#
# Project created by QtCreator 2017-03-22T23:38:15
#
#-------------------------------------------------

QT       += core gui network networkauth

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

win32:QT += winextras

TARGET = Jugemutter
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += src src/sns src/widgets src/views

SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/sns/twitter.cpp \
    src/widgets/autosizetextedit.cpp \
    src/widgets/textcounterlabel.cpp \
    src/twittertextsplitter.cpp \
    src/widgets/tweettextedit.cpp \
    src/views/viewnormaleditor.cpp \
    src/accountaddpopup.cpp \
    src/views/viewsetting.cpp \
    src/widgets/pageselector.cpp \
    src/views/viewsettingaccountpage.cpp \
    src/views/viewsettinggeneralpage.cpp \
    src/views/viewsettingaboutpage.cpp \
    src/accountlist.cpp \
    src/views/viewwelcome.cpp \
    src/postprogress.cpp \
    src/widgets/taginput.cpp \
    src/preference.cpp \
    src/views/viewtestpage.cpp

HEADERS  += src/mainwindow.h \
    src/sns/twitter.h \
    src/widgets/autosizetextedit.h \
    src/widgets/textcounterlabel.h \
    src/twittertextsplitter.h \
    src/widgets/tweettextedit.h \
    src/views/viewnormaleditor.h \
    src/accountaddpopup.h \
    src/views/viewsetting.h \
    src/widgets/pageselector.h \
    src/views/viewsettingaccountpage.h \
    src/views/viewsettinggeneralpage.h \
    src/views/viewsettingaboutpage.h \
    src/accountlist.h \
    src/views/viewwelcome.h \
    src/postprogress.h \
    src/widgets/taginput.h \
    src/preference.h \
    src/views/viewtestpage.h

FORMS    += src/mainwindow.ui \
    src/views/viewnormaleditor.ui \
    src/accountaddpopup.ui \
    src/views/viewsetting.ui \
    src/views/viewsettingaccountpage.ui \
    src/views/viewsettinggeneralpage.ui \
    src/views/viewsettingaboutpage.ui \
    src/views/viewwelcome.ui \
    src/postprogress.ui \
    src/views/viewtestpage.ui

DISTFILES += \
    README.md

RESOURCES += \
    src/resources.qrc

TRANSLATIONS = src/i18n/ja_JP.ts

macx: ICON = resources/icon-macOS.icns
win32: RC_ICONS = resources/icon-Windows.ico
