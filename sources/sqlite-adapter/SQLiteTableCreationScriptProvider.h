#pragma once

#include <dbCommon/ITableCreationScriptProvider.h>


namespace libSQLiteAdapter
{
   class CSQLiteTableCreationScriptProvider : public dbCommon::ITableCreationScriptProvider
   {
   public:
      //--------------------------------------------------------------
      /// \brief       Constructor
      //--------------------------------------------------------------
      CSQLiteTableCreationScriptProvider();

      //--------------------------------------------------------------
      /// \brief       Destructor
      //--------------------------------------------------------------
      virtual ~CSQLiteTableCreationScriptProvider();

      // dbCommon::ITableCreationScriptProvider implementation
      std::string getTableConfiguration() override;
      std::string getTableDevice() override;
      std::string getTablePlugin() override;
      std::string getTableKeyword() override;
      std::string getTablePage() override;
      std::string getTableWidget() override;
      std::string getTablePluginEventLogger() override;
      std::string getTableEventLogger() override;
      std::string getTableAcquisition() override;
      std::string getTableAcquisitionSummary() override;
      std::string getTableRule() override;
      std::string getTableRecipient() override;
      std::string getTableRecipientField() override;
      void getTableAcquisitionIndexes(std::vector<std::string>& indexScripts) override;
      // [END] dbCommon::ITableCreationScriptProvider implementation
   };
} //namespace libSQLiteAdapter 


