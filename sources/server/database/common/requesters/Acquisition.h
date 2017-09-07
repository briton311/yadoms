#pragma once

#include "database/IAcquisitionRequester.h"
#include <dbCommon/IDatabaseRequester.h>

namespace database
{
   namespace common
   {
      namespace requesters
      {
         class CKeyword;

         //--------------------------------------------------------------
         /// \Brief		   Acquisition requester for SQLite database
         //--------------------------------------------------------------
         class CAcquisition: public IAcquisitionRequester
         {
         public:
            //--------------------------------------------------------------
            /// \Brief		   Constructor
            /// \param [in]	databaseRequester the database handler
            //--------------------------------------------------------------
            CAcquisition(boost::shared_ptr<dbCommon::IDatabaseRequester> databaseRequester,
                         boost::shared_ptr<CKeyword> keywordRequester);

            //--------------------------------------------------------------
            /// \Brief		   Destructor
            //--------------------------------------------------------------
            virtual ~CAcquisition();

            // IAcquisitionRequester implementation
            boost::shared_ptr<dbCommon::entities::CAcquisition> saveData(const int keywordId,
                                                                         const std::string& data,
                                                                         boost::posix_time::ptime& dataTime) override;
            boost::shared_ptr<dbCommon::entities::CAcquisition> incrementData(const int keywordId,
                                                                              const std::string& increment,
                                                                              boost::posix_time::ptime& dataTime) override;
            boost::shared_ptr<dbCommon::entities::CAcquisitionSummary> saveSummaryData(const int keywordId,
                                                                                       dbCommon::entities::EAcquisitionSummaryType curType,
                                                                                       boost::posix_time::ptime& dataTime) override;
            void getKeywordsHavingDate(const boost::posix_time::ptime& timeFrom,
                                       const boost::posix_time::ptime& timeTo,
                                       std::vector<int>& results) override;
            bool summaryDataExists(const int keywordId,
                                   dbCommon::entities::EAcquisitionSummaryType curType,
                                   boost::posix_time::ptime& date) override;
            void removeKeywordData(const int keywordId) override;
            boost::shared_ptr<dbCommon::entities::CAcquisition> getAcquisitionByKeywordAndDate(const int keywordId,
                                                                                               boost::posix_time::ptime time) override;
            boost::shared_ptr<dbCommon::entities::CAcquisition> getKeywordLastData(const int keywordId,
                                                                                   bool throwIfNotExists = true) override;
            std::vector<boost::tuple<boost::posix_time::ptime, std::string>> getKeywordData(int keywordId,
                                                                                            boost::posix_time::ptime timeFrom,
                                                                                            boost::posix_time::ptime timeTo) override;
            std::vector<boost::shared_ptr<dbCommon::entities::CAcquisitionSummary>> getKeywordDataByDay(int keywordId,
                                                                                                        boost::posix_time::ptime timeFrom,
                                                                                                        boost::posix_time::ptime timeTo) override;
            std::vector<boost::shared_ptr<dbCommon::entities::CAcquisitionSummary>> getKeywordDataByHour(int keywordId,
                                                                                                         boost::posix_time::ptime timeFrom,
                                                                                                         boost::posix_time::ptime timeTo) override;
            std::vector<boost::shared_ptr<dbCommon::entities::CAcquisitionSummary>> getKeywordDataByMonth(int keywordId,
                                                                                                          boost::posix_time::ptime timeFrom,
                                                                                                          boost::posix_time::ptime timeTo) override;
            std::vector<boost::shared_ptr<dbCommon::entities::CAcquisitionSummary>> getKeywordDataByYear(int keywordId,
                                                                                                         boost::posix_time::ptime timeFrom,
                                                                                                         boost::posix_time::ptime timeTo) override;
            std::string getKeywordHighchartData(int keywordId,
                                                boost::posix_time::ptime timeFrom,
                                                boost::posix_time::ptime timeTo) override;
            std::string getKeywordHighchartDataByHour(int keywordId,
                                                      boost::posix_time::ptime timeFrom,
                                                      boost::posix_time::ptime timeTo) override;
            std::string getKeywordHighchartDataByDay(int keywordId,
                                                     boost::posix_time::ptime timeFrom,
                                                     boost::posix_time::ptime timeTo) override;
            std::string getKeywordHighchartDataByMonth(int keywordId,
                                                       boost::posix_time::ptime timeFrom,
                                                       boost::posix_time::ptime timeTo) override;
            std::string getKeywordHighchartDataByYear(int keywordId,
                                                      boost::posix_time::ptime timeFrom,
                                                      boost::posix_time::ptime timeTo) override;
            int purgeAcquisitions(boost::posix_time::ptime purgeDate) override;
            // [END] IAcquisitionRequester implementation

         private:
            //--------------------------------------------------------------
            /// \brief                    Get the data  by type (avg, min, max)
            /// \param [in] keywordId     keywordId Id
            /// \param [in] timeFrom      The start date (optionnal)
            /// \param [in] timeTo        The end date (optionnal)
            /// \return                   CAcquisitionSummary data
            /// \throw                    CInvalidParameter if deviceId is unknown
            //--------------------------------------------------------------
            std::vector<boost::shared_ptr<dbCommon::entities::CAcquisitionSummary>> getKeywordSummaryDataByType(const dbCommon::entities::EAcquisitionSummaryType& type, int keywordId,
                                                                                                                boost::posix_time::ptime timeFrom,
                                                                                                                boost::posix_time::ptime timeTo) const;

            //--------------------------------------------------------------
            /// \brief                    Get the summary data (highchart js format) : [[date,value],[date,value],...] by acquisition type
            /// \param [in] type          Type of data to select
            /// \param [in] keywordId     keywordId Id
            /// \param [in] timeFrom      The start date (optionnal)
            /// \param [in] timeTo        The end date (optionnal)
            /// \return                   Map of data : (date, value). One value per hour
            /// \throw                    CInvalidParameter if deviceId is unknown
            //--------------------------------------------------------------
            std::string getKeywordHighchartDataByType(const dbCommon::entities::EAcquisitionSummaryType& type,
                                                      int keywordId,
                                                      boost::posix_time::ptime timeFrom,
                                                      boost::posix_time::ptime timeTo) const;

            //--------------------------------------------------------------
            /// \Brief		   Pointer to keyword requester
            //--------------------------------------------------------------
            boost::shared_ptr<CKeyword> m_keywordRequester;

            //--------------------------------------------------------------
            /// \Brief		   Reference to IDatabaseRequester
            //--------------------------------------------------------------
            boost::shared_ptr<dbCommon::IDatabaseRequester> m_databaseRequester;
         };
      }
   }
} //namespace database::common::requesters


