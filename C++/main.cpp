#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>

using namespace std;


enum class DeviceType { Lighting, Thermostat, speaker, sensor};

class SmartDevice {
    protected:
        string name;
        DeviceType type;
        bool isPoweredOn;

    public:
        SmartDevice(string Name, DeviceType Type)
            : name(Name), type(Type), isPoweredOn(false) {}

        virtual ~SmartDevice() = default;

        string getName() const { return name; }
        DeviceType getType() const { return type; }
        bool getPowerStatus() const { return isPoweredOn; }

    virtual void setPower(bool state) {
        isPoweredOn = state;
        cout << "[Device] " << name << (isPoweredOn ? " turned ON." : " turned OFF.") << endl;
    }

    virtual void displayStatus() const = 0;
};


int main() {
    cout << "Hello, World!" << endl;
    return 0;
}
