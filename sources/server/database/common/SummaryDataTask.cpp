#include "stdafx.h"

#include "SummaryDataTask.h"
#include <dbCommon/entities/Entities.h>
#include <shared/plugin/yPluginApi/KeywordDataType.h>
#include <shared/currentTime/Provider.h>
#include <shared/Log.h>
#include <shared/exception/EmptyResult.hpp>

#include "notification/summary/Notification.hpp"
#include "notification/Helpers.hpp"

namespace database
{
   namespace common
   {
      CSummaryDataTask::CSummaryDataTask(boost::shared_ptr<IAcquisitionRequester> acquisitionRequester, boost::shared_ptr<IKeywordRequester> keywordRequester)
         : m_acquisitionRequester(acquisitionRequester), m_keywordRequester(keywordRequester), m_firstRun(true)
      {
      }

      CSummaryDataTask::~CSummaryDataTask()
      {
      }

      void CSummaryDataTask::run()
      {
         try
         {
            YADOMS_LOG_CONFIGURE("Summary computing task");

            YADOMS_LOG(information) << "Start computing summary values";


            if (m_firstRun)
            {
               m_firstRun = false;
               executeFirstRunPass();
            }
            else
            {
               executeCommonPass();
            }
            YADOMS_LOG(information) << "End of computing summary values";
         }
         catch (std::exception& ex)
         {
            YADOMS_LOG(error) << "Error in computing summary values :" << ex.what();
         }
      }


      void CSummaryDataTask::executeFirstRunPass() const
      {
         /*
   
         Algo
         ===============
   
         Contexte
         ------------
         Cette méthode est lancée une seule fois au lancement de l'application
   
         On récupère la dernière donnée (brute) de chaque keyword
   
         Pour chaque keywords de type numeric
            si la date d'enregistrement de la dernière valeur est d'une heure différente de maintenant (comparaison date+heure) et que la donnée résumée HEURE n'existe pas
            alors on recalcule la donnée résumée HEURE
   
            si la date d'enregistrement de la dernière valeur est différente de maintenant (comparaison date) et que la donnée résumée JOUR n'existe pas
            alors on recalcule la donnée résumée JOUR
         */


         //check all keyword last date value
         //foreach
         //    if SummaryHour don't exist, then compute it
         //    if SummaryDay don't exist, then compute it
         auto keywords = m_keywordRequester->getAllKeywords();
         std::vector<boost::shared_ptr<dbCommon::entities::CKeyword>>::iterator i;
         for (i = keywords.begin(); i != keywords.end(); ++i)
         {
            if (i->get()->Type() == shared::plugin::yPluginApi::EKeywordDataType::kNumeric)
            {
               try
               {
                  auto lastData = m_acquisitionRequester->getKeywordLastData(i->get()->Id);

                  //if current day
                  //    if NOT same hour
                  //       compute HOUR
                  //else
                  //    compute hour
                  //    compute day


                  auto acqDate = boost::posix_time::to_tm(lastData->Date());
                  auto now = boost::posix_time::to_tm(boost::posix_time::second_clock::local_time());

                  auto computeHourValue = false, computeDayValue = false, computeMonthValue = false, computeYearValue = false;

                  if (acqDate.tm_year != now.tm_year)
                  {
                     //the year changed, , so compute everything
                     computeYearValue = true;
                     computeMonthValue = true;
                     computeDayValue = true;
                     computeHourValue = true;
                  }
                  else
                  {
                     //same year
                     if (acqDate.tm_mon != now.tm_mon)
                     {
                        //month changes, so compute (month, day and hour)
                        computeMonthValue = true;
                        computeDayValue = true;
                        computeHourValue = true;
                     }
                     else
                     {
                        //same month, same year, do not compute year and month summary values
                        if (acqDate.tm_yday != now.tm_yday)
                        {
                           //day changes, so compute (day and hour)
                           computeDayValue = true;
                           computeHourValue = true;
                        }
                        else
                        {
                           //same year, month and day
                           if (acqDate.tm_hour != now.tm_hour)
                           {
                              //the hour have changed, compute hour summary data
                              computeHourValue = true;
                           }
                        }
                     }
                  }

                  if (computeHourValue)
                  {
                     if (!m_acquisitionRequester->summaryDataExists(i->get()->Id(), dbCommon::entities::EAcquisitionSummaryType::kHour, lastData->Date()))
                     {
                        YADOMS_LOG(debug) << "    Computing HOUR : " << i->get()->FriendlyName() << " @ " << lastData->Date() << " ...";
                        m_acquisitionRequester->saveSummaryData(i->get()->Id(), dbCommon::entities::EAcquisitionSummaryType::kHour, lastData->Date());
                        YADOMS_LOG(debug) << "    Computing HOUR : " << i->get()->FriendlyName() << " @ " << lastData->Date() << " done";
                     }
                  }

                  if (computeDayValue)
                  {
                     if (!m_acquisitionRequester->summaryDataExists(i->get()->Id(), dbCommon::entities::EAcquisitionSummaryType::kDay, lastData->Date()))
                     {
                        YADOMS_LOG(debug) << "    Computing DAY : " << i->get()->FriendlyName() << " @ " << lastData->Date() << " ...";
                        m_acquisitionRequester->saveSummaryData(i->get()->Id(), dbCommon::entities::EAcquisitionSummaryType::kDay, lastData->Date());
                        YADOMS_LOG(debug) << "    Computing DAY : " << i->get()->FriendlyName() << " @ " << lastData->Date() << " done";
                     }
                  }

                  if (computeMonthValue)
                  {
                     if (!m_acquisitionRequester->summaryDataExists(i->get()->Id(), dbCommon::entities::EAcquisitionSummaryType::kMonth, lastData->Date()))
                     {
                        YADOMS_LOG(debug) << "    Computing MONTH : " << i->get()->FriendlyName() << " @ " << lastData->Date() << " ...";
                        m_acquisitionRequester->saveSummaryData(i->get()->Id(), dbCommon::entities::EAcquisitionSummaryType::kMonth, lastData->Date());
                        YADOMS_LOG(debug) << "    Computing MONTH : " << i->get()->FriendlyName() << " @ " << lastData->Date() << " done";
                     }
                  }


                  if (computeYearValue)
                  {
                     if (!m_acquisitionRequester->summaryDataExists(i->get()->Id(), dbCommon::entities::EAcquisitionSummaryType::kYear, lastData->Date()))
                     {
                        YADOMS_LOG(debug) << "    Computing YEAR : " << i->get()->FriendlyName() << " @ " << lastData->Date() << " ...";
                        m_acquisitionRequester->saveSummaryData(i->get()->Id(), dbCommon::entities::EAcquisitionSummaryType::kYear, lastData->Date());
                        YADOMS_LOG(debug) << "    Computing YEAR : " << i->get()->FriendlyName() << " @ " << lastData->Date() << " done";
                     }
                  }
               }
               catch (shared::exception::CEmptyResult& ex)
               {
                  YADOMS_LOG(debug) << "Cannot compute summary values : " << ex.what();
               }
               catch (std::exception& ex)
               {
                  YADOMS_LOG(error) << "Error in computing summary values :" << ex.what();
               }
            }
         }
      }

