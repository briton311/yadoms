#pragma once

#include <shared/enumeration/EnumHelpers.hpp>

namespace libPgsqlAdapter
{
   DECLARE_ENUM_HEADER(ESqlErrorClass,
      ((SuccessfulCompletion))
      ((Warning))
      ((NoData))
      ((SQLStatementNotYetComplete))
      ((ConnectionException))
      ((TriggeredActionException))
      ((FeatureNotSupported))
      ((InvalidTransactionInitiation))
      ((LocatorException))
      ((InvalidGrantor))
      ((InvalidRoleSpecification))
      ((CaseNotFound))
      ((CardinalityViolation))
      ((DataException))
      ((IntegrityConstraintViolation))
      ((InvalidCursorState))
      ((InvalidTransactionState))
      ((InvalidSQLStatementName))
      ((TriggeredDataChangeViolation))
      ((InvalidAuthorizationSpecification))
      ((DependentPrivilegeDescriptorsStillExist))
      ((InvalidTransactionTermination))
      ((SQLRoutineException))
      ((InvalidCursorName))
      ((ExternalRoutineException))
      ((ExternalRoutineInvocationException))
      ((SavepointException))
      ((InvalidCatalogName))
      ((InvalidSchemaName))
      ((TransactionRollback))
      ((SyntaxErrororAccessRuleViolation))
      ((WITHCHECKOPTIONViolation))
      ((InsufficientResources))
      ((ProgramLimitExceeded))
      ((ObjectNotInPrerequisiteState))
      ((OperatorIntervention))
      ((SystemError))
      ((ConfigurationFileError))
      ((ForeignDataWrapperError))
      ((PLpgSQLError))
      ((InternalError))
   )

   class CPgsqlSqlState
   {
   private:
      explicit CPgsqlSqlState(const std::string& code, const std::string& description);

   public:
      static const CPgsqlSqlState& Parse(char* state);

      virtual ~CPgsqlSqlState();

      const std::string& GetCode() const;
      const std::string& GetDescription() const;
      const ESqlErrorClass GetClass() const;
   private:
      std::string m_code;
      std::string m_description;

   private:
      static std::map<std::string, CPgsqlSqlState> m_internalCodes;
   };
} //namespace libPgsqlAdapter


