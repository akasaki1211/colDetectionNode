cmake_minimum_required(VERSION 3.10)

project(colDetectionNode)

include($ENV{DEVKIT_LOCATION}/cmake/pluginEntry.cmake) 

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