#include "stdafx.h"
#include "Acquisition.h"
#include <shared/exception/EmptyResult.hpp>
#include <dbCommon/DatabaseTables.h>
#include <dbCommon/Query.h>
#include <dbCommon/adapters/DatabaseAdapters.h>
#include <dbCommon/adapters/HighchartValueAdapter.hpp>
#include <dbCommon/adapters/SingleValueAdapter.hpp>
#include <dbCommon/adapters/MultipleValueAdapter.hpp>
#include <dbCommon/DatabaseException.hpp>
#include "Keyword.h"

//TODO d�placer dans dbCommon ?

namespace database
{
   namespace common
   {
      namespace requesters
      {
         CAcquisition::CAcquisition(boost::shared_ptr<dbCommon::IDatabaseRequester> databaseRequester, boost::shared_ptr<CKeyword> keywordRequester)
            : m_keywordRequester(keywordRequester), m_databaseRequester(databaseRequester)
         {
         }

         CAcquisition::~CAcquisition()
         {
         }

         // IAcquisitionRequester implementation
         boost::shared_ptr<dbCommon::entities::CAcquisition> CAcquisition::saveData(const int keywordId, const std::string& data, boost::posix_time::ptime& dataTime)
         {
            auto keywordEntity = m_keywordRequester->getKeyword(keywordId);
            if (keywordEntity)
            {
               if (!keywordEntity->Blacklist())
               {
                  auto q = m_databaseRequester->newQuery();
                  q.InsertInto(dbCommon::CAcquisitionTable::getTableName(), dbCommon::CAcquisitionTable::getDateColumnName(), dbCommon::CAcquisitionTable::getKeywordIdColumnName(), dbCommon::CAcquisitionTable::getValueColumnName()).
                   Values(dataTime, keywordId, data);

                  try
                  {
                     if (m_databaseRequester->queryStatement(q) <= 0)
                        throw shared::exception::CEmptyResult("Fail to insert new data");
                  }
                  catch (dbCommon::CDatabaseException& e)
                  {
                     if (e.returnCode() == dbCommon::CDatabaseException::kConstraintViolation)
                     {
                        // Maybe 2 acquisitions were recorded at same time for same keyword. In this case, we prefer to keep last value
                        q.Clear().Update(dbCommon::CAcquisitionTable::getTableName())
                         .Set(dbCommon::CAcquisitionTable::getValueColumnName(), data)
                         .Where(dbCommon::CAcquisitionTable::getDateColumnName(), CQUERY_OP_EQUAL, dataTime)
                         .And(dbCommon::CAcquisitionTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId);
                        m_databaseRequester->queryStatement(q);
                     }
                     else
                     {
                        throw;
                     }
                  }

                  return getAcquisitionByKeywordAndDate(keywordId, dataTime);
               }
               else
               {
                  //blacklisted keyword
                  return boost::shared_ptr<dbCommon::entities::CAcquisition>();
               }
            }
            throw shared::exception::CEmptyResult("The keyword do not exists, cannot add data");
         }

         boost::shared_ptr<dbCommon::entities::CAcquisition> CAcquisition::incrementData(const int keywordId, const std::string& increment, boost::posix_time::ptime& dataTime)
         {
            auto keywordEntity = m_keywordRequester->getKeyword(keywordId);

            if (!keywordEntity)
               throw shared::exception::CEmptyResult("The keyword do not exists, cannot increment data");

            if (keywordEntity->Type() != shared::plugin::yPluginApi::EKeywordDataType::kNumeric)
               throw shared::exception::CEmptyResult("The keyword is not numeric, cannot increment data");

            if (keywordEntity->Blacklist())
               return boost::shared_ptr<dbCommon::entities::CAcquisition>(); //return null instead of exception for performances

            auto qLastKeywordValue = m_databaseRequester->newQuery();
            qLastKeywordValue.Select(qLastKeywordValue.castNumeric(dbCommon::CAcquisitionTable::getValueColumnName())).
                             From(dbCommon::CAcquisitionTable::getTableName()).
                             Where(dbCommon::CAcquisitionTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId).
                             OrderBy(dbCommon::CAcquisitionTable::getDateColumnName(), dbCommon::CQuery::kDesc).
                             Limit(1);

            auto q = m_databaseRequester->newQuery();


            //insert first (if fails, update )
            q.InsertInto(dbCommon::CAcquisitionTable::getTableName(), dbCommon::CAcquisitionTable::getDateColumnName(), dbCommon::CAcquisitionTable::getKeywordIdColumnName(), dbCommon::CAcquisitionTable::getValueColumnName()).
             Values(dataTime, keywordId, q.math(q.coalesce(qLastKeywordValue, 0), CQUERY_OP_PLUS, increment));

            if (m_databaseRequester->queryStatement(q, false) <= 0)
            {
               //update
               q.Clear().Update(dbCommon::CAcquisitionTable::getTableName())
                .Set(dbCommon::CAcquisitionTable::getValueColumnName(), q.math(q.coalesce(qLastKeywordValue, 0), CQUERY_OP_PLUS, increment)).
                Where(dbCommon::CAcquisitionTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId).
                And(dbCommon::CAcquisitionTable::getDateColumnName(), CQUERY_OP_EQUAL, dataTime);

               if (m_databaseRequester->queryStatement(q) <= 0)
                  throw shared::exception::CEmptyResult("Fail to insert new incremental data");
            }

            return getAcquisitionByKeywordAndDate(keywordId, dataTime);
         }

