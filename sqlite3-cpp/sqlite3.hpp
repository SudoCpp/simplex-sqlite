#include <string.hpp>
#include <../build/sqlite3.h>

namespace simplex
{
    class sqlite
    {
        struct sqlite3* database;
        public:
        sqlite(string filePath);
        ~sqlite();

        void query(string sqlQuery);
    };
}