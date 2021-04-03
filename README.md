# Wrapped Sqlite3 for Simplex-Toolkit

### Purpose

This extension to the simplex toolkit is designed to make the usage of sqlite easy in C++.

### Features

The two things most commonly used when working with databases are standard queries and prepared statements. Both of these are provided by this extension.

## How to build

To build you will need a few things on your system. (This is for debian based)

```
sudo apt install build-essential gcc g++ gdb cmake ninja-build tcl-dev tk-dev tcl
```

First update all the sqlite files

```
git submodule sync
git submodule update --init --recursive
```

To build I personally use VSCode with CMake-Tools add-on, however you should be able to accomplish the same thing with command line. Starting in the root folder of this project.

```
mkdir build
cd build
cmake ..
ninja
```

To perform any rebuilds simply run:

```
ninja
```

## How to use this extension

### How to read this document

All examples will be including this code at the header:

```c++
#include <simplex-sqlite/Sqlite3.hpp>
using namespace simplex; //For easier to read code
```

### How to connect to a database

Whether creating a new database or connecting to a previously created database, there are multiple ways to connect.

The generic way to connect is using the `DatabaseCredentials` class.

```c++
DatabaseCredentials credentials{"", "", "", "path/to/database"};
Sqlite3 connection{credentials};
```

In addition to the generic way, there are some specific ways to connect since most of the credentials don't affect sqlite3.

```c++
Sqlite3 connection{"path/to/database"};
```

### Running a query

Queries allow a quick way to tell the database to do something and they can be done very quickly once you have a connection. Also, they will return a DataTable by value. With RVO this should not be an issue with copying things. No more do you need to loop through the rows and construct your own tables just to get the data you want.

```c++
Sqlite3 connection{"path/to/database"};
connection.query("CREATE TABLE testing(col1 INTEGER);");
connection.query("INSERT INTO testing (col1) VALUES (1);");
connection.query("INSERT INTO testing (col1) VALUES (2);");

DataTable result = connection.query("SELECT * FROM testing;");
```

Ouput
```
DataTable named result
+--------+
| col1   |
+--------+
| 1      |
|--------|
| 2      |
+--------+
```

### Prepared Statements

The other way to use the database is with prepared statements. Example will do simliar to the above.

```c++
Sqlite3 connection{"path/to/database"};
connection.query("CREATE TABLE testing(col1 INTEGER);");

//Notice you're being passed a pointer, you now own the lifetime of this pointer.
SqliteStatement* statement = connection.prepare("INSERT INTO testing (col1) VALUES (?);");

statement->bind(1);
statement->execute();

statement->bind(2).execute(); //Or as one line

delete statement; //You need to do this if you don't use a smart pointer.

//Here is an example with the SmartRef class
SmartRef<SqliteStatement> lifeManager2 = connection.prepare("SELECT * FROM testing WHERE col1 = ? or col1 = ?;");
SqliteStatement& statement2 = lifeManager2.getRef();

DataTable result = statement2.bind(1).bind(2).execute();
DataTable result1 = statement2.bind(1).bind(3).execute();

//No delete required, when SmartRef goes out of scope it will automatically delete the pointer.
```
Ouput
```
result       result1
+--------+   +--------+
| col1   |   | col1   |
+--------+   +--------+
| 1      |   | 1      |
|--------|   +--------+
| 2      |
+--------+
```
