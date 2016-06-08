#pragma once
#include <shared/exception/Exception.hpp>

namespace shared { namespace communication {

   //--------------------------------------------------------------
   /// \brief	Exception handling for communication port
   //--------------------------------------------------------------
   class CPortException : public exception::CException
   {
   public:
      enum ECode
      {
         kConnectionClosed,
         kConnectionError
      };

   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in]  code                Connection error code
      /// \param[in]  message             Exception message
      //--------------------------------------------------------------
      CPortException(ECode code, const std::string& message)
         :CException(codeLabelsToString(code) + ", " + message.c_str())
      {
      }

      //--------------------------------------------------------------
      /// \brief      Destructor
      //--------------------------------------------------------------
      virtual ~CPortException() throw()
      {
      }

   private:
      //--------------------------------------------------------------
      /// \brief                 Get the code labels
      /// \param[in] code        The error code
      /// \return                The error label
      //--------------------------------------------------------------
      static const std::string codeLabelsToString(ECode code)
      {
         switch (code)
         {
         case kConnectionClosed:
            return "connection was closed";
         case kConnectionError:
            return "connection error";
         default:
            return boost::lexical_cast<std::string>(code);
         }
      }
   };

} } // namespace shared::communication
