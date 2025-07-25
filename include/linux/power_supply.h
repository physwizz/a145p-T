/*
 *  Universal power supply monitor class
 *
 *  Copyright © 2007  Anton Vorontsov <cbou@mail.ru>
 *  Copyright © 2004  Szabolcs Gyurko
 *  Copyright © 2003  Ian Molton <spyro@f2s.com>
 *
 *  Modified: 2004, Oct     Szabolcs Gyurko
 *
 *  You may use this code as per GPL version 2
 */

#ifndef __LINUX_POWER_SUPPLY_H__
#define __LINUX_POWER_SUPPLY_H__

#include <linux/device.h>
#include <linux/workqueue.h>
#include <linux/leds.h>
#include <linux/spinlock.h>
#include <linux/notifier.h>
#include <linux/types.h>

/*
 * All voltages, currents, charges, energies, time and temperatures in uV,
 * µA, µAh, µWh, seconds and tenths of degree Celsius unless otherwise
 * stated. It's driver's job to convert its raw values to units in which
 * this class operates.
 */

/*
 * For systems where the charger determines the maximum battery capacity
 * the min and max fields should be used to present these values to user
 * space. Unused/unknown fields will not appear in sysfs.
 */

enum {
	POWER_SUPPLY_STATUS_UNKNOWN = 0,
	POWER_SUPPLY_STATUS_CHARGING,
	POWER_SUPPLY_STATUS_DISCHARGING,
	POWER_SUPPLY_STATUS_NOT_CHARGING,
	POWER_SUPPLY_STATUS_FULL,
	POWER_SUPPLY_STATUS_CMD_DISCHARGING,
};

/*P231128-06029 */
/* HS04_U/HS14_U/TabA7 Lite U for P231128-06029 by liufurong at 20231204 start */
enum sec_battery_slate_mode {
	SEC_SLATE_OFF = 0,
	SEC_SLATE_MODE,
	SEC_SMART_SWITCH_SLATE,
	SEC_SMART_SWITCH_SRC,
};
/* HS04_U/HS14_U/TabA7 Lite U for P231128-06029 by liufurong at 20231204 end */

/*HS03s for SR-AL5625-01-278 by wenyaqi at 20210427 start*/
#ifndef HQ_FACTORY_BUILD	//ss version
enum {
	POWER_SUPPLY_CHARGE_TYPE_UNKNOWN = 0,
	POWER_SUPPLY_CHARGE_TYPE_NONE,
	POWER_SUPPLY_CHARGE_TYPE_TRICKLE,
	POWER_SUPPLY_CHARGE_TYPE_FAST,
	POWER_SUPPLY_CHARGE_TYPE_TAPER,
	POWER_SUPPLY_CHARGE_TYPE_SLOW,
};
#else
enum {
	POWER_SUPPLY_CHARGE_TYPE_UNKNOWN = 0,
	POWER_SUPPLY_CHARGE_TYPE_NONE,
	POWER_SUPPLY_CHARGE_TYPE_TRICKLE,
	POWER_SUPPLY_CHARGE_TYPE_FAST,
	POWER_SUPPLY_CHARGE_TYPE_SLOW,
	POWER_SUPPLY_CHARGE_TYPE_TAPER,
};
#endif
/*HS03s for SR-AL5625-01-278 by wenyaqi at 20210427 end*/

enum {
	POWER_SUPPLY_HEALTH_UNKNOWN = 0,
	POWER_SUPPLY_HEALTH_GOOD,
	POWER_SUPPLY_HEALTH_OVERHEAT,
	POWER_SUPPLY_HEALTH_DEAD,
	POWER_SUPPLY_HEALTH_OVERVOLTAGE,
	POWER_SUPPLY_HEALTH_UNSPEC_FAILURE,
	POWER_SUPPLY_HEALTH_COLD,
	POWER_SUPPLY_HEALTH_WATCHDOG_TIMER_EXPIRE,
	POWER_SUPPLY_HEALTH_SAFETY_TIMER_EXPIRE,
	POWER_SUPPLY_HEALTH_OVERCURRENT,
	POWER_SUPPLY_HEALTH_WARM,
	POWER_SUPPLY_HEALTH_COOL,
	POWER_SUPPLY_HEALTH_HOT,
	POWER_SUPPLY_HEALTH_UNDERVOLTAGE,
	POWER_SUPPLY_HEALTH_OVERHEATLIMIT,
	POWER_SUPPLY_HEALTH_MAX,
};

enum {
	POWER_SUPPLY_TECHNOLOGY_UNKNOWN = 0,
	POWER_SUPPLY_TECHNOLOGY_NiMH,
	POWER_SUPPLY_TECHNOLOGY_LION,
	POWER_SUPPLY_TECHNOLOGY_LIPO,
	POWER_SUPPLY_TECHNOLOGY_LiFe,
	POWER_SUPPLY_TECHNOLOGY_NiCd,
	POWER_SUPPLY_TECHNOLOGY_LiMn,
};

enum {
	POWER_SUPPLY_CAPACITY_LEVEL_UNKNOWN = 0,
	POWER_SUPPLY_CAPACITY_LEVEL_CRITICAL,
	POWER_SUPPLY_CAPACITY_LEVEL_LOW,
	POWER_SUPPLY_CAPACITY_LEVEL_NORMAL,
	POWER_SUPPLY_CAPACITY_LEVEL_HIGH,
	POWER_SUPPLY_CAPACITY_LEVEL_FULL,
};

enum {
	POWER_SUPPLY_SCOPE_UNKNOWN = 0,
	POWER_SUPPLY_SCOPE_SYSTEM,
	POWER_SUPPLY_SCOPE_DEVICE,
};

enum {
	POWER_SUPPLY_DP_DM_UNKNOWN = 0,
	POWER_SUPPLY_DP_DM_PREPARE = 1,
	POWER_SUPPLY_DP_DM_UNPREPARE = 2,
	POWER_SUPPLY_DP_DM_CONFIRMED_HVDCP3 = 3,
	POWER_SUPPLY_DP_DM_DP_PULSE = 4,
	POWER_SUPPLY_DP_DM_DM_PULSE = 5,
	POWER_SUPPLY_DP_DM_DP0P6_DMF = 6,
	POWER_SUPPLY_DP_DM_DP0P6_DM3P3 = 7,
	POWER_SUPPLY_DP_DM_DPF_DMF = 8,
	POWER_SUPPLY_DP_DM_DPR_DMR = 9,
	POWER_SUPPLY_DP_DM_HVDCP3_SUPPORTED = 10,
	POWER_SUPPLY_DP_DM_ICL_DOWN = 11,
	POWER_SUPPLY_DP_DM_ICL_UP = 12,
	POWER_SUPPLY_DP_DM_FORCE_5V = 13,
	POWER_SUPPLY_DP_DM_FORCE_9V = 14,
	POWER_SUPPLY_DP_DM_FORCE_12V = 15,
	POWER_SUPPLY_DP_DM_CONFIRMED_HVDCP3P5 = 16,
};

enum {
	POWER_SUPPLY_PL_NONE,
	POWER_SUPPLY_PL_USBIN_USBIN,
	POWER_SUPPLY_PL_USBIN_USBIN_EXT,
	POWER_SUPPLY_PL_USBMID_USBMID,
};

enum {
	POWER_SUPPLY_CHARGER_SEC_NONE = 0,
	POWER_SUPPLY_CHARGER_SEC_CP,
	POWER_SUPPLY_CHARGER_SEC_PL,
	POWER_SUPPLY_CHARGER_SEC_CP_PL,
};

enum {
	POWER_SUPPLY_CP_NONE = 0,
	POWER_SUPPLY_CP_HVDCP3,
	POWER_SUPPLY_CP_HVDCP3P5,
	POWER_SUPPLY_CP_PPS,
	POWER_SUPPLY_CP_WIRELESS,
};

enum {
	POWER_SUPPLY_CONNECTOR_TYPEC,
	POWER_SUPPLY_CONNECTOR_MICRO_USB,
};

enum {
	POWER_SUPPLY_PL_STACKED_BATFET,
	POWER_SUPPLY_PL_NON_STACKED_BATFET,
};

/* Parallel output connection topology */
enum {
	POWER_SUPPLY_PL_OUTPUT_NONE,
	POWER_SUPPLY_PL_OUTPUT_VPH,
	POWER_SUPPLY_PL_OUTPUT_VBAT,
};

