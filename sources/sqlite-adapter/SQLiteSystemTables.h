#pragma once

#include <dbCommon/DatabaseColumn.h>
#include <dbCommon/DatabaseTablesHelpers.hpp>


namespace libSQLiteAdapter
{
#define SQLITEMASTER_TABLE "table"

   DECLARE_TABLE(SqliteMaster,
      (Type)
      (Name)
      (TableName)
      (RootPage)
      (Sql)
   )

} //namespace libSQLiteAdapter 


