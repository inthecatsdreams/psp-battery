#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <psppower.h>

PSP_MODULE_INFO("psp-info", 0, 1, 0);
 
int exit_callback(int arg1, int arg2, void* common){
	sceKernelExitGame();
	return 0;
}
 
int CallbackThread(SceSize args, void* argp) {
	int cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
	sceKernelRegisterExitCallback(cbid);
	sceKernelSleepThreadCB();
 
	return 0;
}
 
int SetupCallbacks(void) {
	int thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
	if (thid >= 0) {
		sceKernelStartThread(thid, 0, 0);
	}
	return thid;
}
int main() { 
    
    int battery_temp, battery_voltage, battery_charging;
    SetupCallbacks();
    pspDebugScreenInit();
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
    pspDebugScreenClear();
    struct SceCtrlData padData;
    if (scePowerIsBatteryExist() == 1){
        battery_temp = scePowerGetBatteryTemp();
        battery_voltage = scePowerGetBatteryVolt();
        battery_charging = scePowerIsBatteryCharging();
        pspDebugScreenPrintf("Battery temperature: %d C\n", battery_temp);
        pspDebugScreenPrintf("Battery voltage: %d v\n", battery_voltage);
        if (battery_charging == 1)
            pspDebugScreenPrintf("Battery is charging\n");
        else
            pspDebugScreenPrintf("Battery is not charging\n");
        
    }
    else {
        pspDebugScreenPrintf("No battery was detected\n");
    }
    
    pspDebugScreenPrintf("Press START to refresh values.\n");

    

    while(1){
        sceCtrlReadBufferPositive(&padData,1);
 
        if(padData.Buttons & PSP_CTRL_START){
            pspDebugScreenPrintf("Refreshing values.");
            main();
        }

    }

}
