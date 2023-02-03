# LR2GAS_pub

A re-written version of LR2GAS based on [mat's ](https://github.com/MatVeiQaaa/LR2GAS) GAS plugin

## Functionalities

 - Allows switching between P-Attack, Hazard, Hard, Groove and Easy gauges during normal play. 
   Gauges higher than the one you chosen will not be switched to.
   Disabled when using Battle or G-Battle, during replay or during course mode.
 - Records gauge shifts in replay files.
 - Allows switching between gauge graphs on result screen by pressing the select key.
 - Restores gauge at enter when returning to song select if the in-game gauge has changed.

## Important

This plugin works through [injection](https://en.wikipedia.org/wiki/DLL_injection), which may cause undesired side effects not limited to memory corruption or system vulnerabilities. Use it at your own risk!

lr2.cpp and setupjmp.cpp are intentionally omitted in this repo. 

Please report crashes/bugs found while using. Thanks.