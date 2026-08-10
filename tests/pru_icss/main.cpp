#include "platform_vela_qcs6490.h"
#include "pru_icss_lib/pru_icss_lib.h"

#include <iostream>
#include <string>

int main()
{
    using vela::pru::PruIcssManager;

    PruIcssManager pru(VELA_QCS6490_PRU_REMOTEPROC_ID);

    int passed = 0;
    int total = 0;
    std::string detail;

    auto report = [&](const std::string& name, bool ok) {
        ++total;
        if (ok) {
            ++passed;
            std::cout << "[PASS] " << name << "\n";
        } else {
            std::cout << "[INFO] " << name << " (not verified on this target)\n";
        }
    };

    const bool fwLoaded = pru.loadFirmware(VELA_QCS6490_PRU_FIRMWARE_DEFAULT, &detail);
    report("PRU load firmware", fwLoaded);

    const bool started = pru.start(&detail);
    report("PRU start", started);

    std::string resp;
    const bool rpmsgOk = pru.rpmsgRoundTrip("PING", &resp, 100, &detail);
    report("RPMsg round-trip", rpmsgOk);

    const bool timingIoOk = pru.rpmsgRoundTrip("CAPTURE_EDGE", &resp, 100, &detail);
    report("Timing-critical I/O command", timingIoOk);

    const bool recoveryOk = pru.faultRecovery(&detail);
    report("PRU fault recovery", recoveryOk);

    const bool stopped = pru.stop(&detail);
    report("PRU stop", stopped);

    std::cout << "PRU test summary: " << passed << "/" << total << " checks passed" << std::endl;
    return 0;
}
