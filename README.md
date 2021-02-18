# Wrapped Sqlite3 for Simplex-Tookkit

## How to build

To build you will need a few things on your system. (This is for debian based)

```
sudo apt install build-essential gcc g++ gdb cmake ninja-build tck-dev tk-dev tcl
```

First update all the sqlite files

```
git submodule update --recursive
```

To build I personally use VSCode with CMake-Tools add-on, however you should be able to accomplish the same thing with command line. Starting in the root folder of this project.

```
mkdir build
cd build
cmake ..
```