         void CAcquisition::removeKeywordData(const int keywordId)
         {
            if (m_keywordRequester->getKeyword(keywordId))
            {
               auto q = m_databaseRequester->newQuery();
               q.DeleteFrom(dbCommon::CAcquisitionTable::getTableName()).
                Where(dbCommon::CAcquisitionTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId);

               m_databaseRequester->queryStatement(q);

               auto qSummary = m_databaseRequester->newQuery();
               qSummary.DeleteFrom(dbCommon::CAcquisitionSummaryTable::getTableName()).
                       Where(dbCommon::CAcquisitionSummaryTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId);
               m_databaseRequester->queryStatement(qSummary);
            }
            else
            {
               throw shared::exception::CEmptyResult("The keyword do not exists, cannot delete data");
            }
         }

         boost::shared_ptr<dbCommon::entities::CAcquisition> CAcquisition::getAcquisitionByKeywordAndDate(const int keywordId, boost::posix_time::ptime time)
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select().
                   From(dbCommon::CAcquisitionTable::getTableName()).
                   Where(dbCommon::CAcquisitionTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId).
                   And(dbCommon::CAcquisitionTable::getDateColumnName(), CQUERY_OP_EQUAL, time);

            dbCommon::adapters::CAcquisitionAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, qSelect);

            if (adapter.getResults().size() >= 1)
            {
               return adapter.getResults()[0];
            }

            throw shared::exception::CEmptyResult((boost::format("Cannot retrieve acquisition for KeywordId=%1% and date=%2%  in database") % keywordId % time).str());
         }

         boost::shared_ptr<dbCommon::entities::CAcquisition> CAcquisition::getKeywordLastData(const int keywordId, bool throwIfNotExists)
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select().
                   From(dbCommon::CAcquisitionTable::getTableName()).
                   Where(dbCommon::CAcquisitionTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId).
                   OrderBy(dbCommon::CAcquisitionTable::getDateColumnName(), dbCommon::CQuery::kDesc).
                   Limit(1);

