#pragma once

#include "IKeyword.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

   //--------------------------------------------------------------
   /// \brief	The Barometric protocol support (reception only)
   //--------------------------------------------------------------
   class CBarometric : public IKeyword
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                      Constructor
      /// \param[in] context              Yadoms APi context
	  /// \param[in] PluginName           The name of the plugin
	  /// \param[in] KeyWordName          The keyword name
      /// \note                           Use this constructor initialising the keyword for this plugin
      //--------------------------------------------------------------
      CBarometric(boost::shared_ptr<yApi::IYPluginApi> context, std::string PluginName,std::string KeyWordName);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CBarometric();

      //--------------------------------------------------------------
      /// \brief	                      Set the value from the container
      /// \param[in] ValueContainer       The container where the value is stored
	  /// \param[in] filter               The name of the information into the container
      /// \note                           Use this constructor initialising the keyword for this plugin
      //--------------------------------------------------------------
	  void SetValue( const shared::CDataContainer & ValueContainer, const std::string & filter);

      // ICapacities implementation
	  virtual void DeclareKeywords (boost::shared_ptr<yApi::IYPluginApi> context ) const;
      virtual void historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const;
      // [END] ICapacities implementation
      
   protected:

   private:
      //--------------------------------------------------------------
      /// \brief	The device name
      //--------------------------------------------------------------
      std::string m_PluginName;

      //--------------------------------------------------------------
      /// \brief	The pressure (hPa)
      //--------------------------------------------------------------
      yApi::historization::CPressure m_pressure;
   };
