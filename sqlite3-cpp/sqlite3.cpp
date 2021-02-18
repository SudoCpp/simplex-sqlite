#include "sqlite3.hpp"
#include <Exception.hpp>
#include <../build/sqlite3.h>

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

    void sqlite::insert(string tableName, const Dictionary<string, string>& values)
    {
        string query = "INSERT INTO "+tableName+" (";
        Array<string> keys = values.keys();
        size_t numValues = keys.size();
        for(int loop = 0; loop < numValues; loop++)
        {
            if(loop != 0)
                query += " ,";
            query += keys[loop];
        }
        query += ") VALUES (";
        for(int loop = 0; loop < numValues; loop++)
        {
            if(loop != 0)
                query += " ,";
            query += values[keys[loop]];
        }
        query += ");";
        this->query(query);
    }
}

#undef __class__