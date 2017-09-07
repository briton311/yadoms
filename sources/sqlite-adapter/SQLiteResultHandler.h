#pragma once

#include "sqlite3.h"
#include <dbCommon/IResultHandler.h>

namespace libSQLiteAdapter
{
   class CSQLiteResultHandler : public dbCommon::IResultHandler
   {
   public:
      explicit CSQLiteResultHandler(sqlite3_stmt* pStatement);
      virtual ~CSQLiteResultHandler();

      // dbCommon::IResultHandler implementation
      int getColumnCount() override;
      std::string getColumnName(const int columnIndex) override;
      bool next_step() override;
      std::string extractValueAsString(const int columnIndex) override;
      int extractValueAsInt(const int columnIndex) override;
      float extractValueAsFloat(const int columnIndex) override;
      double extractValueAsDouble(const int columnIndex) override;
      unsigned char* extractValueAsBlob(const int columnIndex) override;
      bool extractValueAsBool(const int columnIndex) override;
      bool isValueNull(const int columnIndex) override;
      boost::posix_time::ptime extractValueAsBoostTime(const int columnIndex) override;
      Poco::DateTime extractValueAsPocoTime(const int columnIndex) override;
      shared::CDataContainer extractValueAsDataContainer(const int columnIndex) override;
      // [END] - dbCommon::IResultHandler implementation

   private:
      //--------------------------------------------------------------
      /// \Brief		database statement
      //--------------------------------------------------------------
      sqlite3_stmt* m_pStatement;
   };
} //namespace libSQLiteAdapter 


