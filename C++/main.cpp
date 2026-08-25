#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <limits>

enum class DeviceType { Lighting, Thermostat, Speaker, Sensor };

// Base abstract class for smart devices
class SmartDevice {
protected:
    std::string name;
    DeviceType type;
    bool isPoweredOn;

public:
    SmartDevice(const std::string& name, DeviceType type, bool initialPower = false)
        : name(name), type(type), isPoweredOn(initialPower) {}

    virtual ~SmartDevice() = default;

    std::string getName() const { return name; }
    DeviceType getType() const { return type; }
    bool getPowerStatus() const { return isPoweredOn; }

    virtual void setPower(bool state) {
        isPoweredOn = state;
        std::cout << "[Device] " << name << (isPoweredOn ? " turned ON." : " turned OFF.") << std::endl;
    }

    virtual void displayStatus() const = 0;
};

// SmartLight device implementation
class SmartLight : public SmartDevice {
private:
    int colorTemperature; // in Kelvin

public:
    SmartLight(const std::string& name, int defaultKelvin = 3000)
        : SmartDevice(name, DeviceType::Lighting, false), colorTemperature(defaultKelvin) {}

    void setColorTemperature(int kelvin) {
        colorTemperature = kelvin;
        std::cout << "[Lighting] " << name << " color temperature set to " << colorTemperature << "K." << std::endl;
    }

    int getColorTemperature() const { return colorTemperature; }

    void displayStatus() const override {
        std::string status = isPoweredOn ? "ON" : "OFF";
        std::cout << "Light: " << name << " | Power: " << status << " | Temp: " << colorTemperature << "K" << std::endl;
    }
};

// SmartThermostat device implementation
class SmartThermostat : public SmartDevice {
private:
    double targetTemperature; // in Fahrenheit

public:
    SmartThermostat(const std::string& name, double defaultTemp = 72.0)
        : SmartDevice(name, DeviceType::Thermostat, true), targetTemperature(defaultTemp) {}

    void setTemperature(double temp) {
        targetTemperature = temp;
        std::cout << "[Thermostat] " << name << " target temperature set to " << targetTemperature << "°F." << std::endl;
    }

    double getTemperature() const { return targetTemperature; }

    void displayStatus() const override {
        std::cout << "Thermostat: " << name << " | Target Temp: " << targetTemperature << "°F" << std::endl;
    }
};

// MotionSensor device implementation
class MotionSensor : public SmartDevice {
private:
    bool motionDetected;

public:
    MotionSensor(const std::string& name)
        : SmartDevice(name, DeviceType::Sensor, true), motionDetected(false) {}

    void triggerMotion(bool detected) {
        motionDetected = detected;
        std::string status = motionDetected ? "detected movement!" : "reports clear.";
        std::cout << "[Sensor] " << name << " " << status << std::endl;
    }

    bool isMotionDetected() const { return motionDetected; }

    void displayStatus() const override {
        std::string status = motionDetected ? "DETECTED" : "CLEAR";
        std::cout << "Sensor: " << name << " | Motion: " << status << std::endl;
    }
};

// Base abstract class for automation rules
class AutomationRule {
protected:
    std::string ruleName;

public:
    AutomationRule(const std::string& name) : ruleName(name) {}
    virtual ~AutomationRule() = default;

    std::string getRuleName() const { return ruleName; }
    virtual void evaluate() = 0;
};

// Schedule-based automation rule
class ScheduleRule : public AutomationRule {
private:
    std::string scheduledTime;
    std::shared_ptr<SmartDevice> targetDevice;
    double targetValue;

public:
    ScheduleRule(const std::string& name, const std::string& time, std::shared_ptr<SmartDevice> device, double value)
        : AutomationRule(name), scheduledTime(time), targetDevice(device), targetValue(value) {}

    void evaluate() override {
        std::cout << "\n--- Executing Schedule Rule: " << ruleName << " (Scheduled for " << scheduledTime << ") ---" << std::endl;
        if (auto thermostat = std::dynamic_pointer_cast<SmartThermostat>(targetDevice)) {
            thermostat->setTemperature(targetValue);
        } else if (targetDevice) {
            targetDevice->setPower(targetValue > 0);
        }
    }
};

// Event/Sensor trigger-based automation rule
class TriggerRule : public AutomationRule {
private:
    std::shared_ptr<MotionSensor> triggerSensor;
    std::shared_ptr<SmartDevice> actionDevice;

public:
    TriggerRule(const std::string& name, std::shared_ptr<MotionSensor> sensor, std::shared_ptr<SmartDevice> device)
        : AutomationRule(name), triggerSensor(sensor), actionDevice(device) {}

