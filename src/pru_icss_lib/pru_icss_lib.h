#pragma once

#include <cstdint>
#include <string>

namespace vela::pru {

class PruIcssManager {
  public:
    explicit PruIcssManager(int remoteproc_id = 0);

    bool loadFirmware(const std::string& firmware, std::string* detail = nullptr) const;
    bool start(std::string* detail = nullptr) const;
    bool stop(std::string* detail = nullptr) const;
    bool restart(std::string* detail = nullptr) const;
    bool isRunning() const;

    bool rpmsgRoundTrip(const std::string& request,
                        std::string* response,
                        int timeout_ms,
                        std::string* detail = nullptr) const;

    bool faultRecovery(std::string* detail = nullptr) const;

    int remoteprocId() const { return remoteproc_id_; }
    std::string remoteprocPath() const;

  private:
    int remoteproc_id_;

    static bool pathExists(const std::string& path);
    static bool writeFile(const std::string& path, const std::string& value, std::string* detail);
    static std::string readFile(const std::string& path);
    static bool runCmdOk(const std::string& cmd, std::string* out = nullptr);
};

} // namespace vela::pru
