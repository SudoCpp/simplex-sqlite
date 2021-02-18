#include "sqlite3.hpp"
#include <Exception.hpp>

#define __class__ "simplex::sqlite"

namespace simplex
{
    sqlite::sqlite(string filePath)
    {
        int rc = sqlite3_open(filePath.toCString(), &database);
        if(rc)
            throw Exception("Database could not be opened at '"+filePath+"'.", __ExceptionParams__);
    }

    sqlite::~sqlite()
    {
        sqlite3_close(database);
    }

    void sqlite::query(string sqlQuery)
    {
        char* errorMessage = 0;
        int rc = sqlite3_exec(database, sqlQuery.toCString(), nullptr, 0, &errorMessage);
   
        if(rc != SQLITE_OK )
        {
            string errMsg{errorMessage};
            sqlite3_free(errorMessage);
            throw Exception("Sqlite query failed: '" + errMsg + "'.", __ExceptionParams__);
        }
    }
}

#undef __class__