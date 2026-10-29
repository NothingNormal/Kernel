// SPDX-License-Identifier: GPL-2.0-only
/* Host-only regression tests: the driver functions are extracted at run time.
 * These mocks exercise control flow and votes, not actual charging hardware.
 */
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef uint32_t u32;
typedef uint64_t u64;
typedef int64_t ktime_t;
/* struct step_chg_info carries a spinlock for its wakeup pairing; none of
 * the functions extracted for these tests take it. */
typedef int spinlock_t;

#define STEP_CHG_VOTER "STEP_CHG_VOTER"
#define JEITA_VOTER "JEITA_VOTER"
#define DYNAMIC_FV_VOTER "DYNAMIC_FV_VOTER"
#define BATT_PROFILE_VOTER "BATT_PROFILE_VOTER"
#define HIGH_VBAT_VOTER "HIGH_VBAT_VOTER"
#define SIX_PIN_VFLOAT_VOTER "SIX_PIN_VFLOAT_VOTER"
#define pr_err(...) ((void)0)
#define pr_debug(...) ((void)0)
#define VDF_HWVERSION 1
#define JAPAN_HWVERSION 2
#define NORMAL_HWVERSION 3
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define CHECK(cond) do { \
	if (!(cond)) { \
		fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
		exit(1); \
	} \
} while (0)

/* Only the kernel work/notifier objects need host placeholders. */
struct delayed_work { int unused; };
struct notifier_block { int unused; };
#include "qcom_charge_types.h"

struct votable {
	bool active;
	int value;
	int calls;
	const char *client;
};
struct power_supply {
	int step_enabled;
	int sw_jeita_enabled;
	int dynamic_fv_enabled;
	int cycle_count;
	int temp;
	int voltage;
	int voltage_max;
	int charge_type;
	int real_type;
	int hvdcp_type;
	int pd_authentication;
	int fastcharge_mode;
	bool fail_temp;
};
union power_supply_propval { int intval; };

enum {
	POWER_SUPPLY_PROP_STEP_CHARGING_ENABLED,
	POWER_SUPPLY_PROP_SW_JEITA_ENABLED,
	POWER_SUPPLY_PROP_DYNAMIC_FV_ENABLED,
	POWER_SUPPLY_PROP_CYCLE_COUNT,
	POWER_SUPPLY_PROP_TEMP,
	POWER_SUPPLY_PROP_VOLTAGE_NOW,
	POWER_SUPPLY_PROP_VOLTAGE_MAX,
	POWER_SUPPLY_PROP_CHARGE_TYPE,
	POWER_SUPPLY_PROP_PD_AUTHENTICATION,
	POWER_SUPPLY_PROP_REAL_TYPE,
	POWER_SUPPLY_PROP_HVDCP3_TYPE,
	POWER_SUPPLY_PROP_FASTCHARGE_MODE,
};
enum power_supply_type {
	POWER_SUPPLY_TYPE_UNKNOWN,
	POWER_SUPPLY_TYPE_USB_HVDCP_3P5,
};
enum { POWER_SUPPLY_CHARGE_TYPE_TAPER = 1 };

static struct votable fcc_vote, fv_vote, usb_vote, cp_vote;
static bool cp_missing, fcc_missing, input_present;
static struct power_supply *registered_usb;
static int hardware_version;
static ktime_t now_us;

static int power_supply_get_property(struct power_supply *psy, int prop,
				     union power_supply_propval *val)
{
	CHECK(psy != NULL);
	switch (prop) {
	case POWER_SUPPLY_PROP_STEP_CHARGING_ENABLED:
		val->intval = psy->step_enabled;
		break;
	case POWER_SUPPLY_PROP_SW_JEITA_ENABLED:
		val->intval = psy->sw_jeita_enabled;
		break;
	case POWER_SUPPLY_PROP_DYNAMIC_FV_ENABLED:
		val->intval = psy->dynamic_fv_enabled;
		break;
	case POWER_SUPPLY_PROP_CYCLE_COUNT:
		val->intval = psy->cycle_count;
		break;
	case POWER_SUPPLY_PROP_TEMP:
		if (psy->fail_temp)
			return -EIO;
		val->intval = psy->temp;
		break;
	case POWER_SUPPLY_PROP_VOLTAGE_NOW:
		val->intval = psy->voltage;
		break;
	case POWER_SUPPLY_PROP_VOLTAGE_MAX:
		val->intval = psy->voltage_max;
		break;
	case POWER_SUPPLY_PROP_CHARGE_TYPE:
		val->intval = psy->charge_type;
		break;
	case POWER_SUPPLY_PROP_PD_AUTHENTICATION:
		val->intval = psy->pd_authentication;
		break;
	case POWER_SUPPLY_PROP_REAL_TYPE:
		val->intval = psy->real_type;
		break;
	case POWER_SUPPLY_PROP_HVDCP3_TYPE:
		val->intval = psy->hvdcp_type;
		break;
	case POWER_SUPPLY_PROP_FASTCHARGE_MODE:
		val->intval = psy->fastcharge_mode;
		break;
	default:
		return -EINVAL;
	}
	return 0;
}

