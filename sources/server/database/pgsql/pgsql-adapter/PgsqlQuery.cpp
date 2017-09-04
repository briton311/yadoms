#include "stdafx.h"
#include "PgsqlQuery.h"

namespace libPgsqlAdapter
{
   CPgsqlQuery::CPgsqlQuery()
      : CQuery(true)
   {
   }

   CPgsqlQuery::~CPgsqlQuery()
   {
   }

   CPgsqlQuery& CPgsqlQuery::SelectExists(CQuery& subQuery)
   {
      ChangeQueryType(kSelect);
      std::ostringstream ss;
      ss << "SELECT EXISTS( " << subQuery.str() << ") ";
      Append(ss);
      return *this;
   }

   CPgsqlQuery& CPgsqlQuery::CreateDatabase(const std::string& name)
   {
      ChangeQueryType(kCreate);
      std::ostringstream ss;
      ss << "CREATE DATABASE " << name;
      Append(ss);
      return *this;
   }

   std::string CPgsqlQuery::functionDateToIsoString(const std::string& sqlPart)
   {
      return (boost::format("to_char(%1%, 'YYYY\"-\"MM\"-\"DD\"T\"HH24\":\"MI\":\"SS')") % sqlPart).str();
   }
} //namespace libPgsqlAdapter 


