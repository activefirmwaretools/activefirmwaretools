@echo off
setlocal

set OUTDIR=%TEMP%\ActiveAPITest
mkdir "%OUTDIR%" 2>nul

echo.
echo ============================================================
echo  ACTIVE-PRO Automation API - Full Command Test
echo  Output files: %OUTDIR%
echo ============================================================


echo.
echo --- CONNECTION AND STATUS ----------------------------------

echo Hello:
echo   ^> Hello
activeapicommandline "Hello"

echo isConnected:
echo   ^> isConnected
activeapicommandline "isConnected"

echo isCapturing:
echo   ^> isCapturing
activeapicommandline "isCapturing"

echo GetDevicesAttached:
echo   ^> GetDevicesAttached
activeapicommandline "GetDevicesAttached"

echo SelectDevice 1:
echo   ^> SelectDevice 1
activeapicommandline "SelectDevice 1"


echo.
echo --- LIVE SIGNAL READING (requires active capture) ----------
echo NOTE: GetLogic and GetCH1-CH6 are read during the capture below


echo.
echo --- DIGITAL OUTPUT CONTROL ---------------------------------

echo SetD0Mode 0 (tri-state):
echo   ^> SetD0Mode 0
activeapicommandline "SetD0Mode 0"

echo SetD0Mode 1 (low):
echo   ^> SetD0Mode 1
activeapicommandline "SetD0Mode 1"

echo SetD0Mode 2 (high):
echo   ^> SetD0Mode 2
activeapicommandline "SetD0Mode 2"

echo SetD0Mode 3 (PWM):
echo   ^> SetD0Mode 3
activeapicommandline "SetD0Mode 3"

echo SetD0PWM 128:
echo   ^> SetD0PWM 128
activeapicommandline "SetD0PWM 128"

echo SetD1Mode 0 (tri-state):
echo   ^> SetD1Mode 0
activeapicommandline "SetD1Mode 0"

echo SetD1Mode 1 (low):
echo   ^> SetD1Mode 1
activeapicommandline "SetD1Mode 1"

echo SetD1Mode 2 (high):
echo   ^> SetD1Mode 2
activeapicommandline "SetD1Mode 2"

echo SetD1Mode 3 (PWM):
echo   ^> SetD1Mode 3
activeapicommandline "SetD1Mode 3"

echo SetD1PWM 64:
echo   ^> SetD1PWM 64
activeapicommandline "SetD1PWM 64"


echo.
echo --- ANALOG OUTPUT CONTROL ----------------------------------

echo SetA0Mode 0 (tri-state):
echo   ^> SetA0Mode 0
activeapicommandline "SetA0Mode 0"

echo SetA0Mode 1 (0V):
echo   ^> SetA0Mode 1
activeapicommandline "SetA0Mode 1"

echo SetA0Mode 2 (1V):
echo   ^> SetA0Mode 2
activeapicommandline "SetA0Mode 2"

echo SetA0Mode 3 (2V):
echo   ^> SetA0Mode 3
activeapicommandline "SetA0Mode 3"

echo SetA0Mode 4 (3V):
echo   ^> SetA0Mode 4
activeapicommandline "SetA0Mode 4"

echo SetA0Mode 5 (4V):
echo   ^> SetA0Mode 5
activeapicommandline "SetA0Mode 5"

echo SetA0Mode 6 (custom DC):
echo   ^> SetA0Mode 6
activeapicommandline "SetA0Mode 6"

echo SetA0DCLevel 1.75:
echo   ^> SetA0DCLevel 1.75
activeapicommandline "SetA0DCLevel 1.75"

echo SetA1Mode 0 (tri-state):
echo   ^> SetA1Mode 0
activeapicommandline "SetA1Mode 0"

echo SetA1Mode 1 (0V):
echo   ^> SetA1Mode 1
activeapicommandline "SetA1Mode 1"

echo SetA1Mode 2 (1V):
echo   ^> SetA1Mode 2
activeapicommandline "SetA1Mode 2"

echo SetA1Mode 3 (2V):
echo   ^> SetA1Mode 3
activeapicommandline "SetA1Mode 3"

