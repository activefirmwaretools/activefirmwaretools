@echo off
setlocal enabledelayedexpansion

REM ==================================================================
REM  ACTIVE-PRO Automation API - FULL command test
REM
REM  Exercises every command handled by myserver.cpp (the ground-truth
REM  command parser).  Commands are ordered so each one's preconditions
REM  are satisfied:
REM
REM    1. Capture is STOPPED before SelectDevice and before any
REM       reconfiguration.  SelectDevice resets all settings, and channel
REM       / device / trigger / buffer settings only take effect at the
REM       next capture, so they MUST be issued while stopped.
REM    2. Triggers, buffer envelope, channels, outputs, labels, notes
REM       are all configured before the capture starts.
REM    3. A capture runs so the live readers (GetLogic, GetCHx,
REM       GetCaptureSize/Time) have real data.
REM    4. Post-capture work (cursors, zoom, search, trigger navigation,
REM       ApplyChanges, file save/export) runs against the captured data.
REM
REM  A real ACTIVE pod must be attached.  With only the demo device,
REM  StartCapture returns NOACTIVEDEVICE and the live-read section is
REM  meaningless (everything else still responds).
REM ==================================================================

set OUTDIR=%TEMP%\ActiveAPITest
mkdir "%OUTDIR%" 2>nul

echo.
echo ============================================================
echo  ACTIVE-PRO Automation API - Full Command Test
echo  Output files: %OUTDIR%
echo ============================================================


echo.
echo --- 1. CONNECTION AND BASELINE (force STOPPED state) -------

echo Hello:
echo   ^> Hello
activeapicommandline "Hello"

echo StopCapture (defensive - guarantees we start stopped):
echo   ^> StopCapture
activeapicommandline "StopCapture"

echo isCapturing (should be NO):
echo   ^> isCapturing
activeapicommandline "isCapturing"

echo isConnected:
echo   ^> isConnected
activeapicommandline "isConnected"

echo IsProcessing (should be No):
echo   ^> IsProcessing
activeapicommandline "IsProcessing"


echo.
echo --- 2. DEVICE DISCOVERY / SELECTION (stopped) --------------
echo NOTE: SelectDevice resets ALL settings, so it runs FIRST,
echo       before any configuration below.

echo GetDevicesAttached:
echo   ^> GetDevicesAttached
activeapicommandline "GetDevicesAttached"

echo SelectDevice 1:
echo   ^> SelectDevice 1
activeapicommandline "SelectDevice 1"


echo.
echo --- 3. CAPTURE BUFFER / PRE-TRIGGER / POST-TRIGGER ---------
echo (configuration only - takes effect at next capture)

echo SetBufferSize 50:
echo   ^> SetBufferSize 50
activeapicommandline "SetBufferSize 50"

echo GetBufferSize:
echo   ^> GetBufferSize
activeapicommandline "GetBufferSize"

echo SetPreTriggerMode KEEPLAST:
echo   ^> SetPreTriggerMode KEEPLAST
activeapicommandline "SetPreTriggerMode KEEPLAST"

echo SetPreTriggerSeconds 5:
echo   ^> SetPreTriggerSeconds 5
activeapicommandline "SetPreTriggerSeconds 5"

echo GetPreTriggerSeconds:
echo   ^> GetPreTriggerSeconds
activeapicommandline "GetPreTriggerSeconds"

echo SetPreTriggerMode KEEPALL (restore - clean manual capture):
echo   ^> SetPreTriggerMode KEEPALL
activeapicommandline "SetPreTriggerMode KEEPALL"

echo GetPreTriggerMode:
echo   ^> GetPreTriggerMode
activeapicommandline "GetPreTriggerMode"

echo SetPostTriggerSeconds 10 (promotes mode to STOPAFTERSECONDS):
echo   ^> SetPostTriggerSeconds 10
activeapicommandline "SetPostTriggerSeconds 10"

echo GetPostTriggerSeconds:
echo   ^> GetPostTriggerSeconds
activeapicommandline "GetPostTriggerSeconds"

echo SetPostTriggerTriggers 100 (promotes mode to STOPAFTERTRIGGERS):
echo   ^> SetPostTriggerTriggers 100
activeapicommandline "SetPostTriggerTriggers 100"

echo GetPostTriggerTriggers:
echo   ^> GetPostTriggerTriggers
activeapicommandline "GetPostTriggerTriggers"

echo SetPostTriggerMode UNTILFULL (restore - clean manual capture):
echo   ^> SetPostTriggerMode UNTILFULL
activeapicommandline "SetPostTriggerMode UNTILFULL"

