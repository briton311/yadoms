#pragma once
//
// This file provide macros used to declare easily a SQLite table with its name and column names
//
// Example : 
//   Input :
//      DECLARE_TABLE(Plugin, 
//         (Id)("id")
//         (Name)("name")
//         (PluginName)("pluginName")
//         (Configuration)("configuration")
//         (AutoStart)("autoStart")
//         )
//
//    Ouptut :
//         class CPluginTable
//         {
//         public:
//            static const dbCommon::CDatabaseColumn & getTableName() { return m_TableName; }
//            static const dbCommon::CDatabaseColumn & getIdColumn() { return m_ColumnId; }
//            static const dbCommon::CDatabaseColumn & getNameColumn() { return m_ColumnName; }
//            static const dbCommon::CDatabaseColumn & getPluginNameColumn() { return m_ColumnPluginName; }
//            static const dbCommon::CDatabaseColumn & getConfigurationColumn() { return m_ColumnConfiguration; }
//            static const dbCommon::CDatabaseColumn & getEnabledColumn() { return m_ColumnEnabled; }
//         
//         private:
//            static dbCommon::CDatabaseColumn m_TableName;
//            static dbCommon::CDatabaseColumn m_ColumnId;
//            static dbCommon::CDatabaseColumn m_ColumnName;
//            static dbCommon::CDatabaseColumn m_ColumnPluginName;
//            static dbCommon::CDatabaseColumn m_ColumnConfiguration;
//            static dbCommon::CDatabaseColumn m_ColumnEnabled;
//         };
//         
//  Example:
//    Input :
//          DECLARE_TABLE_COLUMN_NAMES(Plugin, 
//              (Id)("id")
//              (Name)("name")
//              (PluginName)("pluginName")
//              (Configuration)("configuration")
//              (AutoStart)("autoStart")
//          )
//
//    Output :
//         std::string CPluginTable::m_TableName = "Plugin";
//         std::string CPluginTable::m_ColumnId = "id";
//         std::string CPluginTable::m_ColumnName = "name";
//         std::string CPluginTable::m_ColumnPluginName = "pluginName";
//         std::string CPluginTable::m_ColumnConfiguration = "configuration";
//         std::string CPluginTable::m_ColumnEnabled = "enabled";


//
/// \brief The column identifier in input sequence (column id is the first of the two values)
//
#define COLUMN_IDENTIFIER 0
//
/// \brief The column name in input sequence (column name is the second of the two values)
//
#define COLUMN_NAME 1


//
/// \brief Macro which declares the static member
//
#define DECLARE_COLUMN_NAME(r, data, elem) static dbCommon::CDatabaseColumn BOOST_PP_CAT(m_Column, elem);

//
/// \brief Macro which declares a getter (used by foreach)
//
#define DECLARE_COLUMN_NAME_GETTER(r, data, elem) \
   static const dbCommon::CDatabaseColumn & BOOST_PP_CAT(BOOST_PP_CAT(get, elem), ColumnName()) { return BOOST_PP_CAT(m_Column, elem); }

//
/// \brief  Macro which declares all the getters
///         For each ID in input sequence, call DECLARE_COLUMN_NAME_GETTER
//
#define DECLARE_GETTERS(_seq) \
   BOOST_PP_SEQ_FOR_EACH(DECLARE_COLUMN_NAME_GETTER, _, _seq)

//
/// \brief  Macro which declares all the getters
///         For each ID in input sequence, call DECLARE_COLUMN_NAME
//
#define DECLARE_MEMBERS(_seq) \
   BOOST_PP_SEQ_FOR_EACH(DECLARE_COLUMN_NAME, _, _seq)

//
/// \brief  Macro used to treat the comlumn name part
//
#define DECLARE_STATIC_VALUE_NAME_PART(elem) elem;

//
/// \brief  Declare one static value (used by BOOST_PP_SEQ_FOR_EACH_I)
//
#define DECLARE_STATIC_VALUE(r, classname, i, elem) \
   dbCommon::CDatabaseColumn BOOST_PP_CAT(C##classname##Table::m_Column,BOOST_PP_SEQ_ELEM(COLUMN_IDENTIFIER, elem))(BOOST_PP_SEQ_ELEM(COLUMN_NAME, elem));

 //
//
/// \brief  Declare all static values
//
#define DELCARE_STATIC_VALUES(_classname, _seq) \
   BOOST_PP_SEQ_FOR_EACH_I(DECLARE_STATIC_VALUE, _classname, _seq)

//
/// \brief  Declare table name static value
//
#define DECLARE_STATIC_TABLENAME_VALUE(_classname, _tablename) \
   const dbCommon::CDatabaseTable C##_classname##Table::m_tableName(_tablename);

//
/// \brief  Declare a table
/// \param  name  the table name
/// \param  _seq  the sequence of columns
//
#define DECLARE_TABLE(name, _seq) \
class BOOST_PP_CAT(C, BOOST_PP_CAT(name, Table)) \
{ \
public:\
   virtual BOOST_PP_CAT(~C, BOOST_PP_CAT(name, Table))() {} \
   static const dbCommon::CDatabaseTable & getTableName() { return m_tableName; } \
   DECLARE_GETTERS(_seq) \
   \
private:\
   static const dbCommon::CDatabaseTable m_tableName; \
   DECLARE_MEMBERS(_seq) \
};

//
/// \brief  Initialize all static memebers of the table
/// \param  name  the table name
/// \param  _seq  the sequence of columns
//
#define DECLARE_TABLE_COLUMN_NAMES(_classname, _tablename, _seq) \
   DECLARE_STATIC_TABLENAME_VALUE(_classname, _tablename)\
   DELCARE_STATIC_VALUES(_classname, _seq)



