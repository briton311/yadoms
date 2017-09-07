#pragma once
#include <dbCommon/entities/Entities.h>

namespace dataAccessLayer
{
   class IConfigurationManager
   {
   public:
      //--------------------------------------------------------------
      /// \brief      Create a new configuration entry
      /// \param [in] configurationToCreate  New configuration informations
      /// \throw      shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void create(dbCommon::entities::CConfiguration& configurationToCreate) = 0;

      //--------------------------------------------------------------
      /// \brief      Checkl a a configuration entry already exists
      /// \param [in] section    the configuration SECTION
      /// \param [in] name       the configuration NAME
      /// \return     true if the configuration exists, false other cases
      //--------------------------------------------------------------
      virtual bool exists(const std::string& section, const std::string& name) = 0;

      //--------------------------------------------------------------
      /// \brief      Read a configuration entry
      /// \param [in] section    the configuration SECTION
      /// \param [in] name       the configuration NAME
      /// \return     The configuration entry found
      /// \throw      shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual boost::shared_ptr<dbCommon::entities::CConfiguration> getConfiguration(const std::string& section, const std::string& name) = 0;

      //--------------------------------------------------------------
      /// \brief      Read all configuration entries for one section
      /// \param [in] section    the configuration SECTION
      /// \return     The entries found
      /// \throw      shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<dbCommon::entities::CConfiguration>> getConfigurations(const std::string& section) = 0;

      //--------------------------------------------------------------
      /// \brief      Read all configuration entries 
      /// \return     The entries found
      /// \throw      shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<dbCommon::entities::CConfiguration>> getConfigurations() = 0;

      //--------------------------------------------------------------
      /// \brief      Update a configuration entry
      /// \param [in] configurationToUpdate  Configuration informations
      /// \throw      shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void updateConfiguration(dbCommon::entities::CConfiguration& configurationToUpdate) = 0;

      //--------------------------------------------------------------
      /// \brief      Remove a configuration entry
      /// \param [in] configurationToRemove  The configuration to delete
      /// \throw      shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void removeConfiguration(dbCommon::entities::CConfiguration& configurationToRemove) = 0;

      //--------------------------------------------------------------
      /// \brief       Destructor
      //--------------------------------------------------------------
      virtual ~IConfigurationManager()
      {
      }
   };
} //namespace dataAccessLayer 


