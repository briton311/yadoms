#include "stdafx.h"
#include <shared/exception/InvalidParameter.hpp>
#include "task/ITask.h"
#include "Database.h"

namespace task
{
   namespace backup
   {
      std::string CDatabase::m_taskName = "database.backup";

      CDatabase::CDatabase(boost::shared_ptr<dbCommon::IDataBackup> dataBackupInterface)
         : m_dataBackupInterface(dataBackupInterface)
      {
         if (!m_dataBackupInterface)
            throw shared::exception::CInvalidParameter("dataBackupInterface");
      }

      CDatabase::~CDatabase()
      {
      }

      const std::string& CDatabase::getName()
      {
         return m_taskName;
      }

      void CDatabase::OnProgressionUpdatedInternal(int remaining, int total, const std::string& message) const
      {
         const auto progression = total != 0 ? (static_cast<float>(total - remaining) * 100.0f / static_cast<float>(total)) : 0.0f;

         if (m_reportRealProgress)
            m_reportRealProgress(true, progression, message, std::string(), shared::CDataContainer::EmptyContainer);
      }

      void CDatabase::doWork(TaskProgressFunc pFunctor)
      {
         m_reportRealProgress = pFunctor;
         m_dataBackupInterface->backupData(boost::bind(&CDatabase::OnProgressionUpdatedInternal, this, _1, _2, _3));
      }
   } //namespace backup
} //namespace task


