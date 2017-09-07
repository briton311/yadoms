#pragma once
#include "IConfigurationManager.h"
#include "database/IConfigurationRequester.h"

namespace dataAccessLayer
{
   class CConfigurationManager : public IConfigurationManager
   {
   public:
      //--------------------------------------------------------------
      /// \brief                                Constructor
      /// \param [in] configurationRequester    The configuration requester
      /// \param [in] notifier                  The notifier
      //--------------------------------------------------------------
      explicit CConfigurationManager(boost::shared_ptr<database::IConfigurationRequester> configurationRequester);

      //--------------------------------------------------------------
      /// \brief       Destructor
      //--------------------------------------------------------------
      virtual ~CConfigurationManager();

      // IConfigurationManager implementation
      void create(dbCommon::entities::CConfiguration& configurationToCreate) override;
      bool exists(const std::string& section, const std::string& name) override;
      boost::shared_ptr<dbCommon::entities::CConfiguration> getConfiguration(const std::string& section, const std::string& name) override;
      std::vector<boost::shared_ptr<dbCommon::entities::CConfiguration>> getConfigurations(const std::string& section) override;
      std::vector<boost::shared_ptr<dbCommon::entities::CConfiguration>> getConfigurations() override;
      void updateConfiguration(dbCommon::entities::CConfiguration& configurationToUpdate) override;
      void removeConfiguration(dbCommon::entities::CConfiguration& configurationToRemove) override;
      // [END] - IConfigurationManager implementation

   protected:
      //--------------------------------------------------------------
      /// \brief  Post an update configuration notification for the [section,name] data
      //--------------------------------------------------------------
      void postUpdateNotification(const std::string& section, const std::string& name) const;

   private:
      //--------------------------------------------------------------
      /// \brief           The real data requester
      //--------------------------------------------------------------
      boost::shared_ptr<database::IConfigurationRequester> m_configurationRequester;
   };
} //namespace dataAccessLayer 