enum {
	POWER_SUPPLY_PD_INACTIVE = 0,
	POWER_SUPPLY_PD_ACTIVE,
	POWER_SUPPLY_PD_PPS_ACTIVE,
};

enum {
	POWER_SUPPLY_QC_CTM_DISABLE = BIT(0),
	POWER_SUPPLY_QC_THERMAL_BALANCE_DISABLE = BIT(1),
	POWER_SUPPLY_QC_INOV_THERMAL_DISABLE = BIT(2),
};

enum {
	POWER_SUPPLY_ALIGN_CHECKING = 0,
	POWER_SUPPLY_ALIGN_MOVE,
	POWER_SUPPLY_ALIGN_CENTERED,
	POWER_SUPPLY_ALIGN_ERROR,
};

enum power_supply_property {
	/* Properties of type `int' */
	POWER_SUPPLY_PROP_STATUS = 0,
	POWER_SUPPLY_PROP_CHARGE_TYPE,
	POWER_SUPPLY_PROP_HEALTH,
#if defined(CONFIG_HQ_PROJECT_HS03S)
	/*HS03s for SR-AL5625-01-249 by wenyaqi at 20210425 start*/
	#ifdef CONFIG_AFC_CHARGER
	POWER_SUPPLY_PROP_HV_CHARGER_STATUS,
	POWER_SUPPLY_PROP_AFC_RESULT,
	POWER_SUPPLY_PROP_HV_DISABLE,
	#endif
	/*HS03s for SR-AL5625-01-249 by wenyaqi at 20210425 end*/
	#ifndef HQ_FACTORY_BUILD	//ss version
	/* HS03s code for SR-AL5625-01-260 by shixuanxuan at 20210425 start */
	POWER_SUPPLY_PROP_BATT_PROTECT,
	POWER_SUPPLY_PROP_BATT_PROTECT_ENABLE,
	/* HS03s code for SR-AL5625-01-260 by shixuanxuan at 20210425 end */
	/*HS03s for SR-AL5625-01-293 by wenyaqi at 20210426 start*/
	POWER_SUPPLY_PROP_BATT_CURRENT_UA_NOW,
	POWER_SUPPLY_PROP_BATTERY_CYCLE,
	/*HS03s for SR-AL5625-01-293 by wenyaqi at 20210426 end*/
	/*HS03s for SR-AL5625-01-276 by wenyaqi at 20210426 start*/
	POWER_SUPPLY_PROP_BATT_SLATE_MODE,
	/*HS03s for SR-AL5625-01-276 by wenyaqi at 20210426 end*/
	/*HS03s for SR-AL5625-01-286 by wenyaqi at 20210426 start*/
	POWER_SUPPLY_PROP_BATT_MISC_EVENT,
	/*HS03s for SR-AL5625-01-286 by wenyaqi at 20210426 end*/
	/*HS03s for SR-AL5625-01-282 by wenyaqi at 20210426 start*/
	POWER_SUPPLY_PROP_BATT_CURRENT_EVENT,
	/*HS03s for SR-AL5625-01-282 by wenyaqi at 20210426 end*/
	/*HS03s for SR-AL5625-01-277 by wenyaqi at 20210427 start*/
	POWER_SUPPLY_PROP_STORE_MODE,
	/*HS03s for SR-AL5625-01-277 by wenyaqi at 20210427 end*/
	#endif
	/*HS03s for SR-AL5625-01-272 by wenyaqi at 20210427 start*/
	#ifdef HQ_FACTORY_BUILD //factory version
	POWER_SUPPLY_PROP_BATT_CAP_CONTROL,
	#endif
	/*HS03s for SR-AL5625-01-272 by wenyaqi at 20210427 end*/
	/*HS03s for AL5626TDEV-224 by liuhong at 20220921 start*/
	#ifndef HQ_FACTORY_BUILD
	POWER_SUPPLY_PROP_BATT_FULL_CAPACITY,
	#endif
	/*HS03s for AL5626TDEV-224 by liuhong at 20220921 end*/
#elif defined(CONFIG_HQ_PROJECT_HS04)
	/*HS03s for SR-AL5625-01-249 by wenyaqi at 20210425 start*/
	#ifdef CONFIG_AFC_CHARGER
	POWER_SUPPLY_PROP_HV_CHARGER_STATUS,
	POWER_SUPPLY_PROP_AFC_RESULT,
	POWER_SUPPLY_PROP_HV_DISABLE,
	#endif
	/*HS03s for SR-AL5625-01-249 by wenyaqi at 20210425 end*/
	#ifndef HQ_FACTORY_BUILD	//ss version
	/* HS03s code for SR-AL5625-01-260 by shixuanxuan at 20210425 start */
	POWER_SUPPLY_PROP_BATT_PROTECT,
	POWER_SUPPLY_PROP_BATT_PROTECT_ENABLE,
	/* HS03s code for SR-AL5625-01-260 by shixuanxuan at 20210425 end */
	/*HS03s for SR-AL5625-01-293 by wenyaqi at 20210426 start*/
	POWER_SUPPLY_PROP_BATT_CURRENT_UA_NOW,
	POWER_SUPPLY_PROP_BATTERY_CYCLE,
	/*HS03s for SR-AL5625-01-293 by wenyaqi at 20210426 end*/
	/*HS03s for SR-AL5625-01-276 by wenyaqi at 20210426 start*/
	POWER_SUPPLY_PROP_BATT_SLATE_MODE,
	/*HS03s for SR-AL5625-01-276 by wenyaqi at 20210426 end*/
	/*HS03s for SR-AL5625-01-286 by wenyaqi at 20210426 start*/
	POWER_SUPPLY_PROP_BATT_MISC_EVENT,
	/*HS03s for SR-AL5625-01-286 by wenyaqi at 20210426 end*/
	/*HS03s for SR-AL5625-01-282 by wenyaqi at 20210426 start*/
	POWER_SUPPLY_PROP_BATT_CURRENT_EVENT,
	/*HS03s for SR-AL5625-01-282 by wenyaqi at 20210426 end*/
	/*HS03s for SR-AL5625-01-277 by wenyaqi at 20210427 start*/
	POWER_SUPPLY_PROP_STORE_MODE,
	/*HS03s for SR-AL5625-01-277 by wenyaqi at 20210427 end*/
	#endif
	/*HS03s for SR-AL5625-01-272 by wenyaqi at 20210427 start*/
	#ifdef HQ_FACTORY_BUILD //factory version
	POWER_SUPPLY_PROP_BATT_CAP_CONTROL,
	#endif
	/*HS03s for SR-AL5625-01-272 by wenyaqi at 20210427 end*/
	/* HS04_T for DEAL6398A-1879 by shixuanxuan at 20221012 start */
#ifndef HQ_FACTORY_BUILD
	POWER_SUPPLY_PROP_BATT_FULL_CAPACITY,
#endif
	POWER_SUPPLY_PROP_SHIPMODE,
	POWER_SUPPLY_PROP_SHIPMODE_REG,
	/* HS04_T for DEAL6398A-1879 by shixuanxuan at 20221012 end*/
#elif defined(CONFIG_HQ_PROJECT_OT8)
	/*TabA7 Lite  code for SR-AX3565-01-108 by gaoxugang at 20201124 start*/
	#if !defined(HQ_FACTORY_BUILD)
	POWER_SUPPLY_PROP_BATT_SLATE_MODE,
	#endif
	/*TabA7 Lite  code for SR-AX3565-01-108 by gaoxugang at 20201124 end*/
	/*TabA7 Lite  code for SR-AX3565-01-109 by gaoxugang at 20201124 start*/
	#if !defined(HQ_FACTORY_BUILD)
	POWER_SUPPLY_PROP_STORE_MODE,
	#endif
	/*TabA7 Lite  code for SR-AX3565-01-109 by gaoxugang at 20201124 end*/
	/*TabA7 Lite  code for SR-AX3565-01-110 by gaoxugang at 20201124 start*/
	#if !defined(HQ_FACTORY_BUILD)
	POWER_SUPPLY_PROP_BATT_MISC_EVENT,
	#endif
	/*TabA7 Lite  code for SR-AX3565-01-110 by gaoxugang at 20201124 end*/
	/* AX3565 for SR-AX3565-01-737 add battery protect function by shixuanxuan at 2021/01/13 start */
	#if !defined(HQ_FACTORY_BUILD)
	POWER_SUPPLY_PROP_BATT_PROTECT,
	POWER_SUPPLY_PROP_BATT_PROTECT_ENABLE,
	#endif
	/* AX3565 for SR-AX3565-01-737 add battery protect function by shixuanxuan at 2021/01/13 end */
	/*TabA7 Lite  code for SR-AX3565-01-113 by gaoxugang at 20201124 start*/
	#if !defined(HQ_FACTORY_BUILD)
	POWER_SUPPLY_PROP_BATT_CURRENT_EVENT,
	#endif
	/*TabA7 Lite  code for SR-AX3565-01-113 by gaoxugang at 20201124 end*/
	/*TabA7 Lite code for OT8-106 add afc charger driver by wenyaqi at 20201210 start*/
	#ifdef CONFIG_AFC_CHARGER
	POWER_SUPPLY_PROP_HV_CHARGER_STATUS,
	POWER_SUPPLY_PROP_AFC_RESULT,
	POWER_SUPPLY_PROP_HV_DISABLE,
	#endif
	/*TabA7 Lite code for OT8-106 add afc charger driver by wenyaqi at 20201210 end*/
	/*TabA7 Lite code for SR-AX3565-01-124 Import battery aging by wenyaqi at 20201221 start*/
	#ifndef HQ_FACTORY_BUILD	//ss version
	POWER_SUPPLY_PROP_BATT_CURRENT_UA_NOW,
	POWER_SUPPLY_PROP_BATTERY_CYCLE,
	#endif
	/*TabA7 Lite code for SR-AX3565-01-124 Import battery aging by wenyaqi at 20201221 end*/
	/*TabA7 Lite code for OT8-739 discharging over 80 by wenyaqi at 20210104 start*/
	#ifdef HQ_FACTORY_BUILD //factory version
	POWER_SUPPLY_PROP_BATT_CAP_CONTROL,
	#endif
	/*TabA7 Lite code for OT8-739 discharging over 80 by wenyaqi at 20210104 end*/
        #ifndef HQ_FACTORY_BUILD
	POWER_SUPPLY_PROP_BATT_FULL_CAPACITY,
	/* Tab A7 lite_U code for AX3565AU-313 by shanxinkai at 20240120 start */
	POWER_SUPPLY_PROP_BATT_SOC_RECHG,
	/* Tab A7 lite_U code for AX3565AU-313 by shanxinkai at 20240120 end */
        #endif
#elif defined(CONFIG_HQ_PROJECT_O22)
	/* hs14 code for  SR-AL6528A-01-259 by zhouyuhang at 2022/09/15 start*/
	POWER_SUPPLY_PROP_SHIPMODE,
	POWER_SUPPLY_PROP_SHIPMODE_REG,
	/* hs14 code for  SR-AL6528A-01-259 by zhouyuhang at 2022/09/15 end*/
	/* hs14 code for  SR-AL6528A-01-339 by shanxinkai at 20220930 start*/
	POWER_SUPPLY_PROP_CHARGE_STATUS,
	/* hs14 code for  SR-AL6528A-01-339 by shanxinkai at 20220930 end*/
	/* hs14 code for SR-AL6528A-445 by shanxinkai at 2022/10/28 start */
	POWER_SUPPLY_PROP_DUMP_CHARGER_IC,
	/* hs14 code for SR-AL6528A-445 by shanxinkai at 2022/10/28 end */
/* hs14 code for SR-AL6528A-01-321 by gaozhengwei at 2022/09/22 start */
#ifdef CONFIG_AFC_CHARGER
	POWER_SUPPLY_PROP_HV_CHARGER_STATUS,
	POWER_SUPPLY_PROP_AFC_RESULT,
	POWER_SUPPLY_PROP_HV_DISABLE,
#endif
/* hs14 code for SR-AL6528A-01-321 by gaozhengwei at 2022/09/22 end */
#ifndef HQ_FACTORY_BUILD
	/* hs14 code for SR-AL6528A-01-338 | SR-AL6528A-01-337 by chengyuanhang at 2022/10/08 start */
	POWER_SUPPLY_PROP_BATTERY_CYCLE,
	/* hs14_u code for AL6528AU-247 by liufurong at 20240123 start */
	POWER_SUPPLY_PROP_BATTERY_CYCLE_DEBUG,
	/* hs14_u code for AL6528AU-247 by liufurong at 20240123 end */
	POWER_SUPPLY_PROP_BATT_CURRENT_UA_NOW,
	/* hs14 code for SR-AL6528A-01-338 | SR-AL6528A-01-337 by chengyuanhang at 2022/10/08 end */
	/* hs14 code for SR-AL6528A-01-346 by zhouyuhang at 2022/10/11 start*/
	POWER_SUPPLY_PROP_BATT_CURRENT_EVENT,
	/* hs14 code for SR-AL6528A-01-346 by zhouyuhang at 2022/10/11 end*/
	/* hs14 code for SR-AL6528A-01-261 | SR-AL6528A-01-343 by chengyuanhang at 2022/10/11 start */
	POWER_SUPPLY_PROP_BATT_FULL_CAPICITY,
	/* hs14_u code for AL6528AU-252 by liufurong at 2024/01/11 start */
	POWER_SUPPLY_PROP_BATT_SOC_RECHG,
	/* hs14_u code for AL6528AU-252 by liufurong at 2024/01/11 end*/
	POWER_SUPPLY_PROP_BATT_MISC_EVENT,
	/* hs14 code for SR-AL6528A-01-261 | SR-AL6528A-01-343 by chengyuanhang at 2022/10/11 end */
	/* hs14 code for SR-AL6528A-01-242 by shanxinkai at 2022/10/12 start */
	POWER_SUPPLY_PROP_BATT_SLATE_MODE,
	/* hs14 code for SR-AL6528A-01-242 by shanxinkai at 2022/10/12 end */
	/* hs14 code for SR-AL6528A-01-244 by shanxinkai at 2022/11/04 start */
	POWER_SUPPLY_PROP_STORE_MODE,
	/* hs14 code for SR-AL6528A-01-244 by shanxinkai at 2022/11/04 end */
	/* hs14 code for AL6528A-1055 by qiaodan at 2023/01/18 start */
	POWER_SUPPLY_PROP_BATT_TEMP,
	POWER_SUPPLY_PROP_BATT_DISCHARGE_LEVEL,
	/* hs14 code for AL6528A-1055 by qiaodan at 2023/01/18 end */
#endif
/* hs14 code for SR-AL6528A-01-244 by shanxinkai at 2022/11/04 start */
#ifdef HQ_FACTORY_BUILD
	POWER_SUPPLY_PROP_BATT_CAP_CONTROL,
#endif
/* hs14 code for SR-AL6528A-01-244 by shanxinkai at 2022/11/04 end */
#elif defined(CONFIG_HQ_PROJECT_A06)
	/* hs14 code for  SR-AL6528A-01-259 by zhouyuhang at 2022/09/15 start*/
	POWER_SUPPLY_PROP_SHIPMODE,
	POWER_SUPPLY_PROP_SHIPMODE_REG,
	/* hs14 code for  SR-AL6528A-01-259 by zhouyuhang at 2022/09/15 end*/
	/* hs14 code for  SR-AL6528A-01-339 by shanxinkai at 20220930 start*/
	POWER_SUPPLY_PROP_CHARGE_STATUS,
	/* hs14 code for  SR-AL6528A-01-339 by shanxinkai at 20220930 end*/
	/* hs14 code for SR-AL6528A-445 by shanxinkai at 2022/10/28 start */
	POWER_SUPPLY_PROP_DUMP_CHARGER_IC,
	/* hs14 code for SR-AL6528A-445 by shanxinkai at 2022/10/28 end */
/* hs14 code for SR-AL6528A-01-321 by gaozhengwei at 2022/09/22 start */
#ifdef CONFIG_AFC_CHARGER
	POWER_SUPPLY_PROP_HV_CHARGER_STATUS,
	POWER_SUPPLY_PROP_AFC_RESULT,
	POWER_SUPPLY_PROP_HV_DISABLE,
#endif
/* hs14 code for SR-AL6528A-01-321 by gaozhengwei at 2022/09/22 end */
#ifndef HQ_FACTORY_BUILD
	/* hs14 code for SR-AL6528A-01-338 | SR-AL6528A-01-337 by chengyuanhang at 2022/10/08 start */
	POWER_SUPPLY_PROP_BATTERY_CYCLE,
	POWER_SUPPLY_PROP_BATT_CURRENT_UA_NOW,
	/* hs14 code for SR-AL6528A-01-338 | SR-AL6528A-01-337 by chengyuanhang at 2022/10/08 end */
	/* hs14 code for SR-AL6528A-01-346 by zhouyuhang at 2022/10/11 start*/
	POWER_SUPPLY_PROP_BATT_CURRENT_EVENT,
	/* hs14 code for SR-AL6528A-01-346 by zhouyuhang at 2022/10/11 end*/
	/* hs14 code for SR-AL6528A-01-261 | SR-AL6528A-01-343 by chengyuanhang at 2022/10/11 start */
	POWER_SUPPLY_PROP_BATT_FULL_CAPICITY,
	POWER_SUPPLY_PROP_BATT_MISC_EVENT,
	/* hs14 code for SR-AL6528A-01-261 | SR-AL6528A-01-343 by chengyuanhang at 2022/10/11 end */
	/* hs14 code for SR-AL6528A-01-242 by shanxinkai at 2022/10/12 start */
	POWER_SUPPLY_PROP_BATT_SLATE_MODE,
	/* hs14 code for SR-AL6528A-01-242 by shanxinkai at 2022/10/12 end */
	/* hs14 code for SR-AL6528A-01-244 by shanxinkai at 2022/11/04 start */
	POWER_SUPPLY_PROP_STORE_MODE,
	/* hs14 code for SR-AL6528A-01-244 by shanxinkai at 2022/11/04 end */
	/* hs14 code for AL6528A-1055 by qiaodan at 2023/01/18 start */
	POWER_SUPPLY_PROP_BATT_TEMP,
	POWER_SUPPLY_PROP_BATT_DISCHARGE_LEVEL,
	/* hs14 code for AL6528A-1055 by qiaodan at 2023/01/18 end */
#endif
/* hs14 code for SR-AL6528A-01-244 by shanxinkai at 2022/11/04 start */
#ifdef HQ_FACTORY_BUILD
	POWER_SUPPLY_PROP_BATT_CAP_CONTROL,
#endif
/* hs14 code for SR-AL6528A-01-244 by shanxinkai at 2022/11/04 end */
#else
// no add new power_supply
#endif
	POWER_SUPPLY_PROP_PRESENT,
	POWER_SUPPLY_PROP_ONLINE,
	POWER_SUPPLY_PROP_AUTHENTIC,
	POWER_SUPPLY_PROP_TECHNOLOGY,
	POWER_SUPPLY_PROP_CYCLE_COUNT,
	POWER_SUPPLY_PROP_VOLTAGE_MAX,
	POWER_SUPPLY_PROP_VOLTAGE_MIN,
	POWER_SUPPLY_PROP_VOLTAGE_MAX_DESIGN,
	POWER_SUPPLY_PROP_VOLTAGE_MIN_DESIGN,
	POWER_SUPPLY_PROP_VOLTAGE_NOW,
	POWER_SUPPLY_PROP_VOLTAGE_AVG,
	POWER_SUPPLY_PROP_VOLTAGE_OCV,
	POWER_SUPPLY_PROP_VOLTAGE_BOOT,
	POWER_SUPPLY_PROP_CURRENT_MAX,
	POWER_SUPPLY_PROP_CURRENT_NOW,
	POWER_SUPPLY_PROP_CURRENT_AVG,
	POWER_SUPPLY_PROP_CURRENT_BOOT,
	POWER_SUPPLY_PROP_POWER_NOW,
	POWER_SUPPLY_PROP_POWER_AVG,
	POWER_SUPPLY_PROP_CHARGE_FULL_DESIGN,
	POWER_SUPPLY_PROP_CHARGE_EMPTY_DESIGN,
	POWER_SUPPLY_PROP_CHARGE_FULL,
	POWER_SUPPLY_PROP_CHARGE_EMPTY,
	POWER_SUPPLY_PROP_CHARGE_NOW,
	POWER_SUPPLY_PROP_CHARGE_AVG,
	POWER_SUPPLY_PROP_CHARGE_COUNTER,
	POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT,
	POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT_MAX,
	POWER_SUPPLY_PROP_CONSTANT_CHARGE_VOLTAGE,
	POWER_SUPPLY_PROP_CONSTANT_CHARGE_VOLTAGE_MAX,
	POWER_SUPPLY_PROP_CHARGE_CONTROL_LIMIT,
	POWER_SUPPLY_PROP_CHARGE_CONTROL_LIMIT_MAX,
	POWER_SUPPLY_PROP_INPUT_CURRENT_LIMIT,
	POWER_SUPPLY_PROP_ENERGY_FULL_DESIGN,
	POWER_SUPPLY_PROP_ENERGY_EMPTY_DESIGN,
	POWER_SUPPLY_PROP_ENERGY_FULL,
	POWER_SUPPLY_PROP_ENERGY_EMPTY,
	POWER_SUPPLY_PROP_ENERGY_NOW,
	POWER_SUPPLY_PROP_ENERGY_AVG,
	POWER_SUPPLY_PROP_CAPACITY, /* in percents! */
	POWER_SUPPLY_PROP_CAPACITY_ALERT_MIN, /* in percents! */
	POWER_SUPPLY_PROP_CAPACITY_ALERT_MAX, /* in percents! */
	POWER_SUPPLY_PROP_CAPACITY_LEVEL,
	POWER_SUPPLY_PROP_TEMP,
	POWER_SUPPLY_PROP_TEMP_MAX,
	POWER_SUPPLY_PROP_TEMP_MIN,
	POWER_SUPPLY_PROP_TEMP_ALERT_MIN,
	POWER_SUPPLY_PROP_TEMP_ALERT_MAX,
	POWER_SUPPLY_PROP_TEMP_AMBIENT,
	POWER_SUPPLY_PROP_TEMP_AMBIENT_ALERT_MIN,
	POWER_SUPPLY_PROP_TEMP_AMBIENT_ALERT_MAX,
	POWER_SUPPLY_PROP_TIME_TO_EMPTY_NOW,
	POWER_SUPPLY_PROP_TIME_TO_EMPTY_AVG,
	POWER_SUPPLY_PROP_TIME_TO_FULL_NOW,
	POWER_SUPPLY_PROP_TIME_TO_FULL_AVG,
	POWER_SUPPLY_PROP_TYPE, /* use power_supply.type instead */
	POWER_SUPPLY_PROP_USB_TYPE,
	POWER_SUPPLY_PROP_SCOPE,
	POWER_SUPPLY_PROP_PRECHARGE_CURRENT,
	POWER_SUPPLY_PROP_CHARGE_TERM_CURRENT,
	POWER_SUPPLY_PROP_CALIBRATE,
	/* Local extensions */
	POWER_SUPPLY_PROP_USB_HC,
	POWER_SUPPLY_PROP_USB_OTG,
	POWER_SUPPLY_PROP_CHARGE_ENABLED,
	POWER_SUPPLY_PROP_SET_SHIP_MODE,
	POWER_SUPPLY_PROP_REAL_TYPE,
	POWER_SUPPLY_PROP_CHARGE_NOW_RAW,
	POWER_SUPPLY_PROP_CHARGE_NOW_ERROR,
	POWER_SUPPLY_PROP_CAPACITY_RAW,
	POWER_SUPPLY_PROP_BATTERY_CHARGING_ENABLED,
	POWER_SUPPLY_PROP_CHARGING_ENABLED,
	POWER_SUPPLY_PROP_STEP_CHARGING_ENABLED,
	POWER_SUPPLY_PROP_STEP_CHARGING_STEP,
	POWER_SUPPLY_PROP_PIN_ENABLED,
	POWER_SUPPLY_PROP_INPUT_SUSPEND,
	POWER_SUPPLY_PROP_INPUT_VOLTAGE_REGULATION,
	POWER_SUPPLY_PROP_INPUT_CURRENT_MAX,
	POWER_SUPPLY_PROP_INPUT_CURRENT_TRIM,
	POWER_SUPPLY_PROP_INPUT_CURRENT_SETTLED,
	POWER_SUPPLY_PROP_INPUT_VOLTAGE_SETTLED,
	POWER_SUPPLY_PROP_VCHG_LOOP_DBC_BYPASS,
	POWER_SUPPLY_PROP_CHARGE_COUNTER_SHADOW,
	POWER_SUPPLY_PROP_HI_POWER,
	POWER_SUPPLY_PROP_LOW_POWER,
	POWER_SUPPLY_PROP_COOL_TEMP,
	POWER_SUPPLY_PROP_WARM_TEMP,
	POWER_SUPPLY_PROP_COLD_TEMP,
	POWER_SUPPLY_PROP_HOT_TEMP,
	POWER_SUPPLY_PROP_SYSTEM_TEMP_LEVEL,
	POWER_SUPPLY_PROP_RESISTANCE,
	POWER_SUPPLY_PROP_RESISTANCE_CAPACITIVE,
	POWER_SUPPLY_PROP_RESISTANCE_ID, /* in Ohms */
	POWER_SUPPLY_PROP_RESISTANCE_NOW,
	POWER_SUPPLY_PROP_FLASH_CURRENT_MAX,
	POWER_SUPPLY_PROP_UPDATE_NOW,
	POWER_SUPPLY_PROP_ESR_COUNT,
	POWER_SUPPLY_PROP_BUCK_FREQ,
	POWER_SUPPLY_PROP_BOOST_CURRENT,
	POWER_SUPPLY_PROP_SAFETY_TIMER_ENABLE,
	POWER_SUPPLY_PROP_CHARGE_DONE,
	POWER_SUPPLY_PROP_FLASH_ACTIVE,
	POWER_SUPPLY_PROP_FLASH_TRIGGER,
	POWER_SUPPLY_PROP_FORCE_TLIM,
	POWER_SUPPLY_PROP_DP_DM,
	POWER_SUPPLY_PROP_INPUT_CURRENT_LIMITED,
	POWER_SUPPLY_PROP_INPUT_CURRENT_NOW,
	POWER_SUPPLY_PROP_CHARGE_QNOVO_ENABLE,
	POWER_SUPPLY_PROP_CURRENT_QNOVO,
	POWER_SUPPLY_PROP_VOLTAGE_QNOVO,
	POWER_SUPPLY_PROP_RERUN_AICL,
	POWER_SUPPLY_PROP_CYCLE_COUNT_ID,
	POWER_SUPPLY_PROP_SAFETY_TIMER_EXPIRED,
	POWER_SUPPLY_PROP_RESTRICTED_CHARGING,
	POWER_SUPPLY_PROP_CURRENT_CAPABILITY,
	POWER_SUPPLY_PROP_TYPEC_MODE,
	POWER_SUPPLY_PROP_TYPEC_CC_ORIENTATION, /* 0: N/C, 1: CC1, 2: CC2 */
	POWER_SUPPLY_PROP_TYPEC_POWER_ROLE,
	POWER_SUPPLY_PROP_TYPEC_SRC_RP,
	POWER_SUPPLY_PROP_PD_ALLOWED,
	POWER_SUPPLY_PROP_PD_ACTIVE,
	POWER_SUPPLY_PROP_PD_IN_HARD_RESET,
	POWER_SUPPLY_PROP_PD_CURRENT_MAX,
	POWER_SUPPLY_PROP_PD_USB_SUSPEND_SUPPORTED,
	POWER_SUPPLY_PROP_CHARGER_TEMP,
	POWER_SUPPLY_PROP_CHARGER_TEMP_MAX,
	POWER_SUPPLY_PROP_PARALLEL_DISABLE,
	POWER_SUPPLY_PROP_PE_START,
	POWER_SUPPLY_PROP_SOC_REPORTING_READY,
	POWER_SUPPLY_PROP_DEBUG_BATTERY,
	POWER_SUPPLY_PROP_FCC_DELTA,
	POWER_SUPPLY_PROP_ICL_REDUCTION,
	POWER_SUPPLY_PROP_PARALLEL_MODE,
	POWER_SUPPLY_PROP_DIE_HEALTH,
	POWER_SUPPLY_PROP_CONNECTOR_HEALTH,
	POWER_SUPPLY_PROP_CTM_CURRENT_MAX,
	POWER_SUPPLY_PROP_HW_CURRENT_MAX,
	POWER_SUPPLY_PROP_PR_SWAP,
	POWER_SUPPLY_PROP_CC_STEP,
	POWER_SUPPLY_PROP_CC_STEP_SEL,
	POWER_SUPPLY_PROP_SW_JEITA_ENABLED,
	POWER_SUPPLY_PROP_PD_VOLTAGE_MAX,
	POWER_SUPPLY_PROP_PD_VOLTAGE_MIN,
	POWER_SUPPLY_PROP_SDP_CURRENT_MAX,
	POWER_SUPPLY_PROP_CONNECTOR_TYPE,
	POWER_SUPPLY_PROP_PARALLEL_BATFET_MODE,
	POWER_SUPPLY_PROP_PARALLEL_FCC_MAX,
	POWER_SUPPLY_PROP_MIN_ICL,
	POWER_SUPPLY_PROP_MOISTURE_DETECTED,
	POWER_SUPPLY_PROP_BATT_PROFILE_VERSION,
	POWER_SUPPLY_PROP_BATT_FULL_CURRENT,
	POWER_SUPPLY_PROP_RECHARGE_SOC,
	POWER_SUPPLY_PROP_HVDCP_OPTI_ALLOWED,
	POWER_SUPPLY_PROP_SMB_EN_MODE,
	POWER_SUPPLY_PROP_SMB_EN_REASON,
	POWER_SUPPLY_PROP_ESR_ACTUAL,
	POWER_SUPPLY_PROP_ESR_NOMINAL,
	POWER_SUPPLY_PROP_SOH,
	POWER_SUPPLY_PROP_CLEAR_SOH,
	POWER_SUPPLY_PROP_FORCE_RECHARGE,
	POWER_SUPPLY_PROP_FCC_STEPPER_ENABLE,
	POWER_SUPPLY_PROP_TOGGLE_STAT,
	POWER_SUPPLY_PROP_MAIN_FCC_MAX,
	POWER_SUPPLY_PROP_FG_RESET,
	POWER_SUPPLY_PROP_QC_OPTI_DISABLE,
	POWER_SUPPLY_PROP_CC_SOC,
	POWER_SUPPLY_PROP_BATT_AGE_LEVEL,
	POWER_SUPPLY_PROP_SCALE_MODE_EN,
	POWER_SUPPLY_PROP_VOLTAGE_VPH,
	POWER_SUPPLY_PROP_CHIP_VERSION,
	POWER_SUPPLY_PROP_THERM_ICL_LIMIT,
	POWER_SUPPLY_PROP_DC_RESET,
	POWER_SUPPLY_PROP_VOLTAGE_MAX_LIMIT,
	POWER_SUPPLY_PROP_REAL_CAPACITY,
	POWER_SUPPLY_PROP_FORCE_MAIN_ICL,
	POWER_SUPPLY_PROP_FORCE_MAIN_FCC,
	POWER_SUPPLY_PROP_COMP_CLAMP_LEVEL,
	POWER_SUPPLY_PROP_ADAPTER_CC_MODE,
	POWER_SUPPLY_PROP_SKIN_HEALTH,
	POWER_SUPPLY_PROP_CHARGE_DISABLE,
	POWER_SUPPLY_PROP_ADAPTER_DETAILS,
	POWER_SUPPLY_PROP_DEAD_BATTERY,
	POWER_SUPPLY_PROP_VOLTAGE_FIFO,
	POWER_SUPPLY_PROP_CC_UAH,
	POWER_SUPPLY_PROP_OPERATING_FREQ,
	POWER_SUPPLY_PROP_AICL_DELAY,
	POWER_SUPPLY_PROP_AICL_ICL,
	POWER_SUPPLY_PROP_RTX,
	POWER_SUPPLY_PROP_CUTOFF_SOC,
	POWER_SUPPLY_PROP_SYS_SOC,
	POWER_SUPPLY_PROP_BATT_SOC,
	/* Capacity Estimation */
	POWER_SUPPLY_PROP_BATT_CE_CTRL,
	POWER_SUPPLY_PROP_CHARGE_FULL_ESTIMATE,
	/* Resistance Estimaton */
	POWER_SUPPLY_PROP_RESISTANCE_AVG,
	POWER_SUPPLY_PROP_RES_FILTER_COUNT,
	POWER_SUPPLY_PROP_AICL_DONE,
	POWER_SUPPLY_PROP_VOLTAGE_STEP,
	POWER_SUPPLY_PROP_OTG_FASTROLESWAP,
	POWER_SUPPLY_PROP_APSD_RERUN,
	POWER_SUPPLY_PROP_APSD_TIMEOUT,
	/* Charge pump properties */
	POWER_SUPPLY_PROP_CP_STATUS1,
	POWER_SUPPLY_PROP_CP_STATUS2,
	POWER_SUPPLY_PROP_CP_ENABLE,
	POWER_SUPPLY_PROP_CP_SWITCHER_EN,
	POWER_SUPPLY_PROP_CP_DIE_TEMP,
	POWER_SUPPLY_PROP_CP_ISNS,
	POWER_SUPPLY_PROP_CP_ISNS_SLAVE,
	POWER_SUPPLY_PROP_CP_TOGGLE_SWITCHER,
	POWER_SUPPLY_PROP_CP_IRQ_STATUS,
	POWER_SUPPLY_PROP_CP_ILIM,
	POWER_SUPPLY_PROP_IRQ_STATUS,
	POWER_SUPPLY_PROP_PARALLEL_OUTPUT_MODE,
	POWER_SUPPLY_PROP_ALIGNMENT,
	POWER_SUPPLY_PROP_MOISTURE_DETECTION_ENABLE,
	POWER_SUPPLY_PROP_CC_TOGGLE_ENABLE,
	POWER_SUPPLY_PROP_FG_TYPE,
	POWER_SUPPLY_PROP_CHARGER_STATUS,
	/* Local extensions of type int64_t */
	POWER_SUPPLY_PROP_CHARGE_COUNTER_EXT,
	POWER_SUPPLY_PROP_CHARGE_CHARGER_STATE,
	/* Properties of type `const char *' */
	POWER_SUPPLY_PROP_MODEL_NAME,
	POWER_SUPPLY_PROP_PTMC_ID,
	POWER_SUPPLY_PROP_MANUFACTURER,
	POWER_SUPPLY_PROP_BATTERY_TYPE,
#if defined(CONFIG_HQ_PROJECT_O22)
	/* hs14 code for SR-AL6528A-01-258 by shanxinkai at 2022/09/13 start */
	POWER_SUPPLY_PROP_CHG_INFO,
	POWER_SUPPLY_PROP_TCPC_INFO,
	/* hs14 code for SR-AL6528A-01-258 by shanxinkai at 2022/09/13 end */
	/* hs14 code for AL6528A-1055 by qiaodan at 2023/01/18 start */
#ifndef HQ_FACTORY_BUILD
	POWER_SUPPLY_PROP_BATT_TYPE,
#endif
	/* hs14 code for AL6528A-1055 by qiaodan at 2023/01/18 end */
#elif defined(CONFIG_HQ_PROJECT_A06)
	/* hs14 code for SR-AL6528A-01-258 by shanxinkai at 2022/09/13 start */
	POWER_SUPPLY_PROP_CHG_INFO,
	POWER_SUPPLY_PROP_TCPC_INFO,
	/* hs14 code for SR-AL6528A-01-258 by shanxinkai at 2022/09/13 end */
#ifndef HQ_FACTORY_BUILD
	POWER_SUPPLY_PROP_BATT_TYPE,
#endif
#else
	// no add new power_supply
#endif
	POWER_SUPPLY_PROP_CYCLE_COUNTS,
	/*
	 * Add local extensions for properties with string values between
	 * MODEL_NAME and SERIAL_NUMBER. Don't add below SERIAL_NUMBER.
	 */
	POWER_SUPPLY_PROP_SERIAL_NUMBER,
	POWER_SUPPLY_PROP_FUELGAUGE_RESET,
	POWER_SUPPLY_PROP_USBPD_RESET,
	POWER_SUPPLY_PROP_FACTORY_MODE,
	POWER_SUPPLY_PROP_PD_SUPPORT,
	POWER_SUPPLY_PROP_FILTER_CFG,
	POWER_SUPPLY_PROP_CHARGE_OTG_CONTROL,
	POWER_SUPPLY_PROP_CHARGE_POWERED_OTG_CONTROL,
	POWER_SUPPLY_PROP_CHARGE_TEMP,
	POWER_SUPPLY_PROP_CHARGE_UNO_CONTROL,
	POWER_SUPPLY_PROP_AFC_CHARGER_MODE,
	POWER_SUPPLY_PROP_VCHGIN,
	POWER_SUPPLY_PROP_VWCIN,
	POWER_SUPPLY_PROP_VBYP,
	POWER_SUPPLY_PROP_VSYS,
	POWER_SUPPLY_PROP_VBAT,
	POWER_SUPPLY_PROP_VGPADC,
	POWER_SUPPLY_PROP_VCC1,
	POWER_SUPPLY_PROP_VCC2,
	POWER_SUPPLY_PROP_ICHGIN,
	POWER_SUPPLY_PROP_IWCIN,
	POWER_SUPPLY_PROP_IOTG,
	POWER_SUPPLY_PROP_ITX,
	POWER_SUPPLY_PROP_CO_ENABLE,
	POWER_SUPPLY_PROP_RR_ENABLE,
	POWER_SUPPLY_PROP_PM_FACTORY,
	POWER_SUPPLY_PROP_TDIE,
	POWER_SUPPLY_PROP_FAST_IAVG,
//	POWER_SUPPLY_PROP_USBPD_RESET,
	POWER_SUPPLY_PROP_CURRENT_FULL,
	POWER_SUPPLY_PROP_USBPD_TEST_READ,
	POWER_SUPPLY_PROP_MAX,
	POWER_SUPPLY_EXT_PROP_MAX = POWER_SUPPLY_PROP_MAX + 256,
};

