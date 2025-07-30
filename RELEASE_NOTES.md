# Release notes

Release notes for the CAENHVAsyn EPICS Module.

## Releases:
* __R2.1.0__: 2025-06-30 lorelli
  * Add reconnection mechanism (#13) by marcomontevechi1

* __R2.0.1__: 2025-03-06 lorelli
  * Upgrade to asyn/R4.39-1.0.2
  * Configuration changes for rhel9 support

* __R2.0.0__: 2024-10-22 jmock
  * Build for rhel7 target - stop building rhel6
    * Update drvCAENHVAsyn.cpp make_pair function call for rhel7 compiler

* __R1.1.3__: 2022-02-22 egumtow
  * Update asyn module to R4.39-1.0.1

* __R1.1.2__: 2020-02-05 J. Mock
  * Add autosave fields to ai, ao, bi, bo, longin, longout template files

* __R1.1.1__: 2019-09-16 J. Vasquez
  * Update definition of RELEASE file. The version of EPICS base should not be defined there.

* __R1.1.0__: 2019-09-13 J. Vasquez
  * Bug fix: reduce a description string to less than 40 chars.
  * Print out the crate map and board information to the IOC shell.
  * Set PREC=2 on analog records.
  * Set PINI=YES on output records so that their are processed at boot time.
  * Rename the class "Chassis" to "Crate" for clarity.
  * Use smart pointer to handle Crate, Board, and Channel objects, to avoid resource leaks.
  * Add exception handling in all asyn I/O methods, related to this driver.

* __R1.0.0__: 2019-09-12 J. Vasquez
  * First stable release.
  * Includes support for parameters of type `PARAM_TYPE_CHSTATUS`, `PARAM_TYPE_BDSTATUS`, and `PARAM_TYPE_BINARY`.
  * Uses template base classes to avoid code duplication.
  * Bug fix: `ZNAM` and `ONAM` label were swapped on bi/bo records.
  * Add support for system parameters of type `SYSPROP_TYPE_REAL`.
  * Other minor bug fixes, and code cleanup.

* __R0.0.0__: 2019-09-06 J. Vasquez
  * First release candidate.
