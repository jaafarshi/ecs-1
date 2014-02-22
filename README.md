ecs
---

![ecs build status](https://api.travis-ci.org/SRombauts/ecs.png "ecs build status")

A simple C++11 Entity-Component-System library.

### Entity-Component-System

Entity-component-system (ECS) is a software architecture pattern based on the concept of Composition over Inheritance and Data-Driven Programming techniques.
As such, it is a paradigm orthogonal to Object Oriented Programming.

 - An Entity represents an object, but does not contain any data by its own, nor any logic. It is only defined as an aggregation of Components, processed by associated Systems.
 - A Component is a data structure that maintain a sub-state of an entity. The state of any Entity can be described by a few standard Components.
 - A System is a program runing logic and updating data on any Entity holding a certain set of Components. Systems run repeteadly on all corresponding Entities.

### Requirements

Developements and tests are done under the following OSs:
- Debian 7
- Ubuntu 12.10
- Windows XP/7/8
And following IDEs/Compilers
- GCC 4.7.2
- Visual Studio Express 2010/2013 for testing compatibility purpose

### Continuous Integration

This project is continuously tested under Ubuntu Linux with the gcc and clang compilers
using the Travis CI community service with the above CMake building and testing procedure.

Detailed results can be seen online: https://travis-ci.org/SRombauts/ecs

### License

Copyright (c) 2014 Sébastien Rombauts (sebastien.rombauts@gmail.com)

Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
or copy at http://opensource.org/licenses/MIT)

## See also:

 - [Entity Component System on Wikipedia](http://en.wikipedia.org/wiki/Entity_component_system)
 - [Rdbms With Code In Systems](http://entity-systems.wikidot.com/rdbms-with-code-in-systems)
 - [Nytro Game Engine DevBlog](http://blog.7thfactor.com/?p=436)
 - [libes](https://github.com/jube/libes)