#if !defined(HQ_FACTORY_BUILD) //ss version
enum batt_misc_event_ss {
	//BATT_MISC_EVENT_NONE = 0x000000,					/* Default value, nothing will be happen */
	BATT_MISC_EVENT_UNDEFINED_RANGE_TYPE = 0x00000001,	/* water detection - not support */
	BATT_MISC_EVENT_TIMEOUT_OPEN_TYPE = 0x00000004,		/* DCD timeout */
	BATT_MISC_EVENT_HICCUP_TYPE = 0x00000020,			/* not use, it happens when water is detected in a interface port */
	BATT_MISC_EVENT_FULL_CAPACITY = 0x01000000,
};
#endif
/*HS03s for SR-AL5625-01-286 by wenyaqi at 20210426 end*/
/*HS03s for SR-AL5625-01-282 by wenyaqi at 20210426 start*/
#if !defined(HQ_FACTORY_BUILD) //ss version
enum batt_current_event_ss {
       SEC_BAT_CURRENT_EVENT_NONE = 0x00000,               /* Default value, nothing will be happen */
       SEC_BAT_CURRENT_EVENT_AFC = 0x00001,                /* Do not use this*/
       SEC_BAT_CURRENT_EVENT_CHARGE_DISABLE = 0x00002,     /* This event is for a head mount VR, It is not need */
       SEC_BAT_CURRENT_EVENT_LOW_TEMP_SWELLING = 0x00010,  /* battery temperature is lower than 18 celsius degree */
       SEC_BAT_CURRENT_EVENT_HIGH_TEMP_SWELLING = 0x00020, /* battery temperature is higher than 41 celsius degree */
       SEC_BAT_CURRENT_EVENT_USB_100MA = 0x00040,          /* USB 2.0 device under test is set in unconfigured state, not enumerate */
	   SEC_BAT_CURRENT_EVENT_SLATE = 0x00800,
};
#endif


