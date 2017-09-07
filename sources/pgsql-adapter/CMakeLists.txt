
if(USE_PGSQL_DATABASE)
   find_package(PostgreSQL)

   if(PostgreSQL_FOUND)
      message(STATUS "Found postgresql ${PostgreSQL_VERSION_STRING}")

      set(PGSQLADAPTER_SRC
         stdafx.cpp
         stdafx.h
         
         pgsql-adapter.cpp
         
			PgsqlQuery.h
			PgsqlQuery.cpp
			PgsqlOptions.h
			PgsqlOptions.cpp 
			PgsqlRequester.h
			PgsqlRequester.cpp
			PgsqlResultHandler.h
			PgsqlResultHandler.cpp
			PgsqlSqlState.h
			PgsqlSqlState.cpp
			PgsqlSystemTables.h
			PgsqlSystemTables.cpp
			PgsqlTableCreationScriptProvider.h
			PgsqlTableCreationScriptProvider.cpp
      )
      
      ########################
      ## Output
      ########################
      add_library(pgsql-adapter SHARED ${PGSQLADAPTER_SRC})
      
      set_property(TARGET pgsql-adapter PROPERTY INCLUDE_DIRECTORIES
         ${SHARED_INCL_DIR}
         ${DBCOMMON_INCLUDE_DIRS}
         ${PostgreSQL_INCLUDE_DIRS}
         ${BOOST_INCL_DIR}
         ${Poco_INCLUDE_DIRS}
         ${CMAKE_CURRENT_SOURCE_DIR})

      target_link_libraries(pgsql-adapter
         ${PostgreSQL_LIBRARIES}
         ${CMAKE_DL_LIBS}
         ${Boost_LIBRARIES}
         ${Poco_FOUND_LIBS}
         yadoms-shared
         dbCommon)

      if(COTIRE_USE)
         set_target_properties(pgsql-adapter PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "stdafx.h")
         
         if(COTIRE_USE_UNITY)
            set_target_properties(pgsql-adapter PROPERTIES COTIRE_ADD_UNITY_BUILD TRUE)
         else()
            set_target_properties(pgsql-adapter PROPERTIES COTIRE_ADD_UNITY_BUILD FALSE)
         endif()
         
         cotire(pgsql-adapter)
         
         if(COTIRE_USE_UNITY)
            target_link_libraries(pgsql-adapter_unity ${LIBS} ${CMAKE_DL_LIBS})
            set_property(TARGET pgsql-adapter_unity PROPERTY INCLUDE_DIRECTORIES ${BOOST_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
         endif()	
      else()
         add_precompiled_header(pgsql-adapter stdafx.h)
      endif() 
      
	
      if(WIN32)
         #add PostgreSQL as dependencies
         if(EXISTS "${PostgreSQL_ROOT}/bin/libpq.dll")
            SET(PostgreSqlDepends "${PostgreSQL_ROOT}/bin/libpq.dll")
         endif()
         
         if(EXISTS "${PostgreSQL_ROOT}/bin/ssleay32.dll")
            SET(PostgreSqlDepends ${PostgreSqlDepends} "${PostgreSQL_ROOT}/bin/ssleay32.dll")
         endif()
         
         if(EXISTS "${PostgreSQL_ROOT}/bin/libeay32.dll")
            SET(PostgreSqlDepends ${PostgreSqlDepends} "${PostgreSQL_ROOT}/bin/libeay32.dll")
         endif()

         if(EXISTS "${PostgreSQL_ROOT}/bin/intl.dll")
            SET(PostgreSqlDepends ${PostgreSqlDepends} "${PostgreSQL_ROOT}/bin/intl.dll")
         endif()

         if(EXISTS "${PostgreSQL_ROOT}/bin/libintl-8.dll")
            SET(PostgreSqlDepends ${PostgreSqlDepends} "${PostgreSQL_ROOT}/bin/libintl-8.dll")
         endif()

         if(EXISTS "${PostgreSQL_ROOT}/bin/libiconv-2.dll")
            SET(PostgreSqlDepends ${PostgreSqlDepends} "${PostgreSQL_ROOT}/bin/libiconv-2.dll")
         endif()
      endif(WIN32)
      
      
      ##################################################################################################
      ## Post build commands
      ##################################################################################################
      
      #copy PostgreSql depends
      FOREACH (PostgreSqlDepend ${PostgreSqlDepends})
         add_custom_command(TARGET pgsql-adapter POST_BUILD
                            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                                ${PostgreSqlDepend} $<TARGET_FILE_DIR:pgsql-adapter>/)

        if(COTIRE_USE)   
           if(COTIRE_USE_UNITY)
              add_custom_command(TARGET pgsql-adapter_unity POST_BUILD
                                 COMMAND ${CMAKE_COMMAND} -E copy_if_different
                                     ${PostgreSqlDepend} $<TARGET_FILE_DIR:pgsql-adapter_unity>/)
           endif(COTIRE_USE_UNITY)
        endif(COTIRE_USE)                          
                                
      ENDFOREACH(PostgreSqlDepend)
   
   else()
      message(WARNING "PostgreSQL not found. Only SQLite will be available")
      add_definitions( -DPGSQL_NOT_FOUND )
      
   endif(PostgreSQL_FOUND)   
   
endif(USE_PGSQL_DATABASE)






