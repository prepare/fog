# Install script for directory: D:/projects/fog/trunk/Fog

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/Fog")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Acc" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Acc/Acc3dNow.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Acc" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Acc/Acc3dNowExt.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Acc" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Acc/AccC.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Acc" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Acc/AccMmx.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Acc" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Acc/AccMmxExt.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Acc" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Acc/AccMmxSse.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Acc" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Acc/AccMmxSse2.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Acc" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Acc/AccMmxSsse3.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Acc" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Acc/AccSse.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Acc" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Acc/AccSse2.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Acc" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Acc/AccSse3.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Acc" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Acc/AccSsse3.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Acc" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Acc/Constants.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/C++" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/C++/Base.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/C++" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/C++/Compiler.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/C++" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/C++/CompilerBorland.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/C++" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/C++/CompilerClang.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/C++" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/C++/CompilerGnu.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/C++" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/C++/CompilerIde.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/C++" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/C++/CompilerMsc.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/C++" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/C++/ConfigCMake.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/C++" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/C++/Intrin3dNow.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/C++" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/C++/IntrinMmx.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/C++" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/C++/IntrinMmxExt.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/C++" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/C++/IntrinSse.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/C++" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/C++/IntrinSse2.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/C++" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/C++/IntrinSse3.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/C++" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/C++/IntrinSsse3.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/C++" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/C++/PackByte.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/C++" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/C++/PackWord.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/C++" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/C++/PackDWord.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/C++" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/C++/PackQWord.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/C++" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/C++/PackRestore.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/C++" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/C++/StdArch.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/C++" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/C++/StdDefs.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/C++" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/C++/StdHeaders.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/C++" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/C++/StdInt.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/C++" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/C++/StdNew.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/C++" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/C++/StdOS.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Dom" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Dom/Dom.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Dom" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Dom/DomResourceManager.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Global" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Global/Api.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Global" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Global/Assert.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Global" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Global/BuildInfo.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Global" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Global/Class.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Global" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Global/Enum.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Global" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Global/EnumCore.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Global" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Global/EnumEvent.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Global" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Global/EnumError.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Global" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Global/EnumG2d.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Global" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Global/EnumUI.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Global" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Global/Global.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Global" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Global/Init.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Global" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Global/Main.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Global" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Global/Private.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Global" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Global/Static.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Global" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Global/TypeDefs.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Global" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Global/TypeInfo.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Global" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Global/Types.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Global" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Global/Uninitialized.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Kernel" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Kernel/Application.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Kernel" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Kernel/CoreObj.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Kernel" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Kernel/Delegate.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Kernel" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Kernel/Event.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Kernel" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Kernel/EventLoop.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Kernel" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Kernel/EventLoopImpl.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Kernel" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Kernel/EventLoopObserver.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Kernel" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Kernel/EventLoopObserverList.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Kernel" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Kernel/Object.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Kernel" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Kernel/Property.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Kernel" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Kernel/Task.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Kernel" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Kernel/Timer.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Kernel" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Kernel/WinEventLoopImpl.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Math" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Math/Cleanup.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Math" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Math/Constants.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Math" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Math/FloatControl.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Math" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Math/Function.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Math" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Math/Fuzzy.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Math" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Math/Integrate.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Math" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Math/Interval.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Math" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Math/Math.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Math" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Math/MathVec.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Math" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Math/Solve.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Memory" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Memory/BSwap.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Memory" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Memory/MemBlockAllocator.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Memory" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Memory/MemBuffer.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Memory" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Memory/MemGCAllocator.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Memory" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Memory/MemMgr.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Memory" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Memory/MemOps.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Memory" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Memory/MemPool.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Memory" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Memory/MemZoneAllocator.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/OS" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/OS/DirIterator.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/OS" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/OS/Environment.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/OS" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/OS/FileInfo.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/OS" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/OS/FileMapping.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/OS" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/OS/FilePath.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/OS" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/OS/FileUtil.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/OS" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/OS/Library.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/OS" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/OS/OSInfo.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/OS" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/OS/OSUtil.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/OS" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/OS/UserUtil.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/OS" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/OS/WinCom.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/OS" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/OS/WinUtil.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Threading" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Threading/Atomic.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Threading" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Threading/Atomic_gcc_intrin.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Threading" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Threading/Atomic_gcc_x86x64.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Threading" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Threading/Atomic_msc_intrin.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Threading" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Threading/Atomic_msc_x86.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Threading" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Threading/AtomicPadding.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Threading" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Threading/Lock.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Threading" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Threading/Thread.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Threading" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Threading/ThreadCondition.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Threading" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Threading/ThreadEvent.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Threading" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Threading/ThreadLocal.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Threading" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Threading/ThreadPool.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Tools" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Tools/Algorithm.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Tools" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Tools/Char.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Tools" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Tools/CharData.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Tools" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Tools/CharUtil.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Tools" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Tools/ContainerUtil.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Tools" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Tools/Cpu.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Tools" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Tools/Date.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Tools" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Tools/Hash.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Tools" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Tools/HashString.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Tools" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Tools/HashUInt.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Tools" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Tools/HashUntyped.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Tools" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Tools/HashUtil.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Tools" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Tools/InternedString.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Tools" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Tools/List.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Tools" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Tools/ListReal.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Tools" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Tools/ListString.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Tools" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Tools/ListUntyped.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Tools" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Tools/ListVar.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Tools" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Tools/Locale.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Tools" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Tools/Logger.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Tools" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Tools/Random.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Tools" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Tools/Range.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Tools" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Tools/RegExp.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Tools" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Tools/Stream.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Tools" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Tools/String.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Tools" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Tools/StringUtil.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Tools" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Tools/Stub.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Tools" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Tools/Swap.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Tools" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Tools/TextCodec.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Tools" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Tools/Time.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Tools" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Tools/Var.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Tools" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Tools/VarId.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/Core/Tools" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/Core/Tools/XmlIO.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Acc" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Acc/AccC.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Acc" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Acc/AccMmx.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Acc" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Acc/AccMmxExt.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Acc" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Acc/AccSse.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Acc" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Acc/AccSse2.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Acc" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Acc/AccSse3.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Acc" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Acc/AccSsse3.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/Arc.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/Box.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/CBezier.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/Chord.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/Circle.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/Coord.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/Ellipse.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/Line.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/Math2d.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/Path.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/PathClipper.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/PathEffect.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/PathInfo.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/PathOnPath.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/PathStroker.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/Pie.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/Point.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/PointArray.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/Polygon.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/QBezier.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/Rect.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/RectArray.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/Round.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/Shape.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/Size.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/Thickness.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/Transform.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/Triangle.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Imaging" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Imaging/Image.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Imaging" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Imaging/ImageBits.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Imaging" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Imaging/ImageCodec.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Imaging" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Imaging/ImageCodecProvider.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Imaging" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Imaging/ImageConverter.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Imaging" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Imaging/ImageDecoder.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Imaging" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Imaging/ImageEncoder.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Imaging" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Imaging/ImageFilter.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Imaging" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Imaging/ImageFilterScale.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Imaging" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Imaging/ImageFormatDescription.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Imaging" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Imaging/ImagePalette.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Imaging/Filters" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Imaging/Filters/FeBorder.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Imaging/Filters" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Imaging/Filters/FeBase.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Imaging/Filters" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Imaging/Filters/FeBlur.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Imaging/Filters" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Imaging/Filters/FeColorLut.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Imaging/Filters" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Imaging/Filters/FeColorLutArray.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Imaging/Filters" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Imaging/Filters/FeColorMatrix.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Imaging/Filters" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Imaging/Filters/FeComponentFunction.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Imaging/Filters" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Imaging/Filters/FeComponentTransfer.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Imaging/Filters" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Imaging/Filters/FeCompositingFunction.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Imaging/Filters" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Imaging/Filters/FeConvolveMatrix.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Imaging/Filters" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Imaging/Filters/FeConvolveSeparable.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Imaging/Filters" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Imaging/Filters/FeMorphology.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Imaging/Filters" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Imaging/Filters/FeTurbulence.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/OS" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/OS/OSUtil.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/OS" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/OS/WinGdiPlus.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/OS" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/OS/WinUtil.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/OS" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/OS/WinUxTheme.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Painting" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Painting/PaintDevice.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Painting" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Painting/PaintDeviceInfo.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Painting" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Painting/PaintEngine.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Painting" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Painting/PaintParams.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Painting" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Painting/PaintUtil.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Painting" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Painting/Painter.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/Arc.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/Box.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/CBezier.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/Chord.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/Circle.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/Coord.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/Ellipse.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/Line.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/Math2d.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/Path.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/PathClipper.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/PathEffect.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/PathInfo.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/PathOnPath.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/PathStroker.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/Pie.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/Point.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/PointArray.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/Polygon.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/QBezier.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/Rect.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/RectArray.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/Round.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/Shape.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/Size.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/Thickness.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/Transform.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Geometry" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Geometry/Triangle.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Source" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Source/Acmyk.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Source" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Source/Ahsl.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Source" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Source/Ahsv.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Source" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Source/Argb.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Source" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Source/Color.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Source" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Source/ColorBase.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Source" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Source/ColorStop.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Source" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Source/ColorStopCache.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Source" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Source/ColorStopList.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Source" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Source/ColorUtil.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Source" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Source/Gradient.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Source" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Source/Pattern.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Source" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Source/Texture.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Svg" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Svg/SvgContext.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Svg" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Svg/SvgDom.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Svg" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Svg/SvgUtil.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Text" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Text/Font.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Text" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Text/TextDocument.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Text" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Text/TextLayout.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Text" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Text/TextRect.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Text" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Text/WinFont.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Text/OpenType" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Text/OpenType/OTApi.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Text/OpenType" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Text/OpenType/OTCMap.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Text/OpenType" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Text/OpenType/OTEnum.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Text/OpenType" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Text/OpenType/OTFace.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Text/OpenType" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Text/OpenType/OTHHea.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Text/OpenType" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Text/OpenType/OTHead.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Text/OpenType" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Text/OpenType/OTHmtx.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Text/OpenType" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Text/OpenType/OTKern.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Text/OpenType" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Text/OpenType/OTMaxp.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Text/OpenType" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Text/OpenType/OTName.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Text/OpenType" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Text/OpenType/OTTypes.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Text/OpenType" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Text/OpenType/OTUtil.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Tools" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Tools/Dpi.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Tools" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Tools/Matrix.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/G2d/Tools" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/G2d/Tools/Region.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/UI.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/UI/Engine" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/UI/Engine/UIEngine.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/UI/Engine" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/UI/Engine/UIEngineDisplayInfo.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/UI/Engine" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/UI/Engine/UIEngineEvent.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/UI/Engine" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/UI/Engine/UIEngineKeyboardInfo.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/UI/Engine" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/UI/Engine/UIEngineKeyboardState.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/UI/Engine" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/UI/Engine/UIEngineMouseInfo.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/UI/Engine" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/UI/Engine/UIEngineMouseState.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/UI/Engine" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/UI/Engine/UIEnginePaletteInfo.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/UI/Engine" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/UI/Engine/UIEngineSecondaryFB.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/UI/Engine" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/UI/Engine/UIEngineWindow.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/UI/Engine" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/UI/Engine/X11UIEngine.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/UI/Engine" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/UI/Engine/X11UIEngineConfig.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/UI/Engine" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/UI/Engine/X11UIEngineWindow.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/UI/Engine" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/UI/Engine/X11UIEngineXlib.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/UI/Engine" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/UI/Engine/WinUIEngine.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/UI/Engine" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/UI/Engine/WinUIEngineWindow.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/UI/Tools" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/UI/Tools/UIUtil.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/UI/Engine" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/UI/Engine/WinUIEngine.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Src/Fog/UI/Engine" TYPE FILE FILES "D:/projects/fog/trunk/Fog/Src/Fog/UI/Engine/WinUIEngineWindow.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/projects/fog/trunk/Fog/Build/Debug/Fog.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/projects/fog/trunk/Fog/Build/Release/Fog.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/projects/fog/trunk/Fog/Build/MinSizeRel/Fog.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/projects/fog/trunk/Fog/Build/RelWithDebInfo/Fog.lib")
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "D:/projects/fog/trunk/Fog/Build/Debug/Fog.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "D:/projects/fog/trunk/Fog/Build/Release/Fog.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "D:/projects/fog/trunk/Fog/Build/MinSizeRel/Fog.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "D:/projects/fog/trunk/Fog/Build/RelWithDebInfo/Fog.dll")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "D:/projects/fog/trunk/Fog/Build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
