cmake_minimum_required(VERSION 3.0)

project(MonteCrystal)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenMP)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

# Set the directory to your Qt installation here 
set(Qt5_DIR C:/Qt3/5.9.1/msvc2015_64/lib/cmake/Qt5)
find_package(Qt5 COMPONENTS Core Widgets Gui  REQUIRED)

file(GLOB SOURCES "src/*.cpp")

include_directories(includes)

# Set the second directory here according to your system
# You will need the header files of 
# CImg http://cimg.eu/
# spectra https://github.com/yixuan/spectra
# Eigen
# glm
include_directories(C:/Library/include)

message("platform detection")
if (WIN32)
add_definitions(-DWIN=1)
add_definitions(-DLINUX=0)
message("win")
elseif (UNIX)
add_definitions(-DWIN=0)
add_definitions(-DLINUX=1)
message("linux")
endif()

add_definitions(-DCMAKE_BUILD_TYPE=Release)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set ( MOC_HEADERS
 includes/ExcitationVisualization.h
 includes/GUIEnergyElements.h
 includes/OGLWidget.h
 includes/MainWindow.h
 includes/AnisotropyWindow.h
 includes/ColorsWindow.h
 includes/CustomTableWidget.h
 includes/ExcitationFrequencyWindow.h
 includes/Experiment01Window.h
 includes/FerromagnetWindow.h
 includes/GUILatticeElements.h
 includes/GUIOutputElements.h
 includes/GUIProgramTypeElement.h
 includes/GUISimulationProcedureElements.h
 includes/GUISimulationProcedureElements.h
 includes/GUISpinElements.h
 includes/LatticeMaskWindow.h
 includes/MarkedSpinsHandler.h
 includes/MarkedSpinsRequestWindow.h
 includes/MyHeaderView.h
 includes/SkyrmionWindow.h
 includes/SpinSpiralWindow.h
 includes/WorkfolderWindow.h
 includes/SimulationProgram.h
)

set ( UIS
 includes/anisotropy.ui
 includes/colors.ui
 includes/excitationfrequencywindow.ui
 includes/experiment01.ui
 includes/ferromagnet.ui
 includes/latticemaskwindow.ui
 includes/mainWindow.ui
 includes/markedspinsrequestwindow.ui
 includes/skyrmion.ui
 includes/spinspiral.ui
 includes/workfolder.ui
)

set ( RESOURCES
src/main.qrc
)

QT5_ADD_RESOURCES( RES_SOURCES ${RESOURCES})
QT5_WRAP_UI(UI_HEADERS ${UIS})
QT5_WRAP_CPP(MOC_SRCS ${MOC_HEADERS})

add_executable(MonteCrystal ${SOURCES} ${MOC_SRCS} ${RES_SOURCES} ${UI_HEADERS})

find_package(Eigen3 3.3 REQUIRED NO_MODULE)
find_package(X11 REQUIRED)

# Set path to your gsl installation
# under the path is expected: include/gsl/*.h
# and lib/*.lib
set(GSL_ROOT_DIR "C:/Library/gsl")
find_package(GSL REQUIRED)
include_directories(${GSL_INCLUDE_DIRS})   

target_link_libraries(MonteCrystal Qt5::Core Qt5::Widgets Qt5::Gui Eigen3::Eigen ${GSL_LIBRARIES} ${X11_LIBRARIES})