echo GetPostTriggerMode:
echo   ^> GetPostTriggerMode
activeapicommandline "GetPostTriggerMode"


echo.
echo --- 4. LOGIC CHANNEL ENABLE / DISABLE (stopped) -----------

echo LogicCH0On..LogicCH7On:
for %%C in (0 1 2 3 4 5 6 7) do (
   echo   ^> LogicCH%%COn
   activeapicommandline "LogicCH%%COn"
)

echo LogicCH6Off / LogicCH7Off (then restore):
echo   ^> LogicCH6Off
activeapicommandline "LogicCH6Off"
echo   ^> LogicCH7Off
activeapicommandline "LogicCH7Off"
echo   ^> LogicCH6On
activeapicommandline "LogicCH6On"
echo   ^> LogicCH7On
activeapicommandline "LogicCH7On"


echo.
echo --- 5. ANALOG CHANNEL ENABLE / DISABLE (stopped) ----------
echo NOTE: CH5-CH8 enable on ACTIVE-PRO Ultra only; on PRO they
echo       still return OK but have no hardware channel.

echo AnalogCH1On..AnalogCH8On:
for %%C in (1 2 3 4 5 6 7 8) do (
   echo   ^> AnalogCH%%COn
   activeapicommandline "AnalogCH%%COn"
)

echo AnalogCH3Off / AnalogCH4Off (then restore):
echo   ^> AnalogCH3Off
activeapicommandline "AnalogCH3Off"
echo   ^> AnalogCH4Off
activeapicommandline "AnalogCH4Off"
echo   ^> AnalogCH3On
activeapicommandline "AnalogCH3On"
echo   ^> AnalogCH4On
activeapicommandline "AnalogCH4On"


echo.
echo --- 6. DIGITAL THRESHOLD LEVEL (stopped) -------------------

for %%V in (1.0 1.8 2.5 3.3 5.0) do (
   echo   ^> ThresholdLevel %%V
   activeapicommandline "ThresholdLevel %%V"
)
echo ThresholdLevel 3.3 (restore default):
echo   ^> ThresholdLevel 3.3
activeapicommandline "ThresholdLevel 3.3"


echo.
echo --- 7. DEVICE DECODER MODES + SETTINGS (stopped) ----------
echo NOTE: DeviceXMode resets that device's label and setting to the
echo       protocol default, so set mode BEFORE setting the baud rate.
echo       SPI/LIN/RS232 modes are A/C-only; using them on B/D returns
echo       ERROR INVALIDMODE.  Devices are left enabled so the trigger
echo       text/values channel lists are populated below.

echo Device A = ACTIVE 2-Wire (mode 1):
echo   ^> DeviceAMode 1
activeapicommandline "DeviceAMode 1"

echo Device A label + (demonstrate UART baud, then back to ADP):
echo   ^> DeviceAMode 6
activeapicommandline "DeviceAMode 6"
echo   ^> DeviceASetting 115200
activeapicommandline "DeviceASetting 115200"
echo   ^> DeviceAMode 1
activeapicommandline "DeviceAMode 1"

echo Device B = UART 8,N,1 (mode 6) @ 9600:
echo   ^> DeviceBMode 6
activeapicommandline "DeviceBMode 6"
echo   ^> DeviceBSetting 9600
activeapicommandline "DeviceBSetting 9600"

echo Device C = SPI SS-low SCK-rising (mode 8, A/C only):
echo   ^> DeviceCMode 8
activeapicommandline "DeviceCMode 8"

echo Device D = I2C (mode 3):
echo   ^> DeviceDMode 3
activeapicommandline "DeviceDMode 3"


echo.
echo --- 8. CHANNEL / DEVICE LABELS (stopped) ------------------

echo LogicCH0Label..LogicCH7Label:
for %%C in (0 1 2 3 4 5 6 7) do (
   echo   ^> LogicCH%%CLabel D%%C Signal
   activeapicommandline "LogicCH%%CLabel D%%C Signal"
)

echo AnalogCH1Label..AnalogCH8Label:
for %%C in (1 2 3 4 5 6 7 8) do (
   echo   ^> AnalogCH%%CLabel CH%%C Voltage
   activeapicommandline "AnalogCH%%CLabel CH%%C Voltage"
)

echo DeviceALabel..DeviceDLabel:
echo   ^> DeviceALabel Device A Port
activeapicommandline "DeviceALabel Device A Port"
echo   ^> DeviceBLabel Device B Port
activeapicommandline "DeviceBLabel Device B Port"
echo   ^> DeviceCLabel Device C Port
activeapicommandline "DeviceCLabel Device C Port"
echo   ^> DeviceDLabel Device D Port
activeapicommandline "DeviceDLabel Device D Port"


