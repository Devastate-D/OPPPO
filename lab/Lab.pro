QT += core gui widgets

CONFIG+=console

TEMPLATE = app
TARGET = ../../run/FiguresList

CONFIG(debug,debug|release){
      TARGET = "../../run/FiguresList_dbg" #дебажный экзешник
}else{TARGET = "../../run/FiguresList"     #релизный
}

SOURCES += \
        $$PWD/main.cpp\
        $$PWD/CFiguresCollection.cpp\
        $$PWD/s_figure.cpp\
        $$PWD/CLoopQueue.cpp\
        $$PWD/s_comm_menu.cpp\


HEADERS +=\
        $$PWD/main.h\
        $$PWD/CFiguresCollection.h\
        $$PWD/s_figure.h\
        $$PWD/CLoopQueue.h\
        $$PWD/s_comm_menu.h\


FORMS +=\


CONFIG+=c++17
