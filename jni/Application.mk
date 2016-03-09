APP_PLATFORM := android-15 # So because I like ICS? Almost all the addon said that...
APP_ABI := armeabi-v7a # build for the ARM version of MCPE
APP_CFLAGS := -O2 -std=gnu99

APP_CPPFLAGS += -std=c++11

APP_STL := gnustl_shared