echo.
echo --- 9. DIGITAL OUTPUT CONTROL -----------------------------

echo SetD0Mode 0..3 (tri-state / low / high / PWM):
for %%M in (0 1 2 3) do (
   echo   ^> SetD0Mode %%M
   activeapicommandline "SetD0Mode %%M"
)
echo SetD0PWM 128:
echo   ^> SetD0PWM 128
activeapicommandline "SetD0PWM 128"

echo SetD1Mode 0..3:
for %%M in (0 1 2 3) do (
   echo   ^> SetD1Mode %%M
   activeapicommandline "SetD1Mode %%M"
)
echo SetD1PWM 64:
echo   ^> SetD1PWM 64
activeapicommandline "SetD1PWM 64"


echo.
echo --- 10. ANALOG OUTPUT CONTROL -----------------------------

echo SetA0Mode 0..6 (tri-state / 0V / 1V / 2V / 3V / 4V / custom DC):
for %%M in (0 1 2 3 4 5 6) do (
   echo   ^> SetA0Mode %%M
   activeapicommandline "SetA0Mode %%M"
)
echo SetA0DCLevel 1.75:
echo   ^> SetA0DCLevel 1.75
activeapicommandline "SetA0DCLevel 1.75"

echo SetA1Mode 0..10 (incl. ramp / sine / square / triangle):
for %%M in (0 1 2 3 4 5 6 7 8 9 10) do (
   echo   ^> SetA1Mode %%M
   activeapicommandline "SetA1Mode %%M"
)
echo SetA1DCLevel 2.5:
echo   ^> SetA1DCLevel 2.5
activeapicommandline "SetA1DCLevel 2.5"
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
echo --- 11. NOTES ---------------------------------------------

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
echo --- 12. AI SNAPSHOT ANALYSIS CONTEXT ----------------------

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
echo --- 13. TRIGGER CONFIGURATION (stopped, before capture) ---
echo Source type is a two-stage selector: pick the source TYPE first
echo (it repopulates the channel + condition combos), then the channel,
echo then the condition, then any threshold / pulse-width / text param.

echo SetTriggerMode OFF / NORMAL / AUTO:
for %%M in (OFF NORMAL AUTO) do (
   echo   ^> SetTriggerMode %%M
   activeapicommandline "SetTriggerMode %%M"
)
echo GetTriggerMode:
echo   ^> GetTriggerMode
activeapicommandline "GetTriggerMode"

echo Walk every source type token + read back channel list:
for %%T in (NONE ANALOG DIGITAL ATEXT AVALUES BTEXT BVALUES CTEXT CVALUES DTEXT DVALUES APPGRAPH BPPGRAPH CPPGRAPH DPPGRAPH) do (
   echo   ^> SetTriggerSourceType %%T
   activeapicommandline "SetTriggerSourceType %%T"
   echo   ^> GetTriggerSourceType
   activeapicommandline "GetTriggerSourceType"
   echo   ^> GetTriggerChannelList
   activeapicommandline "GetTriggerChannelList"
)

echo ANALOG source: channel 1, RISING, threshold 1.5 V:
echo   ^> SetTriggerSourceType ANALOG
activeapicommandline "SetTriggerSourceType ANALOG"
echo   ^> SetTriggerChannel 1
activeapicommandline "SetTriggerChannel 1"
echo   ^> GetTriggerChannel
activeapicommandline "GetTriggerChannel"
echo   ^> SetTriggerCondition RISING
activeapicommandline "SetTriggerCondition RISING"
echo   ^> SetTriggerThreshold 1.5
activeapicommandline "SetTriggerThreshold 1.5"
echo   ^> GetTriggerThreshold
activeapicommandline "GetTriggerThreshold"
echo   ^> GetTriggerCondition
activeapicommandline "GetTriggerCondition"

echo DIGITAL source: channel 1, pulse-high-greater, width 500 us:
echo   ^> SetTriggerSourceType DIGITAL
activeapicommandline "SetTriggerSourceType DIGITAL"
echo   ^> SetTriggerChannel 1
activeapicommandline "SetTriggerChannel 1"
echo   ^> SetTriggerCondition PULSEHIGHGREATER
activeapicommandline "SetTriggerCondition PULSEHIGHGREATER"
echo   ^> SetTriggerPulseWidth 500 US
activeapicommandline "SetTriggerPulseWidth 500 US"
echo   ^> GetTriggerPulseWidth
activeapicommandline "GetTriggerPulseWidth"

