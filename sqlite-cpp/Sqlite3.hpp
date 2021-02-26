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
#ifndef SIMPLEX_SQLITE_HPP
#define SIMPLEX_SQLITE_HPP

#include <Database.hpp>
#include "Array.hpp"

struct sqlite3;
struct sqlite3_stmt;
namespace simplex
{
    class SqliteStatement : public DatabaseStatement
    {
        struct sqlite3* database;
        struct sqlite3_stmt* statement;
        int bindLocation;
        Array<string> columnNames;
        public:
        SqliteStatement(const string& sqlQuery, struct sqlite3* database);
        virtual ~SqliteStatement();
        virtual SqliteStatement& bind(const string& text);
        virtual SqliteStatement& bind(int number);
        virtual SqliteStatement& bind(double number);
        virtual SqliteStatement& bind(const char* blob, int blobSize);
        virtual DataTable execute();
    };

    class Sqlite3 : public Database
    {
        struct sqlite3* database;
        public:
        Sqlite3(string filePath);
        Sqlite3(const DatabaseCredentials& connectionSettings);
        // In Memory
        Sqlite3();
        ~Sqlite3();

        virtual DataTable query(const string& sqlQuery);
        virtual SqliteStatement* prepare(const string& sqlQuery);
        
        private:
        void openConnection(const string& filePath);
    };
}

#endif //SIMPLEX_SQLITE_HPP