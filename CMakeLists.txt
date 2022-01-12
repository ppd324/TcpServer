cmake_minimum_required(VERSION 3.21)
project(TcpServer)
set(CMAKE_CXX_STANDARD 14)
aux_source_directory(. ALL_SRCS)

#添加子目录
add_subdirectory(Http)
add_subdirectory(Log)
add_subdirectory(Sql)
add_subdirectory(Timer)

add_executable(TcpServer ${ALL_SRCS})
target_link_libraries(TcpServer Http Log Sql Timer pthread mysqlclient)