static int power_supply_set_property(struct power_supply *psy, int prop,
				     const union power_supply_propval *val)
{
	CHECK(psy != NULL);
	if (prop == POWER_SUPPLY_PROP_FASTCHARGE_MODE)
		psy->fastcharge_mode = val->intval;
	else if (prop == POWER_SUPPLY_PROP_VOLTAGE_MAX)
		psy->voltage_max = val->intval;
	else
		CHECK(false);
	return 0;
}

struct device_node {
	const u32 *data;
	int length;
};
static int of_property_count_elems_of_size(struct device_node *node,
					   const char *name, int size)
{
	CHECK(node != NULL && name != NULL && size == sizeof(u32));
	return node->length;
}
static int of_property_read_u32_array(struct device_node *node, const char *name,
				      u32 *dest, int length)
{
	CHECK(node != NULL && name != NULL && length == node->length);
	memcpy(dest, node->data, length * sizeof(u32));
	return 0;
}

static struct power_supply *power_supply_get_by_name(const char *name)
{
	CHECK(!strcmp(name, "usb"));
	return registered_usb;
}

static struct votable *find_votable(const char *name)
{
	if (!strcmp(name, "CP_DISABLE"))
		return cp_missing ? NULL : &cp_vote;
	if (!strcmp(name, "FCC"))
		return fcc_missing ? NULL : &fcc_vote;
	if (!strcmp(name, "FV"))
		return &fv_vote;
	if (!strcmp(name, "USB_ICL"))
		return &usb_vote;
	CHECK(false);
	return NULL;
}

static int vote(struct votable *v, const char *client, bool enabled, int value)
{
	CHECK(v != NULL);
	CHECK(client != NULL);
	v->calls++;
	v->active = enabled;
	v->value = value;
	v->client = client;
	return 0;
}
static int get_effective_result(struct votable *v) { return v->value; }
static int lct_check_hwversion(void) { return hardware_version; }
static ktime_t ktime_get(void) { return now_us; }
static int64_t ktime_us_delta(ktime_t a, ktime_t b) { return a - b; }
static bool is_input_present(struct step_chg_info *chip)
{
	(void)chip;
	return input_present;
}
/* Generated from the real functions and constants, not rewritten copies. */
#include "qcom_charge_extracted.h"
#include "qcom_charge_profiles.h"

struct fixture {
	struct step_chg_info chip;
	struct step_chg_cfg step;
	struct jeita_fcc_cfg jeita_fcc;
	struct jeita_fv_cfg jeita_fv;
	struct dynamic_fv_cfg dynamic_fv;
	struct power_supply batt, bms, usb;
};