echo ATEXT source: channel 1, CONTAINS, pattern text:
echo   ^> SetTriggerSourceType ATEXT
activeapicommandline "SetTriggerSourceType ATEXT"
echo   ^> SetTriggerChannel 1
activeapicommandline "SetTriggerChannel 1"
echo   ^> SetTriggerCondition CONTAINS
activeapicommandline "SetTriggerCondition CONTAINS"
echo   ^> SetTriggerText ERROR
activeapicommandline "SetTriggerText ERROR"
echo   ^> GetTriggerText
activeapicommandline "GetTriggerText"

echo AVALUES source: EQUALS, threshold 42:
echo   ^> SetTriggerSourceType AVALUES
activeapicommandline "SetTriggerSourceType AVALUES"
echo   ^> SetTriggerChannel 1
activeapicommandline "SetTriggerChannel 1"
echo   ^> SetTriggerCondition EQUALS
activeapicommandline "SetTriggerCondition EQUALS"
echo   ^> SetTriggerThreshold 42
activeapicommandline "SetTriggerThreshold 42"

echo FINAL trigger config: DIGITAL CH1 RISING, mode NORMAL (so it can fire):
echo   ^> SetTriggerSourceType DIGITAL
activeapicommandline "SetTriggerSourceType DIGITAL"
echo   ^> SetTriggerChannel 1
activeapicommandline "SetTriggerChannel 1"
echo   ^> SetTriggerCondition RISING
activeapicommandline "SetTriggerCondition RISING"
echo   ^> SetTriggerMode NORMAL
activeapicommandline "SetTriggerMode NORMAL"


echo.
echo --- 14. TAB / DEVICE VISIBILITY + DISPLAY -----------------

for %%T in (ShowInputs ShowTriggers ShowOutputs ShowList ShowSettings ShowNotes ShowCode ShowLiveUI ShowDevices CloseTabs) do (
   echo   ^> %%T
   activeapicommandline "%%T"
)

echo Hide then Show each waveform device:
for %%D in (A B C D) do (
   echo   ^> HideDevice%%D
   activeapicommandline "HideDevice%%D"
   echo   ^> ShowDevice%%D
   activeapicommandline "ShowDevice%%D"
)

echo SetDarkMode / SetLightMode:
echo   ^> SetDarkMode
activeapicommandline "SetDarkMode"
echo   ^> SetLightMode
activeapicommandline "SetLightMode"


echo.
echo --- 15. CAPTURE RUN (live readers need real data) ---------

echo NewCapture:
echo   ^> NewCapture
activeapicommandline "NewCapture"

echo StartCapture:
echo   ^> StartCapture
activeapicommandline "StartCapture"

echo isCapturing (should be YES):
echo   ^> isCapturing
activeapicommandline "isCapturing"

echo Capturing for 3 seconds...
TIMEOUT /T 3 /NOBREAK

echo Live reads during capture:
echo   ^> GetLogic
activeapicommandline "GetLogic"
for %%C in (1 2 3 4 5 6) do (
   echo   ^> GetCH%%C
   activeapicommandline "GetCH%%C"
)
echo   ^> GetCaptureSize
activeapicommandline "GetCaptureSize"
echo   ^> GetCaptureTime
activeapicommandline "GetCaptureTime"

echo StopCapture:
echo   ^> StopCapture
activeapicommandline "StopCapture"

echo isCapturing (should be NO):
echo   ^> isCapturing
activeapicommandline "isCapturing"

echo Wait for post-capture processing to finish (poll IsProcessing)...
call :WAITPROC


echo.
echo --- 16. ANALOG INTROSPECTION (read-only scaling) ----------
echo (CH1, CH4, CH8 representative of CH1..CH8 pattern)

for %%C in (1 4 8) do (
   echo   ^> GetCH%%COffset
   activeapicommandline "GetCH%%COffset"
   echo   ^> GetCH%%CScale
   activeapicommandline "GetCH%%CScale"
   echo   ^> GetCH%%CUnits
   activeapicommandline "GetCH%%CUnits"
)
echo   ^> GetCurrentResistor
activeapicommandline "GetCurrentResistor"
echo   ^> GetBatteryCapacity
activeapicommandline "GetBatteryCapacity"
echo   ^> GetAnalogRange
activeapicommandline "GetAnalogRange"
echo   ^> GetAnalogSampleRate
activeapicommandline "GetAnalogSampleRate"


echo.
echo --- 17. CURSORS / ZOOM / SEARCH (post-capture) ------------