enum power_supply_type {
	POWER_SUPPLY_TYPE_UNKNOWN = 0,
	POWER_SUPPLY_TYPE_BATTERY,
	POWER_SUPPLY_TYPE_UPS,
	POWER_SUPPLY_TYPE_MAINS,
	POWER_SUPPLY_TYPE_USB,			/* Standard Downstream Port */
	POWER_SUPPLY_TYPE_USB_DCP,		/* Dedicated Charging Port */
	POWER_SUPPLY_TYPE_USB_CDP,		/* Charging Downstream Port */
	POWER_SUPPLY_TYPE_USB_ACA,		/* Accessory Charger Adapters */
	POWER_SUPPLY_TYPE_USB_TYPE_C,		/* Type C Port */
	POWER_SUPPLY_TYPE_USB_PD,		/* Power Delivery Port */
	POWER_SUPPLY_TYPE_USB_PD_DRP,		/* PD Dual Role Port */
	POWER_SUPPLY_TYPE_APPLE_BRICK_ID,	/* Apple Charging Method */
	POWER_SUPPLY_TYPE_USB_HVDCP,		/* High Voltage DCP */
	POWER_SUPPLY_TYPE_USB_HVDCP_3,		/* Efficient High Voltage DCP */
	POWER_SUPPLY_TYPE_USB_HVDCP_3P5,	/* Efficient High Voltage DCP */
	POWER_SUPPLY_TYPE_WIRELESS,		/* Accessory Charger Adapters */
	POWER_SUPPLY_TYPE_USB_FLOAT,		/* Floating charger */
	POWER_SUPPLY_TYPE_BMS,			/* Battery Monitor System */
	POWER_SUPPLY_TYPE_PARALLEL,		/* Parallel Path */
	POWER_SUPPLY_TYPE_MAIN,			/* Main Path */
	POWER_SUPPLY_TYPE_UFP,			/* Type-C UFP */
	POWER_SUPPLY_TYPE_DFP,			/* Type-C DFP */
	POWER_SUPPLY_TYPE_CHARGE_PUMP,		/* Charge Pump */
	/* Added by Customer */
	POWER_SUPPLY_TYPE_MISC,
	POWER_SUPPLY_TYPE_HV_WIRELESS,
	POWER_SUPPLY_TYPE_PMA_WIRELESS,
	POWER_SUPPLY_TYPE_CARDOCK,
	POWER_SUPPLY_TYPE_UARTOFF,
	POWER_SUPPLY_TYPE_OTG,
	POWER_SUPPLY_TYPE_LAN_HUB,
	POWER_SUPPLY_TYPE_MHL_500,
	POWER_SUPPLY_TYPE_MHL_900,
	POWER_SUPPLY_TYPE_MHL_1500,
	POWER_SUPPLY_TYPE_MHL_USB,
	POWER_SUPPLY_TYPE_SMART_OTG,
	POWER_SUPPLY_TYPE_SMART_NOTG,
	POWER_SUPPLY_TYPE_POWER_SHARING,
	POWER_SUPPLY_TYPE_HV_MAINS,
	POWER_SUPPLY_TYPE_HV_MAINS_12V,
	POWER_SUPPLY_TYPE_HV_PREPARE_MAINS, 
	POWER_SUPPLY_TYPE_HV_ERR,
	POWER_SUPPLY_TYPE_MHL_USB_100,
	POWER_SUPPLY_TYPE_MHL_2000,
	POWER_SUPPLY_TYPE_HV_UNKNOWN,
	POWER_SUPPLY_TYPE_MDOCK_TA,
	POWER_SUPPLY_TYPE_HMT_CONNECTED,
	POWER_SUPPLY_TYPE_HMT_CHARGE,
	POWER_SUPPLY_TYPE_WIRELESS_PACK,
	POWER_SUPPLY_TYPE_WIRELESS_PACK_TA,
	POWER_SUPPLY_TYPE_WIRELESS_STAND,
	POWER_SUPPLY_TYPE_WIRELESS_HV_STAND,
	POWER_SUPPLY_TYPE_PDIC,
	POWER_SUPPLY_TYPE_HV_MAINS_CHG_LIMIT,
	POWER_SUPPLY_TYPE_HV_QC20,
	POWER_SUPPLY_TYPE_HV_QC30,
	POWER_SUPPLY_TYPE_POGO,
	POWER_SUPPLY_TYPE_MAX,
};

