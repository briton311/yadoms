#pragma once
#include "database/common/DatabaseColumn.h"
#include "database/common/DatabaseTablesHelpers.hpp"


namespace libPgsqlAdapter
{
   DECLARE_TABLE(PgsqlTables,
      (Database)
      (Schema)
      (Table)
      (TableType)
      (SelfColumn)
      (Generation)
      (DatabaseUserType)
      (SchemaUserType)
      (TableUserType)
      (IsInsertable)
      (IsTyped)
      (CommitAction)
   )

   DECLARE_TABLE(PgDatabase,
      (DatabaseName)
   )
} //namespace libPgsqlAdapter 


