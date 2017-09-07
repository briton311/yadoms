#pragma once
#include "task/ITask.h"
#include <dbCommon/IDataBackup.h>

namespace task
{
   namespace backup
   {
      //------------------------------------------
      ///\brief   Database backup task
      //-----------------------------------------
      class CDatabase : public ITask
      {
      public:
         //------------------------------------------
         ///\brief   Constructor
         //------------------------------------------
         explicit CDatabase(boost::shared_ptr<dbCommon::IDataBackup> dataBackupInterface);

         //------------------------------------------
         ///\brief   Destructor
         //------------------------------------------
         virtual ~CDatabase();

         // ITask implementation
         const std::string& getName() override;
         void doWork(TaskProgressFunc pFunctor) override;
         // [END] ITask implementation

      private:
         //------------------------------------------
         ///\brief   Internal progress handler
         //------------------------------------------
         void OnProgressionUpdatedInternal(int remaining, int total, const std::string& message = std::string()) const;

         //------------------------------------------
         ///\brief   The task name
         //------------------------------------------
         static std::string m_taskName;

         //------------------------------------------
         ///\brief   The backup data interface
         //------------------------------------------
         boost::shared_ptr<dbCommon::IDataBackup> m_dataBackupInterface;

         //------------------------------------------
         ///\brief   The function pointer for reporting progression
         //------------------------------------------
         TaskProgressFunc m_reportRealProgress;
      };
   } //namespace backup
} //namespace task


