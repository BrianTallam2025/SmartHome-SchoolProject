from abc import ABC, abstractmethod
from enum import Enum

class DeviceType(Enum):
    LIGHTING = 1
    THERMOSTAT = 2
    SPEAKER = 3
    SENSOR = 4


class SmartDevice(ABC):
    def __init__(self, name: str, device_type: DeviceType):
        self.name = name
        self.type = device_type
        self.is_powered_on = False

    def set_power(self, state: bool):
        self.is_powered_on = state
        status = "ON" if self.is_powered_on else "OFF"
        print(f"[Device] {self.name} turned {status}.")

    @abstractmethod
    def show_status(self):
        pass


class SmartLight(SmartDevice):
    def __init__(self, name: str, default_kelvin: int = 3000):
        super().__init__(name, DeviceType.LIGHTING)
        self.color_temperature = default_kelvin

    def set_color_temperature(self, kelvin: int):
        self.color_temperature = kelvin
        print(f"[Lighting] {self.name} color temperature set to {self.color_temperature}K.")

    def show_status(self):
        status = "ON" if self.is_powered_on else "OFF"
        print(f"Light: {self.name} | Power: {status} | Temp: {self.color_temperature}K")


class SmartThermostat(SmartDevice):
    def __init__(self, name: str, default_temp: float = 72.0):
        super().__init__(name, DeviceType.THERMOSTAT)
        self.target_temperature = default_temp
        self.is_powered_on = True  # Thermostats default to running

    def set_temperature(self, temp: float):
        self.target_temperature = temp
        print(f"[Thermostat] {self.name} target temperature set to {self.target_temperature}°F.")

    def show_status(self):
        print(f"Thermostat: {self.name} | Target Temp: {self.target_temperature}°F")


class MotionSensor(SmartDevice):
    def __init__(self, name: str):
        super().__init__(name, DeviceType.SENSOR)
        self.motion_detected = False
        self.is_powered_on = True

    def trigger_motion(self, detected: bool):
        self.motion_detected = detected
        status = "detected movement!" if self.motion_detected else "reports clear."
        print(f"[Sensor] {self.name} {status}")

    def show_status(self):
        status = "DETECTED" if self.motion_detected else "CLEAR"
        print(f"Sensor: {self.name} | Motion: {status}")