/**
 * @file platform_vela_qcs6490.h
 * @brief Vela Qualcomm QCS6490 (RB3 Gen 2) platform constants.
 *
 * This header centralizes platform paths and capability values used by HAL
 * modules. Deterministic/bit-precise I/O is expected to be delegated to the
 * Hexagon DSP real-time core and controlled from Linux via remoteproc + RPMsg.
 */

#pragma once

// CPU family: Kryo (4x Cortex-A78 + 4x Cortex-A55).
static constexpr int    VELA_QCS6490_CPU_CORES        = 8;
static constexpr double VELA_QCS6490_CPU_MAX_FREQ_MHZ = 2600.0;
static constexpr double VELA_QCS6490_CPU_MIN_FREQ_MHZ = 300.0;
static constexpr double VELA_QCS6490_CPU_TEMP_MAX_C   = 105.0;

// GPU: Adreno 643.
static constexpr int    VELA_QCS6490_GPU_SHADER_CORES = 8;
static constexpr double VELA_QCS6490_GPU_MAX_FREQ_MHZ = 850.0;
static constexpr double VELA_QCS6490_GPU_MIN_FREQ_MHZ = 300.0;
static constexpr double VELA_QCS6490_GPU_TEMP_MAX_C   = 95.0;
static constexpr const char* VELA_QCS6490_GPU_DEVFREQ_PATH = "/sys/class/devfreq/";
static constexpr const char* VELA_QCS6490_GPU_DEVICE_PATH  = "/sys/devices/platform/";
static constexpr const char* VELA_QCS6490_V3D_DEV_PATH     = "/dev/dri/card0";
static constexpr const char* VELA_QCS6490_V3D_RENDER_PATH  = "/dev/dri/renderD128";

// Memory: LPDDR5 up to 12GB.
static constexpr long long VELA_QCS6490_MEM_8GB_BYTES = 8LL * 1024 * 1024 * 1024;
static constexpr long long VELA_QCS6490_MEM_4GB_BYTES = 4LL * 1024 * 1024 * 1024;
static constexpr double VELA_QCS6490_MEM_SPEED_MHZ    = 3200.0;
static constexpr double VELA_QCS6490_MEM_BW_GBS       = 51.2;
static constexpr int    VELA_QCS6490_MEM_BUS_BITS     = 128;

// PCIe: none on QCS6490 (external via expansion only).
static constexpr int    VELA_QCS6490_PCIE_GEN    = 0;
static constexpr int    VELA_QCS6490_PCIE_LANES  = 0;
static constexpr double VELA_QCS6490_PCIE_BW_GBS = 0.0;
static constexpr const char* VELA_QCS6490_PCIE_RC_PATH = "";

// USB.
static constexpr const char* VELA_QCS6490_XHCI_DRIVER_PATH = "/sys/bus/platform/drivers/dwc3";
static constexpr int VELA_QCS6490_USB3_PORT_COUNT = 2;
static constexpr int VELA_QCS6490_USB2_PORT_COUNT = 2;

// Ethernet.
static constexpr const char* VELA_QCS6490_ETH_IFACE      = "eth0";
static constexpr int         VELA_QCS6490_ETH_SPEED_MBPS = 1000;

// Camera/video (V4L2, multiple CSI + ISP).
static constexpr const char* VELA_QCS6490_CSI_DEVICE0 = "/dev/video0";
static constexpr const char* VELA_QCS6490_CSI_DEVICE1 = "/dev/video1";
static constexpr const char* VELA_QCS6490_MEDIA_CTL0  = "/dev/media0";
static constexpr const char* VELA_QCS6490_UNICAM_PATH = "/sys/bus/platform/devices";

// Display nodes.
static constexpr const char* VELA_QCS6490_DRM_CARD0 = "/dev/dri/card0";
static constexpr const char* VELA_QCS6490_DRM_CARD1 = "/dev/dri/card1";

// Audio.
static constexpr const char* VELA_QCS6490_I2S_DEVICE = "/sys/bus/platform/devices";
static constexpr const char* VELA_QCS6490_ALSA_CARDS = "/proc/asound/cards";

// Serial (QUP console UARTs).
static constexpr const char* VELA_QCS6490_DEBUG_UART = "/dev/ttyS0";
static constexpr const char* VELA_QCS6490_UART1      = "/dev/ttyS1";

// Thermal thresholds.
static constexpr const char* VELA_QCS6490_THERMAL_CPU_ZONE   = "/sys/class/thermal/thermal_zone0";
static constexpr const char* VELA_QCS6490_THERMAL_SB_ZONE    = "/sys/class/thermal/thermal_zone1";
static constexpr double      VELA_QCS6490_THERMAL_THROTTLE_C = 95.0;
static constexpr double      VELA_QCS6490_THERMAL_SHUTDOWN_C = 110.0;

// GPIO via libgpiod.
static constexpr const char* VELA_QCS6490_GPIOCHIP_PATH = "/dev/gpiochip0";
static constexpr int         VELA_QCS6490_GPIO_PIN_COUNT = 128;

// Hexagon DSP remoteproc + RPMsg.
static constexpr int         VELA_QCS6490_PRU_REMOTEPROC_ID = 0;
static constexpr const char* VELA_QCS6490_PRU_REMOTEPROC_BASE = "/sys/class/remoteproc";
static constexpr const char* VELA_QCS6490_PRU_RPMSG_DEVICE = "/dev/rpmsg_q6";
static constexpr const char* VELA_QCS6490_PRU_FIRMWARE_DEFAULT = "qcs6490-q6-fw";

// Storage paths.
static constexpr const char* VELA_QCS6490_MMCBLK_PATH = "/dev/mmcblk0";
static constexpr const char* VELA_QCS6490_NVME_PATH   = "/dev/nvme0n1";

// PMIC/power supply.
static constexpr const char* VELA_QCS6490_POWER_SUPPLY_PATH = "/sys/class/power_supply";

// Optional external AI accelerators.
static constexpr const char* VELA_QCS6490_HAILO8_PCIE_VENDOR = "1e60";
static constexpr const char* VELA_QCS6490_CORAL_USB_VENDOR   = "18d1";

// Platform identification.
static constexpr const char* PLATFORM_NAME      = "Vela QCS6490 Platform";
static constexpr const char* PLATFORM_SOC       = "Qualcomm QCS6490";
static constexpr const char* PLATFORM_IOBRIDGE  = "Hexagon DSP";
static constexpr const char* VELA_QCS6490_DT_MODEL_PATH = "/proc/device-tree/model";
