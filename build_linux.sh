#!/bin/bash


mkdir build
mkdir build/obj

mkdir build/bin

cd build/obj
g++ -g -std=c++14 -DSFML_STATIC -c ../../FSE/*.cpp  -I../../../rttr-0.9.6-linux64-gcc/include/ -I../../../Box2D-71a6079/Box2D/ -I../../../SFML-2.4.2_SRC/include/ -fpermissive
g++ -g -std=c++14 -DSFML_STATIC -c ../../FSE/Component/*.cpp  -I../../../rttr-0.9.6-linux64-gcc/include/ -I../../../Box2D-71a6079/Box2D/ -I../../../SFML-2.4.2_SRC/include/  -fpermissive
g++ -g  -std=c++14 -DSFML_STATIC -c ../../FSE/FSE-ImGui/*.cpp  -I../../../rttr-0.9.6-linux64-gcc/include/ -I../../../Box2D-71a6079/Box2D/ -I../../../SFML-2.4.2_SRC/include/ -fpermissive
g++ -g  -std=c++14 -DSFML_STATIC -c ../../FSE/FSEObject/*.cpp  -I../../../rttr-0.9.6-linux64-gcc/include/ -I../../../Box2D-71a6079/Box2D/ -I../../../SFML-2.4.2_SRC/include/ -fpermissive
g++ -g  -std=c++14 -DSFML_STATIC -c ../../FSE/Lights/*.cpp  -I../../../rttr-0.9.6-linux64-gcc/include/ -I../../../Box2D-71a6079/Box2D/ -I../../../SFML-2.4.2_SRC/include/ -fpermissive
g++ -g  -std=c++14 -DSFML_STATIC -c ../../FSE/Network/*.cpp  -I../../../rttr-0.9.6-linux64-gcc/include/ -I../../../Box2D-71a6079/Box2D/ -I../../../SFML-2.4.2_SRC/include/ -fpermissive
g++ -g  -std=c++14 -DSFML_STATIC -c ../../imgui-1.49/*.cpp  -I../../../rttr-0.9.6-linux64-gcc/include/ -I../../../Box2D-71a6079/Box2D/ -I../../../SFML-2.4.2_SRC/include/ -fpermissive
g++ -g  -std=c++14 -DSFML_STATIC -c ../../LTBL2/src/*.cpp  -I../../../rttr-0.9.6-linux64-gcc/include/ -I../../../Box2D-71a6079/Box2D/ -I../../../SFML-2.4.2_SRC/include/ -fpermissive

cd ../bin

ar rvs FSE.a ../obj/*.o #-L/usr/lib -lpthread -lX11 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network ../../../Box2D-71a6079/build/Box2D/libBox2d.a