echo SetA1Mode 4 (3V):
echo   ^> SetA1Mode 4
activeapicommandline "SetA1Mode 4"

echo SetA1Mode 5 (4V):
echo   ^> SetA1Mode 5
activeapicommandline "SetA1Mode 5"

echo SetA1Mode 6 (custom DC):
echo   ^> SetA1Mode 6
activeapicommandline "SetA1Mode 6"

echo SetA1DCLevel 2.5:
echo   ^> SetA1DCLevel 2.5
activeapicommandline "SetA1DCLevel 2.5"

echo SetA1Mode 7 (ramp):
echo   ^> SetA1Mode 7
activeapicommandline "SetA1Mode 7"

echo SetA1Mode 8 (sine):
echo   ^> SetA1Mode 8
activeapicommandline "SetA1Mode 8"

echo SetA1Mode 9 (square):
echo   ^> SetA1Mode 9
activeapicommandline "SetA1Mode 9"

echo SetA1Mode 10 (triangle):
echo   ^> SetA1Mode 10
activeapicommandline "SetA1Mode 10"

echo SetA1Minimum 0.0:
echo   ^> SetA1Minimum 0.0
activeapicommandline "SetA1Minimum 0.0"

echo SetA1Maximum 3.3:
echo   ^> SetA1Maximum 3.3
activeapicommandline "SetA1Maximum 3.3"

echo SetA1Steps 50:
echo   ^> SetA1Steps 50
activeapicommandline "SetA1Steps 50"


echo.
echo --- INPUT CHANNEL SETTINGS ---------------------------------

echo ThresholdLevel 1.0:
echo   ^> ThresholdLevel 1.0
activeapicommandline "ThresholdLevel 1.0"

echo ThresholdLevel 1.8:
echo   ^> ThresholdLevel 1.8
activeapicommandline "ThresholdLevel 1.8"

echo ThresholdLevel 2.5:
echo   ^> ThresholdLevel 2.5
activeapicommandline "ThresholdLevel 2.5"

echo ThresholdLevel 3.3:
echo   ^> ThresholdLevel 3.3
activeapicommandline "ThresholdLevel 3.3"

echo ThresholdLevel 5.0:
echo   ^> ThresholdLevel 5.0
activeapicommandline "ThresholdLevel 5.0"

echo ThresholdLevel 3.3 (restore default):
echo   ^> ThresholdLevel 3.3
activeapicommandline "ThresholdLevel 3.3"


echo.
echo --- DEVICE MODE --------------------------------------------

echo DeviceAMode 0 (OFF):
echo   ^> DeviceAMode 0
activeapicommandline "DeviceAMode 0"

echo DeviceAMode 1 (ACTIVE 2-Wire):
echo   ^> DeviceAMode 1
activeapicommandline "DeviceAMode 1"

echo DeviceAMode 6 (UART 8,N,1):
echo   ^> DeviceAMode 6
activeapicommandline "DeviceAMode 6"

echo DeviceASetting 115200:
echo   ^> DeviceASetting 115200
activeapicommandline "DeviceASetting 115200"

echo DeviceAMode 7 (UART Inverted):
echo   ^> DeviceAMode 7
activeapicommandline "DeviceAMode 7"

echo DeviceASetting 9600:
echo   ^> DeviceASetting 9600
activeapicommandline "DeviceASetting 9600"

echo DeviceAMode 3 (I2C):
echo   ^> DeviceAMode 3
activeapicommandline "DeviceAMode 3"

echo DeviceAMode 8 (SPI SS-low SCK-rising):
echo   ^> DeviceAMode 8
activeapicommandline "DeviceAMode 8"

echo DeviceAMode 14 (1-Wire):
echo   ^> DeviceAMode 14
activeapicommandline "DeviceAMode 14"

echo DeviceAMode 16 (LIN):
echo   ^> DeviceAMode 16
activeapicommandline "DeviceAMode 16"

echo DeviceASetting 20000:
echo   ^> DeviceASetting 20000
activeapicommandline "DeviceASetting 20000"

echo DeviceAMode 32 (RS232):
echo   ^> DeviceAMode 32
activeapicommandline "DeviceAMode 32"

