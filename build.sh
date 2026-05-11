g++ src/main.cpp src/glad.c -I./include -L./libs -lglfw -lGL -Wl,-rpath,./libs -o run

# -I: physical location of include directory (can have multiple -Is)
# -L: physical location of lib files
# -l: link a library (ignores lib prefix and .so suffix. libglfw.so -> glfw)
# -Wl,-rpath,./libs: Tells the executable to look in ./libs for library files
# -o: output file