static void init_fixture(struct fixture *f)
{
	memset(f, 0, sizeof(*f));
	memset(&fcc_vote, 0, sizeof(fcc_vote));
	memset(&fv_vote, 0, sizeof(fv_vote));
	memset(&usb_vote, 0, sizeof(usb_vote));
	memset(&cp_vote, 0, sizeof(cp_vote));
	cp_missing = false;
	fcc_missing = false;
	input_present = true;
	hardware_version = NORMAL_HWVERSION;
	now_us = 6000000;
	f->chip.step_chg_config = &f->step;
	f->chip.jeita_fcc_config = &f->jeita_fcc;
	f->chip.jeita_fv_config = &f->jeita_fv;
	f->chip.dynamic_fv_config = &f->dynamic_fv;
	f->chip.batt_psy = &f->batt;
	f->chip.bms_psy = &f->bms;
	f->chip.usb_psy = &f->usb;
	registered_usb = &f->usb;
	f->chip.step_chg_cfg_valid = true;
	f->chip.sw_jeita_cfg_valid = true;
	f->chip.dynamic_fv_cfg_valid = true;
	f->chip.dynamic_fv_index = -EINVAL;
	f->chip.step_index = -EINVAL;
	f->chip.jeita_fcc_index = -EINVAL;
	f->chip.jeita_fv_index = -EINVAL;
	f->step.param.psy_prop = POWER_SUPPLY_PROP_VOLTAGE_NOW;
	f->step.param.hysteresis = 100000;
	f->step.param.prop_name = "VBATT";
	f->jeita_fcc.param.psy_prop = POWER_SUPPLY_PROP_TEMP;
	f->jeita_fcc.param.hysteresis = 5;
	f->jeita_fcc.param.prop_name = "BATT_TEMP";
	f->jeita_fv.param.psy_prop = POWER_SUPPLY_PROP_TEMP;
	f->jeita_fv.param.hysteresis = 5;
	f->jeita_fv.param.prop_name = "BATT_TEMP";
	f->dynamic_fv.prop_name = "BATT_CYCLE_COUNT";
	memcpy(f->step.fcc_cfg, profile_atl_step, sizeof(f->step.fcc_cfg));
	memcpy(f->jeita_fcc.fcc_cfg, profile_atl_jeita_fcc,
	       sizeof(f->jeita_fcc.fcc_cfg));
	memcpy(f->jeita_fv.fv_cfg, profile_atl_jeita_fv,
	       sizeof(f->jeita_fv.fv_cfg));
	memcpy(f->dynamic_fv.fv_cfg, profile_atl_dynamic_fv,
	       sizeof(f->dynamic_fv.fv_cfg));
	f->batt.step_enabled = 1;
	f->batt.sw_jeita_enabled = 1;
	f->batt.temp = 250;
	f->batt.voltage = 3900000;
	f->batt.voltage_max = 4480000;
}

static void check_lookup(struct range_data *ranges, int threshold,
			 int old_index, int hyst, int expected_index,
			 int expected_value)
{
	int index = -999, value = -999;

	CHECK(get_val(ranges, hyst, old_index, threshold, &index, &value) == 0);
	CHECK(index == expected_index);
	CHECK(value == expected_value);
}

