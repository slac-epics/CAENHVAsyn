# How Does the Automatic Generation of Asyn Parameters and PVs Works?

## Overview

This EPICS module, called **CAENHVAsyn**,  integrates CAEN's HV Power Supplies into EPICS using Asyn and CAEN HV Wrapper Libraries.

This document describes how the automatic generation of PVs works.

## Auto-generation of PVs

The auto-generation of PVs is disabled by default. It can be enabled by defining a PV name prefix, as describe in [README.configureDriver.md](README.configureDriver.md).

## System Scanning

When the driver is instantiate, it will initially connect to the HV Power Supply crate and scan it to find out how many system parameter, boards, board parameters, channels per board, and channel parameter exists in the system. Each parameter is identify (besides it slot and channel number) by a string name as described in the *CAEN HV Wrapper Library* documentation. The type of each parameter is also found during this scanning process.

For each parameter found, an associate Asyn parameter is created. Is the PV auto-generation is enabled, 1 or 2 PVs will be created for each parameters (one for reading and one for writing, so parameters with R/W access will have 2 associate PVs). Alternatively, you can manually create you own PVs and use the auto-generated Asyn parameter name to access that particular parameter.

### Debug Information File

At the end of the scanning, an output file is created with with all the information found in the system. It includes all the parameters found in the system, its type and properties, as well as the Asyn paramater and PV name generated for each one. The output file is located at `/tmp/CAENHVAsyn_<ASYN_PORT_NAME>_crateInfo.txt`, where **ASYN_PORT_NAME** is the Asyn port name used for the driver.

## Asyn Parameter and PV Name

The parameters are subdivided into three categories. In each case, a modified version of the string that defines the HV Power Supply parameter is used as the trailing part of the name. The system string is converted to all upper cases, and white spaces are removed. For example, the system property `Clr Alarm` is converted to `CLRALARM`. This modified version of the system parameter is referred to as *PROCESSED_SYSTEM_PARAMETER* in the following descriptions.

The PV names have bot ha prefix and a suffix part. The prefix part is defined when enabling the auto-generation feature as describe above. The suffix will depend on the type of access of the respective system parameter, in the following way:

- If the system parameter has read-only access, the prefix will be `Rd`,
- If the system parameter has write-only access, the prefix will be `St`,
- If the system parameter has read-write access, 2 PVs will be generated, on to reading with suffix `Rd`, and one for writing with suffix `St`

### System Properties

The Asyn parameter name related to system properties has the following structure:

```
C_<PROCESSED_SYSTEM_PARAMETER>
```

The PV name, on the other hand has the following structure:

```
<PREFIX>:C:<PROCESSED_SYSTEM_PARAMETER>:<SUFFIX>
```

For example, the system parameter `CPULoad`, which has read-only access, will be accessible though the Asyn parameter called `C_CPULOAD`, and a PV called `<PREFIX>:C:CPULOAD:Rd` will be generated.

### Board Parameters

The Asyn parameter name related to board properties have the following structure:

```
S<SLOT_NUMBER>_<PROCESSED_SYSTEM_PARAMETER>
```

The PV name, on the other hand has the following structure:

```
<PREFIX>:S<SLOT_NUMBER>:<PROCESSED_SYSTEM_PARAMETER>:<SUFFIX>
```

Where:
- **SLOT_NUMBER** is a 2-digital decimal number representing the slot number (starting at zero), where the board is installed.

For example, the board parameter `HVMax`, which has read-only access, of the board installed in the first slot, will be accessible though the Asyn parameter called `S00_HVMAX`, and a PV called `<PREFIX>:S00:HVMAX:Rd` will be generated.

#### Parameters of type PARAM_TYPE_BDSTATUS

For board properties of type `PARAM_TYPE_BDSTATUS`, one/two bi/bo PVs are generated for each bit status. All the PV will use the same Asyn parameter name, with a appropiate mask value to selecte the correct bit in the status word. The name of each indivial PV will have the following strcuture:

```
<PREFIX>:S<SLOT_NUMBER>:<PROCESSED_SYSTEM_PARAMETER><BIT_SUFFIX>:<SUFFIX>
```

Where the **BIT_SUFFIX** wil be different for each bit status as described in the following table:

Bit position    | BIT_SUFFIX    | Bit Function Description
----------------|---------------|---------------------------------------
 0              | _PF           | Board is in power-fail status
 1              | _FCE          | Board has a firmware checksum error
 2              | _CEHV         | Board has calibration error on HV
 3              | _CET          | Board has a calibration error on temperature
 4              | _UT           | Board is in under-temperature status
 5              | _OT           | Board is in over-temperature status

#### Parameters of type PARAM_TYPE_CHSTATUS

For board properties of type `PARAM_TYPE_BDSTATUS`, one/two bi/bo PVs are generated for each bit status. All the PV will use the same Asyn parameter name, with a appropiate mask value to selecte the correct bit in the status word. The name of each indivial PV will have the following strcuture:

```
<PREFIX>:S<SLOT_NUMBER>:<PROCESSED_SYSTEM_PARAMETER><BIT_SUFFIX>:<SUFFIX>
```

Where the **BIT_SUFFIX** wil be different for each bit status as described in the following tables. For SYx527 crates:

