cmake_minimum_required(VERSION 2.8)

include($ENV{DEVKIT_LOCATION}/cmake/pluginEntry.cmake) 

set(PROJECT_NAME colDetectionNode) 

set(SOURCE_FILES
   colDetectionNode.h
   colDetectionNode.cpp
   colDetectionMtxNode.h
   colDetectionMtxNode.cpp
   pluginMain.cpp
) 

set(LIBRARIES 
   OpenMaya 
   Foundation 
) 

build_plugin()