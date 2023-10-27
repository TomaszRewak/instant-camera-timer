# About the project

This modification of the Instax Mini 12 camera allows you to take pictures with a 10 second delay (while not losing the ability to take pictures immediately). The original camera does not have such a function.

The mechanism is simple:
- Press the button of the camera for less than 1 second to take a picture immediately (just like in the original camera).
- Press the button of the camera for more than 2 seconds to take a picture after a 10 second delay.

# Disclaimer

This project is not affiliated with Fujifilm Holdings Corporation.

If you choose to replicate this project, you do so at your own risk. I am not responsible for any damage or harm caused by it. You may damage your camera, void your warranty, and incur other unexpected expenses or losses. By following this guide, you accept all responsibility and liability.

# How it works

The ATTiny13A microcontroller is inserted between the capture button of the camera and the camera's circuit board. The microcontroller monitors the button state and redefines the behavior of its output.

The project consists of the .c program file that is to be loaded onto the microcontroller and a sequence of instructions for installing the microcontroller in the camera.

# What you need

Components:
- ATTiny13A microcontroller (either the PU or the SU version - I used the PU version for testing and the SU version for the final setup)
- 100kΩ resistor
- Wires
- Solder
- Isolation tape

Tools:
- Microcontroller programmer (I used USBasp)
- Soldering iron
- Voltmeter
- Test board
- Screwdriver
- Tweezers

# Step-by-step instructions

### 1. Compile the program and upload it to the microcontroller

The program is written in C and can be compiled with the avr-gcc compiler.

Run the following commands in the Linux terminal (works also with the WSL):

```bash
avr-gcc main.c -o main.elf -mmcu=attiny13a -DF_CPU=1200000UL -Os
avr-objcopy main.elf -O ihex main.hex
```

First command compiles the program and creates an executable file. Second command converts the executable file to a hex file that can be uploaded to the microcontroller.

Now you need to upload the program to the microcontroller. I did it by running the following command in Windows PowerShell:

```bash
avrdude -c usbasp -p attiny13a -U flash:w:main.hex
```

Please note that you may need to change the programmer type (usbasp) based on the programmer you use.

### 2. Remove the camera's back and front covers

Use a screwdriver to remove the screws on the sides of the camera. Then remove the back cover.

Unfortunately, one of the screws (that holds both the back and the front covers together) is located under a clip-in picture exit slot (which cannot be easily removed). I did not know that, and so I broke the plastic part of this screw holder. I recommend you to be a bit more careful when dealing with it (if it's even possible to remove the cover without breaking it).

After removing the back cover, the front cover should come off easily.

Warning: the camera's flash capacitor may still be charged! Do not touch the board or any circuitry inside the camera with your hands or any metal objects. I've learned it the hard way xD

### 3. Test the microcontroller using a test board

It's a good idea to test the microcontroller before making any modifications to the camera. Just plug it into a test board and use a voltmeter to check if its behavior is as expected.

You will need to follow this circuit diagram (we will also use it later, when connecting the microcontroller to the camera):

<p align="center">
  <img src="https://github.com/TomaszRewak/instant-camera-timer/blob/master/resources/Circuit.png?raw=true" width=800/>
</p>

The expected behavior is as follows:
- When idle, the `Board` pin (`PB0`) should read 5V.
- When you short the `Button` pin (`PB1`) to the ground for less than a second, the `Board` pin (`PB0`) should read 0V for 0.5 seconds and then revert back to 5V.
- When you short the `Button` pin (`PB1`) to the ground for more than 2 seconds, the `Board` pin (`PB0`) should maintain the 5V state for 10 seconds, then switch to 0V for 0.5 seconds and then revert back to 5V.

<p align="center">
  <img src="https://github.com/TomaszRewak/instant-camera-timer/blob/master/resources/Step%201.jpg?raw=true" width=800/>
</p>

### 4. Locate the 4 points of interest on the camera's circuit board

The circuit board is located on the front of the camera. On it, you will find 4 points of interest:

1. `3.3V`: Just right to the camera's lens you will find a small silver pin labeled as 3.3V. It should have a voltage of 3.3V between it and the ground - but only when the camera is turned on. It will be out voltage source.
2. `ground`: Bottom left to the flash you will find a black wire that comes from the direction of the battery. The point where it's soldered to the board is our ground.
3. `button out`: At the top of the camera you will find the capture button (the one you press to take a picture). It has two wires connected to it. When the camera is turned on, the voltage between the bottom wire and the ground should be 5V. That's our `button out` pin.
4. `board in`: The right most wire at the bottom of the board is the same wire that is connected to the `button out` pin. The place where it's soldered to the board is our `board in` pin.

<p align="center">
  <img src="https://github.com/TomaszRewak/instant-camera-timer/blob/master/resources/Step%202.jpg?raw=true" width=800/>
</p>

### 5. Cut the wire that connects the button to the board

Cut the wire that connects the `button out` pin to the `board in` pin. Be careful when doing it, as there are two wires of the same color running next to each other.

You should cut it just right to the flash of the camera (as shown in the picture below), to ensure a proper length of both ends of the wire for later use.

<p align="center">
  <img src="https://github.com/TomaszRewak/instant-camera-timer/blob/master/resources/Step%203.jpg?raw=true" width=800/>
</p>

### 6. Solder wires to the `button out` and `board in` pins

Solder wires to the `button out` and `board in` pins. We will first use them for testing, but later on we will connect them directly to the microcontroller.

### 7. Connect the camera to the test board

To test if everything works as expected, you can connect the camera to the test board. Just fallow the previously presented circuit diagram. If everything is connected correctly, the camera should already exhibit the new behavior.

<p align="center">
  <img src="https://github.com/TomaszRewak/instant-camera-timer/blob/master/resources/Step%204.jpg?raw=true" width=800/>
</p>

### 8. Solder the microcontroller to the camera

Now it's time to solder the microcontroller to the camera. Use the wires you previously soldered to the `button out` and `board in` pins, as well as the two end of the wire that you cut in the step 5. Don't forget to solder in the 100kΩ resistor as well.

Make sure the the wire lengths are correct for the microcontroller to sit comfortably left to the flash.

<p align="center">
  <img src="https://github.com/TomaszRewak/instant-camera-timer/blob/master/resources/Step%205.jpg?raw=true" width=800/>
</p>

(please don't judge my soldering skills :x)

### 9. Wrap the microcontroller in isolation tape

Wrap the microcontroller in isolation tape to prevent it from shorting with the camera's circuit board. Place it left to the flash and make sure that it doesn't move from that position.

Make sure that wires are not blocking any of the sensors and don't collide with any dents in the camera's front cover. Also be mindful of any moving parts (like the rotating lens cover).

<p align="center">
  <img src="https://github.com/TomaszRewak/instant-camera-timer/blob/master/resources/Step%206.jpg?raw=true" width=800/>
</p>

### 10. Put the camera back together

Put the camera's front and back covers back together.

Be extra careful when pressing in the rotating lens cover. It's very easy to break its plastic holders (as I did xD).

### 11. Enjoy!

Now you can enjoy your new camera feature!

<p align="center">
  <img src="https://github.com/TomaszRewak/instant-camera-timer/blob/master/resources/Photo.jpg?raw=true" width=800/>
</p>