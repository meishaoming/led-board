
```
JLinkExe -SelectEmuBySN 030000299 -If SWD -Device STM32F030C8 -AutoConnect 1 -RTTTelnetPort 19021 -Speed 10000

JLinkRTTClient -RTTTelnetPort 19021
```


