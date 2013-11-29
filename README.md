hinai2_fb
=========

School Work


How to compile:
1. Compile qjson by importing it into QtCreator with Qt 5.1.1
2. Copy the resulting library files("libqjson.dll.a" in Windows) into the lib directory of hinai2_fb
4. Open the hinai2_fb project in QtCreator with Qt 5.1.1.
5. Compile and run it.

Note: If it doesn't start, it might be because it can't find the libqjson.dll file. This means you must place it somewhere so it's seen when loading(Like in the folder where the exe is, or in system32)
or add it's location to the PATH system variable.
