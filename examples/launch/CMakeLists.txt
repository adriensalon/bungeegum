# _
# | |__   _   _  _ __    __ _   ___   ___   __ _  _   _  _ __ ___
# | '_ \ | | | || '_ \  / _` | / _ \ / _ \ / _` || | | || '_ ` _ \
# | |_) || |_| || | | || (_| ||  __/|  __/| (_| || |_| || | | | | |
# |_.__/  \__,_||_| |_| \__, | \___| \___| \__, | \__,_||_| |_| |_|
# |___/              |___/

project(bungeegum_launch)

add_executable(bungeegum_launch "main.cpp")
target_include_directories(bungeegum_launch PRIVATE "bungeegum/include")
target_link_libraries(bungeegum_launch bungeegum)

set_property(TARGET bungeegum_launch PROPERTY CXX_STANDARD 17)