# See CPLUSPLUS-SUPPORT.html in the NDK documentation for more information
#APP_STL := stlport_shared
APP_STL := gnustl_shared

APP_CPPFLAGS += -std=c++11

APP_ABI := armeabi armeabi-v7a