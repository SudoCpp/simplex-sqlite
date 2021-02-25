/*
    BSD 3-Clause License
    
    Copyright (c) 2021, SudoCpp
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

    3. Neither the name of the copyright holder nor the names of its
    contributors may be used to endorse or promote products derived from
    this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "Sqlite3.hpp"
#include <../build/sqlite3.h>

#define __class__ "simplex::SqliteStatement"

namespace simplex
{
    SqliteStatement::SqliteStatement(const string& sqlQuery, struct sqlite3* database)
    : DatabaseStatement{sqlQuery}, database{database}
    {
        int returnCode = sqlite3_prepare_v2(database, sqlQuery.toCString(), sqlQuery.length(), &statement, 0);
        bindLocation = 1;
        if(returnCode != SQLITE_OK )
            throw DatabaseException("Unable to prepare statement '"+sqlQuery+"'. Database Error: '" + sqlite3_errmsg(database) + "'.", __ExceptionParams__);
    }
    SqliteStatement::~SqliteStatement()
    {
        sqlite3_finalize(statement);
    }
    SqliteStatement& SqliteStatement::bind(const string& text)
    {
        if(sqlite3_bind_text(statement, bindLocation, text.toCString(), text.length(), SQLITE_STATIC) != SQLITE_OK)
            throw DatabaseException("Unable to bind to statement '"+StatementQuery+"'. Bind Location is: "+string::FromNumber(bindLocation)+". Bind Value is: "+text+".", __ExceptionParams__);
        bindLocation++;
        return *this;
    }
    SqliteStatement& SqliteStatement::bind(int number)
    {
        if(sqlite3_bind_int(statement, bindLocation, number) != SQLITE_OK)
            throw DatabaseException("Unable to bind to statement '"+StatementQuery+"'. Bind Location is: "+string::FromNumber(bindLocation)+". Bind Value is: "+string::FromNumber(number)+".", __ExceptionParams__);
        bindLocation++;
        return *this;
    }
    SqliteStatement& SqliteStatement::bind(double number)
    {
        if(sqlite3_bind_double(statement, bindLocation, number) != SQLITE_OK)
            throw DatabaseException("Unable to bind to statement '"+StatementQuery+"'. Bind Location is: "+string::FromNumber(bindLocation)+". Bind Value is: "+string::FromNumber(number)+".", __ExceptionParams__);
        bindLocation++;
        return *this;
    }
    SqliteStatement& SqliteStatement::bind(const char* blob, int blobSize)
    {
        if(sqlite3_bind_blob(statement, bindLocation, blob, blobSize, SQLITE_STATIC) != SQLITE_OK)
            throw DatabaseException("Unable to bind to statement '"+StatementQuery+"'. Bind Location is: "+string::FromNumber(bindLocation)+". Bind Value is: Blob.", __ExceptionParams__);
        bindLocation++;
        return *this;
    }
    DataTable SqliteStatement::execute()
    {
        bindLocation = 1;
        DataTable table{};
        bool firstPass = true;
        while(sqlite3_step(statement) == SQLITE_ROW)
        {
            int columnCount = sqlite3_column_count(statement);
            if(firstPass)
                if(columnNames.size() == 0)
                {
                    for(int loop = 0; loop < columnCount; loop++)
                    {
                        string columnName = sqlite3_column_name(statement, loop);
                        columnNames.add(columnName);
                        table.addColumn(columnName);
                    }
                }
                else
                {
                    for(string columnName : columnNames)
                        table.addColumn(columnName);
                }
            firstPass = false;
            Array<string> values{};
            for(int loop = 0; loop < columnCount; loop++)
                values.add((const char*)sqlite3_column_text(statement, loop));
        }
        
        sqlite3_reset(statement);
        return table;
    }
}

#undef __class__