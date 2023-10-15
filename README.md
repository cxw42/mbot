# mbot

Dependencies: `sudo apt install python3-hidapi libhidapi-dev`

- Set up the mbot to output serial data at 57600
- Run `sudo python3 enumerate.py`.  It sees the device and gets data,
  but the data is corrupted.

## A test

Outputting only 0x49 at 57600 over 2.4G gave results:
```
...
b'\x1d\x86\x98\x86\x98\x86\x98\x86\x98\x86\x98\x86\x98\x86\x98\x86\x98\x86\x98\x86\x98\x86\x98\x86\x98\x86\x98\x86\x98\x86'
b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'
b'\x1d\x98\x86\x98\x86\x98\x86\x98\x86\x98\x86\x98\x86\x98\x86\x98\x86\x98\x86\x86\x98\x86\x98\x86\x98\x86\x98\x86\x98\x86'
...
```

## Refs
- <https://github.com/Ted-CAcert/mymbot/wiki/mBot-2.4G-Wireless-Serial>
- <http://docs.makeblock.com/diy-platform/en/electronic-modules/main-control-boards/images/mcore_Mcore.png>
- <https://cdn.sparkfun.com/datasheets/Dev/Arduino/Other/CH340DS1.PDF>
- <https://trezor.github.io/cython-hidapi/examples.html#finding-devices>
- <https://docs.rs/hidapi/latest/hidapi/struct.HidApi.html>
- <http://docs.makeblock.com/diy-platform/en/electronic-modules/communicators/2-4g-wireless-serial.html>