echo DeviceASetting 9600:
echo   ^> DeviceASetting 9600
activeapicommandline "DeviceASetting 9600"

echo DeviceAMode 34 (MDIO):
echo   ^> DeviceAMode 34
activeapicommandline "DeviceAMode 34"

echo DeviceAMode 35 (UART 9,N,1):
echo   ^> DeviceAMode 35
activeapicommandline "DeviceAMode 35"

echo DeviceASetting 9600:
echo   ^> DeviceASetting 9600
activeapicommandline "DeviceASetting 9600"

echo DeviceAMode 37 (ACTIVE SWV):
echo   ^> DeviceAMode 37
activeapicommandline "DeviceAMode 37"

echo DeviceASetting 115200:
echo   ^> DeviceASetting 115200
activeapicommandline "DeviceASetting 115200"

echo DeviceBMode 6 (UART 8,N,1):
echo   ^> DeviceBMode 6
activeapicommandline "DeviceBMode 6"

echo DeviceBSetting 9600:
echo   ^> DeviceBSetting 9600
activeapicommandline "DeviceBSetting 9600"

echo DeviceCMode 8 (SPI SS-low SCK-rising):
echo   ^> DeviceCMode 8
activeapicommandline "DeviceCMode 8"

echo DeviceDMode 3 (I2C):
echo   ^> DeviceDMode 3
activeapicommandline "DeviceDMode 3"

echo DeviceAMode 0 (restore OFF):
echo   ^> DeviceAMode 0
activeapicommandline "DeviceAMode 0"

echo DeviceBMode 0 (restore OFF):
echo   ^> DeviceBMode 0
activeapicommandline "DeviceBMode 0"

echo DeviceCMode 0 (restore OFF):
echo   ^> DeviceCMode 0
activeapicommandline "DeviceCMode 0"

echo DeviceDMode 0 (restore OFF):
echo   ^> DeviceDMode 0
activeapicommandline "DeviceDMode 0"


echo.
echo --- LOGIC CHANNEL ENABLE / DISABLE -------------------------

echo LogicCH0On:
echo   ^> LogicCH0On
activeapicommandline "LogicCH0On"

echo LogicCH1On:
echo   ^> LogicCH1On
activeapicommandline "LogicCH1On"

echo LogicCH2On:
echo   ^> LogicCH2On
activeapicommandline "LogicCH2On"

echo LogicCH3On:
echo   ^> LogicCH3On
activeapicommandline "LogicCH3On"

echo LogicCH4On:
echo   ^> LogicCH4On
activeapicommandline "LogicCH4On"

echo LogicCH5On:
echo   ^> LogicCH5On
activeapicommandline "LogicCH5On"

echo LogicCH6On:
echo   ^> LogicCH6On
activeapicommandline "LogicCH6On"

echo LogicCH7On:
echo   ^> LogicCH7On
activeapicommandline "LogicCH7On"

echo LogicCH0Off:
echo   ^> LogicCH0Off
activeapicommandline "LogicCH0Off"

echo LogicCH1Off:
echo   ^> LogicCH1Off
activeapicommandline "LogicCH1Off"

echo LogicCH0On (restore):
echo   ^> LogicCH0On
activeapicommandline "LogicCH0On"

echo LogicCH1On (restore):
echo   ^> LogicCH1On
activeapicommandline "LogicCH1On"


echo.
echo --- ANALOG CHANNEL ENABLE / DISABLE ------------------------

echo AnalogCH1On:
echo   ^> AnalogCH1On
activeapicommandline "AnalogCH1On"

echo AnalogCH2On:
echo   ^> AnalogCH2On
activeapicommandline "AnalogCH2On"

echo AnalogCH3On:
echo   ^> AnalogCH3On
activeapicommandline "AnalogCH3On"

echo AnalogCH4On:
echo   ^> AnalogCH4On
activeapicommandline "AnalogCH4On"

echo AnalogCH5On (Ultra only):
echo   ^> AnalogCH5On
activeapicommandline "AnalogCH5On"

echo AnalogCH6On (Ultra only):
echo   ^> AnalogCH6On
activeapicommandline "AnalogCH6On"