Bit position    | BIT_SUFFIX    | Bit Function Description
----------------|---------------|---------------------------------------
  0             | _ON           | Channel is on
  1             | _RU           | Channel is ramping up
  2             | _RD           | Channel is ramping down
  3             | _OC           | Channel is in overcurrent
  4             | _OV           | Channel is in overvoltage
  5             | _UV           | Channel is in undervoltage
  6             | _ET           | Channel is in external trip
  7             | _MV           | Channel is in max V
  8             | _ED           | Channel is in external disable
  9             | _IT           | Channel is in internal trip
 10             | _CE           | Channel is in calibration error
 11             | _UN           | Channel is unplugged
 13             | _OVP          | Channel is in OverVoltage Protection
 14             | _PF           | Channel is in Power Fail
 15             | _TE           | Channel is in Temperature Error

And for Smart HV crates:

Bit position    | BIT_SUFFIX    | Bit Function Description
----------------|---------------|---------------------------------------
  0             | _ON           | Channel is on
  1             | _RU           | Channel is ramping up
  2             | _RD           | Channel is ramping down
  3             | _OC           | Channel is in overcurrent
  4             | _OV           | Channel is in overvoltage
  5             | _UV           | Channel is in undervoltage
  6             | _ET           | Channel is in external trip
  7             | _OP           | Channel power exceeds max power
  8             | _TW           | Channel is in Temperature Warning
  9             | _TE           | Channel is in Temperature Error
 10             | _KL           | Channel switch is in KILL position
 11             | _ED           | Channel is in external disable
 12             | _DS           | Channel is disabled
 13             | _FL           | Channel is in generic failure mode
 14             | _LK           | Channel is locked (local mode)
 15             | _VL           | Channel is voltage limited (via trimmer)

### Channel Parameters

The Asyn parameter name related to channel properties have the following structure:

```
S<SLOT_NUMBER>_C<CHANNEL_NUMBER>_<PROCESSED_SYSTEM_PARAMETER>
```

The PV name, on the other hand has the following structure:

```
<PREFIX>:S<SLOT_NUMBER>:C<CHANNEL_NUMBER>:<PROCESSED_SYSTEM_PARAMETER>:<SUFFIX>
```

Where:
- **SLOT_NUMBER** is a 2-digital decimal number representing the slot number (starting at zero), where the board is installed.
- **CHANNEL_NUMBER** is a 2-digital decimal number representing the channel number (starting at zero), on the respective board.

For example, the board parameter `V0Set` which has read-write access, of the fifth channel, of the board installed in the second slot, will be accessible though the Asyn parameter called `S01_C04_V0SET`, and 2 PVs will be generated called `<PREFIX>:S01:C04:V0SET:St` (for writing) `<PREFIX>:S01:C04:V0SET:Rd` (for reading).

#### Parameters of type PARAM_TYPE_CHSTATUS

For channel properties of type `PARAM_TYPE_BDSTATUS`, one/two bi/bo PVs are generated for each bit status. All the PV will use the same Asyn parameter name, with a appropiate mask value to selecte the correct bit in the status word. The name of each indivial PV will have the following strcuture:

```
<PREFIX>:S<SLOT_NUMBER>:C<CHANNEL_NUMBER>:<PROCESSED_SYSTEM_PARAMETER><BIT_SUFFIX>:<SUFFIX>
```

Where the **BIT_SUFFIX** wil be different for each bit status as described in the following table:

Bit position    | BIT_SUFFIX    | Bit Function Description
----------------|---------------|---------------------------------------
  0             | _ON           | Channel is on
  1             | _RU           | Channel is ramping up
  2             | _RD           | Channel is ramping down
  3             | _OC           | Channel is in overcurrent
  4             | _OV           | Channel is in overvoltage
  5             | _UV           | Channel is in undervoltage
  6             | _ET           | Channel is in external trip
  7             | _MV           | Channel is in max V
  8             | _ED           | Channel is in external disable
  9             | _IT           | Channel is in internal trip
 10             | _CE           | Channel is in calibration error
 11             | _UN           | Channel is unplugged
 13             | _OVP          | Channel is in OverVoltage Protection
 14             | _PF           | Channel is in Power Fail
 15             | _TE           | Channel is in Temperature Error

## Asyn Parameter Type

Depending on the type of parameter found on the HV Power supply crate, an appropriate Asyn parameter type is used according to this table. The table also shows which type of record, and which DTYP field is auto-generated. If you define PV manually, you should use the same type of record as describe in the table.

HV Power Supply Parameter Type  | Asyn Parameter            | Record type (R/W) | DTYP field (R/W)
--------------------------------|---------------------------|-------------------|---------------
SYSPROP_TYPE_STR                | asynParamOctet            | waveform          | asynOctetRead/asynOctetWrite
SYSPROP_TYPE_REAL               | asynParamInt32            | longin/longout    | asynInt32
SYSPROP_TYPE_UINT2              | asynParamInt32            | longin/longout    | asynInt32
SYSPROP_TYPE_UINT4              | asynParamInt32            | longin/longout    | asynInt32
SYSPROP_TYPE_INT2               | asynParamInt32            | longin/longout    | asynInt32
SYSPROP_TYPE_INT4               | asynParamInt32            | longin/longout    | asynInt32
SYSPROP_TYPE_BOOLEAN            | asynParamInt32            | longin/longout    | asynInt32
PARAM_TYPE_NUMERIC              | asynParamFloat64          | ai/ao             | asynFloat64
PARAM_TYPE_ONOFF                | asynParamUInt32Digital    | bi/bo             | asynUInt32Digital
PARAM_TYPE_BDSTATUS             | asynParamUInt32Digital    | bi/bo             | asynUInt32Digital
PARAM_TYPE_CHSTATUS             | asynParamUInt32Digital    | bi/bo             | asynUInt32Digital
PARAM_TYPE_BINARY               | asynParamInt32            | longin/longout    | asynInt32
