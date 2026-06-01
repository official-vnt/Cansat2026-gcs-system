# Ground Station Commands

## 1. START_CALIBRATION
**Purpose:** Calibrates onboard sensors before launch.

**Functions:**
* IMU calibration
* Gyroscope bias calculation
* Accelerometer offset correction
* Sensor health verification

**Expected Response**
```text
CALIBRATION_STARTED
CALIBRATION_COMPLETE
```

## 2. ARM_SYSTEM
**Purpose:** Places the CanSat into mission-ready mode.
**Functions:**
* Enables critical command execution
* Verifies sensor readiness
* Verifies telemetry link
* Unlocks deployment and recovery functions
**Expected Response**
```text
SYSTEM_ARMED
```
## 3. DEPLOY_ARMS
**Purpose:** Deploys the CanSat's aerodynamic/control arms.
**Functions:**
* Activates deployment mechanism
* Opens external arms
* Updates mission state
**Expected Response**
```text
ARMS_DEPLOYED
```
## 4. START_MOTOR
**Purpose:** Starts the propulsion/stabilization motor.
**Functions:**
* Enables motor controller
* Starts motor rotation
* Initiates active flight control
**Expected Response**
```text
MOTOR_STARTED
```
## 5. STOP_MOTOR
**Purpose:** Stops all motor activity.
**Functions:**
* Disables motor output
* Returns propulsion system to safe state
**Expected Response**
```text
MOTOR_STOPPED
```
## 6. BUZZER_ON
**Purpose:** Activates the recovery buzzer.
**Functions:**
* Starts audible beacon
* Assists recovery team in locating the CanSat
**Expected Response**
```text
BUZZER_ON
```
## 7. BUZZER_OFF
**Purpose:** Deactivates the recovery buzzer.
**Functions:**
* Stops audible beacon signal
**Expected Response**
```text
BUZZER_OFF
```
## 8. REQUEST_GPS
**Purpose:** Requests the latest GPS information.
**Functions:**
* Retrieves latitude
* Retrieves longitude
* Retrieves GPS altitude
* Retrieves satellite count
**Expected Response**
```text
LAT: xx.xxxxxx
LON: xx.xxxxxx
ALT: xxx.x
SAT: xx
```

## 9. RESET_SYSTEM
**Purpose:** Performs a software reset of the CanSat.
**Functions:**

* Restarts STM32
* Reinitializes sensors
* Re-establishes telemetry communication

**Expected Response**

```text
SYSTEM_RESET
```

# Recommended Mission Sequence

```text
1. START_CALIBRATION
2. ARM_SYSTEM
3. REQUEST_STATUS
4. REQUEST_TELEMETRY
5. DEPLOY_ARMS
6. START_MOTOR
7. REQUEST_GPS
8. ACTIVATE_RECOVERY_MODE
9. SHUTDOWN_TELEMETRY
10. BUZZER_ON
```
