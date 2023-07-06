/* Copyright 2018 NXP
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __IMX_COMMON_CONFIG_H
#define __IMX_COMMON_CONFIG_H

#ifdef CONFIG_ARM64
    #define MFG_BOOT_CMD "booti "
#else
    #define MFG_BOOT_CMD "bootz "
#endif

#ifdef CONFIG_USB_PORT_AUTO
    #define FASTBOOT_CMD "echo \"Run fastboot ...\"; fastboot auto; "
#else
    #define FASTBOOT_CMD "echo \"Run fastboot ...\"; fastboot 0; "
#endif

/* define the nandfit partiton environment for uuu */
#if defined(CONFIG_IMX8MM) || defined(CONFIG_IMX8MQ) || \
	defined(CONFIG_IMX8QM) || defined(CONFIG_IMX8QXP) || \
	defined(CONFIG_IMX8DXL) || defined(CONFIG_IMX8MN) || \
	defined(CONFIG_IMX8MP)
#define MFG_NAND_FIT_PARTITION "nandfit_part=yes\0"
#else
#define MFG_NAND_FIT_PARTITION ""
#endif

#define CONFIG_MFG_ENV_SETTINGS_DEFAULT \
	"mfgtool_args=setenv bootargs console=${console},${baudrate} " \
		"rdinit=/linuxrc " \
		"clk_ignore_unused "\
		"\0" \
	"kboot="MFG_BOOT_CMD"\0"\
	"bootcmd_mfg=run mfgtool_args;" \
        "if iminfo ${initrd_addr}; then " \
            "if test ${tee} = yes; then " \
                "bootm ${tee_addr} ${initrd_addr} ${fdt_addr}; " \
            "else " \
                MFG_BOOT_CMD "${loadaddr} ${initrd_addr} ${fdt_addr}; " \
            "fi; " \
        "else " \
		FASTBOOT_CMD  \
        "fi;\0" \
	MFG_NAND_FIT_PARTITION \

#define UBUNTU_ENV_LOAD_BOOT_CONFIG \
    "load_uc=" \
      "setenv kernel_bootpart ${mmc_seed_part};"\
      "load ${devtype} ${mmcdev}:${kernel_bootpart} ${loadaddr} ${core_state};" \
      "env import -v -c ${loadaddr} ${filesize} ${recovery_vars};" \
      "if test \"${snapd_recovery_mode}\" = \"run\"; then " \
        "setenv bootargs \"console=${console} snapd_recovery_mode=${snapd_recovery_mode} ${snapd_standard_params}\";" \
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
        "setenv bootargs \"console=${console} snapd_recovery_mode=${snapd_recovery_mode} snapd_recovery_system=${snapd_recovery_system} ${snapd_standard_params}\";" \
        "setenv kernel_prefix \"/systems/${snapd_recovery_system}/kernel/\"; " \
      "fi; " \
      "run loadfiles\0"

#define UBUNTU_ENV_LOAD_BOOT_FILES \
  "load_kernel=load ${devtype} ${mmcdev}:${kernel_bootpart} ${loadaddr} ${kernel_prefix}/${kernel_filename}\0" \
  "load_fdt=load ${devtype} ${mmcdev}:${kernel_bootpart} ${fdt_addr} ${kernel_prefix}/dtbs/freescale/${fdt_file}\0" \
  "load_initrd=load ${devtype} ${mmcdev}:${kernel_bootpart} ${initrd_addr} ${kernel_prefix}/${initrd_filename}; setenv initrd_size ${filesize}\0" \
  "loadfiles=run load_kernel; run load_initrd; run load_fdt\0"

#define UBUNTU_ENV_LOAD_FIT_BOOT_FILES \
    "loadfiles=load ${devtype} ${mmcdev}:${kernel_bootpart} ${fitloadaddr} ${kernel_prefix}/${kernel_filename}\0"

#define UBUNTU_ENV_DEFAULT \
  "kernel_filename=kernel.img\0" \
  "initrd_filename=initrd.img\0" \
  "core_state=/uboot/ubuntu/boot.sel\0" \
  "kernel_vars=snap_kernel snap_try_kernel kernel_status\0" \
  "recovery_vars=snapd_recovery_mode snapd_recovery_system snapd_recovery_kernel\0" \
  "snapd_recovery_mode=install\0" \
  "snapd_standard_params=panic=-1 systemd.gpt_auto=0 rd.systemd.unit=basic.target\0" \
  UBUNTU_ENV_LOAD_BOOT_CONFIG

#endif
