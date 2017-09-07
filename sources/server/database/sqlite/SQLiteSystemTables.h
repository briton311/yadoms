#pragma once

#include <dbCommon/DatabaseColumn.h>
#include <dbCommon/DatabaseTablesHelpers.hpp>


namespace database
{
   namespace sqlite
   {
#define SQLITEMASTER_TABLE "table"

      DECLARE_TABLE(SqliteMaster,
         (Type)
         (Name)
         (TableName)
         (RootPage)
         (Sql)
      )
   } //namespace sqlite
} //namespace database 