enum power_supply_usb_type {
	POWER_SUPPLY_USB_TYPE_UNKNOWN = 0,
	POWER_SUPPLY_USB_TYPE_SDP,		/* Standard Downstream Port */
	POWER_SUPPLY_USB_TYPE_DCP,		/* Dedicated Charging Port */
	POWER_SUPPLY_USB_TYPE_CDP,		/* Charging Downstream Port */
	POWER_SUPPLY_USB_TYPE_ACA,		/* Accessory Charger Adapters */
	POWER_SUPPLY_USB_TYPE_C,		/* Type C Port */
	POWER_SUPPLY_USB_TYPE_PD,		/* Power Delivery Port */
	POWER_SUPPLY_USB_TYPE_PD_DRP,		/* PD Dual Role Port */
	POWER_SUPPLY_USB_TYPE_PD_PPS,		/* PD Programmable Power Supply */
	POWER_SUPPLY_USB_TYPE_APPLE_BRICK_ID,	/* Apple Charging Method */
};

/* Indicates USB Type-C CC connection status */
enum power_supply_typec_mode {
	POWER_SUPPLY_TYPEC_NONE,

	/* Acting as source */
	POWER_SUPPLY_TYPEC_SINK,		/* Rd only */
	POWER_SUPPLY_TYPEC_SINK_POWERED_CABLE,	/* Rd/Ra */
	POWER_SUPPLY_TYPEC_SINK_DEBUG_ACCESSORY,/* Rd/Rd */
	POWER_SUPPLY_TYPEC_SINK_AUDIO_ADAPTER,	/* Ra/Ra */
	POWER_SUPPLY_TYPEC_POWERED_CABLE_ONLY,	/* Ra only */