echo AnalogCH7On (Ultra only):
echo   ^> AnalogCH7On
activeapicommandline "AnalogCH7On"

echo AnalogCH8On (Ultra only):
echo   ^> AnalogCH8On
activeapicommandline "AnalogCH8On"

echo AnalogCH2Off:
echo   ^> AnalogCH2Off
activeapicommandline "AnalogCH2Off"

echo AnalogCH3Off:
echo   ^> AnalogCH3Off
activeapicommandline "AnalogCH3Off"

echo AnalogCH2On (restore):
echo   ^> AnalogCH2On
activeapicommandline "AnalogCH2On"

echo AnalogCH3On (restore):
echo   ^> AnalogCH3On
activeapicommandline "AnalogCH3On"


echo.
echo --- CHANNEL LABELS -----------------------------------------

echo LogicCH0Label:
echo   ^> LogicCH0Label D0 Signal
activeapicommandline "LogicCH0Label D0 Signal"

echo LogicCH1Label:
echo   ^> LogicCH1Label D1 Signal
activeapicommandline "LogicCH1Label D1 Signal"

echo LogicCH2Label:
echo   ^> LogicCH2Label D2 Signal
activeapicommandline "LogicCH2Label D2 Signal"

echo LogicCH3Label:
echo   ^> LogicCH3Label D3 Signal
activeapicommandline "LogicCH3Label D3 Signal"

echo LogicCH4Label:
echo   ^> LogicCH4Label D4 Signal
activeapicommandline "LogicCH4Label D4 Signal"

echo LogicCH5Label:
echo   ^> LogicCH5Label D5 Signal
activeapicommandline "LogicCH5Label D5 Signal"

echo LogicCH6Label:
echo   ^> LogicCH6Label D6 Signal
activeapicommandline "LogicCH6Label D6 Signal"

echo LogicCH7Label:
echo   ^> LogicCH7Label D7 Signal
activeapicommandline "LogicCH7Label D7 Signal"

echo AnalogCH1Label:
echo   ^> AnalogCH1Label CH1 Voltage
activeapicommandline "AnalogCH1Label CH1 Voltage"

echo AnalogCH2Label:
echo   ^> AnalogCH2Label CH2 Voltage
activeapicommandline "AnalogCH2Label CH2 Voltage"

echo AnalogCH3Label:
echo   ^> AnalogCH3Label CH3 Voltage
activeapicommandline "AnalogCH3Label CH3 Voltage"

echo AnalogCH4Label:
echo   ^> AnalogCH4Label CH4 Voltage
activeapicommandline "AnalogCH4Label CH4 Voltage"

echo AnalogCH5Label (Ultra only):
echo   ^> AnalogCH5Label CH5 Voltage
activeapicommandline "AnalogCH5Label CH5 Voltage"

echo AnalogCH6Label (Ultra only):
echo   ^> AnalogCH6Label CH6 Voltage
activeapicommandline "AnalogCH6Label CH6 Voltage"

echo AnalogCH7Label (Ultra only):
echo   ^> AnalogCH7Label CH7 Voltage
activeapicommandline "AnalogCH7Label CH7 Voltage"

echo AnalogCH8Label (Ultra only):
echo   ^> AnalogCH8Label CH8 Voltage
activeapicommandline "AnalogCH8Label CH8 Voltage"

echo DeviceALabel:
echo   ^> DeviceALabel Device A Port
activeapicommandline "DeviceALabel Device A Port"

echo DeviceBLabel:
echo   ^> DeviceBLabel Device B Port
activeapicommandline "DeviceBLabel Device B Port"

echo DeviceCLabel:
echo   ^> DeviceCLabel Device C Port
activeapicommandline "DeviceCLabel Device C Port"

echo DeviceDLabel:
echo   ^> DeviceDLabel Device D Port
activeapicommandline "DeviceDLabel Device D Port"


echo.
echo --- NOTES --------------------------------------------------

echo ClearNote:
echo   ^> ClearNote
activeapicommandline "ClearNote"

echo AppendNote (line 1):
echo   ^> AppendNote API test run started
activeapicommandline "AppendNote API test run started"

