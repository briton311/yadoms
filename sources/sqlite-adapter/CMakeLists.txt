
if(USE_SQLITE_DATABASE)

   set(SQLITEADAPTER_SRC
      stdafx.cpp
      stdafx.h
      
      sqlite-adapter.cpp

		SQLiteQuery.h
		SQLiteQuery.cpp
		SQLiteOptions.h
		SQLiteOptions.cpp
		SQLiteRequester.h
		SQLiteRequester.cpp
		SQLiteResultHandler.h
		SQLiteResultHandler.cpp
		SQLiteSystemTables.h
		SQLiteSystemTables.cpp
		SQLiteTableCreationScriptProvider.h
		SQLiteTableCreationScriptProvider.cpp
   )
   
   ########################
   ## Output
   ########################
   add_library(sqlite-adapter SHARED ${SQLITEADAPTER_SRC})
   
   set_property(TARGET sqlite-adapter PROPERTY INCLUDE_DIRECTORIES
      ${SHARED_INCL_DIR}
      ${DBCOMMON_INCLUDE_DIRS}
      ${SQLITE_INCLUDE_DIRECTORY}
      ${BOOST_INCL_DIR}
      ${Poco_INCLUDE_DIRS}
      ${CMAKE_CURRENT_SOURCE_DIR})
      
      message("SQLITE_LIBRARIES=${SQLITE_LIBRARIES}")

   target_link_libraries(sqlite-adapter
      ${SQLITE_LIBRARIES}
      ${CMAKE_DL_LIBS}
      ${Boost_LIBRARIES}
      ${Poco_FOUND_LIBS}
      yadoms-shared
      dbCommon)

   if(COTIRE_USE)
      set_target_properties(sqlite-adapter PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "stdafx.h")
      
      if(COTIRE_USE_UNITY)
         set_target_properties(sqlite-adapter PROPERTIES COTIRE_ADD_UNITY_BUILD TRUE)
      else()
         set_target_properties(sqlite-adapter PROPERTIES COTIRE_ADD_UNITY_BUILD FALSE)
      endif()
      
      cotire(sqlite-adapter)
      
      if(COTIRE_USE_UNITY)
         target_link_libraries(sqlite-adapter_unity ${LIBS} ${CMAKE_DL_LIBS})
         set_property(TARGET sqlite-adapter_unity PROPERTY INCLUDE_DIRECTORIES ${BOOST_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
      endif()	
   else()
      add_precompiled_header(sqlite-adapter stdafx.h)
   endif() 
   
endif(USE_SQLITE_DATABASE)






