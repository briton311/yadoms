// All includes needed by plugins to compile with yPluginApi

////////////////////////////////////////
// Standard Template Library
////////////////////////////////////////
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

////////////////////////////////////////
// Boost libraries
//
// ! please keep for full list alphabetic ordered !!
////////////////////////////////////////
#include <boost/algorithm/string/predicate.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/property_tree/ptree.hpp>

#include <boost/preprocessor.hpp>
#include <boost/type_traits/is_enum.hpp>
#include <boost/utility/enable_if.hpp>


//dont't include boost/thread.hpp besause it increase build files more than 10MB for nothing and can be too heavy for small targets (raspberry)
#include <boost/thread/thread.hpp>
#include <boost/thread/exceptions.hpp>
#include <boost/thread/recursive_mutex.hpp>

