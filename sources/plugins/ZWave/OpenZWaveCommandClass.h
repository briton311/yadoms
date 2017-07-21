#pragma once

#include <shared/enumeration/EnumHelpers.hpp>

   DECLARE_ENUM_HEADER(ECommandClass,
      ((NoOperation)(0x00))
      ((Basic)(0x20))
      ((ControllerReplication)(0x21))
      ((ApplicationStatus)(0x22))
      ((ZipServices)(0x23))
      ((ZipServer)(0x24))
      ((SwitchBinary)(0x25))
      ((SwitchMultilevel)(0x26))
      ((SwitchAll)(0x27))
      ((SwitchToggleBinary)(0x28))
      ((SwitchToggleMultilevel)(0x29))
      ((ChimneyFan)(0x2a))
      ((SceneActivation)(0x2b))
      ((SceneActuatorConf)(0x2c))
      ((SceneControllerConf)(0x2d))
      ((ZipClient)(0x2e))
      ((ZipAdvServices)(0x2f))
      ((SensorBinary)(0x30))
      ((SensorMultilevel)(0x31))
      ((Meter)(0x32))
      ((Color)(0x33))
      ((ZipAdvClient)(0x34))
      ((MeterPulse)(0x35))
      ((ThermostatHeating)(0x38))
      ((ThermostatMode)(0x40))
      ((ThermostatOperatingState)(0x42))
      ((ThermostatSetpoint)(0x43))
      ((ThermostatFanMode)(0x44))
      ((ThermostatFanState)(0x45))
      ((ClimateControlSchedule)(0x46))
      ((ThermostatSetback)(0x47))
      ((BasicWindowCovering)(0x50))
      ((MtpWindowCovering)(0x51))
      ((MultiInstance)(0x60))
      ((DoorLock)(0x62))
      ((UserCode)(0x63))
      ((Configuration)(0x70))
      ((Alarm)(0x71))
      ((ManufacturerSpecific)(0x72))
      ((Powerlevel)(0x73))
      ((Protection)(0x75))
      ((Lock)(0x76))
      ((NodeNaming)(0x77))
      ((FirmwareUpdateMd)(0x7a))
      ((GroupingName)(0x7b))
      ((RemoteAssociationActivate)(0x7c))
      ((RemoteAssociation)(0x7d))
      ((Battery)(0x80))
      ((Clock)(0x81))
      ((Hail)(0x82))
      ((WakeUp)(0x84))
      ((Association)(0x85))
      ((Version)(0x86))
      ((Indicator)(0x87))
      ((Proprietary)(0x88))
      ((Language)(0x89))
      ((Time)(0x8a))
      ((TimeParameters)(0x8b))
      ((GeographicLocation)(0x8c))
      ((Composite)(0x8d))
      ((MultiInstanceAssociation)(0x8e))
      ((MultiCmd)(0x8f))
      ((EnergyProduction)(0x90))
      ((ManufacturerProprietary)(0x91))
      ((ScreenMd)(0x92))
      ((ScreenAttributes)(0x93))
      ((SimpleAvControl)(0x94))
      ((AvContentDirectoryMd)(0x95))
      ((AvRendererStatus)(0x96))
      ((AvContentSearchMd)(0x97))
      ((Security)(0x98))
      ((AvTaggingMd)(0x99))
      ((IpConfiguration)(0x9a))
      ((AssociationCommandConfiguration)(0x9b))
      ((SensorAlarm)(0x9c))
      ((SilenceAlarm)(0x9d))
      ((SensorConfiguration)(0x9e))
      ((NonInteroperable)(0xf0))
   );
   