	/* Acting as sink */
	POWER_SUPPLY_TYPEC_SOURCE_DEFAULT,	/* Rp default */
	POWER_SUPPLY_TYPEC_SOURCE_MEDIUM,	/* Rp 1.5A */
	POWER_SUPPLY_TYPEC_SOURCE_HIGH,		/* Rp 3A */
	POWER_SUPPLY_TYPEC_DAM_DEFAULT,		/* Rp-1.5A/Rp-3A */
	POWER_SUPPLY_TYPEC_DAM_MEDIUM,		/* Rp-Default/Rp-1.5A */
	POWER_SUPPLY_TYPEC_DAM_HIGH,		/* Rp-Default/Rp-3A */

	/* Non Compliant */
	POWER_SUPPLY_TYPEC_NON_COMPLIANT,
	POWER_SUPPLY_TYPEC_RP_STD_STD,		/* Rp-Default/Rp-Default */
	POWER_SUPPLY_TYPEC_RP_MED_MED,		/* Rp-1.5A/Rp-1.5A */
	POWER_SUPPLY_TYPEC_RP_HIGH_HIGH,	/* Rp-3A/Rp-3A */
};

enum power_supply_typec_src_rp {
	POWER_SUPPLY_TYPEC_SRC_RP_STD,
	POWER_SUPPLY_TYPEC_SRC_RP_1P5A,
	POWER_SUPPLY_TYPEC_SRC_RP_3A
};

