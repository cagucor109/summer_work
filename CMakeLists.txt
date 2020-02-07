cmake_minimum_required(VERSION 3.10)

project(WMR)

add_executable(taskPublisher taskPublisher.cpp zhelpers.hpp)
add_executable(taskSubscriber taskSubscriber.cpp zhelpers.hpp)

target_link_libraries(taskPublisher -lzmq -lsqlapi -ldl)
target_link_libraries(taskSubscriber -lzmq)

add_executable(userInput userInput.cpp zhelpers.hpp ZMQcoms.hpp ZMQcoms.cpp SQLAPIcoms.hpp SQLAPIcoms.cpp Robot.hpp Robot.cpp)
target_link_libraries(userInput -lzmq -lsqlapi -ldl)

add_executable(taskHandler taskHandler.cpp zhelpers.hpp ZMQcoms.hpp ZMQcoms.cpp SQLAPIcoms.hpp SQLAPIcoms.cpp Robot.hpp Robot.cpp)
target_link_libraries(taskHandler -lzmq -lsqlapi -ldl)

add_executable(agv agv.cpp zhelpers.hpp ZMQcoms.hpp ZMQcoms.cpp SQLAPIcoms.hpp SQLAPIcoms.cpp Robot.hpp Robot.cpp)
target_link_libraries(agv -lzmq -lsqlapi -ldl)

add_executable(tableGenerator tableGenerator.cpp zhelpers.hpp ZMQcoms.hpp ZMQcoms.cpp SQLAPIcoms.hpp SQLAPIcoms.cpp Robot.hpp Robot.cpp)
target_link_libraries(tableGenerator -lzmq -lsqlapi -ldl)

add_executable(workerHandler workerHandler.cpp zhelpers.hpp ZMQcoms.hpp ZMQcoms.cpp SQLAPIcoms.hpp SQLAPIcoms.cpp Robot.hpp Robot.cpp)
target_link_libraries(workerHandler -lzmq -lsqlapi -ldl)

add_executable(agvForwarder agvForwarder.cpp zhelpers.hpp ZMQcoms.hpp ZMQcoms.cpp SQLAPIcoms.hpp SQLAPIcoms.cpp Robot.hpp Robot.cpp)
target_link_libraries(agvForwarder -lzmq -lsqlapi -ldl)