      void CSummaryDataTask::executeCommonPass() const
      {
         /*
   
         Algo
         ===============
   
         Contexte
         ------------
         Cette méthode est lancée chaque heure
   
         Pour tous les keywords ayant émis à l'heure précédente, on lance le calcul des valeurs résumées (si la valeur n'existe pas)
         Pour tous les keywords ayant émis le jour précédent, on lance le calcul des valeurs résumées (si la valeur n'existe pas)
   
         */
         auto now = shared::currentTime::Provider().now();
         boost::posix_time::time_duration oneHour = boost::posix_time::hours(1);

         boost::posix_time::ptime currentHour(now.date(), boost::posix_time::hours(now.time_of_day().hours()));
         auto previousHour = currentHour - oneHour;

         std::vector<int> keywordToTreat;
         std::vector<boost::shared_ptr<dbCommon::entities::CAcquisitionSummary>> summaryAcquisitions;

         m_acquisitionRequester->getKeywordsHavingDate(previousHour, currentHour, keywordToTreat);
         for (auto i = keywordToTreat.begin(); i != keywordToTreat.end(); ++i)
         {
            try
            {
               if (!m_acquisitionRequester->summaryDataExists(*i, dbCommon::entities::EAcquisitionSummaryType::kHour, previousHour))
               {
                  YADOMS_LOG(debug) << "    Computing HOUR : " << *i << " @ " << previousHour << " ...";
                  auto insertedValue = m_acquisitionRequester->saveSummaryData(*i, dbCommon::entities::EAcquisitionSummaryType::kHour, previousHour);
                  summaryAcquisitions.push_back(insertedValue);
                  YADOMS_LOG(debug) << "    Computing HOUR : " << *i << " @ " << previousHour << " done";
               }
            }
            catch (shared::exception::CEmptyResult& ex)
            {
               YADOMS_LOG(information) << "Cannot compute HOUR summary value : " << ex.what();
            }
            catch (std::exception& ex)
            {
               YADOMS_LOG(error) << "Error in computing HOUR summary values :" << ex.what();
            }
         }

         //check if day have changed
         if (currentHour.date() != previousHour.date())
         {
            boost::posix_time::time_duration oneDay = boost::posix_time::hours(24);
            boost::posix_time::ptime currentDay(now.date());
            auto previousDay = currentDay - oneDay;

            keywordToTreat.clear();
            m_acquisitionRequester->getKeywordsHavingDate(previousDay, currentDay, keywordToTreat);
            for (std::vector<int>::iterator i = keywordToTreat.begin(); i != keywordToTreat.end(); ++i)
            {
               try
               {
                  if (!m_acquisitionRequester->summaryDataExists(*i, dbCommon::entities::EAcquisitionSummaryType::kDay, previousDay))
                  {
                     YADOMS_LOG(debug) << "    Computing DAY : " << *i << " @ " << previousDay << " ...";
                     auto insertedValue = m_acquisitionRequester->saveSummaryData(*i, dbCommon::entities::EAcquisitionSummaryType::kDay, previousDay);
                     summaryAcquisitions.push_back(insertedValue);
                     YADOMS_LOG(debug) << "    Computing DAY : " << *i << " @ " << previousDay << " done";
                  }
               }
               catch (shared::exception::CEmptyResult& ex)
               {
                  YADOMS_LOG(information) << "Cannot compute DAY summary value : " << ex.what();
               }
               catch (std::exception& ex)
               {
                  YADOMS_LOG(error) << "Error in computing DAY summary values :" << ex.what();
               }
            }
         }

         //post notification
         if (!summaryAcquisitions.empty())
         {
            auto notificationData(boost::make_shared<notification::summary::CNotification>(summaryAcquisitions));
            notification::CHelpers::postNotification(notificationData);
         }
      }
   } //namespace common
} //namespace database 


