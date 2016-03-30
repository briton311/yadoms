#pragma once

#include "OpenZWaveNodeKeywordBase.h"
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

//--------------------------------------------------------------
/// \brief	    ZWave keyword based on generic historizer
//--------------------------------------------------------------
template<class T>
class COpenZWaveNodeKeywordGeneric : public COpenZWaveNodeKeywordBase
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   COpenZWaveNodeKeywordGeneric(boost::shared_ptr< shared::plugin::yPluginApi::historization::CSingleHistorizableData<T> > keyword, OpenZWave::ValueID & valueId)
      : COpenZWaveNodeKeywordBase(valueId), m_keyword(keyword)
   {

   }

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~COpenZWaveNodeKeywordGeneric()
   {

   }

   // IOpenZWaveKeyword implementation
   virtual bool sendCommand(const std::string & commandData)
   {
      m_keyword->setCommand(commandData);
      return realSendCommand<T>(m_keyword->get());
   }

   virtual boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> getLastKeywordValue()
   {
      m_keyword->set(extractLastValue<T>());
      return m_keyword;
   }
   // [END] IOpenZWaveKeyword implementation

   //--------------------------------------------------------------
   /// \brief	    Create a generic keyword using a generic historizer
   /// \param [in] historizer The generic historizer to use
   /// \param [in] vID        The keyword OpenZWave::ValueId source
   /// \return    A IOpenZWaveNodeKeyword shared pointer
   //--------------------------------------------------------------
   static boost::shared_ptr<IOpenZWaveNodeKeyword> create(boost::shared_ptr< shared::plugin::yPluginApi::historization::CSingleHistorizableData<T> > historizer, OpenZWave::ValueID & vID)
   {
      return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordGeneric<T>(historizer, vID));
   }

   //--------------------------------------------------------------
   /// \brief	    Create a generic keyword using its daatype
   /// \param [in] vID           The keyword OpenZWave::ValueId source
   /// \param [in] vLabel        The keyword label
   /// \param [in] accessMode    The keyword access mode
   /// \param [in] units         The keyword units
   /// \param [in] dataType      The keyword data type
   /// \param [in] measureType   The keyword measure type (absolute, cumulative,..)
   /// \return    A IOpenZWaveNodeKeyword shared pointer
   //--------------------------------------------------------------
   static boost::shared_ptr<IOpenZWaveNodeKeyword> createFromDataType(OpenZWave::ValueID & vID, const std::string & vLabel, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, const std::string &units, shared::plugin::yPluginApi::EKeywordDataType dataType, shared::plugin::yPluginApi::historization::EMeasureType measureType = shared::plugin::yPluginApi::historization::EMeasureType::kAbsolute)
   {
      boost::shared_ptr< shared::plugin::yPluginApi::historization::CSingleHistorizableData<T> > historizer(new shared::plugin::yPluginApi::historization::CSingleHistorizableData<T>(COpenZWaveHelpers::GenerateKeywordName(vID), COpenZWaveNodeKeywordFactory::getCapacity(vLabel, units, dataType), accessMode, measureType));
      return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordGeneric<T>(historizer, vID));
   }


private:
   //--------------------------------------------------------------
   /// \brief	    Switch value handler
   //--------------------------------------------------------------
   boost::shared_ptr< shared::plugin::yPluginApi::historization::CSingleHistorizableData<T> > m_keyword;
};