    void evaluate() override {
        std::cout << "\n--- Evaluating Trigger Rule: " << ruleName << " ---" << std::endl;
        if (triggerSensor && !triggerSensor->isMotionDetected()) {
            std::cout << "Condition met: No motion detected by " << triggerSensor->getName() << "." << std::endl;
            if (actionDevice) {
                actionDevice->setPower(false);
            }
        } else {
            std::cout << "Condition not met: Active motion detected. Leaving device state unchanged." << std::endl;
        }
    }
};

// Smart home hub orchestrator
class SmartHomeHub {
private:
    std::vector<std::shared_ptr<SmartDevice>> devices;
    std::vector<std::shared_ptr<AutomationRule>> automationEngine;

public:
    void addDevice(std::shared_ptr<SmartDevice> device) {
        devices.push_back(device);
    }

    void addRule(std::shared_ptr<AutomationRule> rule) {
        automationEngine.push_back(rule);
    }

    void runAutomations() {
        for (const auto& rule : automationEngine) {
            rule->evaluate();
        }
    }

    void showSystemStatus() const {
        std::cout << "\n================ CURRENT SMART HOME STATUS ================" << std::endl;
        for (const auto& device : devices) {
            device->displayStatus();
        }
        std::cout << "===========================================================" << std::endl;
    }
};

// Helper for interactive pause
void pauseMenu() {
    std::cout << "\nPress 0 to go back to the menu: ";
    std::string dummy;
    std::getline(std::cin, dummy);
}

// Main interactive control panel
int main() {
    SmartHomeHub myHub;

    auto livingRoomLight = std::make_shared<SmartLight>("Living Room Light", 5500);
    auto mainThermostat = std::make_shared<SmartThermostat>("Main Thermostat", 70.0);
    auto hallwaySensor = std::make_shared<MotionSensor>("Hallway Sensor");

    myHub.addDevice(livingRoomLight);
    myHub.addDevice(mainThermostat);
    myHub.addDevice(hallwaySensor);

    myHub.addRule(std::make_shared<ScheduleRule>("Morning Temp Schedule", "09:00 AM", mainThermostat, 73.0));
    myHub.addRule(std::make_shared<TriggerRule>("Vacant Room Energy Saver", hallwaySensor, livingRoomLight));

    while (true) {
        std::cout << "\n--- SMART HOME INTERACTIVE CONTROL PANEL ---" << std::endl;
        std::cout << "1. View Current System Status" << std::endl;
        std::cout << "2. Toggle Living Room Light (ON/OFF)" << std::endl;
        std::cout << "3. Change Thermostat Temperature" << std::endl;
        std::cout << "4. Trigger Hallway Motion (True/False)" << std::endl;
        std::cout << "5. Run Automation Rules Engine" << std::endl;
        std::cout << "6. Exit Program" << std::endl;

        std::cout << "\nSelect an option (1-6): ";
        std::string choice;
        if (!std::getline(std::cin, choice)) {
            break;
        }

        // Trim whitespace
        choice.erase(0, choice.find_first_not_of(" \t\n\r"));
        choice.erase(choice.find_last_not_of(" \t\n\r") + 1);

        if (choice == "1") {
            myHub.showSystemStatus();
            pauseMenu();
        } else if (choice == "2") {
            bool currentState = livingRoomLight->getPowerStatus();
            livingRoomLight->setPower(!currentState);
            pauseMenu();
        } else if (choice == "3") {
            std::cout << "Enter new target temperature (°F): ";
            std::string tempStr;
            std::getline(std::cin, tempStr);
            try {
                double newTemp = std::stod(tempStr);
                mainThermostat->setTemperature(newTemp);
            } catch (const std::exception&) {
                std::cout << "Invalid input! Please enter a valid numerical value." << std::endl;
            }
            pauseMenu();
        } else if (choice == "4") {
            std::cout << "Is there movement in the hallway? (yes/no): ";
            std::string motionInput;
            std::getline(std::cin, motionInput);

            std::string lowerInput = motionInput;
            std::transform(lowerInput.begin(), lowerInput.end(), lowerInput.begin(), ::tolower);
            lowerInput.erase(0, lowerInput.find_first_not_of(" \t\n\r"));
            lowerInput.erase(lowerInput.find_last_not_of(" \t\n\r") + 1);

            if (lowerInput == "yes" || lowerInput == "y" || lowerInput == "true") {
                hallwaySensor->triggerMotion(true);
            } else {
                hallwaySensor->triggerMotion(false);
            }
            pauseMenu();
        } else if (choice == "5") {
            myHub.runAutomations();
            pauseMenu();
        } else if (choice == "6") {
            std::cout << "\nShutting down Smart Home Central Hub. Goodbye." << std::endl;
            break;
        } else {
            std::cout << "Invalid choice, please pick a number from 1 to 6." << std::endl;
        }
    }

    return 0;
}
