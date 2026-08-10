# Vela-HAL – Hardware Abstraction Library for Vela Platform

## Overview

Vela-HAL is a comprehensive hardware abstraction and testing suite targeting the **Vela Platform** (Sitara AM62x/AM64x SoC + PRU-ICSS south-bridge). It provides a modular, extensible architecture for testing and exercising hardware interfaces and system components commonly found in automotive and industrial applications.

> **Port history**: Originally developed for NVIDIA Jetson AGX Orin (Tegra234).
> Ported to Vela Platform (Sitara AM62x/AM64x + PRU-ICSS) — all platform-specific paths, constants,
> and device nodes updated for the Pi 5 hardware.

### Target Platform

| Property | Value |
|---|---|
| SoC | Sitara AM62x/AM64x (quad-core Cortex-A76 @ 2.4 GHz) |
| South-bridge | PRU-ICSS (USB 3, PCIe Gen3 x1, GPIO, I2S, CSI/DSI) |
| GPU | VideoCore VII (12 shader quad cores, 800 MHz max) |
| Memory | 4 GB / 8 GB LPDDR4X-4267 |
| AI Accelerator | Hailo-8 HAT+ (PCIe, ~26 INT8 TOPS) or Coral USB Edge TPU (~4 TOPS) |
| OS | Vela OS (64-bit) / Ubuntu 24.04 ARM64 |

### Key Features

- **Modular Library Architecture**: Separate libraries for CPU, GPU, DLA (Hailo-8/Coral), video codec (V4L2 M2M), camera (Unicam/PRU-ICSS), audio (PRU-ICSS I2S), PCIe, USB (xhci-hcd), GPIO (gpiochip4), storage, Ethernet, and more.
- **Vela Platform Header**: Centralised constants in `src/platform_vela_qcs6490.h` (device paths, thermal thresholds, memory bandwidth).
- **Cross-compilation via Docker**: ARM64 Ubuntu 24.04 container (`rpi5-builder`).
- **Comprehensive Testing**: Unit tests, integration tests, and deployment workflows.
- **Documentation Generation**: Integrated Doxygen support.
- **Code Quality Tools**: clang-format and cppcheck.

## Prerequisites

### System Requirements

- **Operating System**: Linux (Ubuntu 20.04+ recommended for host)
- **Compiler**: GCC 11+ or Clang 14+
- **Build Tools**:
  - CMake 3.10+
  - Ninja build system
  - Make
  - Git
- Docker installed and running (for cross-compilation)

## Build Process

The project supports two primary build methods:

1. **Native Compilation**: Build directly on a Vela Platform running 64-bit OS.
2. **Docker-based Cross-Compilation**: Build for ARM64 on any Linux host using the `rpi5-builder` container.

### Native Compilation (on Pi 5)

#### Install Dependencies
```bash
sudo apt update
sudo apt install -y build-essential cmake ninja-build git \
                    protobuf-compiler libprotobuf-dev libprotoc-dev \
                    libgrpc++-dev libsqlite3-dev libboost-system-dev \
                    libboost-date-time-dev doxygen clang-format cppcheck
```

#### Build and Run Tests
```bash
make build
make run
```

### Docker-based Cross-Compilation

The Docker image is based on `ubuntu:22.04` (ARM64) and is named `Vela`.

#### Build Process

1. **Ensure Docker is Installed**
   ```bash
   make docker-install
   ```

2. **Build the Docker Image**
   ```bash
   make docker-image
   ```

   This creates a Docker image named `Vela` based on `ubuntu:22.04` with all necessary ARM64 build dependencies pre-installed.

3. **Build**
   ```bash
   make docker-build
   ```

   This command:
   - Runs the Docker container with the project mounted
   - Configures the build system with Ninja generator
   - Builds all targets
   - Outputs binaries in the `build/cross/` directory

4. **Deploy and Run on Target Pi 5**

```bash
   # For Pi 5 Primary (192.168.1.3)
   make run-a
   # For Pi 5 Secondary (192.168.1.13)
   make run-b
   # Specify custom IP address
   make run-b IP_B=192.168.200.2
```

## Platform-Specific Notes

### AI Inference (DLA)
- **Hailo-8** (PCIe HAT+): detected at `/dev/hailo0`, kernel module `hailo_pci` (vendor `0x1e60`)
- **Coral USB Edge TPU**: detected via USB vendor `18d1` at `/dev/apex_0`

### GPU (VideoCore VII)
- devfreq sysfs: `/sys/class/devfreq/fde00000.gpu/`
- DRM: `/dev/dri/card0`, `/dev/dri/renderD128`
- Module: `v3d`

### Video Codec
- H.264 encode/decode: `sitara_compatible-codec` at `/dev/video10` (encoder), `/dev/video11` (decoder)
- H.265/HEVC decode: `rpivid` at `/dev/video18` or `/dev/video19`

### Camera (CSI)
- Unicam CSI-2 controller: `/sys/bus/platform/devices/1f00110000.csi`
- V4L2 device: `/dev/video0`, media controller: `/dev/media0`

### Debug UART
- `/dev/ttyAMA0` (115200 8N1 by default on Pi 5)

## Make Targets

For the full list of supported commands, run:

```bash
make help
```

## Contributing

1. Follow the established code formatting (use `make format`)
2. Run static analysis before submitting (`make cppcheck`)
3. Add tests for new functionality
4. Update documentation as needed
