/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright 2018 NXP
 */

#ifndef __IMX8M_EVK_H
#define __IMX8M_EVK_H

#include <linux/sizes.h>
#include <linux/stringify.h>
#include <asm/arch/imx-regs.h>
#include "imx_env.h"

#define CONFIG_SYS_BOOTM_LEN		(64 * SZ_1M)

#define CONFIG_SPL_MAX_SIZE		(154 * 1024)
#define CONFIG_SYS_MONITOR_LEN		(512 * 1024)

#ifdef CONFIG_SPL_BUILD
/*#define CONFIG_ENABLE_DDR_TRAINING_DEBUG*/
#define CONFIG_SPL_STACK		0x187FF0
#define CONFIG_SPL_BSS_START_ADDR      0x00180000
#define CONFIG_SPL_BSS_MAX_SIZE        0x2000	/* 8 KB */
#define CONFIG_SYS_SPL_MALLOC_START    0x42200000
#define CONFIG_SYS_SPL_MALLOC_SIZE    0x80000	/* 512 KB */
#define CONFIG_SYS_SPL_PTE_RAM_BASE    0x41580000

/* malloc f used before GD_FLG_FULL_MALLOC_INIT set */
#define CONFIG_MALLOC_F_ADDR		0x182000
/* For RAW image gives a error info not panic */
#define CONFIG_SPL_ABORT_ON_RAW_IMAGE

#undef CONFIG_DM_MMC

#define CONFIG_POWER_BD71837
#endif

/* ENET Config */
/* ENET1 */
#if defined(CONFIG_FEC_MXC)
#define CONFIG_ETHPRIME                 "FEC"
#define PHY_ANEG_TIMEOUT 20000

#define CONFIG_FEC_XCV_TYPE             RGMII
#define CONFIG_FEC_MXC_PHYADDR          0

#define IMX_FEC_BASE			0x30BE0000
#endif

#ifdef CONFIG_DISTRO_DEFAULTS
#define BOOT_TARGET_DEVICES(func) \
	func(USB, usb, 0) \
	func(MMC, mmc, 1) \
	func(MMC, mmc, 0)

#include <config_distro_bootcmd.h>
#else
#define BOOTENV
#endif

/* RuhanvdB -> Patch UBoot for Ubuntu Core 20 */
#define CONFIG_ENV_WHITELIST  "snap_kernel snap_try_kernel kernel_status snapd_recovery_mode snapd_recovery_system snapd_recovery_kernel"
/*
 * Another approach is add the clocks for inmates into clks_init_on
 * in clk-imx8mq.c, then clk_ingore_unused could be removed.
 */

#define UBUNTU_ENV_LOAD_BOOT_CONFIG \
    "load_uc=" \
      "setenv kernel_bootpart ${mmc_seed_part};"\
      "load ${devtype} ${mmcdev}:${kernel_bootpart} ${loadaddr} ${core_state};" \
      "env import -v -c ${loadaddr} ${filesize} ${recovery_vars};" \
      "if test \"${snapd_recovery_mode}\" = \"run\"; then " \
        "setenv bootargs \"snapd_recovery_mode=${snapd_recovery_mode} ${snapd_standard_params}\";" \
        "setenv kernel_bootpart ${mmc_boot_part}; " \
        "load ${devtype} ${mmcdev}:${kernel_bootpart} ${loadaddr} ${core_state}; " \
        "env import -v -c ${loadaddr} ${filesize} ${kernel_vars}; " \
        "setenv kernel_name ${snap_kernel}; " \
        "if test -n \"${kernel_status}\"; then " \
          "if test \"${kernel_status}\" = \"try\"; then " \
            "if test -n \"${snap_try_kernel}\"; then " \
              "setenv kernel_status trying; " \
              "setenv kernel_name \"${snap_try_kernel}\"; " \
            "fi; " \
          "elif test \"${kernel_status}\" = \"trying\"; then " \
            "setenv kernel_status \"\"; " \
          "fi;" \
          "env export -c ${loadaddr} ${kernel_vars}; " \
          "save ${devtype} ${mmcdev}:${kernel_bootpart} ${loadaddr} ${core_state} ${filesize}; " \
        "fi; " \
        "setenv kernel_prefix \"/uboot/ubuntu/${kernel_name}/\"; " \
      "else " \
        "setenv bootargs \"snapd_recovery_mode=${snapd_recovery_mode} snapd_recovery_system=${snapd_recovery_system} ${snapd_standard_params}\";" \
        "setenv kernel_prefix \"/systems/${snapd_recovery_system}/kernel/\"; " \
      "fi; " \
      "run loadfiles\0"

