# Vela-HAL Build and Deployment

## Purpose and scope

Vela-HAL is the C++17 hardware abstraction and validation library for the
QCS6490 platform and its peripherals. CMake builds static/shared libraries,
protobuf contracts, tests, and CPack packages. The root Makefile supports
native Ninja builds, ARM64 Docker builds, quality tools, and target-device
deployment.

## 1. Prerequisites

- Linux, GNU Make, CMake 3.10+, Ninja, GCC/Clang with C++17, and Threads.
- Protobuf/gRPC, SQLite3, and the platform libraries required by the selected
  modules; GoogleTest is needed for the CTest targets.
- clang-format, cppcheck, Doxygen/Graphviz for the optional quality targets.
- Docker/buildx for ARM64/QEMU builds; `sshpass`, SSH, target hardware, and
  credentials for deployment. Override `IP_A`, `IP_B`, `HAL_SSH_USER`, and
  `HAL_SSH_PASS` rather than relying on defaults.

## 2. Setup

From the leaf, install native dependencies or build the `Vela` ARM64 image
with `make docker-install` and `make docker-image`. `make setup-completion`
can add the checked-in Make completion script to `~/.bashrc`.

## 3. Build and compile

- `make configure` configures `build/native` with Ninja; `make build` compiles
  libraries, protobuf code, and applications.
- `make docker-configure` and `make docker-build` compile ARM64 into
  `build/cross`; `make all` aliases `docker-build`.
- `make run [name] [args...]` runs native applications with `LD_LIBRARY_PATH`;
  `make list` shows native and cross binaries.

## 4. Tests and lint

`make test` runs native CTest and `make docker-test` runs CTest in ARM64/QEMU.
`make format`, `make cppcheck`, and `make doc` invoke clang-format, cppcheck,
and Doxygen. Their prerequisite checks fail when the corresponding tools are
missing; install them before using those targets.

## 5. Packaging and artifacts

Native libraries and applications are in `build/native/lib` and `bin`; cross
outputs are in `build/cross/lib` and `bin`. `make package-deb`, `make
package-rpm`, and `make package-ipk` invoke CPack; `make package` produces DEB
and IPK. Docker equivalents are `make docker-package-deb`,
`docker-package-rpm`, `docker-package-ipk`, and `docker-package`.

## 6. Deployment, install, and run

Install native CMake artifacts with `sudo cmake --install build/native`. For
cross targets use `make deploy-a IP_A=... HAL_SSH_USER=... HAL_SSH_PASS=...`
or the corresponding `deploy-b`; `make deploy` does both and copies `lib/` and
`bin/` to `/tmp/acu-test`. `make run-a [app] [args...]` and `make run-b ...`
run applications remotely from that directory.

## 7. Cleanup

`make clean` removes `build/`; `make docker-clean` removes the container cross
build. `make docker-shell` opens the ARM64 build environment.

## 8. Hardware, cloud, and manual verification caveats

Compilation and CTest do not prove QCS6490 device-node, firmware, driver,
permission, cabling, Hailo/Coral, camera, audio, or peripheral behavior.
Physical deployment and remote execution require the exact board revision and
network access. Validate module results on hardware and review the documented
platform paths against the active kernel before accepting a release.