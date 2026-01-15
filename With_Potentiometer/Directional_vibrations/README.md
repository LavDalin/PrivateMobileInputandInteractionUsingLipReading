This prototype is designed to create spatial haptic sensations using an array of vibration motors controlled by an Arduino. By grouping motors and firing them in specific sequences, the system can simulate directional movement (Up, Down, Left, and Right).

This is particularly useful for tactile navigation aids, wearable haptic vests, or immersive sensory research.

Features:
Intuitive Serial Control: Control the "flow" of vibration simply by typing directions into your computer.

Dynamic Intensity: Integrated potentiometer support allows you to adjust the vibration strength in real-time.

Structured Motor Groups: Uses a 2D array architecture to manage 6 different motor points organized into vertical and horizontal axes.

Smooth Transitions: Built-in pulse and delay logic ensures the user can distinctively feel the direction of the sequence.

Hardware Setup
Wiring Diagram
The system expects a potentiometer on A0 and motors connected to the following PWM-capable pins:

Vertical Array:

Top: Pins 3, 5

Middle: Pins 6, 9

Bottom: Pins 12, 12

Horizontal Array:

Left Side: Pin 9

Right Side: Pin 6

[!NOTE] Since vibration motors often draw more current than an Arduino pin can safely provide, it is highly recommended to use a transistor (like the PN2222) or a Darlington Array (ULN2003) to drive the motors.

Operating Instructions
Initialize: Power your Arduino and open the Serial Monitor (set to 9600 Baud).

Adjust Intensity: Turn the potentiometer to set your desired vibration strength.

Command the Direction:

Type up to feel the vibration move from your feet/bottom toward your head/top.

Type down to reverse the flow.

Type left or right for horizontal patterns.

Customization
You can easily tune the "speed" of the sensation by changing these values at the top of the code:

C++

const int pulseDuration = 300;      // How long each group vibrates
const int delayBetweenGroups = 1000; // Time to wait before the next sweep
A Note on Pin 12
In your current code, the "Bottom" group uses Pin 12 twice. If you intend to have two distinct motors for the bottom group, you might want to move one to another PWM-capable pin (like Pin 10 or 11) to ensure you have full control over the intensity.