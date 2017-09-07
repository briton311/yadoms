#pragma once
#include <dbCommon/DatabaseColumn.h>
#include <dbCommon/DatabaseTablesHelpers.hpp>


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


