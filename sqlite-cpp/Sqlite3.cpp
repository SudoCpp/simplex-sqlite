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
#include <Exception.hpp>
#include <../build/sqlite3.h>

#define __class__ "simplex::Sqlite3"

namespace simplex
{
    Sqlite3::Sqlite3(string filePath) : Database{DatabaseConnection{"","",filePath}}
    {
        openConnection(filePath);
    }

    Sqlite3::Sqlite3() : Database{DatabaseConnection{"","",":memory:"}}
    {
        openConnection(":memory:");
    }

    Sqlite3::Sqlite3(const DatabaseConnection& connectionSettings) : Database{connectionSettings}
    {
        openConnection(connectionSettings.DatabaseName);
    }

    void Sqlite3::openConnection(const string& filePath)
    {
        int rc = sqlite3_open(filePath.toCString(), &database);
        if(rc)
            throw DatabaseException("Database could not be opened at '"+filePath+"'.", __ExceptionParams__);
    }

    Sqlite3::~Sqlite3()
    {
        sqlite3_close(database);
    }

    DataTable Sqlite3::query(const string& sqlQuery)
    {
        char* errorMessage = 0;
        int rc = sqlite3_exec(database, sqlQuery.toCString(), nullptr, 0, &errorMessage);
   
        if(rc != SQLITE_OK )
        {
            string errMsg{errorMessage};
            sqlite3_free(errorMessage);
            throw DatabaseException("Sqlite query failed: '" + errMsg + "'.", __ExceptionParams__);
        }
        return DataTable{};
    }

    std::shared_ptr<DatabaseStatement> Sqlite3::prepare(const string& sqlQuery)
    {
        return prepareInternal(sqlQuery); //smart pointers don't support covariants
    }

    std::shared_ptr<SqliteStatement> Sqlite3::prepareInternal(const string& sqlQuery)
    {
        return std::make_shared<SqliteStatement>(sqlQuery, database);
    }
}

#undef __class__