            dbCommon::adapters::CAcquisitionAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, qSelect);

            if (adapter.getResults().size() >= 1)
            {
               return adapter.getResults()[0];
            }
            if (throwIfNotExists)
            {
               throw shared::exception::CEmptyResult((boost::format("Cannot retrieve any acquisition for the keyword id=%1% in database") % keywordId).str());
            }
            return boost::shared_ptr<dbCommon::entities::CAcquisition>();
         }

         std::vector<boost::tuple<boost::posix_time::ptime, std::string>> CAcquisition::getKeywordData(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo)
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select(dbCommon::CAcquisitionTable::getDateColumnName(), dbCommon::CAcquisitionTable::getValueColumnName()).
                   From(dbCommon::CAcquisitionTable::getTableName()).
                   Where(dbCommon::CAcquisitionTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId);

            if (!timeFrom.is_not_a_date_time())
            {
               qSelect.And(dbCommon::CAcquisitionTable::getDateColumnName(), CQUERY_OP_SUP_EQUAL, timeFrom);
               if (!timeTo.is_not_a_date_time())
               {
                  qSelect.And(dbCommon::CAcquisitionTable::getDateColumnName(), CQUERY_OP_INF_EQUAL, timeTo);
               }
            }

            qSelect.OrderBy(dbCommon::CAcquisitionTable::getDateColumnName());

            dbCommon::adapters::CMultipleValueAdapter<boost::posix_time::ptime, std::string> mva;
            m_databaseRequester->queryEntities(&mva, qSelect);

            return mva.getResults();
         }

         std::vector<boost::shared_ptr<dbCommon::entities::CAcquisitionSummary>> CAcquisition::getKeywordDataByHour(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo)
         {
            return getKeywordSummaryDataByType(dbCommon::entities::EAcquisitionSummaryType::kHour, keywordId, timeFrom, timeTo);
         }

         std::vector<boost::shared_ptr<dbCommon::entities::CAcquisitionSummary>> CAcquisition::getKeywordDataByDay(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo)
         {
            return getKeywordSummaryDataByType(dbCommon::entities::EAcquisitionSummaryType::kDay, keywordId, timeFrom, timeTo);
         }

         std::vector<boost::shared_ptr<dbCommon::entities::CAcquisitionSummary>> CAcquisition::getKeywordDataByMonth(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo)
         {
            return getKeywordSummaryDataByType(dbCommon::entities::EAcquisitionSummaryType::kMonth, keywordId, timeFrom, timeTo);
         }

         std::vector<boost::shared_ptr<dbCommon::entities::CAcquisitionSummary>> CAcquisition::getKeywordDataByYear(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo)
         {
            return getKeywordSummaryDataByType(dbCommon::entities::EAcquisitionSummaryType::kYear, keywordId, timeFrom, timeTo);
         }

         std::vector<boost::shared_ptr<dbCommon::entities::CAcquisitionSummary>> CAcquisition::getKeywordSummaryDataByType(const dbCommon::entities::EAcquisitionSummaryType& type, int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo) const
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select().
                   From(dbCommon::CAcquisitionSummaryTable::getTableName()).
                   Where(dbCommon::CAcquisitionSummaryTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId).
                   And(dbCommon::CAcquisitionSummaryTable::getTypeColumnName(), CQUERY_OP_EQUAL, type);

            if (!timeFrom.is_not_a_date_time())
            {
               qSelect.And(dbCommon::CAcquisitionSummaryTable::getDateColumnName(), CQUERY_OP_SUP_EQUAL, timeFrom);
               if (!timeTo.is_not_a_date_time())
               {
                  qSelect.And(dbCommon::CAcquisitionSummaryTable::getDateColumnName(), CQUERY_OP_INF_EQUAL, timeTo);
               }
            }

            qSelect.OrderBy(dbCommon::CAcquisitionSummaryTable::getDateColumnName());

            if (type == dbCommon::entities::EAcquisitionSummaryType::kDay)
               qSelect.Limit(2000); //more than 5 years
            else
               qSelect.Limit(200); // more than 8 days


            dbCommon::adapters::CAcquisitionSummaryAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, qSelect);

            return adapter.getResults();
         }

         std::string CAcquisition::getKeywordHighchartData(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo)
         {
            //this query is optimized to get only one field to read
            //the output data is a single column (without brackets):  ["dateiso",value]
            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select(qSelect.concatenate(qSelect.dateToIsoString(dbCommon::CAcquisitionTable::getDateColumnName()), ",", dbCommon::CAcquisitionTable::getValueColumnName())).
                   From(dbCommon::CAcquisitionTable::getTableName()).
                   Where(dbCommon::CAcquisitionTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId);

            if (!timeFrom.is_not_a_date_time())
            {
               qSelect.And(dbCommon::CAcquisitionTable::getDateColumnName(), CQUERY_OP_SUP_EQUAL, timeFrom);
               if (!timeTo.is_not_a_date_time())
               {
                  qSelect.And(dbCommon::CAcquisitionTable::getDateColumnName(), CQUERY_OP_INF_EQUAL, timeTo);
               }
            }

            qSelect.OrderBy(dbCommon::CAcquisitionTable::getDateColumnName());
            dbCommon::adapters::CHighchartValueAdapter mva;

            m_databaseRequester->queryEntities(&mva, qSelect);

            return mva.getRawResults();
         }

         std::string CAcquisition::getKeywordHighchartDataByType(const dbCommon::entities::EAcquisitionSummaryType& type, int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo) const
         {
            //this query is optimized to get only one field to read
            //the output data is a single column (without brackets):  ["dateiso",value]
            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select(qSelect.concatenate(qSelect.dateToIsoString(dbCommon::CAcquisitionSummaryTable::getDateColumnName()), ",", dbCommon::CAcquisitionSummaryTable::getAvgColumnName())).
                   From(dbCommon::CAcquisitionSummaryTable::getTableName()).
                   Where(dbCommon::CAcquisitionSummaryTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId).
                   And(dbCommon::CAcquisitionSummaryTable::getTypeColumnName(), CQUERY_OP_EQUAL, type);

            if (!timeFrom.is_not_a_date_time())
            {
               qSelect.And(dbCommon::CAcquisitionSummaryTable::getDateColumnName(), CQUERY_OP_SUP_EQUAL, timeFrom);
               if (!timeTo.is_not_a_date_time())
               {
                  qSelect.And(dbCommon::CAcquisitionSummaryTable::getDateColumnName(), CQUERY_OP_INF_EQUAL, timeTo);
               }
            }

            qSelect.OrderBy(dbCommon::CAcquisitionSummaryTable::getDateColumnName());
            dbCommon::adapters::CHighchartValueAdapter mva;

            m_databaseRequester->queryEntities(&mva, qSelect);

            return mva.getRawResults();
         }

         std::string CAcquisition::getKeywordHighchartDataByHour(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo)
         {
            return getKeywordHighchartDataByType(dbCommon::entities::EAcquisitionSummaryType::kHour, keywordId, timeFrom, timeTo);
         }

         std::string CAcquisition::getKeywordHighchartDataByDay(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo)
         {
            return getKeywordHighchartDataByType(dbCommon::entities::EAcquisitionSummaryType::kDay, keywordId, timeFrom, timeTo);
         }

         std::string CAcquisition::getKeywordHighchartDataByMonth(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo)
         {
            return getKeywordHighchartDataByType(dbCommon::entities::EAcquisitionSummaryType::kMonth, keywordId, timeFrom, timeTo);
         }

         std::string CAcquisition::getKeywordHighchartDataByYear(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo)
         {
            return getKeywordHighchartDataByType(dbCommon::entities::EAcquisitionSummaryType::kYear, keywordId, timeFrom, timeTo);
         }

         void CAcquisition::getKeywordsHavingDate(const boost::posix_time::ptime& timeFrom, const boost::posix_time::ptime& timeTo, std::vector<int>& results)
         {
            /*
      
            select distinct keywordId
            from Acquisition
            where date >= '20151105T100000'
            and date < '20151105T110000'
      
            */
            auto q = m_databaseRequester->newQuery();
            q.Select(q.distinct(dbCommon::CAcquisitionTable::getKeywordIdColumnName()))
             .From(dbCommon::CAcquisitionTable::getTableName())
             .Where(dbCommon::CAcquisitionTable::getDateColumnName(), CQUERY_OP_SUP_EQUAL, timeFrom)
             .And(dbCommon::CAcquisitionTable::getDateColumnName(), CQUERY_OP_INF, timeTo);

            dbCommon::adapters::CSingleValueAdapterWithContainer<int> intVectorAdapter(results);
            m_databaseRequester->queryEntities(&intVectorAdapter, q);
         }


         boost::shared_ptr<dbCommon::entities::CAcquisitionSummary> CAcquisition::saveSummaryData(const int keywordId, dbCommon::entities::EAcquisitionSummaryType curType, boost::posix_time::ptime& dataTime)
         {
            /* 
            INSERT OR REPLACE INTO AcquisitionSummary (type, date, keywordId, mean, min, max)
            SELECT "Hour", "startDate", 38, avg(cast(value as real)), min(cast(value as real)), max(cast(value as real))
            FROM Acquisition acq
            where acq.keywordId = 38
            and acq.date>= "startDate"
            and acq.date<= "endDate"
            
            */
            auto keyword = m_keywordRequester->getKeyword(keywordId);

            if (keyword)
            {
               if (keyword->Type() == shared::plugin::yPluginApi::EKeywordDataType::kNumeric)
               {
                  //just compute good dates
                  //hourDate : is the start of the hour (current day) => minutes and seconds set to 0
                  //hourDateEnd : is the end of the hour (current day) => minutes and seconds set to 59
                  //dayDate : is the start of the day : current day, with hour, minte and second set to 0
                  //dayDateEnd : is the end of the day : current day, with hour set to 23, minutes and seconds at 59


                  //to optimize queries, just do
                  //    update line
                  //       if >1 line is updated, nothing to do
                  //       else (0) no line updated, then insert new line
                  //
                  // so in good cases : only 1 query is realized
                  // so in bad cases : only 2 queries are realized


                  boost::posix_time::ptime fromDate, toDate;
                  auto pt_tm = boost::posix_time::to_tm(dataTime);

                  //Hour summary data are commputed from raw acquisitions
                  //Other ones, are taken from summarydata (really simplify calculous, and queries)

                  if (curType == dbCommon::entities::EAcquisitionSummaryType::kHour)
                  {
                     fromDate = boost::posix_time::ptime(dataTime.date(), boost::posix_time::hours(pt_tm.tm_hour));
                     toDate = boost::posix_time::ptime(dataTime.date(), boost::posix_time::hours(pt_tm.tm_hour) + boost::posix_time::minutes(59) + boost::posix_time::seconds(59));

                     if (m_databaseRequester->supportInsertOrUpdateStatement())
                     {
                        //process the request
                        auto q = m_databaseRequester->newQuery();
                        q.InsertOrReplaceInto(dbCommon::CAcquisitionSummaryTable::getTableName(), dbCommon::CAcquisitionSummaryTable::getTypeColumnName(), dbCommon::CAcquisitionSummaryTable::getDateColumnName(), dbCommon::CAcquisitionSummaryTable::getKeywordIdColumnName(), dbCommon::CAcquisitionSummaryTable::getAvgColumnName(), dbCommon::CAcquisitionSummaryTable::getMinColumnName(), dbCommon::CAcquisitionSummaryTable::getMaxColumnName()).
                         Select(curType, fromDate, keywordId, q.averageWithCast(q.fromSubquery("acq", dbCommon::CAcquisitionTable::getValueColumnName())),
                                q.minWithCast(q.fromSubquery("acq", dbCommon::CAcquisitionTable::getValueColumnName())),
                                q.maxWithCast(q.fromSubquery("acq", dbCommon::CAcquisitionTable::getValueColumnName()))).
                         From(q.as(dbCommon::CAcquisitionTable::getTableName(), "acq")).
                         Where(q.fromSubquery("acq", dbCommon::CAcquisitionTable::getKeywordIdColumnName()), CQUERY_OP_EQUAL, keywordId).
                         And(q.fromSubquery("acq", dbCommon::CAcquisitionTable::getDateColumnName()), CQUERY_OP_SUP_EQUAL, fromDate).
                         And(q.fromSubquery("acq", dbCommon::CAcquisitionTable::getDateColumnName()), CQUERY_OP_INF_EQUAL, toDate);
                        if (m_databaseRequester->queryStatement(q) <= 0)
                           throw shared::exception::CEmptyResult("Fail to insert summary value (insert or update) " + curType.toString() + " data");
                     }
                     else
                     {
                        //update
                        auto compute = m_databaseRequester->newQuery();

                        compute.Select(compute.as(compute.averageWithCast(dbCommon::CAcquisitionTable::getValueColumnName()), "avg"),
                                       compute.as(compute.minWithCast(dbCommon::CAcquisitionTable::getValueColumnName()), "min"),
                                       compute.as(compute.maxWithCast(dbCommon::CAcquisitionTable::getValueColumnName()), "max")).
                               From(dbCommon::CAcquisitionTable::getTableName()).
                               Where(dbCommon::CAcquisitionTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId).
                               And(dbCommon::CAcquisitionTable::getDateColumnName(), CQUERY_OP_SUP_EQUAL, fromDate).
                               And(dbCommon::CAcquisitionTable::getDateColumnName(), CQUERY_OP_INF_EQUAL, toDate);

                        auto q = m_databaseRequester->newQuery();


                        q.With("acq", compute).
                         Update(dbCommon::CAcquisitionSummaryTable::getTableName())
                         .Set(dbCommon::CAcquisitionSummaryTable::getAvgColumnName(), q.fromSubquery("acq", "avg"),
                              dbCommon::CAcquisitionSummaryTable::getMinColumnName(), q.fromSubquery("acq", "min"),
                              dbCommon::CAcquisitionSummaryTable::getMaxColumnName(), q.fromSubquery("acq", "max"))
                         .FromWith("acq")
                         .Where(dbCommon::CAcquisitionSummaryTable::getTypeColumnName(), CQUERY_OP_EQUAL, curType.toString())
                         .And(dbCommon::CAcquisitionSummaryTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId)
                         .And(dbCommon::CAcquisitionSummaryTable::getDateColumnName(), CQUERY_OP_EQUAL, fromDate);

                        if (m_databaseRequester->queryStatement(q) <= 0)
                        {
                           //fail to update, then insert
                           //insert
                           q.Clear();
                           q.InsertInto(dbCommon::CAcquisitionSummaryTable::getTableName(), dbCommon::CAcquisitionSummaryTable::getTypeColumnName(), dbCommon::CAcquisitionSummaryTable::getDateColumnName(), dbCommon::CAcquisitionSummaryTable::getKeywordIdColumnName(), dbCommon::CAcquisitionSummaryTable::getAvgColumnName(), dbCommon::CAcquisitionSummaryTable::getMinColumnName(), dbCommon::CAcquisitionSummaryTable::getMaxColumnName()).
                            Select(curType, fromDate, keywordId, q.averageWithCast(q.fromSubquery("acq", dbCommon::CAcquisitionTable::getValueColumnName())),
                                   q.minWithCast(q.fromSubquery("acq", dbCommon::CAcquisitionTable::getValueColumnName())),
                                   q.maxWithCast(q.fromSubquery("acq", dbCommon::CAcquisitionTable::getValueColumnName()))).
                            From(q.as(dbCommon::CAcquisitionTable::getTableName(), "acq")).
                            Where(q.fromSubquery("acq", dbCommon::CAcquisitionTable::getKeywordIdColumnName()), CQUERY_OP_EQUAL, keywordId).
                            And(q.fromSubquery("acq", dbCommon::CAcquisitionTable::getDateColumnName()), CQUERY_OP_SUP_EQUAL, fromDate).
                            And(q.fromSubquery("acq", dbCommon::CAcquisitionTable::getDateColumnName()), CQUERY_OP_INF_EQUAL, toDate);

                           if (m_databaseRequester->queryStatement(q) <= 0)
                              throw shared::exception::CEmptyResult("Fail to insert summary value " + curType.toString() + " data");
                        }
                     }
                  }
                  else
                  {
                     dbCommon::entities::EAcquisitionSummaryType toQuery;

                     //all types but Hour
                     switch (curType)
                     {
                     case dbCommon::entities::EAcquisitionSummaryType::kDayValue:
                        fromDate = boost::posix_time::ptime(dataTime.date());
                        toDate = boost::posix_time::ptime(dataTime.date(),
                                                          boost::posix_time::hours(23) + boost::posix_time::minutes(59) + boost::posix_time::seconds(59));
                        toQuery = dbCommon::entities::EAcquisitionSummaryType::kHour;
                        break;

                     case dbCommon::entities::EAcquisitionSummaryType::kMonthValue:
                        fromDate = boost::posix_time::ptime(boost::gregorian::date(dataTime.date().year(),
                                                                                   dataTime.date().month(),
                                                                                   1));
                        toDate = boost::posix_time::ptime(boost::gregorian::date(dataTime.date().year(),
                                                                                 dataTime.date().month(),
                                                                                 boost::gregorian::gregorian_calendar::end_of_month_day(dataTime.date().year(),
                                                                                                                                        dataTime.date().month())),
                                                          boost::posix_time::hours(23) + boost::posix_time::minutes(59) + boost::posix_time::seconds(59));
                        toQuery = dbCommon::entities::EAcquisitionSummaryType::kDay;
                        break;

                     case dbCommon::entities::EAcquisitionSummaryType::kYearValue:
                        fromDate = boost::posix_time::ptime(boost::gregorian::date(dataTime.date().year(),
                                                                                   1,
                                                                                   1));
                        toDate = boost::posix_time::ptime(boost::gregorian::date(dataTime.date().year(),
                                                                                 12,
                                                                                 boost::gregorian::gregorian_calendar::end_of_month_day(dataTime.date().year(), 12)),
                                                          boost::posix_time::hours(23) + boost::posix_time::minutes(59) + boost::posix_time::seconds(59));
                        toQuery = dbCommon::entities::EAcquisitionSummaryType::kMonth;
                        break;
                     }


                     if (m_databaseRequester->supportInsertOrUpdateStatement())
                     {
                        auto q = m_databaseRequester->newQuery();
                        q.InsertOrReplaceInto(dbCommon::CAcquisitionSummaryTable::getTableName(), dbCommon::CAcquisitionSummaryTable::getTypeColumnName(), dbCommon::CAcquisitionSummaryTable::getDateColumnName(), dbCommon::CAcquisitionSummaryTable::getKeywordIdColumnName(), dbCommon::CAcquisitionSummaryTable::getAvgColumnName(), dbCommon::CAcquisitionSummaryTable::getMinColumnName(), dbCommon::CAcquisitionSummaryTable::getMaxColumnName()).
                         Select(curType, fromDate, keywordId, q.average(q.fromSubquery("acq", dbCommon::CAcquisitionSummaryTable::getAvgColumnName())),
                                q.min(q.fromSubquery("acq", dbCommon::CAcquisitionSummaryTable::getMinColumnName())),
                                q.max(q.fromSubquery("acq", dbCommon::CAcquisitionSummaryTable::getMaxColumnName()))).
                         From(q.as(dbCommon::CAcquisitionSummaryTable::getTableName(), "acq")).
                         Where(q.fromSubquery("acq", dbCommon::CAcquisitionSummaryTable::getKeywordIdColumnName()), CQUERY_OP_EQUAL, keywordId).
                         And(q.fromSubquery("acq", dbCommon::CAcquisitionSummaryTable::getTypeColumnName()), CQUERY_OP_EQUAL, toQuery.toString()).
                         And(q.fromSubquery("acq", dbCommon::CAcquisitionSummaryTable::getDateColumnName()), CQUERY_OP_SUP_EQUAL, fromDate).
                         And(q.fromSubquery("acq", dbCommon::CAcquisitionSummaryTable::getDateColumnName()), CQUERY_OP_INF_EQUAL, toDate);

                        if (m_databaseRequester->queryStatement(q) <= 0)
                           throw shared::exception::CEmptyResult("Fail to insert summary value (insert or update) " + curType.toString() + " data");
                     }
                     else
                     {
                        //update
                        auto compute = m_databaseRequester->newQuery();

                        compute.Select(compute.as(compute.average(dbCommon::CAcquisitionSummaryTable::getAvgColumnName()), "avg"),
                                       compute.as(compute.min(dbCommon::CAcquisitionSummaryTable::getMinColumnName()), "min"),
                                       compute.as(compute.max(dbCommon::CAcquisitionSummaryTable::getMaxColumnName()), "max")).
                               From(dbCommon::CAcquisitionSummaryTable::getTableName()).
                               Where(dbCommon::CAcquisitionSummaryTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId).
                               And(dbCommon::CAcquisitionSummaryTable::getTypeColumnName(), CQUERY_OP_EQUAL, toQuery.toString()).
                               And(dbCommon::CAcquisitionSummaryTable::getDateColumnName(), CQUERY_OP_SUP_EQUAL, fromDate).
                               And(dbCommon::CAcquisitionSummaryTable::getDateColumnName(), CQUERY_OP_INF_EQUAL, toDate);

                        auto q = m_databaseRequester->newQuery();

                        q.With("acq", compute).
                         Update(dbCommon::CAcquisitionSummaryTable::getTableName())
                         .Set(dbCommon::CAcquisitionSummaryTable::getAvgColumnName(), q.fromSubquery("acq", "avg"),
                              dbCommon::CAcquisitionSummaryTable::getMinColumnName(), q.fromSubquery("acq", "min"),
                              dbCommon::CAcquisitionSummaryTable::getMaxColumnName(), q.fromSubquery("acq", "max"))
                         .FromWith("acq")
                         .Where(dbCommon::CAcquisitionSummaryTable::getTypeColumnName(), CQUERY_OP_EQUAL, curType.toString())
                         .And(dbCommon::CAcquisitionSummaryTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId)
                         .And(dbCommon::CAcquisitionSummaryTable::getDateColumnName(), CQUERY_OP_EQUAL, fromDate);

                        if (m_databaseRequester->queryStatement(q) <= 0)
                        {
                           //fail to update, then insert
                           //insert
                           q.Clear();
                           q.InsertInto(dbCommon::CAcquisitionSummaryTable::getTableName(), dbCommon::CAcquisitionSummaryTable::getTypeColumnName(), dbCommon::CAcquisitionSummaryTable::getDateColumnName(), dbCommon::CAcquisitionSummaryTable::getKeywordIdColumnName(), dbCommon::CAcquisitionSummaryTable::getAvgColumnName(), dbCommon::CAcquisitionSummaryTable::getMinColumnName(), dbCommon::CAcquisitionSummaryTable::getMaxColumnName()).
                            Select(curType, fromDate, keywordId, q.average(q.fromSubquery("acq", dbCommon::CAcquisitionSummaryTable::getAvgColumnName())),
                                   q.min(q.fromSubquery("acq", dbCommon::CAcquisitionSummaryTable::getMinColumnName())),
                                   q.max(q.fromSubquery("acq", dbCommon::CAcquisitionSummaryTable::getMaxColumnName()))).
                            From(q.as(dbCommon::CAcquisitionSummaryTable::getTableName(), "acq")).
                            Where(q.fromSubquery("acq", dbCommon::CAcquisitionSummaryTable::getKeywordIdColumnName()), CQUERY_OP_EQUAL, keywordId).
                            And(q.fromSubquery("acq", dbCommon::CAcquisitionSummaryTable::getTypeColumnName()), CQUERY_OP_EQUAL, toQuery.toString()).
                            And(q.fromSubquery("acq", dbCommon::CAcquisitionSummaryTable::getDateColumnName()), CQUERY_OP_SUP_EQUAL, fromDate).
                            And(q.fromSubquery("acq", dbCommon::CAcquisitionSummaryTable::getDateColumnName()), CQUERY_OP_INF_EQUAL, toDate);

                           if (m_databaseRequester->queryStatement(q) <= 0)
                              throw shared::exception::CEmptyResult("Fail to insert summary value " + curType.toString() + " data");
                        }
                     }
                  }

                  //get the result
                  std::vector<boost::shared_ptr<dbCommon::entities::CAcquisitionSummary>> all;
                  switch (curType)
                  {
                  case dbCommon::entities::EAcquisitionSummaryType::kHourValue:
                     all = getKeywordDataByHour(keywordId, fromDate, toDate);
                     break;

                  case dbCommon::entities::EAcquisitionSummaryType::kDayValue:
                     all = getKeywordDataByDay(keywordId, fromDate, toDate);
                     break;

                  case dbCommon::entities::EAcquisitionSummaryType::kMonthValue:
                     all = getKeywordDataByMonth(keywordId, fromDate, toDate);
                     break;

                  case dbCommon::entities::EAcquisitionSummaryType::kYearValue:
                     all = getKeywordDataByYear(keywordId, fromDate, toDate);
                     break;
                  }
                  if (!all.empty())
                     return all[0];
               }
               //keyword is not numeric, no data to avg, min and max !
               return boost::shared_ptr<dbCommon::entities::CAcquisitionSummary>();
            }
            throw shared::exception::CEmptyResult("The keyword do not exists, cannot add summary data");
         }

         bool CAcquisition::summaryDataExists(const int keywordId, dbCommon::entities::EAcquisitionSummaryType curType, boost::posix_time::ptime& dataTime)
         {
            //determine the real date of summary data 
            boost::posix_time::ptime fromDate;
            auto pt_tm = boost::posix_time::to_tm(dataTime);
            if (curType == dbCommon::entities::EAcquisitionSummaryType::kHour)
            {
               fromDate = boost::posix_time::ptime(dataTime.date(), boost::posix_time::hours(pt_tm.tm_hour));
            }
            else if (curType == dbCommon::entities::EAcquisitionSummaryType::kDay)
            {
               fromDate = boost::posix_time::ptime(dataTime.date());
            }

            auto checkq = m_databaseRequester->newQuery();
            checkq.SelectCount().
                  From(dbCommon::CAcquisitionSummaryTable::getTableName()).
                  Where(dbCommon::CAcquisitionSummaryTable::getTypeColumnName(), CQUERY_OP_EQUAL, curType).
                  And(dbCommon::CAcquisitionSummaryTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId).
                  And(dbCommon::CAcquisitionSummaryTable::getDateColumnName(), CQUERY_OP_EQUAL, fromDate);

            return (m_databaseRequester->queryCount(checkq) > 0);
         }

         int CAcquisition::purgeAcquisitions(boost::posix_time::ptime purgeDate)
         {
            auto q = m_databaseRequester->newQuery();
            q.DeleteFrom(dbCommon::CAcquisitionTable::getTableName()).
             Where(dbCommon::CAcquisitionTable::getDateColumnName(), CQUERY_OP_INF, purgeDate);

            auto count = m_databaseRequester->queryStatement(q);
            if (count < 0)
               throw shared::exception::CException("Fail to purge database");
            return count;
         }

         // [END] IAcquisitionRequester implementation
      } //namespace requesters
   } //namespace common
} //namespace database 


