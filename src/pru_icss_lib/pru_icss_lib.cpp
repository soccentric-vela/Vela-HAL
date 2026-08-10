#include "pru_icss_lib.h"

#include <array>
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace vela::pru {

namespace {

std::string trim(std::string v)
{
    while (!v.empty() && (v.back() == '\n' || v.back() == '\r' || v.back() == ' ' || v.back() == '\t')) {
        v.pop_back();
    }
    return v;
}

std::string captureCmd(const std::string& cmd)
{
    std::array<char, 512> buffer{};
    std::string out;
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        return out;
    }
    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe) != nullptr) {
        out += buffer.data();
    }
    pclose(pipe);
    return trim(out);
}

} // namespace

PruIcssManager::PruIcssManager(int remoteproc_id) : remoteproc_id_(remoteproc_id)
{
}

std::string PruIcssManager::remoteprocPath() const
{
    return "/sys/class/remoteproc/remoteproc" + std::to_string(remoteproc_id_);
}

bool PruIcssManager::pathExists(const std::string& path)
{
    struct stat st {};
    return stat(path.c_str(), &st) == 0;
}

bool PruIcssManager::writeFile(const std::string& path, const std::string& value, std::string* detail)
{
    std::ofstream out(path);
    if (!out.is_open()) {
        if (detail) {
            *detail = "Cannot open " + path + ": " + std::strerror(errno);
        }
        return false;
    }
    out << value;
    if (!out.good()) {
        if (detail) {
            *detail = "Write failed for " + path;
        }
        return false;
    }
    return true;
}

std::string PruIcssManager::readFile(const std::string& path)
{
    std::ifstream in(path);
    if (!in.is_open()) {
        return "";
    }
    std::ostringstream ss;
    ss << in.rdbuf();
    return trim(ss.str());
}

bool PruIcssManager::runCmdOk(const std::string& cmd, std::string* out)
{
    const std::string data = captureCmd(cmd + " 2>&1");
    if (out) {
        *out = data;
    }
    return !data.empty();
}

bool PruIcssManager::loadFirmware(const std::string& firmware, std::string* detail) const
{
    const std::string base = remoteprocPath();
    if (!pathExists(base)) {
        if (detail) {
            *detail = "Missing remoteproc path: " + base;
        }
        return false;
    }

    std::string d;
    if (!stop(&d)) {
        // continue: stop may fail when already stopped
    }

    if (!writeFile(base + "/firmware", firmware, detail)) {
        return false;
    }
    if (detail) {
        *detail = "Firmware set to " + firmware;
    }
    return true;
}

bool PruIcssManager::start(std::string* detail) const
{
    return writeFile(remoteprocPath() + "/state", "start", detail);
}

bool PruIcssManager::stop(std::string* detail) const
{
    return writeFile(remoteprocPath() + "/state", "stop", detail);
}

bool PruIcssManager::restart(std::string* detail) const
{
    std::string stop_detail;
    stop(&stop_detail);
    const bool ok = start(detail);
    if (!ok && detail && detail->empty()) {
        *detail = stop_detail;
    }
    return ok;
}

bool PruIcssManager::isRunning() const
{
    const std::string state = readFile(remoteprocPath() + "/state");
    return state == "running";
}

bool PruIcssManager::rpmsgRoundTrip(const std::string& request,
                                    std::string* response,
                                    int timeout_ms,
                                    std::string* detail) const
{
    const std::string dev = "/dev/rpmsg_pru30";
    if (!pathExists(dev)) {
        if (detail) {
            *detail = "RPMsg endpoint missing: " + dev;
        }
        return false;
    }

    const int fd = open(dev.c_str(), O_RDWR | O_NONBLOCK);
    if (fd < 0) {
        if (detail) {
            *detail = "Failed to open RPMsg endpoint: " + std::string(std::strerror(errno));
        }
        return false;
    }

    ssize_t written = write(fd, request.data(), request.size());
    if (written < 0) {
        if (detail) {
            *detail = "RPMsg write failed: " + std::string(std::strerror(errno));
        }
        close(fd);
        return false;
    }

    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);
    struct timeval tv {};
    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000) * 1000;

    const int ready = select(fd + 1, &rfds, nullptr, nullptr, &tv);
    if (ready <= 0) {
        if (detail) {
            *detail = ready == 0 ? "RPMsg read timeout" : "RPMsg select failed";
        }
        close(fd);
        return false;
    }

    std::array<char, 512> buf{};
    const ssize_t n = read(fd, buf.data(), buf.size());
    close(fd);
    if (n <= 0) {
        if (detail) {
            *detail = "RPMsg read failed";
        }
        return false;
    }

    if (response) {
        response->assign(buf.data(), static_cast<size_t>(n));
    }
    if (detail) {
        *detail = "RPMsg round-trip ok";
    }
    return true;
}

bool PruIcssManager::faultRecovery(std::string* detail) const
{
    std::string d;
    const bool stopped = stop(&d);
    const bool started = start(&d);
    if (detail) {
        *detail = (stopped ? "Stop ok; " : "Stop failed; ") + std::string(started ? "restart ok" : "restart failed");
    }
    return started;
}

} // namespace vela::pru