static void test_ranges(void)
{
	struct range_data ranges[MAX_STEP_CHG_ENTRIES];
	const u32 overlapping[] = { 100, 200, 1000, 199, 300, 900 };
	struct device_node node = { (const u32 *)profile_j6b_step, 9 };
	int count, threshold, idx, value, rc, expected, t;

	/* Cover 1-5 populated entries, bounds and clamping, independently. */
	for (count = 1; count <= MAX_STEP_CHG_ENTRIES; count++) {
		memset(ranges, 0, sizeof(ranges));
		for (idx = 0; idx < count; idx++) {
			ranges[idx].low_threshold = idx * 10 + 1;
			ranges[idx].high_threshold = idx * 10 + 10;
			ranges[idx].value = 1000 - idx * 100;
		}
		for (threshold = 1; threshold <= count * 10 + 10; threshold++) {
			expected = (threshold - 1) / 10;
			if (expected >= count)
				expected = count - 1;
			check_lookup(ranges, threshold, -EINVAL, 0, expected,
				     ranges[expected].value);
		}
		idx = value = -999;
		rc = get_val(ranges, 0, -EINVAL, 0, &idx, &value);
		CHECK(rc == -ENODATA);
	}
	memset(ranges, 0, sizeof(ranges));
	idx = value = -999;
	CHECK(get_val(ranges, 0, -EINVAL, 10, &idx, &value) == -ENODATA);

	/* The actual DT parser clamps the J6B profile to its existing maximum. */
	CHECK(read_range_data_from_node(&node, "qcom,step-chg-ranges", ranges,
					4480000, 5900000) == 0);
	CHECK(ranges[0].value == 5900000 && ranges[1].value == 4900000);
	CHECK(ranges[2].value == 3900000 && ranges[3].high_threshold == 0);
	check_lookup(ranges, 3900000, -EINVAL, 100000, 0, 5900000);
	check_lookup(ranges, 4460000, -EINVAL, 100000, 2, 3900000);
	node.length = 8;
	CHECK(read_range_data_from_node(&node, "bad-length", ranges, 4480000,
					5900000) == -EINVAL);
	node.length = 18;
	CHECK(read_range_data_from_node(&node, "too-many", ranges, 4480000,
					5900000) == -EINVAL);
	node.data = overlapping;
	node.length = ARRAY_SIZE(overlapping);
	CHECK(read_range_data_from_node(&node, "overlap", ranges, 4480000,
					5900000) == -EINVAL);
	CHECK(ranges[0].value == 0 && ranges[1].value == 0);

	memcpy(ranges, profile_atl_step, sizeof(ranges));
	check_lookup(ranges, 3900000, -EINVAL, 100000, 0, 3600000);
	check_lookup(ranges, 4300000, -EINVAL, 100000, 1, 3300000);
	check_lookup(ranges, 4440000, -EINVAL, 100000, 2, 3000000);
	check_lookup(ranges, 4300000, 0, 100000, 0, 3600000);
	check_lookup(ranges, 4350001, 0, 100000, 1, 3300000);
	check_lookup(ranges, 4600000, 1, 100000, 2, 3000000);

	/* Five-entry JEITA tables keep selecting the same ranges. */
	memcpy(ranges, profile_atl_jeita_fcc, sizeof(ranges));
	for (threshold = -10; threshold <= 650; threshold++) {
		t = threshold < 0 ? 0 : threshold;
		if (t <= 50)
			expected = 0;
		else if (t <= 100)
			expected = 1;
		else if (t <= 150)
			expected = 2;
		else if (t <= 450)
			expected = 3;
		else
			expected = 4;
		check_lookup(ranges, threshold, -EINVAL, 5, expected,
			     ranges[expected].value);
	}
	/* Hysteresis holds the previous JEITA range until its margin. */
	check_lookup(ranges, 451, 3, 5, 3, ranges[3].value);
	check_lookup(ranges, 456, 3, 5, 4, ranges[4].value);
	check_lookup(ranges, 449, 4, 5, 4, ranges[4].value);
	check_lookup(ranges, 445, 4, 5, 3, ranges[3].value);
	puts("PASS: populated ranges, profile lookup, bounds and hysteresis");
}

static void test_step(void)
{
	struct fixture f;

	init_fixture(&f);
	CHECK(handle_step_chg_config(&f.chip) == 0);
	CHECK(f.chip.step_index == 0);
	CHECK(fcc_vote.active && fcc_vote.value == 3600000);
	now_us += 6000000;
	f.batt.voltage = 4300000;
	CHECK(handle_step_chg_config(&f.chip) == 0);
	CHECK(f.chip.step_index == 0 && fcc_vote.value == 3600000);
	now_us += 6000000;
	f.batt.voltage = 4360000;
	CHECK(handle_step_chg_config(&f.chip) == 0);
	CHECK(f.chip.step_index == 1 && fcc_vote.value == 3300000);
	now_us += 6000000;
	f.batt.step_enabled = 0;
	CHECK(handle_step_chg_config(&f.chip) == 0);
	CHECK(!fcc_vote.active);

	/* Exercise the driver's optional taper path with the real range lookup. */
	init_fixture(&f);
	f.chip.taper_fcc = true;
	CHECK(handle_step_chg_config(&f.chip) == 0);
	CHECK(fcc_vote.active && fcc_vote.value == 3600000);
	f.batt.voltage = 4360000;
	for (int step = 0; step < 7; step++) {
		now_us += 6000000;
		CHECK(handle_step_chg_config(&f.chip) == 0);
		CHECK(fcc_vote.value >= 3300000 && fcc_vote.value <= 3600000);
	}
	CHECK(f.chip.step_index == 1 && fcc_vote.value == 3300000);
	puts("PASS: generic step-charging votes, taper and disabled state");
}

static void test_dynamic_fv(void)
{
	struct fixture f;

	init_fixture(&f);
	f.batt.dynamic_fv_enabled = 1;
	f.bms.cycle_count = 250;
	CHECK(handle_dynamic_fv(&f.chip) == 0);
	CHECK(f.chip.dynamic_fv_index == 2);
	CHECK(fv_vote.active && fv_vote.value == 4440000 && fv_vote.calls == 2);
	CHECK(!strcmp(fv_vote.client, BATT_PROFILE_VOTER));
	CHECK(f.bms.voltage_max == 4440000);

	/* Do not lower the programmed FV while battery voltage exceeds it. */
	init_fixture(&f);
	f.batt.dynamic_fv_enabled = 1;
	f.bms.cycle_count = 500;
	f.batt.voltage = 4450000;
	CHECK(handle_dynamic_fv(&f.chip) == 0);
	CHECK(fv_vote.calls == 0 && f.bms.voltage_max == 0);
	puts("PASS: dynamic-FV profile votes and voltage guard");
}

