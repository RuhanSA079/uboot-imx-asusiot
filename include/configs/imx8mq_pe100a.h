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

#define CONFIG_SYS_BOOTM_LEN		(32 * SZ_1M)

#define CONFIG_SPL_MAX_SIZE		(150 * 1024)
#define CONFIG_SYS_MONITOR_LEN		(512 * 1024)
#define CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_USE_SECTOR
#define CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR	(0x300 + CONFIG_SECONDARY_BOOT_SECTOR_OFFSET)

#ifdef CONFIG_SPL_BUILD
/*#define CONFIG_ENABLE_DDR_TRAINING_DEBUG*/
#define CONFIG_SPL_LDSCRIPT		"arch/arm/cpu/armv8/u-boot-spl.lds"
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
#undef CONFIG_DM_PMIC
#undef CONFIG_DM_PMIC_PFUZE100

#define CONFIG_SYS_I2C

#define CONFIG_POWER
#define CONFIG_POWER_I2C
#define CONFIG_POWER_BD71837
#endif

#define CONFIG_REMAKE_ELF

/* ENET Config */
/* ENET1 */
#if defined(CONFIG_FEC_MXC)
#define CONFIG_ETHPRIME                 "FEC"
#define PHY_ANEG_TIMEOUT 3000

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

#define CONFIG_SYS_MMC_ENV_DEV		2

/*
 * Another approach is add the clocks for inmates into clks_init_on
 * in clk-imx8mq.c, then clk_ingore_unused could be removed.
 */
#define JAILHOUSE_ENV \
	"jh_clk= \0 " \
	"jh_mmcboot=setenv fdtfile imx8mq-evk-root.dtb; " \
		"setenv jh_clk clk_ignore_unused mem=2000M; " \
			   "if run loadimage; then " \
				   "run mmcboot; " \
			   "else run jh_netboot; fi; \0" \
	"jh_netboot=setenv fdtfile imx8mq-evk-root.dtb; setenv jh_clk clk_ignore_unused mem=2000M; run netboot; \0 "

#define CONFIG_MFG_ENV_SETTINGS \
	CONFIG_MFG_ENV_SETTINGS_DEFAULT \
	"initrd_addr=0x43800000\0" \
	"initrd_high=0xffffffffffffffff\0" \
	"emmc_dev=0\0"\
	"sd_dev=1\0" \

/* Initial environment variables */
#define CONFIG_EXTRA_ENV_SETTINGS		\
	CONFIG_MFG_ENV_SETTINGS \
	BOOTENV \
	JAILHOUSE_ENV \
	"scriptaddr=0x43500000\0" \
	"kernel_addr_r=" __stringify(CONFIG_LOADADDR) "\0" \
	UBUNTU_ENV_DEFAULT \
	UBUNTU_ENV_LOAD_BOOT_FILES \
	"mmc_seed_part=1\0" \
	"mmc_boot_part=2\0" \
	"devtype=mmc\0" \
	"console=ttymxc0,115200\0" \
	"conf_addr=0x40000000\0"		\
	"cmdline_addr=0x41000000\0"		\
	"fdt_overlay_addr=0x42000000\0"		\
	"fdt_addr_r=0x43000000\0"		\
	"fdt_addr=0x43000000\0"			\
	"fdt_high=0xffffffffffffffff\0"		\
	"boot_fdt=try\0" \
	"fdtfile=imx8mq-pe100a.dtb\0" \
	"bootm_size=0x10000000\0" \
	"mmcdev="__stringify(CONFIG_SYS_MMC_ENV_DEV)"\0" \
	"mmcpart=" __stringify(CONFIG_SYS_MMC_IMG_LOAD_PART) "\0" \
	"fastboot=echo Enter Fastboot Mode ...; " \
		"fastboot 0;\0" \
	"mmcargs=setenv bootargs ${jh_clk} console=${console} root=${mmcroot}\0" 

#define CONFIG_BOOTCOMMAND \
	   "run boot_uc;"

/* Link Definitions */
#define CONFIG_LOADADDR			0x40480000

#define CONFIG_SYS_LOAD_ADDR           CONFIG_LOADADDR

#define CONFIG_SYS_INIT_RAM_ADDR        0x40000000
#define CONFIG_SYS_INIT_RAM_SIZE        0x80000
#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)


/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		SZ_32M

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
#define CONFIG_SYS_PROMPT_HUSH_PS2	"> "
#define CONFIG_SYS_CBSIZE		1024
#define CONFIG_SYS_MAXARGS		64
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + \
					sizeof(CONFIG_SYS_PROMPT) + 16)

#define CONFIG_IMX_BOOTAUX

#define CONFIG_SYS_FSL_USDHC_NUM	2
#define CONFIG_SYS_FSL_ESDHC_ADDR       0

#define CONFIG_SYS_MMC_IMG_LOAD_PART	1

/* I2C Configs */
#define CONFIG_SYS_I2C_SPEED		  100000

/* USB configs */
#ifndef CONFIG_SPL_BUILD

#define CONFIG_CMD_USB_MASS_STORAGE
#define CONFIG_USB_GADGET_MASS_STORAGE
#define CONFIG_USB_FUNCTION_MASS_STORAGE

#define CONFIG_CMD_READ

#endif

#define CONFIG_SERIAL_TAG
#define CONFIG_FASTBOOT_USB_DEV 0


#define CONFIG_USB_MAX_CONTROLLER_COUNT         2

#define CONFIG_USBD_HS
#define CONFIG_USB_GADGET_VBUS_DRAW 2

#ifndef CONFIG_SPL_BUILD
#define CONFIG_DM_PMIC
#endif

#ifdef CONFIG_DM_VIDEO
#define CONFIG_VIDEO_LOGO
#define CONFIG_BMP_16BPP
#define CONFIG_BMP_24BPP
#define CONFIG_BMP_32BPP
#define CONFIG_VIDEO_BMP_RLE8
#define CONFIG_VIDEO_BMP_LOGO
#endif

#ifdef CONFIG_ANDROID_SUPPORT
#include "imx8mq_evk_android.h"
#endif

#endif