echo AppendNote (line 2):
echo   ^> AppendNote Second note line
activeapicommandline "AppendNote Second note line"

echo GetNote:
echo   ^> GetNote
activeapicommandline "GetNote"


echo.
echo --- ANALYSIS CONTEXT ---------------------------------------

echo ClearAnalysisContext:
echo   ^> ClearAnalysisContext
activeapicommandline "ClearAnalysisContext"

echo AppendAnalysisContext (line 1):
echo   ^> AppendAnalysisContext Testing all API commands
activeapicommandline "AppendAnalysisContext Testing all API commands"

echo AppendAnalysisContext (line 2):
echo   ^> AppendAnalysisContext Second context line
activeapicommandline "AppendAnalysisContext Second context line"

echo GetAnalysisContext:
echo   ^> GetAnalysisContext
activeapicommandline "GetAnalysisContext"


echo.
echo --- TAB VISIBILITY -----------------------------------------

echo ShowInputs:
echo   ^> ShowInputs
activeapicommandline "ShowInputs"

echo ShowOutputs:
echo   ^> ShowOutputs
activeapicommandline "ShowOutputs"

echo ShowList:
echo   ^> ShowList
activeapicommandline "ShowList"

echo ShowSettings:
echo   ^> ShowSettings
activeapicommandline "ShowSettings"

echo ShowNotes:
echo   ^> ShowNotes
activeapicommandline "ShowNotes"

echo ShowCode:
echo   ^> ShowCode
activeapicommandline "ShowCode"

echo ShowLiveUI:
echo   ^> ShowLiveUI
activeapicommandline "ShowLiveUI"

echo ShowDevices:
echo   ^> ShowDevices
activeapicommandline "ShowDevices"

echo CloseTabs:
echo   ^> CloseTabs
activeapicommandline "CloseTabs"


echo.
echo --- WAVEFORM DEVICE VISIBILITY -----------------------------

echo ShowDeviceA:
echo   ^> ShowDeviceA
activeapicommandline "ShowDeviceA"

echo HideDeviceA:
echo   ^> HideDeviceA
activeapicommandline "HideDeviceA"

echo ShowDeviceB:
echo   ^> ShowDeviceB
activeapicommandline "ShowDeviceB"

echo HideDeviceB:
echo   ^> HideDeviceB
activeapicommandline "HideDeviceB"

echo ShowDeviceC:
echo   ^> ShowDeviceC
activeapicommandline "ShowDeviceC"

echo HideDeviceC:
echo   ^> HideDeviceC
activeapicommandline "HideDeviceC"

echo ShowDeviceD:
echo   ^> ShowDeviceD
activeapicommandline "ShowDeviceD"

echo HideDeviceD:
echo   ^> HideDeviceD
activeapicommandline "HideDeviceD"

echo ShowDeviceA (restore):
echo   ^> ShowDeviceA
activeapicommandline "ShowDeviceA"


echo.
echo --- CAPTURE (3 second capture) -----------------------------

echo NewCapture:
echo   ^> NewCapture
activeapicommandline "NewCapture"

echo StartCapture:
echo   ^> StartCapture
activeapicommandline "StartCapture"

TIMEOUT /T 3 /NOBREAK

echo isCapturing (should be YES):
echo   ^> isCapturing
activeapicommandline "isCapturing"

echo GetLogic:
echo   ^> GetLogic
activeapicommandline "GetLogic"

echo GetCH1:
echo   ^> GetCH1
activeapicommandline "GetCH1"

echo GetCH2:
echo   ^> GetCH2
activeapicommandline "GetCH2"

echo GetCH3:
echo   ^> GetCH3
activeapicommandline "GetCH3"

echo GetCH4 (Ultra only):
echo   ^> GetCH4
activeapicommandline "GetCH4"

echo GetCH5 (Ultra only):
echo   ^> GetCH5
activeapicommandline "GetCH5"

echo GetCH6 (Ultra only):
echo   ^> GetCH6
activeapicommandline "GetCH6"

echo GetCaptureSize:
echo   ^> GetCaptureSize
activeapicommandline "GetCaptureSize"