static void test_jeita(void)
{
	struct fixture f;
	int calls;

	/* A missing CP votable must not suppress JEITA FCC/FV votes. */
	init_fixture(&f);
	f.chip.six_pin_battery = true;
	cp_missing = true;
	CHECK(handle_jeita(&f.chip) == 0);
	CHECK(fcc_vote.active && fcc_vote.value == 3600000);
	CHECK(fv_vote.active && fv_vote.value == 4480000);
	CHECK(f.chip.jeita_last_update_time == now_us);
	CHECK(!strcmp(fcc_vote.client, JEITA_VOTER));
	CHECK(!strcmp(fv_vote.client, JEITA_VOTER));

	/* Do not update fast-charge mode until a required CP is ready. */
	init_fixture(&f);
	f.chip.six_pin_battery = true;
	cp_missing = true;
	f.usb.pd_authentication = 1;
	CHECK(handle_jeita(&f.chip) == 0);
	CHECK(fcc_vote.active && fv_vote.active && !f.usb.fastcharge_mode);
	cp_missing = false;
	now_us += 6000000;
	CHECK(handle_jeita(&f.chip) == 0);
	CHECK(f.usb.fastcharge_mode);

	/* Cover the separate six-pin J6B profile at normal and warm temperatures. */
	init_fixture(&f);
	f.chip.six_pin_battery = true;
	cp_missing = true;
	memcpy(f.jeita_fcc.fcc_cfg, profile_j6b_jeita_fcc,
	       sizeof(f.jeita_fcc.fcc_cfg));
	memcpy(f.jeita_fv.fv_cfg, profile_j6b_jeita_fv,
	       sizeof(f.jeita_fv.fv_cfg));
	CHECK(handle_jeita(&f.chip) == 0);
	CHECK(fcc_vote.active && fcc_vote.value == 5900000);
	CHECK(fv_vote.active && fv_vote.value == 4480000);
	cp_missing = false;
	now_us += 6000000;
	f.batt.temp = 500;
	CHECK(handle_jeita(&f.chip) == 0);
	CHECK(cp_vote.active && fcc_vote.active && fcc_vote.value == 2300000);
	CHECK(fv_vote.active && fv_vote.value == 4100000);

	/* A late-registering pump gets its disable vote at a cold temperature. */
	init_fixture(&f);
	f.chip.six_pin_battery = true;
	f.batt.temp = 100;
	cp_missing = true;
	CHECK(handle_jeita(&f.chip) == 0);
	CHECK(fcc_vote.active && fv_vote.active && cp_vote.calls == 0);
	cp_missing = false;
	now_us += 6000000;
	CHECK(handle_jeita(&f.chip) == 0);
	CHECK(cp_vote.active && !strcmp(cp_vote.client, JEITA_VOTER));

	/* First event before USB registration: retain JEITA FCC/FV protection. */
	init_fixture(&f);
	f.chip.six_pin_battery = true;
	f.chip.usb_psy = NULL;
	registered_usb = NULL;
	CHECK(handle_jeita(&f.chip) == 0);
	CHECK(fcc_vote.active && fv_vote.active);
	CHECK(f.chip.usb_psy == NULL);

	/* Once USB registers, the same helper obtains it before property reads. */
	now_us += 6000000;
	registered_usb = &f.usb;
	CHECK(handle_jeita(&f.chip) == 0);
	CHECK(f.chip.usb_psy == &f.usb && fcc_vote.active && fv_vote.active);

	/* Missing USB must not skip the separate JEITA input-suspend check. */
	init_fixture(&f);
	f.chip.six_pin_battery = true;
	f.chip.usb_psy = NULL;
	registered_usb = NULL;
	f.batt.temp = 500;
	f.batt.voltage = 4200000;
	f.batt.charge_type = POWER_SUPPLY_CHARGE_TYPE_TAPER;
	CHECK(handle_jeita(&f.chip) == 0);
	CHECK(fv_vote.active && fv_vote.value == 4100000);
	CHECK(usb_vote.active && usb_vote.value == 0);

	/* A sensor read error must not fabricate new charging votes. */
	init_fixture(&f);
	f.chip.six_pin_battery = true;
	cp_missing = true;
	f.batt.fail_temp = true;
	CHECK(handle_jeita(&f.chip) == -EIO);
	CHECK(fcc_vote.calls == 0 && fv_vote.calls == 0);

	/* Existing handling when the mandatory FCC votable is missing. */
	init_fixture(&f);
	fcc_missing = true;
	CHECK(handle_jeita(&f.chip) == -EINVAL);
	CHECK(fv_vote.calls == 0);

	/* Starting inside either margin must leave the previous state alone. */
	init_fixture(&f);
	f.chip.six_pin_battery = true;
	f.batt.temp = 101;
	CHECK(handle_jeita(&f.chip) == 0 && cp_vote.calls == 0);
	init_fixture(&f);
	f.chip.six_pin_battery = true;
	f.batt.temp = 449;
	CHECK(handle_jeita(&f.chip) == 0 && cp_vote.calls == 0);

	/* Charge-pump disable vote retains both 0.5 C margins. */
	init_fixture(&f);
	f.chip.six_pin_battery = true;
	f.batt.temp = 100;
	CHECK(handle_jeita(&f.chip) == 0 && cp_vote.active);
	calls = cp_vote.calls;
	now_us += 6000000;
	f.batt.temp = 101;
	CHECK(handle_jeita(&f.chip) == 0 && cp_vote.active && cp_vote.calls == calls);
	now_us += 6000000;
	f.batt.temp = 105;
	CHECK(handle_jeita(&f.chip) == 0 && cp_vote.active && cp_vote.calls == calls);
	now_us += 6000000;
	f.batt.temp = 106;
	CHECK(handle_jeita(&f.chip) == 0 && !cp_vote.active);
	now_us += 6000000;
	f.batt.temp = 450;
	CHECK(handle_jeita(&f.chip) == 0 && cp_vote.active);
	calls = cp_vote.calls;
	now_us += 6000000;
	f.batt.temp = 449;
	CHECK(handle_jeita(&f.chip) == 0 && cp_vote.active && cp_vote.calls == calls);
	now_us += 6000000;
	f.batt.temp = 445;
	CHECK(handle_jeita(&f.chip) == 0 && cp_vote.active && cp_vote.calls == calls);
	now_us += 6000000;
	f.batt.temp = 444;
	CHECK(handle_jeita(&f.chip) == 0 && !cp_vote.active);

	/* Hardware variants use the same CP gate; other variants skip it. */
	init_fixture(&f);
	hardware_version = VDF_HWVERSION;
	f.batt.temp = 450;
	CHECK(handle_jeita(&f.chip) == 0 && cp_vote.active);
	init_fixture(&f);
	hardware_version = JAPAN_HWVERSION;
	f.batt.temp = 450;
	CHECK(handle_jeita(&f.chip) == 0 && cp_vote.active);
	init_fixture(&f);
	f.batt.temp = 450;
	CHECK(handle_jeita(&f.chip) == 0 && cp_vote.calls == 0);

	/* Registered USB still receives existing fast-charge mode updates. */
	init_fixture(&f);
	f.chip.six_pin_battery = true;
	f.usb.pd_authentication = 1;
	f.bms.fastcharge_mode = 1;
	f.usb.fastcharge_mode = 1;
	f.batt.temp = 450;
	CHECK(handle_jeita(&f.chip) == 0);
	CHECK(f.usb.fastcharge_mode == 0);
	now_us += 6000000;
	f.bms.fastcharge_mode = 0;
	f.batt.temp = 250;
	CHECK(handle_jeita(&f.chip) == 0);
	CHECK(f.usb.fastcharge_mode == 1);

	/* QC3.5 profiles not using a CP keep their existing fast-charge path. */
	init_fixture(&f);
	f.chip.qc3p5_ffc_batt = true;
	f.usb.pd_authentication = 1;
	CHECK(handle_jeita(&f.chip) == 0);
	CHECK(f.usb.fastcharge_mode == 1 && cp_vote.calls == 0);

	/* Disabled SW JEITA still clears its FCC/FV/input votes. */
	init_fixture(&f);
	CHECK(handle_jeita(&f.chip) == 0);
	CHECK(fcc_vote.active && fv_vote.active);
	f.batt.sw_jeita_enabled = 0;
	CHECK(handle_jeita(&f.chip) == 0);
	CHECK(!fcc_vote.active && !fv_vote.active && !usb_vote.active);
	puts("PASS: JEITA votes, absent pump, errors and pump hysteresis");
}