#define UBUNTU_ENV_DEFAULT \
  "mmc_seed_part=1\0" \
  "mmc_boot_part=2\0" \
  "devtype=mmc\0" \
  "mmcdev=1\0" \
  "mmcpart=1\0" \
  "kernel_filename=kernel.img\0" \
  "initrd_filename=initrd.img\0" \
  "core_state=/uboot/ubuntu/boot.sel\0" \
  "kernel_vars=snap_kernel snap_try_kernel kernel_status\0" \
  "recovery_vars=snapd_recovery_mode snapd_recovery_system snapd_recovery_kernel\0" \
  "snapd_recovery_mode=install\0" \
  "snapd_standard_params=panic=-1 systemd.gpt_auto=0 rd.systemd.unit=basic.target net.ifnames=0\0" \
  UBUNTU_ENV_LOAD_BOOT_CONFIG

#define UBUNTU_ENV_LOAD_FIT_BOOT_FILES \
    "loadfiles=load ${devtype} ${mmcdev}:${kernel_bootpart} ${fitloadaddr} ${kernel_prefix}/${kernel_filename}\0"

#define CONFIG_MFG_ENV_SETTINGS \
	CONFIG_MFG_ENV_SETTINGS_DEFAULT \
	"initrd_addr=0x43800000\0" \
	"initrd_high=0xffffffffffffffff\0" \
	"emmc_dev=0\0"\

/* Initial environment variables */
#define CONFIG_EXTRA_ENV_SETTINGS		\
	CONFIG_MFG_ENV_SETTINGS \
 	UBUNTU_ENV_DEFAULT \
	UBUNTU_ENV_LOAD_FIT_BOOT_FILES \
	"boot_uc=run load_uc;bootm ${fitloadaddr}#conf-0\0" \
	"fitloadaddr=0x45000000\0" \
	"fdt_high=0xffffffffffffffff\0"		\
	"fdt_addr_r=0x43000000\0"		\
	"fdt_addr=0x43000000\0"			\
	"fdt_file=imx8mq-pe100a.dtb\0"		\
	"mmcdev=0\0" \
	"mmcpart=1\0" \
	"mmcautodetect=yes\0" \
	"fastboot=echo Enter Fastboot Mode ...; " \
		"fastboot 0;\0" \
	"fi;"

#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND \
	   "mmc dev ${mmcdev}; if mmc rescan; then " \
		   "echo ASUSIoT PE100A Core20 VBoot -> Booting...;" \
			   "run boot_uc;" \
		"fi"

/* Link Definitions */

#define CONFIG_SYS_INIT_RAM_ADDR        0x40000000
#define CONFIG_SYS_INIT_RAM_SIZE        0x80000
#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

#define CONFIG_MMCROOT			"/dev/mmcblk1p2"  /* USDHC2 */

#define CONFIG_SYS_SDRAM_BASE           0x40000000

#if defined(CONFIG_TARGET_IMX8MQ_PE100A_2G)
/* one bank support */
#define PHYS_SDRAM			0x40000000
#define PHYS_SDRAM_SIZE			0x80000000 /* 2GB DDR*/

#elif defined(CONFIG_TARGET_IMX8MQ_PE100A)
/* two bank support */
#define PHYS_SDRAM			0x40000000
#define PHYS_SDRAM_SIZE			0xc0000000 /* 3GB */
#define PHYS_SDRAM_2			0x100000000
#define PHYS_SDRAM_2_SIZE		0x40000000 /* 1GB */

#endif

#define CONFIG_MXC_UART_BASE		UART1_BASE_ADDR

/* Monitor Command Prompt */
#define CONFIG_SYS_CBSIZE		1024
#define CONFIG_SYS_MAXARGS		64
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + \
					sizeof(CONFIG_SYS_PROMPT) + 16)

#define CONFIG_SYS_FSL_USDHC_NUM	2
#define CONFIG_SYS_FSL_ESDHC_ADDR       0

#define CONFIG_CMD_READ
#define CONFIG_SERIAL_TAG
#define CONFIG_FASTBOOT_USB_DEV 0

#define CONFIG_USB_MAX_CONTROLLER_COUNT         2

#define CONFIG_USBD_HS
#define CONFIG_USB_GADGET_VBUS_DRAW 2

#ifdef CONFIG_ANDROID_SUPPORT
#include "imx8mq_evk_android.h"
#endif

#endif
