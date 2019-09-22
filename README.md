# Geek Room Sensor

To flash:
```bash
esptool --port COM3 --baud 115200 write_flash --flash_size=detect 0x00000 .\ai-thinker-0.9.5.2.bin --verify
```