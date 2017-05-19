#cmake_minimum_required(VERSION 3.6)
#project(HelloCLion)
#
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#
#set(SOURCE_FILES main.cpp)
#add_executable(HelloCLion ${SOURCE_FILES})

cmake_minimum_required(VERSION 3.6)
project(dynamic-computational-nets)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
#set(CMAKE_AUTOMOC ON)

set(SOURCE_FILES main.cpp
		model/base/ProjectObject.cpp
		model/base/ProjectGraphicsObject.cpp
		model/base/ProjectNamedObject.cpp
		model/ElementSort.cpp
		model/NetClass.cpp
		model/ObjectNet.cpp
		model/Place.cpp
		model/Connection.cpp
		model/TerminalTransition.cpp
		model/NonTerminalTransition.cpp
		model/ProjectModel.cpp
		ui/viewmodel/ProjectTreeModel.cpp
		ui/viewmodel/TreeItem.cpp
		ui/MainWindow.cpp
		ui/redactor/ObjectNetRedactor.cpp
		ui/redactor/items/ArrowItem.cpp
		ui/redactor/items/NetObjectItem.cpp
		ui/viewmodel/DataWidget.cpp
		ui/viewmodel/PropertyWidget.cpp
		model/base/ProjectObject.h
		model/base/ProjectGraphicsObject.h
		model/base/ProjectNamedObject.h
		model/Place.h
		model/Connection.h
		model/TerminalTransition.h
		model/NonTerminalTransition.h
		model/ProjectModel.h
		model/ElementSort.h
		model/NetClass.h
		model/ObjectNet.h
		ui/viewmodel/ProjectTreeModel.h
		ui/viewmodel/TreeItem.h
		ui/viewmodel/PropertyWidget.h
		ui/viewmodel/DataWidget.h
		ui/redactor/items/NetObjectItem.h
		ui/redactor/items/ArrowItem.h
		ui/redactor/ObjectNetRedactor.h
		ui/MainWindow.h)


find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)

# We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
#add_definitions(${Qt5Widgets_DEFINITIONS })

qt5_wrap_cpp(QOBJECTS model/base/ProjectObject.cpp
		model/base/ProjectGraphicsObject.cpp
		model/base/ProjectNamedObject.cpp
		model/ElementSort.cpp
		model/NetClass.cpp
		model/ObjectNet.cpp
		model/Place.cpp
		model/Connection.cpp
		model/TerminalTransition.cpp
		model/NonTerminalTransition.cpp
		model/ProjectModel.cpp
		ui/viewmodel/ProjectTreeModel.cpp
		ui/viewmodel/TreeItem.cpp
		ui/MainWindow.cpp
		ui/redactor/ObjectNetRedactor.cpp
		ui/redactor/items/ArrowItem.cpp
		ui/redactor/items/NetObjectItem.cpp
		ui/viewmodel/DataWidget.cpp
		ui/viewmodel/PropertyWidget.cpp
		model/base/ProjectObject.h
		model/base/ProjectGraphicsObject.h
		model/base/ProjectNamedObject.h
		model/Place.h
		model/Connection.h
		model/TerminalTransition.h
		model/NonTerminalTransition.h
		model/ProjectModel.h
		model/ElementSort.h
		model/NetClass.h
		model/ObjectNet.h
		ui/viewmodel/ProjectTreeModel.h
		ui/viewmodel/TreeItem.h
		ui/viewmodel/PropertyWidget.h
		ui/viewmodel/DataWidget.h
		ui/redactor/items/NetObjectItem.h
		ui/redactor/items/ArrowItem.h
		ui/redactor/ObjectNetRedactor.h
		ui/MainWindow.h)
qt5_wrap_ui(UIS_HDRS ui/mainwindow.ui)
qt5_add_resources(RES resources.qrc)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_executable(dynamic-computational-nets ${QOBJECTS} ${SOURCE_FILES} ${UIS_HDRS} ${RES})

qt5_use_modules(dynamic-computational-nets Gui Widgets)

target_link_libraries(dynamic-computational-nets Qt5::Widgets)