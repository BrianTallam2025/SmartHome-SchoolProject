from abc import ABC, abstractmethod

class AutomationRule(ABC):
    def __init__(self, name: str):
        self.rule_name = name

    @abstractmethod
    def evaluate(self):
        pass

class ScheduleRule(AutomationRule):
    def __init__(self, name: str, time: str, target_device, target_value: float):
        super().__init__(name)
        self.scheduled_time = time
        self.target_device = target_device
        self.target_value = target_value

    def evaluate(self):
        print(f"\n--- Executing Schedule Rule: {self.rule_name} (Scheduled for {self.scheduled_time}) ---")
        if hasattr(self.target_device, 'target_temperature'):
            self.target_device.set_temperature(self.target_value)
        else:
            self.target_device.set_power(self.target_value > 0)

class TriggerRule(AutomationRule):
    def __init__(self, name: str, trigger_sensor, action_device):
        super().__init__(name)
        self.trigger_sensor = trigger_sensor
        self.action_device = action_device

    def evaluate(self):
        print(f"\n--- Evaluating Trigger Rule: {self.rule_name} ---")
        if not self.trigger_sensor.motion_detected:
            print(f"Condition met: No motion detected by {self.trigger_sensor.name}.")
            self.action_device.set_power(False)
        else:
            print("Condition not met: Active motion detected. Leaving device state unchanged.")

class SmartHomeHub:
    def __init__(self):
        self.devices = []
        self.automation_engine = []

    def add_device(self, device):
        self.devices.append(device)

    def add_rule(self, rule: AutomationRule):
        self.automation_engine.append(rule)

    def run_automations(self):
        for rule in self.automation_engine:
            rule.evaluate()

    def show_system_status(self):
        print("\n================ CURRENT SMART HOME STATUS ================")
        for device in self.devices:
            device.show_status()
        print("===========================================================")