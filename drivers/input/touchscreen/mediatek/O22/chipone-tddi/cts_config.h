#ifndef CTS_CONFIG_H
#define CTS_CONFIG_H

/** Driver version */
#define CFG_CTS_DRIVER_MAJOR_VERSION        3
#define CFG_CTS_DRIVER_MINOR_VERSION        0
#define CFG_CTS_DRIVER_PATCH_VERSION        9

#define CFG_CTS_DRIVER_VERSION              "v3.0.9"

/*For Goole Security*/
//#define CFG_CTS_FOR_GKI

/** Whether reset pin is used */
#define CFG_CTS_HAS_RESET_PIN

/* #define CONFIG_CTS_I2C_HOST */
#ifndef CONFIG_CTS_I2C_HOST

#ifndef CFG_CTS_HAS_RESET_PIN
#define CFG_CTS_HAS_RESET_PIN
#endif

#define CFG_CTS_SPI_SPEED_KHZ               6000

#endif

//#define CFG_CTS_FORCE_UP
#define CFG_DUMP_INT_DATA

//#define CFG_CTS_PALM_DETECT
#ifdef CFG_CTS_PALM_DETECT
#define CFG_CTS_PALM_EVENT  240
#define CFG_CTS_PALM_ID     0x80
#endif

#define CONFIG_GENERIC_HARDIRQS
//#define CFG_CTS_FW_LOG_REDIRECT

/** Whether force download firmware to chip */
/* #define CFG_CTS_FIRMWARE_FORCE_UPDATE */

/** Use build in firmware or firmware file in fs*/
/* #define CFG_CTS_DRIVER_BUILTIN_FIRMWARE */
#define CFG_CTS_FIRMWARE_IN_FS
#ifdef CFG_CTS_FIRMWARE_IN_FS
/* Add FW custom process. */
/* #define CFG_CTS_FW_UPDATE_SYS */

/* Load config fw bin as default. */
/* #define CFG_CTS_FW_UPDATE_FILE_LOAD  */
#ifdef CFG_CTS_FW_UPDATE_FILE_LOAD
#define CFG_CTS_FW_FILE_NAME_MAX_LEN        128
#define CFG_CTS_FW_FILE_PATH                "/vendor/firmware/"
#define CFG_CTS_FW_FILE_NAME_VENDOR         "chipone"
#endif /*CFG_CTS_FW_UPDATE_FILE_LOAD */

#define CFG_CTS_FIRMWARE_FILENAME_9911           "ICNL9911.bin"
#define CFG_CTS_FIRMWARE_FILENAME_9916           "ICNL9916.bin"
#define CFG_CTS_FIRMWARE_FILENAME_9916C          "ICNL9916C.bin"
#define CFG_CTS_FIRMWARE_FILENAME_9922           "ICNL9922.bin"
// #define CFG_CTS_FIRMWARE_FILENAME_9922C          "ICNL9922C.bin"
#define CFG_CTS_FIRMWARE_FILENAME_9922C          "nl9922c_jz_tm_firmware.bin"

#define CFG_CTS_FIRMWARE_FILEPATH_9911           "/vendor/firmware/ICNL9911.bin"
#define CFG_CTS_FIRMWARE_FILEPATH_9916           "/vendor/firmware/ICNL9916.bin"
#define CFG_CTS_FIRMWARE_FILEPATH_9916C          "/vendor/firmware/ICNL9916C.bin"
#define CFG_CTS_FIRMWARE_FILEPATH_9922           "/vendor/firmware/ICNL9922.bin"
// #define CFG_CTS_FIRMWARE_FILEPATH_9922C          "/vendor/firmware/ICNL9922C.bin"
#define CFG_CTS_FIRMWARE_FILEPATH_9922C          "/vendor/firmware/nl9922c_jz_tm_firmware.bin"
#endif /* CFG_CTS_FIRMWARE_IN_FS */

/* IC type support */
#define CFG_CTS_CHIP_NAME                   "ICNL9911S"

#ifdef CONFIG_PROC_FS
    /* Proc FS for backward compatibility for APK tool com.ICN85xx */
#define CONFIG_CTS_LEGACY_TOOL
#endif /* CONFIG_PROC_FS */

