

set(DBCOMMON_SRC
   stdafx.cpp
   stdafx.h
   
	dbCommon/DatabaseColumn.h
	dbCommon/DatabaseColumn.cpp
	dbCommon/DatabaseTables.h
	dbCommon/DatabaseTables.cpp
   dbCommon/DatabaseTablesHelpers.hpp
   dbCommon/DatabaseException.hpp
   dbCommon/IDataBackup.h
   dbCommon/IDatabaseEngine.h
   dbCommon/IDatabaseRequester.h
   dbCommon/IResultHandler.h
   dbCommon/ITableCreationScriptProvider.h
   dbCommon/ITransactionalProvider.h
 	dbCommon/Query.h
	dbCommon/Query.cpp
	dbCommon/QuerySpecializations.h
   
	dbCommon/adapters/AdapterHelpers.hpp
	dbCommon/adapters/DatabaseAdapters.h
	dbCommon/adapters/DatabaseAdapters.cpp
	dbCommon/adapters/GenericAdapter.h
	dbCommon/adapters/GenericAdapter.cpp
	dbCommon/adapters/HighchartValueAdapter.hpp
	dbCommon/adapters/IResultAdapter.h
	dbCommon/adapters/MultipleValueAdapter.hpp
	dbCommon/adapters/SingleValueAdapter.hpp
	dbCommon/adapters/SqlExtension.hpp

   dbCommon/entities/Entities.h
   dbCommon/entities/Entities.cpp
   dbCommon/entities/EntityHelpers.hpp
)

source_group(dbCommon dbCommon/*.*)
source_group(dbCommon\\adapters dbCommon/adapters/*.*)
source_group(dbCommon\\entities dbCommon/entities/*.*)

########################
## Output
########################
add_library(dbCommon STATIC ${DBCOMMON_SRC})

set_property(TARGET dbCommon PROPERTY INCLUDE_DIRECTORIES
   ${SHARED_INCL_DIR}
   ${BOOST_INCL_DIR}
   ${Poco_INCLUDE_DIRS}
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/dbCommon)

target_link_libraries(dbCommon
   ${CMAKE_DL_LIBS}
   ${Boost_LIBRARIES}
   ${Poco_FOUND_LIBS})
   
   
set(DBCOMMON_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "dbCommon include directories")

   

if(COTIRE_USE)
   set_target_properties(dbCommon PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "stdafx.h")
   
   if(COTIRE_USE_UNITY)
      set_target_properties(dbCommon PROPERTIES COTIRE_ADD_UNITY_BUILD TRUE)
   else()
      set_target_properties(dbCommon PROPERTIES COTIRE_ADD_UNITY_BUILD FALSE)
   endif()
   
   cotire(dbCommon)
   
   if(COTIRE_USE_UNITY)
      target_link_libraries(dbCommon_unity ${LIBS} ${CMAKE_DL_LIBS})
      set_property(TARGET dbCommon_unity PROPERTY INCLUDE_DIRECTORIES ${BOOST_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
   endif()	
else()
   add_precompiled_header(dbCommon stdafx.h)
endif() 






