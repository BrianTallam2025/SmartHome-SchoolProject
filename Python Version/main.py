from devices import SmartLight, SmartThermostat, MotionSensor
from automation import SmartHomeHub, ScheduleRule, TriggerRule

def main():
    my_hub = SmartHomeHub()

    # 1. Setup our starting devices
    living_room_light = SmartLight("Living Room Light", default_kelvin=5500)
    main_thermostat = SmartThermostat("Main Thermostat", default_temp=70.0)
    hallway_sensor = MotionSensor("Hallway Sensor")

    my_hub.add_device(living_room_light)
    my_hub.add_device(main_thermostat)
    my_hub.add_device(hallway_sensor)

    # 2. Add our automation rules
    my_hub.add_rule(ScheduleRule("Morning Temp Schedule", "09:00 AM", main_thermostat, 73.0))
    my_hub.add_rule(TriggerRule("Vacant Room Energy Saver", hallway_sensor, living_room_light))

    # 3. Interactive Menu Loop
    while True:
        print("\n--- 🏠 SMART HOME INTERACTIVE CONTROL PANEL ---")
        print("1. View Current System Status")
        print("2. Toggle Living Room Light (ON/OFF)")
        print("3. Change Thermostat Temperature")
        print("4. Trigger Hallway Motion (True/False)")
        print("5. Run Automation Rules Engine")
        print("6. Exit Program")
        
        choice = input("\nSelect an option (1-6): ").strip()

        if choice == "1":
            my_hub.show_system_status()
            
        elif choice == "2":
            current_state = living_room_light.is_powered_on
            living_room_light.set_power(not current_state)
            
        elif choice == "3":
            try:
                new_temp = float(input("Enter new target temperature (°F): "))
                main_thermostat.set_temperature(new_temp)
            except ValueError:
                print("❌ Invalid input! Please enter a valid number.")
                
        elif choice == "4":
            motion_input = input("Is there movement in the hallway? (yes/no): ").strip().lower()
            if motion_input in ["yes", "y", "true"]:
                hallway_sensor.trigger_motion(True)
            else:
                hallway_sensor.trigger_motion(False)
                
        elif choice == "5":
            my_hub.run_automations()
            
        elif choice == "6":
            print("\nShutting down Smart Home Central Hub. Goodbye! 👋")
            break
            
        else:
            print("❌ Invalid choice, please pick a number from 1 to 6.")

if __name__ == "__main__":
    main()