#ifdef CONFIG_SYSFS
    /* Sys FS for gesture report, debug feature etc. */
#define CONFIG_CTS_SYSFS
#endif /* CONFIG_SYSFS */

#define CFG_CTS_MAX_TOUCH_NUM               (10)    // 5

/* Virtual key support */
/* #define CONFIG_CTS_VIRTUALKEY */
#ifdef CONFIG_CTS_VIRTUALKEY
#define CFG_CTS_MAX_VKEY_NUM            (4)
#define CFG_CTS_NUM_VKEY                (3)
#define CFG_CTS_VKEY_KEYCODES           {KEY_BACK, KEY_HOME, KEY_MENU}
#endif /* CONFIG_CTS_VIRTUALKEY */

/* Gesture wakeup */
#define CFG_CTS_GESTURE
#ifdef CFG_CTS_GESTURE
#define GESTURE_UP                          0x11
#define GESTURE_C                           0x12
#define GESTURE_O                           0x13
#define GESTURE_M                           0x14
#define GESTURE_W                           0x15
#define GESTURE_E                           0x16
#define GESTURE_S                           0x17
#define GESTURE_Z                           0x1d
#define GESTURE_V                           0x1e
#define GESTURE_D_TAP                       0x50
#define GESTURE_DOWN                        0x22
#define GESTURE_LEFT                        0x23
#define GESTURE_RIGHT                       0x24
#define GESTURE_TAP                         0x7f

#define CFG_CTS_NUM_GESTURE             (14u)
#define CFG_CTS_GESTURE_REPORT_KEY
/*hs14 code for  SR-AL6528A-01-494 by hehaoran5 at 20220919 start*/
#define CFG_CTS_GESTURE_KEYMAP          \
{                                       \
    { GESTURE_C, KEY_C,},               \
    { GESTURE_W, KEY_W,},               \
    { GESTURE_V, KEY_V,},               \
    { GESTURE_D_TAP, KEY_WAKEUP,},          \
    { GESTURE_Z, KEY_Z,},               \
    { GESTURE_M, KEY_M,},               \
    { GESTURE_O, KEY_O,},               \
    { GESTURE_E, KEY_E,},               \
    { GESTURE_S, KEY_S,},               \
    { GESTURE_UP, KEY_UP,},             \
    { GESTURE_DOWN, KEY_DOWN,},         \
    { GESTURE_LEFT, KEY_LEFT,},         \
    { GESTURE_RIGHT, KEY_RIGHT,},       \
    { GESTURE_TAP, KEY_F2,},            \
}
/*hs14 code for  SR-AL6528A-01-494 by hehaoran5 at 20220919 end*/
#define CFG_CTS_GESTURE_REPORT_TRACE    0
#endif /* CFG_CTS_GESTURE */

/* #define CONFIG_CTS_GLOVE */
/*hs14 code for SR-AL6528A-01-464 by hehaoran5 at 202201010 start*/
//#define CONFIG_CTS_EARJACK_DETECT
/*hs14 code for SR-AL6528A-01-464 by hehaoran5 at 202201010 end*/
#define CONFIG_CTS_CHARGER_DETECT

//#define CONFIG_CTS_TP_PROXIMITY

/* ESD protection */
/* #define CONFIG_CTS_ESD_PROTECTION */
#ifdef CONFIG_CTS_ESD_PROTECTION
#define CFG_CTS_ESD_PROTECTION_CHECK_PERIOD         (2 * HZ)
#define CFG_CTS_ESD_FAILED_CONFIRM_CNT              3
#endif /* CONFIG_CTS_ESD_PROTECTION */

/* Use slot protocol (protocol B), comment it if use protocol A. */
#define CONFIG_CTS_SLOTPROTOCOL

//#define CTS_CONFIG_MKDIR_FOR_CTS_TEST

#ifdef CONFIG_CTS_LEGACY_TOOL
#define CFG_CTS_TOOL_PROC_FILENAME      "cts_tool"
#endif /* CONFIG_CTS_LEGACY_TOOL */

#define CFG_CTS_UPDATE_CRCCHECK
/****************************************************************************
 * Platform configurations
 ****************************************************************************/

#include "cts_plat_qcom_config.h"

#endif /* CTS_CONFIG_H */
