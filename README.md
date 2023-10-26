# Build and upload

On Linux:

```bash
avr-gcc main.c -o main.elf -mmcu=attiny13 -DF_CPU=1200000UL -Os
avr-objcopy main.elf -O ihex main.hex
```

On Windows:

```bash
avrdude -c usbasp -p attiny13 -U flash:w:main.hex
```

# Board

100kΩ