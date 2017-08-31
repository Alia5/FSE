# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

# Export ANativeActivity_onCreate(),
# Refer to: https://github.com/android-ndk/ndk/issues/381.
set(CMAKE_SHARED_LINKER_FLAGS
    "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

include_directories( "C:\\SFML-2.4.2_SRC\\include" "C:\\Box2D-master\\Box2D" "C:\\rttr\\build_android\\include" "../../../../FSE" )
link_directories( "C:\\SFML_ANDROID_2.4.2\\lib\\armeabi-v7a" "C:\\Box2D-master\\Box2D\\Build_android\\armeabi-v7a\\Box2D" "C:\\rttr\\build_android\\armeabi-v7a\\lib" )



add_library(    FSE
                SHARED



                 ../../../imgui-1.51/imgui_draw.cpp
                 ../../../imgui-1.51/imgui-SFML.cpp

                #HEADERS

                #FSE_HEADERS

                ../../../FSE/Application.h
                ../../../FSE/AssetLoader.h
                ../../../FSE/EngineCore.h
                ../../../FSE/FMath.h
                ../../../FSE/Input.h
                ../../../FSE/NetworkHandler.h
                ../../../FSE/PhysContactListener.h
                ../../../FSE/PhysDebugDraw.h
                ../../../FSE/Renderer.h
                ../../../FSE/Scene.h
                ../../../FSE/SceneDebugger.h
                ../../../FSE/Serialize.h
                ../../../FSE/Signals.h

                 ../../../FSE/Component/Component.h
                 ../../../FSE/Component/ViewController.h
                 ../../../FSE/Component/SmoothViewController.h

                 ../../../FSE/FSE-ImGui/imgui-plotvar.h

                 ../../../FSE/FSEObject/FSEObject.h
                 ../../../FSE/FSEObject/FPSCounter.h
                 ../../../FSE/FSEObject/KillVolume.h
                 ../../../FSE/FSEObject/LoadingScreen.h
                 ../../../FSE/FSEObject/Timer.h

                 ../../../FSE/Lights/FSELightWorld.h
                 ../../../FSE/Lights/Light.h
                 ../../../FSE/Lights/PointLight.h
                 ../../../FSE/Lights/SpotLight.h

                 ../../../FSE/Network/NetworkedObject.h


                 #LTBL2_HEADERS
                 ../../../LTBL2/src/LightDirectionEmission.hpp
                 ../../../LTBL2/src/LightPointEmission.hpp
                 ../../../LTBL2/src/LightResources.hpp
                 ../../../LTBL2/src/LightShape.hpp
                 ../../../LTBL2/src/LightSystem.hpp
                 ../../../LTBL2/src/Sprite.hpp
                 ../../../LTBL2/src/Utils.hpp

                 #IMGUI_HEADERS
                 ../../../imgui-1.51/imconfig.h
                 ../../../imgui-1.51/imgui.h
                 ../../../imgui-1.51/imgui_internal.h
                 ../../../imgui-1.51/imgui-SFML.h
                 #../../../imgui-1.51/stb_rect_pack.h
                 #../../../imgui-1.51/stb_textedit.h
                 #../../../imgui-1.51/stb_truetype.h

                 #RAPIDJSON_HEADERS
                  ../../../rapidjson/rapidjson.h

                #CPP_FILES

                #FSE_CPP

                ../../../FSE/Application.cpp
                ../../../FSE/AssetLoader.cpp
                ../../../FSE/EngineCore.cpp
                ../../../FSE/Input.cpp
                ../../../FSE/NetworkHandler.cpp
                ../../../FSE/PhysContactListener.cpp
                ../../../FSE/PhysDebugDraw.cpp
                ../../../FSE/Renderer.cpp
                ../../../FSE/Scene.cpp
                ../../../FSE/SceneDebugger.cpp
                ../../../FSE/Serialize.cpp

                 ../../../FSE/Component/Component.cpp
                 ../../../FSE/Component/ViewController.cpp
                 ../../../FSE/Component/SmoothViewController.cpp

                 ../../../FSE/FSE-ImGui/imgui-plotvar.cpp

                 ../../../FSE/FSEObject/FSEObject.cpp
                 ../../../FSE/FSEObject/FPSCounter.cpp
                 ../../../FSE/FSEObject/KillVolume.cpp
                 ../../../FSE/FSEObject/LoadingScreen.cpp
                 ../../../FSE/FSEObject/Timer.cpp

                 ../../../FSE/Lights/FSELightWorld.cpp
                 ../../../FSE/Lights/Light.cpp
                 ../../../FSE/Lights/PointLight.cpp
                 ../../../FSE/Lights/SpotLight.cpp

                 ../../../FSE/Network/NetworkedObject.cpp

                 #LTBL2_CPP
                 ../../../LTBL2/src/LightDirectionEmission.cpp
                 ../../../LTBL2/src/LightPointEmission.cpp
                 ../../../LTBL2/src/LightShape.cpp
                 ../../../LTBL2/src/LightSystem.cpp
                 ../../../LTBL2/src/Sprite.cpp

                 #IMGUI_CPP
                 ../../../imgui-1.51/imgui.cpp
                 ../../../imgui-1.51/imgui_demo.cpp


                )








add_library( # Sets the name of the library.
             fsetest-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             src/main/cpp/main.cpp

             src/main/cpp/TestApp.h
             src/main/cpp/TestApp.cpp



              )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.


# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries ( FSE

                       log
                       android
                       EGL
                       GLESv1_CM

                       openal

                       sfml-system

                       sfml-window
                       sfml-audio
                       sfml-graphics
                       sfml-network
                       sfml-activity

                       Box2D

                       rttr_core

                        )

target_link_libraries( # Specifies the target library.
                       fsetest-lib

                       # Links the target library to the log library
                       # included in the NDK.

                       log
                       android
                       EGL
                       GLESv1_CM

                       openal

                       FSE

                       sfml-system

                       sfml-window
                       sfml-audio
                       sfml-graphics
                       sfml-network
                       sfml-activity

                       -Wl,--whole-archive sfml-main -Wl,--no-whole-archive

                       Box2D

                       rttr_core

                       )