echo SetCursorCurrent 0.0:
echo   ^> SetCursorCurrent 0.0
activeapicommandline "SetCursorCurrent 0.0"
echo   ^> GetCursorCurrent
activeapicommandline "GetCursorCurrent"

echo SetCursorX1 0.5:
echo   ^> SetCursorX1 0.5
activeapicommandline "SetCursorX1 0.5"
echo   ^> GetCursorX1
activeapicommandline "GetCursorX1"

echo SetCursorX2 2.0:
echo   ^> SetCursorX2 2.0
activeapicommandline "SetCursorX2 2.0"
echo   ^> GetCursorX2
activeapicommandline "GetCursorX2"

echo ZoomAll:
echo   ^> ZoomAll
activeapicommandline "ZoomAll"

echo ZoomFrom 0.0 1.0:
echo   ^> ZoomFrom 0.0 1.0
activeapicommandline "ZoomFrom 0.0 1.0"

echo Search ERROR (from current cursor; NOTFOUND if no match):
echo   ^> SetCursorCurrent 0.0
activeapicommandline "SetCursorCurrent 0.0"
echo   ^> Search ERROR
activeapicommandline "Search ERROR"


echo.
echo --- 18. TRIGGER STATUS / NAVIGATION (post-capture) --------
echo NOTE: NOTFOUND / 0 are valid responses when no trigger fired.

echo   ^> IsTriggered
activeapicommandline "IsTriggered"
echo   ^> GetTriggerCount
activeapicommandline "GetTriggerCount"
echo   ^> GetTriggerIndex
activeapicommandline "GetTriggerIndex"
echo   ^> GetTriggerCursor
activeapicommandline "GetTriggerCursor"
echo   ^> SetTriggerCursor 0.5
activeapicommandline "SetTriggerCursor 0.5"
echo   ^> FirstTrigger
activeapicommandline "FirstTrigger"
echo   ^> NextTrigger
activeapicommandline "NextTrigger"
echo   ^> PrevTrigger
activeapicommandline "PrevTrigger"
echo   ^> LastTrigger
activeapicommandline "LastTrigger"
echo   ^> GoToTrigger 1
activeapicommandline "GoToTrigger 1"
echo   ^> CenterTrigger
activeapicommandline "CenterTrigger"
echo   ^> ReprocessTriggers
activeapicommandline "ReprocessTriggers"


echo.
echo --- 19. APPLY-CHANGES PIPELINE (post-capture, stopped) ----
echo Make a pending change, confirm not processing, then ApplyChanges.

echo Change a label to create a pending change:
echo   ^> AnalogCH1Label CH1 Voltage Reading
activeapicommandline "AnalogCH1Label CH1 Voltage Reading"

echo   ^> IsProcessing
activeapicommandline "IsProcessing"
echo   ^> ApplyChanges   (OK, or ERROR NOPENDING if nothing changed)
activeapicommandline "ApplyChanges"
call :WAITPROC
echo   ^> CancelProcessing   (NotRunning when no pipeline is active)
activeapicommandline "CancelProcessing"


echo.
echo --- 20. FILE OPERATIONS (post-capture, data present) ------
echo Cursors X1=0.5 X2=2.0 set above bound the between-cursors saves.

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

echo OpenConfiguration (reload what we just saved):
echo   ^> OpenConfiguration %OUTDIR%\test_config.ini
activeapicommandline "OpenConfiguration %OUTDIR%\test_config.ini"

echo OpenCapture (reload what we just saved):
echo   ^> OpenCapture %OUTDIR%\test.active
activeapicommandline "OpenCapture %OUTDIR%\test.active"


echo.
echo ============================================================
echo  All commands sent.  Output files are in: %OUTDIR%
echo  The only command NOT yet sent is Exit (it closes the app).
echo ============================================================
echo.

pause

echo.
echo Sending Exit - this will CLOSE the ACTIVE-PRO application.
echo Press Ctrl+C now to skip, or any key to send Exit.
pause
echo   ^> Exit
activeapicommandline "Exit"

goto :EOF


REM ------------------------------------------------------------
REM  WAITPROC - block until the post-capture pipeline is idle.
REM  IsProcessing returns YES/NO (upper-cased by the server).
REM ------------------------------------------------------------
:WAITPROC
activeapicommandline "IsProcessing" > "%OUTDIR%\_proc.txt"
type "%OUTDIR%\_proc.txt"
findstr /I "YES" "%OUTDIR%\_proc.txt" >nul && (
   TIMEOUT /T 1 /NOBREAK >nul
   goto WAITPROC
)
goto :EOF