struct smb_charger {
	bool health_not_good;
	int index_vfloat;
	struct six_pin_step_data six_pin_step_cfg[MAX_STEP_ENTRIES];
	u32 start_step_vbat;
	int trigger_taper_count;
	int taper_ibat_hv_step;
	struct votable *fcc_votable;
};
struct branch_result { bool drop, step, set; };

static struct branch_result run_sixpin_branch(struct smb_charger *chg,
					     bool drop_fcc_enable,
					     int current_fcc)
{
	bool step_cfg_enable = false, set_fcc_fv_enable = false;
	int taper_ibat_step = 400000;

#include "qcom_sixpin_branch.h"
	return (struct branch_result) { drop_fcc_enable, step_cfg_enable,
					set_fcc_fv_enable };
}

static void test_sixpin(void)
{
	struct smb_charger chg = {0};
	struct branch_result result;
	const int invalid_indexes[] = { MAX_STEP_ENTRIES - 1, -1, MAX_STEP_ENTRIES };
	size_t i;

	memset(&fcc_vote, 0, sizeof(fcc_vote));
	chg.fcc_votable = &fcc_vote;
	chg.six_pin_step_cfg[1].fcc_step_ua = 4800000;
	chg.six_pin_step_cfg[2].fcc_step_ua = 3800000;
	chg.index_vfloat = 0;
	result = run_sixpin_branch(&chg, true, 5000000);
	CHECK(result.drop && !result.step && !result.set);
	CHECK(fcc_vote.active && fcc_vote.value == 4600000);
	CHECK(!strcmp(fcc_vote.client, SIX_PIN_VFLOAT_VOTER));
	memset(&fcc_vote, 0, sizeof(fcc_vote));
	result = run_sixpin_branch(&chg, true, 4819999);
	CHECK(result.drop && result.step && result.set && fcc_vote.calls == 0);
	result = run_sixpin_branch(&chg, true, 4820000);
	CHECK(result.drop && !result.step && fcc_vote.active);
	chg.taper_ibat_hv_step = 200000;
	result = run_sixpin_branch(&chg, true, 5000000);
	CHECK(fcc_vote.active && fcc_vote.value == 4800000);
	CHECK(!strcmp(fcc_vote.client, HIGH_VBAT_VOTER));
	chg.taper_ibat_hv_step = 0;
	chg.index_vfloat = 1;
	result = run_sixpin_branch(&chg, true, 4000000);
	CHECK(result.drop && fcc_vote.active && fcc_vote.value == 3600000);
	memset(&fcc_vote, 0, sizeof(fcc_vote));
	result = run_sixpin_branch(&chg, true, 3500000);
	CHECK(result.drop && result.step && result.set && fcc_vote.calls == 0);

	/* The final and invalid indexes must not access the next array entry. */
	for (i = 0; i < ARRAY_SIZE(invalid_indexes); i++) {
		memset(&fcc_vote, 0, sizeof(fcc_vote));
		chg.index_vfloat = invalid_indexes[i];
		result = run_sixpin_branch(&chg, true, 4000000);
		CHECK(!result.drop && !result.step && !result.set);
		CHECK(fcc_vote.calls == 0);
	}
	chg.index_vfloat = 0;
	chg.health_not_good = true;
	result = run_sixpin_branch(&chg, true, 5000000);
	CHECK(!result.drop && !result.step && !result.set);
	puts("PASS: six-pin taper branches and array bounds");
}

int main(int argc, char **argv)
{
	const char *which = argc > 1 ? argv[1] : "all";

	if (!strcmp(which, "all") || !strcmp(which, "ranges"))
		test_ranges();
	if (!strcmp(which, "all") || !strcmp(which, "step"))
		test_step();
	if (!strcmp(which, "all") || !strcmp(which, "dynamic"))
		test_dynamic_fv();
	if (!strcmp(which, "all") || !strcmp(which, "jeita"))
		test_jeita();
	if (!strcmp(which, "all") || !strcmp(which, "sixpin"))
		test_sixpin();
	return 0;
}
