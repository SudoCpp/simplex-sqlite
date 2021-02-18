cmake_minimum_required(VERSION 3.0.0)
project(sqlite3 VERSION 1.0.0)

include(CTest)
enable_testing()

include_directories(sqlite/src)
include_directories(build)
include_directories(sqlite/ext/fts3)
include_directories(sqlite/ext/icu)
include_directories(sqlite/ext/rtree)
include_directories(sqlite/ext/session)
include_directories(sqlite/ext/userauth)
include_directories(sqlite/ext/rbu)

set(TOP "../sqlite")
set(USLEEP "-DHAVE_USLEEP=1")
set(THREADSAFE "-DTHREADSAFE=0")
set(THREADLIB "-lpthread -lm -ldl")
set(EXE "")

set(SourceFiles
    sqlite/src/alter.c
    sqlite/src/analyze.c
    sqlite/src/attach.c
    sqlite/src/auth.c
    sqlite/src/backup.c
    sqlite/src/bitvec.c
    sqlite/src/btmutex.c
    sqlite/src/btree.c
    sqlite/src/build.c
    sqlite/src/callback.c
    sqlite/src/complete.c
    sqlite/src/ctime.c
    sqlite/src/date.c
    sqlite/src/dbpage.c
    sqlite/src/dbstat.c
    sqlite/src/delete.c
    sqlite/src/expr.c
    sqlite/src/fault.c
    sqlite/src/fkey.c
    sqlite/src/func.c
    sqlite/src/global.c
    sqlite/src/hash.c
    sqlite/src/insert.c
    sqlite/src/legacy.c
    sqlite/src/loadext.c
    sqlite/src/main.c
    sqlite/src/malloc.c
    sqlite/src/mem0.c
    sqlite/src/mem1.c
    sqlite/src/mem2.c
    sqlite/src/mem3.c
    sqlite/src/mem5.c
    sqlite/src/memdb.c
    sqlite/src/memjournal.c
    sqlite/src/mutex.c
    sqlite/src/mutex_noop.c
    sqlite/src/mutex_unix.c
    sqlite/src/mutex_w32.c
    sqlite/src/notify.c
    sqlite/src/os.c
    sqlite/src/os_unix.c
    sqlite/src/os_win.c
    sqlite/src/pager.c
    sqlite/src/pcache.c
    sqlite/src/pcache1.c
    sqlite/src/pragma.c
    sqlite/src/prepare.c
    sqlite/src/printf.c
    sqlite/src/random.c
    sqlite/src/resolve.c
    sqlite/src/rowset.c
    sqlite/src/select.c
    sqlite/src/status.c
    sqlite/src/table.c
    sqlite/src/tclsqlite.c
    sqlite/src/threads.c
    sqlite/src/tokenize.c
    sqlite/src/treeview.c
    sqlite/src/trigger.c
    sqlite/src/utf.c
    sqlite/src/update.c
    sqlite/src/upsert.c
    sqlite/src/util.c
    sqlite/src/vacuum.c
    sqlite/src/vdbe.c
    sqlite/src/vdbeapi.c
    sqlite/src/vdbeaux.c
    sqlite/src/vdbeblob.c
    sqlite/src/vdbemem.c
    sqlite/src/vdbesort.c
    sqlite/src/vdbetrace.c
    sqlite/src/vdbevtab.c
    sqlite/src/vtab.c
    sqlite/src/wal.c
    sqlite/src/walker.c
    sqlite/src/where.c
    sqlite/src/wherecode.c
    sqlite/src/whereexpr.c
    sqlite/src/window.c
    # Source code for extensions
    # fts3
    sqlite/ext/fts3/fts3.c
    sqlite/ext/fts3/fts3_aux.c
    sqlite/ext/fts3/fts3_expr.c
    sqlite/ext/fts3/fts3_hash.c
    sqlite/ext/fts3/fts3_icu.c
    sqlite/ext/fts3/fts3_porter.c
    sqlite/ext/fts3/fts3_snippet.c
    sqlite/ext/fts3/fts3_tokenizer.c
    sqlite/ext/fts3/fts3_tokenizer1.c
    sqlite/ext/fts3/fts3_tokenize_vtab.c
    sqlite/ext/fts3/fts3_unicode.c
    sqlite/ext/fts3/fts3_unicode2.c
    sqlite/ext/fts3/fts3_write.c

    sqlite/ext/icu/icu.c

    #Disabling rtree because it uses SQLITE_OK but doesn't include sqlite3.h
    #sqlite/ext/rtree/rtree.c
    #sqlite/ext/rtree/geopoly.c

    sqlite/ext/session/sqlite3session.c

    sqlite/ext/userauth/userauth.c

    sqlite/ext/rbu/sqlite3rbu.c

    sqlite/ext/misc/json1.c
    sqlite/ext/misc/stmt.c)

find_package(TCL REQUIRED)
include_directories("${TCL_INCLUDE_PATH}")

add_executable(mksourceid ../sqlite/tool/mksourceid.c)
add_custom_target(CreateSqliteH ALL COMMAND tclsh ../sqlite/tool/mksqlite3h.tcl ../sqlite > sqlite3.h)
add_executable(lemon ../sqlite/tool/lemon.c)
add_custom_target(CreateParseH ALL COMMAND cp ../sqlite/tool/lempar.c . && ./lemon -s ../sqlite/src/parse.y && mv ../sqlite/src/parse.h .)
add_custom_target(CreateOpcodesH ALL COMMAND cat parse.h ../sqlite/src/vdbe.c | tclsh ../sqlite/tool/mkopcodeh.tcl >opcodes.h)
add_executable(mkkeywordhash ../sqlite/tool/mkkeywordhash.c)
add_custom_target(CreateKeywordhashH ALL COMMAND ./mkkeywordhash >keywordhash.h)
add_library(sqlite3 ${SourceFiles})
target_link_libraries(sqlite3 ${TCL_LIBRARIES})

add_dependencies(CreateSqliteH mksourceid)
add_dependencies(CreateParseH lemon)
add_dependencies(CreateOpcodesH CreateParseH)
add_dependencies(CreateKeywordhashH mkkeywordhash)
add_dependencies(sqlite3 CreateSqliteH CreateOpcodesH CreateKeywordhashH)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
