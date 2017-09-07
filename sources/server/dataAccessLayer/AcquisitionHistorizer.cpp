#include "stdafx.h"
#include <shared/currentTime/Provider.h>
#include "AcquisitionHistorizer.h"
#include <dbCommon/ITransactionalProvider.h>

#include "notification/acquisition/Notification.hpp"
#include "notification/summary/Notification.hpp"
#include "notification/Helpers.hpp"

namespace dataAccessLayer
{
   CAcquisitionHistorizer::CAcquisitionHistorizer(boost::shared_ptr<database::IDataProvider> dataProvider)
      : m_dataProvider(dataProvider)
   {
   }

   void CAcquisitionHistorizer::saveData(int keywordId, const shared::plugin::yPluginApi::historization::IHistorizable& data)
   {
      //use ptime as variable, because saveData needs a reference
      auto currentDate = shared::currentTime::Provider().now();

      //get current transactional engine
      auto transactionalEngine = m_dataProvider->getTransactionalEngine();

      try
      {
         //if possible create transaction
         if (transactionalEngine)
            transactionalEngine->transactionBegin();

         //save data
         saveData(keywordId, data, currentDate);

         //if possible commit transaction
         if (transactionalEngine)
            transactionalEngine->transactionCommit();
      }
      catch (std::exception&)
      {
         //if possible rollback transaction
         if (transactionalEngine)
            transactionalEngine->transactionRollback();
         throw; // rethrow exception, catch is just here to handle transaction
      }
   }

   void CAcquisitionHistorizer::saveData(std::vector<int> keywordIdVect, const std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>>& dataVect)
   {
      //use ptime as variable, because saveData needs a reference
      auto currentDate = shared::currentTime::Provider().now();

      //get current transactional engine
      auto transactionalEngine = m_dataProvider->getTransactionalEngine();

      try
      {
         //if possible create transaction
         if (transactionalEngine)
            transactionalEngine->transactionBegin();

         //save all data
         for (unsigned int keywordIdCount = 0; keywordIdCount < keywordIdVect.size(); ++keywordIdCount)
         {
            saveData(keywordIdVect[keywordIdCount], *dataVect[keywordIdCount], currentDate);
         }

         //if possible commit transaction
         if (transactionalEngine)
            transactionalEngine->transactionCommit();
      }
      catch (std::exception&)
      {
         //if possible rollback transaction
         if (transactionalEngine)
            transactionalEngine->transactionRollback();
         throw; // rethrow exception, catch is just here to handle transaction
      }
   }

   void CAcquisitionHistorizer::saveData(int keywordId, const shared::plugin::yPluginApi::historization::IHistorizable& data, boost::posix_time::ptime& dataTime)
   {
      boost::shared_ptr<dbCommon::entities::CAcquisition> acq;

      //save data
      if (data.getMeasureType() == shared::plugin::yPluginApi::historization::EMeasureType::kIncrement)
         acq = m_dataProvider->getAcquisitionRequester()->incrementData(keywordId, data.formatValue(), dataTime);
      else
         acq = m_dataProvider->getAcquisitionRequester()->saveData(keywordId, data.formatValue(), dataTime);

      if (acq)
      {
         //only update summary data if already exists
         //if not exists it will be created by SQLiteSummaryDataTask
         std::vector<boost::shared_ptr<dbCommon::entities::CAcquisitionSummary>> acquisitionSummary;

         if (m_dataProvider->getAcquisitionRequester()->summaryDataExists(keywordId, dbCommon::entities::EAcquisitionSummaryType::kHour, dataTime))
            acquisitionSummary.push_back(m_dataProvider->getAcquisitionRequester()->saveSummaryData(keywordId, dbCommon::entities::EAcquisitionSummaryType::kHour, dataTime));
         if (m_dataProvider->getAcquisitionRequester()->summaryDataExists(keywordId, dbCommon::entities::EAcquisitionSummaryType::kDay, dataTime))
            acquisitionSummary.push_back(m_dataProvider->getAcquisitionRequester()->saveSummaryData(keywordId, dbCommon::entities::EAcquisitionSummaryType::kDay, dataTime));
         if (m_dataProvider->getAcquisitionRequester()->summaryDataExists(keywordId, dbCommon::entities::EAcquisitionSummaryType::kMonth, dataTime))
            acquisitionSummary.push_back(m_dataProvider->getAcquisitionRequester()->saveSummaryData(keywordId, dbCommon::entities::EAcquisitionSummaryType::kMonth, dataTime));
         if (m_dataProvider->getAcquisitionRequester()->summaryDataExists(keywordId, dbCommon::entities::EAcquisitionSummaryType::kYear, dataTime))
            acquisitionSummary.push_back(m_dataProvider->getAcquisitionRequester()->saveSummaryData(keywordId, dbCommon::entities::EAcquisitionSummaryType::kYear, dataTime));

         //post notification

         //post notification
         auto notificationData = boost::make_shared<notification::acquisition::CNotification>(acq);
         notification::CHelpers::postNotification(notificationData);

         if (!acquisitionSummary.empty())
         {
            auto notificationDataSummary = boost::make_shared<notification::summary::CNotification>(acquisitionSummary);
            notification::CHelpers::postNotification(notificationDataSummary);
         }
      }
   }
} //namespace dataAccessLayer 


