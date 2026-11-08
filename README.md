# Automatic Touchless Hand Washer

An Arduino-based touchless hand-washing prototype. An HC-SR04 ultrasonic sensor detects a hand or object near the outlet and activates a water pump through a relay module.

> **Project status:** Prototype firmware. Verify relay polarity, pump supply, wiring, plumbing, and enclosure before operating with water.

## Features

- Contactless object detection using an HC-SR04 ultrasonic sensor.
- Default detection threshold of 10 cm.
- Relay-controlled water pump.
- Default pump runtime of 5 seconds.
- Default cooldown of 2 seconds.
- Serial Monitor output at 9600 baud.
- Echo timeout handling: a missing echo does not activate the pump.
- Correct ultrasonic distance calculation using the round-trip echo time.

## Hardware requirements

| Component | Purpose |
|---|---|
| Arduino Uno or compatible board | Main controller |
| HC-SR04 ultrasonic sensor | Detects a hand near the dispenser |
| Relay module | Switches the pump supply |
| DC water pump | Delivers water |
| External pump power supply | Supplies the pump's rated voltage and current |
| Water reservoir, tubing and nozzle | Stores and dispenses water |
| Jumper wires | Electrical connections |

The pump voltage and current are not specified by the firmware. Select a supply and relay with ratings appropriate for the pump.

## Pin connections

| Module signal | Arduino pin |
|---|---:|
| HC-SR04 TRIG | D9 |
| HC-SR04 ECHO | D10 |
| Relay IN | D7 |
| HC-SR04 VCC | 5V* |
| HC-SR04 GND | GND |
| Relay module VCC/GND | As required by the relay module |

*The common HC-SR04 is a 5 V module. Confirm the requirements of your exact sensor. If using a 3.3 V controller, level-shift the ECHO signal as required.

### Pump power wiring

Do **not** power the water pump from an Arduino GPIO pin or the Arduino 5 V pin. Use a separate power supply that matches the pump's rated voltage and current. Connect the pump through the relay's correctly rated contact terminals, following the markings on your relay module. Ensure the controller and relay input have the required common reference where applicable.

## How it works

1. The Arduino sends a 10-microsecond trigger pulse to the HC-SR04.
2. The sensor returns an ECHO pulse. Its duration represents the sound's round-trip travel time.
3. The firmware calculates distance in centimetres:

   `distance_cm = (echo_time_us * 0.0343) / 2`

   The division by two accounts for the sound travelling to the target and back.
4. If a valid distance is greater than 0 cm and no more than 10 cm, the Arduino activates the relay.
5. The pump runs for 5 seconds, switches off, and the controller waits 2 seconds before checking for another activation.
6. If no echo arrives within 30 ms, the reading is treated as invalid and the pump remains off.

## Configuration

The following constants are at the top of `hand_washer.ino`:

| Constant | Default | Description |
|---|---:|---|
| `DETECTION_DISTANCE_CM` | 10 | Maximum distance that triggers the pump |
| `PUMP_RUNTIME_MS` | 5000 | Pump ON duration in milliseconds |
| `COOLDOWN_MS` | 2000 | Waiting period after the pump switches off |
| `ECHO_TIMEOUT_US` | 30000 | Maximum time to wait for the sensor echo |

For example, to run the pump for 3 seconds, change `PUMP_RUNTIME_MS` from `5000` to `3000`.

## Upload instructions

1. Install the [Arduino IDE](https://www.arduino.cc/en/software).
2. Connect the Arduino board to your computer using USB.
3. Open `hand_washer.ino` in the Arduino IDE.
4. Select the correct board under **Tools → Board**.
5. Select the correct serial port under **Tools → Port**.
6. Click **Verify** to compile, then click **Upload**.
7. Open **Tools → Serial Monitor** and select **9600 baud**.
8. Test sensor readings and relay operation with the pump disconnected first.
9. Connect and test the pump only after confirming the relay logic and power wiring.

The sketch uses standard Arduino core functions and does not require third-party libraries.

## Relay polarity

The firmware is configured for an **active-HIGH relay input**:

- `HIGH`: relay energized; pump ON.
- `LOW`: relay de-energized; pump OFF.

Some relay boards are active-LOW. If yours is active-LOW, change `setPump()` so that ON writes `LOW` and OFF writes `HIGH`. Test the relay with the pump disconnected and confirm that the pump remains OFF during startup and reset.

## Troubleshooting

| Problem | Checks |
|---|---|
| Serial Monitor continually shows `No echo` | Check sensor power, TRIG/ECHO wiring, ground, and sensor orientation. |
| Distance readings are unstable | Secure the sensor; avoid angled, soft, or highly irregular target surfaces. |
| Pump remains ON | Check active-HIGH/active-LOW relay logic and the sensor's field of view. |
| Pump does not turn ON | Check the measured distance, detection threshold, relay input, and pump supply. |
| Arduino resets when the pump switches | Use a suitable separate pump supply; check current draw, wiring, and electrical noise. |

## Safety precautions

- Keep water away from the Arduino, USB connector, and exposed electrical contacts.
- Use a suitable low-voltage DC pump and correctly rated switching hardware.
- Provide appropriate fusing and electrical protection for the pump supply.
- Enclose the electronics in a suitable splash-resistant enclosure.
- Disconnect power before changing wiring.
- This prototype is not a certified hygiene device or a commercial water-dispensing product.

## Current limitations and possible improvements

The current sketch uses blocking `delay()` calls while the pump runs and during cooldown. It does not include water-level sensing, leak detection, flow monitoring, dry-run protection, distance filtering, or a user interface.

Possible future improvements include:

- Non-blocking pump timing with `millis()`.
- Filtering or averaging ultrasonic readings.
- Low-water-level detection.
- Water-flow monitoring and dry-run protection.
- A splash-resistant enclosure and improved plumbing.
- Adjustable pump duration and detection distance.

## Repository structure

```text
Automatic_Hand_Washer/
├── hand_washer.ino
├── README.md
├── LICENSE
└── .gitignore
```

## License

This project is released under the MIT License. See [LICENSE](LICENSE).
