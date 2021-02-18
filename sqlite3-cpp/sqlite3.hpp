#include <string.hpp>
#include <Dictionary.hpp>

struct sqlite3;
namespace simplex
{
    class sqlite
    {
        struct sqlite3* database;
        public:
        sqlite(string filePath);
        ~sqlite();

        void query(string sqlQuery);
        //Dictionary where keys are column names and value is value. Remember to enter strings wrapped in ''
        void insert(string tableName, const Dictionary<string, string>& values);
    };
}