enum power_supply_typec_power_role {
	POWER_SUPPLY_TYPEC_PR_NONE,		/* CC lines in high-Z */
	POWER_SUPPLY_TYPEC_PR_DUAL,
	POWER_SUPPLY_TYPEC_PR_SINK,
	POWER_SUPPLY_TYPEC_PR_SOURCE,
};

enum power_supply_notifier_events {
	PSY_EVENT_PROP_CHANGED,
	PSY_EVENT_PROP_ADDED,
};

union power_supply_propval {
	int intval;
	const char *strval;
	int64_t int64val;
};

struct device_node;
struct power_supply;

/* Run-time specific power supply configuration */
struct power_supply_config {
	struct device_node *of_node;
	struct fwnode_handle *fwnode;

	/* Driver private data */
	void *drv_data;

	char **supplied_to;
	size_t num_supplicants;
};

/* Description of power supply */
struct power_supply_desc {
	const char *name;
	enum power_supply_type type;
	enum power_supply_usb_type *usb_types;
	size_t num_usb_types;
	enum power_supply_property *properties;
	size_t num_properties;

	/*
	 * Functions for drivers implementing power supply class.
	 * These shouldn't be called directly by other drivers for accessing
	 * this power supply. Instead use power_supply_*() functions (for
	 * example power_supply_get_property()).
	 */
	int (*get_property)(struct power_supply *psy,
			    enum power_supply_property psp,
			    union power_supply_propval *val);
	int (*set_property)(struct power_supply *psy,
			    enum power_supply_property psp,
			    const union power_supply_propval *val);
	/*
	 * property_is_writeable() will be called during registration
	 * of power supply. If this happens during device probe then it must
	 * not access internal data of device (because probe did not end).
	 */
	int (*property_is_writeable)(struct power_supply *psy,
				     enum power_supply_property psp);
	void (*external_power_changed)(struct power_supply *psy);
	void (*set_charged)(struct power_supply *psy);

