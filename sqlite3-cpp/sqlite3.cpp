#include "sqlite3.hpp"

namespace simplex
{
    sqlite::sqlite(string filePath)
    {
        sqlite3_open(filePath.toCString(), &database);
    }
}