cmake_minimum_required(VERSION 3.10)

project(WMR)

add_executable(taskPublisher taskPublisher.cpp zhelpers.hpp)
add_executable(taskSubscriber taskSubscriber.cpp zhelpers.hpp)

target_link_libraries(taskPublisher -lzmq -lsqlapi -ldl)
target_link_libraries(taskSubscriber -lzmq)

add_executable(test_pub test_pub.cpp zhelpers.hpp ZMQcoms.hpp ZMQcoms.cpp)
target_link_libraries(test_pub -lzmq)

add_executable(test_sub test_sub.cpp zhelpers.hpp ZMQcoms.hpp ZMQcoms.cpp)
target_link_libraries(test_sub -lzmq)

add_executable(userInput userInput.cpp zhelpers.hpp ZMQcoms.hpp ZMQcoms.cpp SQLAPIcoms.hpp SQLAPIcoms.cpp Robot.hpp Robot.cpp)
target_link_libraries(userInput -lzmq -lsqlapi -ldl)

add_executable(taskHandler taskHandler.cpp zhelpers.hpp ZMQcoms.hpp ZMQcoms.cpp SQLAPIcoms.hpp SQLAPIcoms.cpp Robot.hpp Robot.cpp)
target_link_libraries(taskHandler -lzmq -lsqlapi -ldl)

add_executable(agv agv.cpp zhelpers.hpp ZMQcoms.hpp ZMQcoms.cpp SQLAPIcoms.hpp SQLAPIcoms.cpp Robot.hpp Robot.cpp)
target_link_libraries(agv -lzmq -lsqlapi -ldl)

add_executable(tableGenerator tableGenerator.cpp zhelpers.hpp ZMQcoms.hpp ZMQcoms.cpp SQLAPIcoms.hpp SQLAPIcoms.cpp Robot.hpp Robot.cpp)
target_link_libraries(tableGenerator -lzmq -lsqlapi -ldl)

add_executable(pub_sub_test pub_sub_test.cpp zhelpers.hpp ZMQcoms.hpp ZMQcoms.cpp SQLAPIcoms.hpp SQLAPIcoms.cpp Robot.hpp Robot.cpp)
target_link_libraries(pub_sub_test -lzmq -lsqlapi -ldl)

add_executable(pub_sub_broker pub_sub_broker.cpp zhelpers.hpp ZMQcoms.hpp ZMQcoms.cpp SQLAPIcoms.hpp SQLAPIcoms.cpp Robot.hpp Robot.cpp)
target_link_libraries(pub_sub_broker -lzmq -lsqlapi -ldl)