# How to Configure the Driver

## Overview

This modules integrates CAEN's HV Power Supplies into EPICS using Asyn and CAEN HV Wrapper Libraries.

This document describes how to configure the driver and load it into your IOC.

## CAENHVAsyn Configuration

In order to use the **CAENHVAsyn** EPICS module in your application you must call **CAENHVAsynConfig** on your IOC's st.cmd script.

With the following parameters


CAENHVAsynConfig(PORT_NAME, SYSTEM_TYPE, IP_ADDR, USER_NAME, PASSWORD)

| Parameter                  | Description
|----------------------------|-----------------------------
| PORT_NAME                  | The name given to this asyn port driver.
| SYSTEM_TYPE                | Type of system of the HV Power supply crate.
| IP_ADDR                    | IP address of the HV Power supply crate.
| USER_NAME                  | User name to access the HV Power supply crate.
| PASSWORD                   | Password to access the HV Power supply crate.

**Notes:**
- **SYSTEM_TYPE**: The system type is identified by a integer number, describe in the *CAEN HV Wrapper Library* documentation. Currently only SYx527 (value from 0 to 3) and SMART HV (13) are supported by this driver.


## Optional configuration parameters

Some parameters used by the driver have default values that can be changed calling functions in your **st.cmd**. The following is a list these paramaters,
their default values, and the function used to change them.

| Parameter                                          | Default value     | Function to set a new value
|----------------------------------------------------|-------------------|-------------------------------------
| Name prefix used for auto-generated PVs            | (empty)           | CAENHVAsynSetEpicsPrefix(const char* prefix)

You must call these functions in your **st.cmd** before calling **CAENHVAsynConfig**. The changes will apply to all instances of CAENHVAsyn you have in
your application.

**Notes:**
- If the PV name prefix parameter is empty (its default value), the auto-generation of PVs will be disabled.