	/*
	 * Set if thermal zone should not be created for this power supply.
	 * For example for virtual supplies forwarding calls to actual
	 * sensors or other supplies.
	 */
	bool no_thermal;
	/* For APM emulation, think legacy userspace. */
	int use_for_apm;
};

struct power_supply {
	const struct power_supply_desc *desc;

	char **supplied_to;
	size_t num_supplicants;

	char **supplied_from;
	size_t num_supplies;
	struct device_node *of_node;

	/* Driver private data */
	void *drv_data;

	/* private */
	struct device dev;
	struct work_struct changed_work;
	struct delayed_work deferred_register_work;
	spinlock_t changed_lock;
	bool changed;
	bool initialized;
	bool removing;
	atomic_t use_cnt;
#ifdef CONFIG_THERMAL
	struct thermal_zone_device *tzd;
	struct thermal_cooling_device *tcd;
#endif

#ifdef CONFIG_LEDS_TRIGGERS
	struct led_trigger *charging_full_trig;
	char *charging_full_trig_name;
	struct led_trigger *charging_trig;
	char *charging_trig_name;
	struct led_trigger *full_trig;
	char *full_trig_name;
	struct led_trigger *online_trig;
	char *online_trig_name;
	struct led_trigger *charging_blink_full_solid_trig;
	char *charging_blink_full_solid_trig_name;
#endif
};

/*
 * This is recommended structure to specify static power supply parameters.
 * Generic one, parametrizable for different power supplies. Power supply
 * class itself does not use it, but that's what implementing most platform
 * drivers, should try reuse for consistency.
 */

struct power_supply_info {
	const char *name;
	int technology;
	int voltage_max_design;
	int voltage_min_design;
	int charge_full_design;
	int charge_empty_design;
	int energy_full_design;
	int energy_empty_design;
	int use_for_apm;
};

/*
 * This is the recommended struct to manage static battery parameters,
 * populated by power_supply_get_battery_info(). Most platform drivers should
 * use these for consistency.
 * Its field names must correspond to elements in enum power_supply_property.
 * The default field value is -EINVAL.
 * Power supply class itself doesn't use this.
 */

struct power_supply_battery_info {
	int energy_full_design_uwh;	    /* microWatt-hours */
	int charge_full_design_uah;	    /* microAmp-hours */
	int voltage_min_design_uv;	    /* microVolts */
	int precharge_current_ua;	    /* microAmps */
	int charge_term_current_ua;	    /* microAmps */
	int constant_charge_current_max_ua; /* microAmps */
	int constant_charge_voltage_max_uv; /* microVolts */
};

extern struct atomic_notifier_head power_supply_notifier;
extern int power_supply_reg_notifier(struct notifier_block *nb);
extern void power_supply_unreg_notifier(struct notifier_block *nb);
extern struct power_supply *power_supply_get_by_name(const char *name);
extern void power_supply_put(struct power_supply *psy);
#ifdef CONFIG_OF
extern struct power_supply *power_supply_get_by_phandle(struct device_node *np,
							const char *property);
extern struct power_supply *devm_power_supply_get_by_phandle(
				    struct device *dev, const char *property);
#else /* !CONFIG_OF */
static inline struct power_supply *
power_supply_get_by_phandle(struct device_node *np, const char *property)
{ return NULL; }
static inline struct power_supply *
devm_power_supply_get_by_phandle(struct device *dev, const char *property)
{ return NULL; }
#endif /* CONFIG_OF */

extern int power_supply_get_battery_info(struct power_supply *psy,
					 struct power_supply_battery_info *info);
extern void power_supply_changed(struct power_supply *psy);
extern int power_supply_am_i_supplied(struct power_supply *psy);
extern int power_supply_set_input_current_limit_from_supplier(
					 struct power_supply *psy);
extern int power_supply_set_battery_charged(struct power_supply *psy);

#ifdef CONFIG_POWER_SUPPLY
extern int power_supply_is_system_supplied(void);
#else
static inline int power_supply_is_system_supplied(void) { return -ENOSYS; }
#endif

extern int power_supply_get_property(struct power_supply *psy,
			    enum power_supply_property psp,
			    union power_supply_propval *val);
extern int power_supply_set_property(struct power_supply *psy,
			    enum power_supply_property psp,
			    const union power_supply_propval *val);
extern int power_supply_property_is_writeable(struct power_supply *psy,
					enum power_supply_property psp);
extern void power_supply_external_power_changed(struct power_supply *psy);

extern struct power_supply *__must_check
power_supply_register(struct device *parent,
				 const struct power_supply_desc *desc,
				 const struct power_supply_config *cfg);
extern struct power_supply *__must_check
power_supply_register_no_ws(struct device *parent,
				 const struct power_supply_desc *desc,
				 const struct power_supply_config *cfg);
extern struct power_supply *__must_check
devm_power_supply_register(struct device *parent,
				 const struct power_supply_desc *desc,
				 const struct power_supply_config *cfg);
extern struct power_supply *__must_check
devm_power_supply_register_no_ws(struct device *parent,
				 const struct power_supply_desc *desc,
				 const struct power_supply_config *cfg);
extern void power_supply_unregister(struct power_supply *psy);
extern int power_supply_powers(struct power_supply *psy, struct device *dev);

#define to_power_supply(device) container_of(device, struct power_supply, dev)

extern void *power_supply_get_drvdata(struct power_supply *psy);
/* For APM emulation, think legacy userspace. */
extern struct class *power_supply_class;

static inline bool power_supply_is_amp_property(enum power_supply_property psp)
{
	switch (psp) {
	case POWER_SUPPLY_PROP_CHARGE_FULL_DESIGN:
	case POWER_SUPPLY_PROP_CHARGE_EMPTY_DESIGN:
	case POWER_SUPPLY_PROP_CHARGE_FULL:
	case POWER_SUPPLY_PROP_CHARGE_EMPTY:
	case POWER_SUPPLY_PROP_CHARGE_NOW:
	case POWER_SUPPLY_PROP_CHARGE_AVG:
	case POWER_SUPPLY_PROP_CHARGE_COUNTER:
	case POWER_SUPPLY_PROP_PRECHARGE_CURRENT:
	case POWER_SUPPLY_PROP_CHARGE_TERM_CURRENT:
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT:
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT_MAX:
	case POWER_SUPPLY_PROP_CURRENT_MAX:
	case POWER_SUPPLY_PROP_CURRENT_NOW:
	case POWER_SUPPLY_PROP_CURRENT_AVG:
	case POWER_SUPPLY_PROP_CURRENT_BOOT:
		return 1;
	default:
		break;
	}

	return 0;
}

static inline bool power_supply_is_watt_property(enum power_supply_property psp)
{
	switch (psp) {
	case POWER_SUPPLY_PROP_ENERGY_FULL_DESIGN:
	case POWER_SUPPLY_PROP_ENERGY_EMPTY_DESIGN:
	case POWER_SUPPLY_PROP_ENERGY_FULL:
	case POWER_SUPPLY_PROP_ENERGY_EMPTY:
	case POWER_SUPPLY_PROP_ENERGY_NOW:
	case POWER_SUPPLY_PROP_ENERGY_AVG:
	case POWER_SUPPLY_PROP_VOLTAGE_MAX:
	case POWER_SUPPLY_PROP_VOLTAGE_MIN:
	case POWER_SUPPLY_PROP_VOLTAGE_MAX_DESIGN:
	case POWER_SUPPLY_PROP_VOLTAGE_MIN_DESIGN:
	case POWER_SUPPLY_PROP_VOLTAGE_NOW:
	case POWER_SUPPLY_PROP_VOLTAGE_AVG:
	case POWER_SUPPLY_PROP_VOLTAGE_OCV:
	case POWER_SUPPLY_PROP_VOLTAGE_BOOT:
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_VOLTAGE:
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_VOLTAGE_MAX:
	case POWER_SUPPLY_PROP_POWER_NOW:
		return 1;
	default:
		break;
	}

	return 0;
}

/* hs14_u code for AL6528AU-252 by liufurong at 2024/01/11 start */
typedef enum batt_protection {
	BASE_MODE,
	OPTION_MODE,
	SLEEP_MODE,
	HIGHSOC_MODE
} BATT_PROTECTION_T;
/* hs14_u code for AL6528AU-252 by liufurong at 2024/01/11 end */

#endif /* __LINUX_POWER_SUPPLY_H__ */