echo GetCaptureTime:
echo   ^> GetCaptureTime
activeapicommandline "GetCaptureTime"

echo StopCapture:
echo   ^> StopCapture
activeapicommandline "StopCapture"

echo isCapturing (should be NO):
echo   ^> isCapturing
activeapicommandline "isCapturing"


echo.
echo --- CURSOR CONTROL -----------------------------------------

echo SetCursorCurrent 0.5:
echo   ^> SetCursorCurrent 0.5
activeapicommandline "SetCursorCurrent 0.5"

echo GetCursorCurrent:
echo   ^> GetCursorCurrent
activeapicommandline "GetCursorCurrent"

echo SetCursorX1 0.5:
echo   ^> SetCursorX1 0.5
activeapicommandline "SetCursorX1 0.5"

echo GetCursorX1:
echo   ^> GetCursorX1
activeapicommandline "GetCursorX1"

echo SetCursorX2 2.0:
echo   ^> SetCursorX2 2.0
activeapicommandline "SetCursorX2 2.0"

echo GetCursorX2:
echo   ^> GetCursorX2
activeapicommandline "GetCursorX2"


echo.
echo --- VIEW AND NAVIGATION ------------------------------------

echo ZoomAll:
echo   ^> ZoomAll
activeapicommandline "ZoomAll"

echo ZoomFrom 0.0 1.0:
echo   ^> ZoomFrom 0.0 1.0
activeapicommandline "ZoomFrom 0.0 1.0"

echo Search ERROR:
echo   ^> Search ERROR
activeapicommandline "Search ERROR"

echo SetCursorCurrent 0.0 (reset for next search):
echo   ^> SetCursorCurrent 0.0
activeapicommandline "SetCursorCurrent 0.0"

echo Search BOOT:
echo   ^> Search BOOT
activeapicommandline "Search BOOT"


echo.
echo --- DISPLAY ------------------------------------------------

echo SetDarkMode:
echo   ^> SetDarkMode
activeapicommandline "SetDarkMode"

echo SetLightMode:
echo   ^> SetLightMode
activeapicommandline "SetLightMode"


echo.
echo --- FILE OPERATIONS ----------------------------------------

echo SaveCapture:
echo   ^> SaveCapture %OUTDIR%\test.active
activeapicommandline "SaveCapture %OUTDIR%\test.active"

echo SaveBetweenCursors:
echo   ^> SaveBetweenCursors %OUTDIR%\test_range.active
activeapicommandline "SaveBetweenCursors %OUTDIR%\test_range.active"

echo SaveConfiguration:
echo   ^> SaveConfiguration %OUTDIR%\test_config.ini
activeapicommandline "SaveConfiguration %OUTDIR%\test_config.ini"

echo ExportBetweenCursors:
echo   ^> ExportBetweenCursors %OUTDIR%\test_export.csv
activeapicommandline "ExportBetweenCursors %OUTDIR%\test_export.csv"

echo ExportDataBetweenCursors:
echo   ^> ExportDataBetweenCursors %OUTDIR%\test_export2.csv
activeapicommandline "ExportDataBetweenCursors %OUTDIR%\test_export2.csv"

echo ExportAISnapshotBetweenCursors:
echo   ^> ExportAISnapshotBetweenCursors %OUTDIR%\test_snapshot.aft
activeapicommandline "ExportAISnapshotBetweenCursors %OUTDIR%\test_snapshot.aft"

echo SaveScreenshot:
echo   ^> SaveScreenshot %OUTDIR%\test_screenshot.png
activeapicommandline "SaveScreenshot %OUTDIR%\test_screenshot.png"

echo OpenConfiguration:
echo   ^> OpenConfiguration %OUTDIR%\test_config.ini
activeapicommandline "OpenConfiguration %OUTDIR%\test_config.ini"

echo OpenCapture:
echo   ^> OpenCapture %OUTDIR%\test.active
activeapicommandline "OpenCapture %OUTDIR%\test.active"


echo.
echo ============================================================
echo  Done.  Check output files in: %OUTDIR%
echo ============================================================
echo.

pause
