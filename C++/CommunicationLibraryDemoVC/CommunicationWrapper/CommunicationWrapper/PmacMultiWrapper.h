#pragma once

#include <vcclr.h>
#using <mscorlib.dll>

using namespace System;
using namespace ODT::Common::Core;
using namespace ODT::PowerPmacComLib;

namespace LegacyPmacWrapper {

    public ref class PmacDeviceWrapper
    {
    private:
        ODT::PowerPmacComLib::ISyncGpasciiCommunicationInterface^ _gpasciiDevice;
        System::String^ _ipAddress;

    public:
        PmacDeviceWrapper(System::String^ ipAddress)
        {
            _ipAddress = ipAddress;
            _gpasciiDevice = nullptr;
        }

        ~PmacDeviceWrapper()
        {
            this->!PmacDeviceWrapper();
        }

        !PmacDeviceWrapper()
        {
            Disconnect();
        }

        bool Connect(System::String^ user, System::String^ password)
        {
            if (_gpasciiDevice != nullptr && IsConnected) return true;

            // 이전 접속 실패로 남은 찌꺼기(Deadlock 원인) 강제 청소
            Disconnect();

            try
            {
                _gpasciiDevice = ODT::PowerPmacComLib::Connect::CreateSyncGpascii(ODT::PowerPmacComLib::CommunicationGlobals::ConnectionTypes::SSH, nullptr);
                _gpasciiDevice->ConnectGpAscii(_ipAddress, 22, user, password);
                return IsConnected;
            }
            catch (Exception^)
            {
                Disconnect(); // 실패 시 찌꺼기 객체 즉시 파괴
                return false;
            }
        }

        void Disconnect()
        {
            if (IsConnected)
            {
                _gpasciiDevice->DisconnectGpascii();
            }
        }

        System::String^ SendCommand(System::String^ command)
        {
            if (!IsConnected) return "Error: Not connected";

            String^ response = "";
            auto status = _gpasciiDevice->GetResponse(command, response);
            
            if (status == ODT::PowerPmacComLib::Status::Ok)
            {
                return response;
            }
            return "Error: Command failed";
        }

        property bool IsConnected
        {
            bool get()
            {
                if (_gpasciiDevice == nullptr) return false;
                return _gpasciiDevice->GpAsciiConnected;
            }
        }
    };

    public ref class PmacManagerWrapper
    {
    private:
        System::Collections::Generic::Dictionary<System::String^, PmacDeviceWrapper^>^ _devices;

    public:
        PmacManagerWrapper()
        {
            _devices = gcnew Collections::Generic::Dictionary<String^, PmacDeviceWrapper^>();
        }

        ~PmacManagerWrapper()
        {
            this->!PmacManagerWrapper();
        }

        !PmacManagerWrapper()
        {
            DisconnectAll();
            if (_devices != nullptr)
            {
                _devices->Clear();
                _devices = nullptr;
            }
        }

        void AddDevice(System::String^ ipAddress)
        {
            if (!_devices->ContainsKey(ipAddress))
            {
                _devices->Add(ipAddress, gcnew PmacDeviceWrapper(ipAddress));
            }
        }

        PmacDeviceWrapper^ GetDevice(System::String^ ipAddress)
        {
            if (_devices->ContainsKey(ipAddress))
            {
                return _devices[ipAddress];
            }
            return nullptr;
        }

        void DisconnectAll()
        {
            for each (auto kvp in _devices)
            {
                kvp.Value->Disconnect();
            }
        }
    };
}
