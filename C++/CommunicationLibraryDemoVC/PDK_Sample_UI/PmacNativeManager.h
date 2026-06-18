// PmacNativeManager.h
// Opaque Pointer (Handle) 기반의 순수 Native C++ 다중 장비 매니저
#pragma once

#include <string>
#include <map>
#include "PDK_Library.h"

// 장비 한 대를 독립적으로 관리하는 Native C++ 클래스 (내부적으로 DLL Handle 소유)
class PmacNativeDevice
{
public:
    std::string ipAddress;
    HPmacDevice hDevice = nullptr;

    PmacNativeDevice() { 
        hDevice = PmacCreateDevice(); 
    }
    
    ~PmacNativeDevice() { 
        if (hDevice) {
            PmacDestroyDevice(hDevice); 
            hDevice = nullptr;
        }
    }

    // 복사 방지 (Handle 이중 해제 방지)
    PmacNativeDevice(const PmacNativeDevice&) = delete;
    PmacNativeDevice& operator=(const PmacNativeDevice&) = delete;

    // 이동 의미론 지원 (std::map 삽입용)
    PmacNativeDevice(PmacNativeDevice&& other) noexcept {
        ipAddress = other.ipAddress;
        hDevice = other.hDevice;
        other.hDevice = nullptr;
    }
    PmacNativeDevice& operator=(PmacNativeDevice&& other) noexcept {
        if (this != &other) {
            if (hDevice) PmacDestroyDevice(hDevice);
            ipAddress = other.ipAddress;
            hDevice = other.hDevice;
            other.hDevice = nullptr;
        }
        return *this;
    }
};

class PmacNativeManager
{
public:
    PmacNativeManager() {}
    ~PmacNativeManager() { DisconnectAll(); }

    void AddDevice(const std::string& ip)
    {
        if (m_devices.find(ip) == m_devices.end())
        {
            PmacNativeDevice dev;
            dev.ipAddress = ip;
            m_devices.insert(std::make_pair(ip, std::move(dev)));
        }
    }

    bool Connect(const std::string& ip)
    {
        AddDevice(ip);
        return PmacConnect(m_devices[ip].hDevice, ip.c_str(), "root", "deltatau");
    }

    void Disconnect(const std::string& ip)
    {
        auto it = m_devices.find(ip);
        if (it != m_devices.end())
        {
            PmacDisconnect(it->second.hDevice);
        }
    }

    void DisconnectAll()
    {
        for (auto& kv : m_devices)
        {
            PmacDisconnect(kv.second.hDevice);
        }
    }

    bool IsConnected(const std::string& ip)
    {
        auto it = m_devices.find(ip);
        if (it == m_devices.end()) return false;
        return PmacIsConnected(it->second.hDevice);
    }

    std::string SendCommand(const std::string& ip, const std::string& command)
    {
        if (!IsConnected(ip))
            return "Error: Not connected";

        const char* responsePtr = nullptr;
        auto it = m_devices.find(ip);
        int ret = PmacGetResponse(it->second.hDevice, command.c_str(), &responsePtr);
        
        if (ret == SUCCESS && responsePtr != nullptr)
            return std::string(responsePtr);

        return "Error: Command failed";
    }

private:
    std::map<std::string, PmacNativeDevice> m_devices;
};

