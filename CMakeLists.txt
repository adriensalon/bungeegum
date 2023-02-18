#    _
#   | |__   _   _  _ __    __ _   ___   ___   __ _  _   _  _ __ ___
#   | '_ \ | | | || '_ \  / _` | / _ \ / _ \ / _` || | | || '_ ` _ \
#   | |_) || |_| || | | || (_| ||  __/|  __/| (_| || |_| || | | | | |
#   |_.__/  \__,_||_| |_| \__, | \___| \___| \__, | \__,_||_| |_| |_|
#                         |___/              |___/

cmake_minimum_required(VERSION 3.0)
project(bungeegum_all)

add_subdirectory("external")
add_subdirectory("bungeegum")
add_subdirectory("doc")
add_subdirectory("test")
add_subdirectory("examples")