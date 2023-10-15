# mbot

Dependencies: `sudo apt install python3-hidapi libhidapi-dev`

- Set up the mbot to output serial data at 57600
- Run `sudo python3 enumerate.py`.  It sees the device and gets data,
  but the data is corrupted.

## udev

1. Make sure the 2.4G adapter is unplugged
2. `sudo cp 99-Makeblock2.4G.rules /etc/udev/rules.d/`
3. `sudo udevadm control --reload-rules && sudo udevadm trigger`
4. Plug the 2.4G adapter back in.

You should have a symlink `/dev/mbot24g` pointing to the adapter.

## A test

### 2.4G, 57600

Outputting only 0x49 (`I`) at 57600 over 2.4G gave results:
```
...
b'\x1d\x86\x98\x86\x98\x86\x98\x86\x98\x86\x98\x86\x98\x86\x98\x86\x98\x86\x98\x86\x98\x86\x98\x86\x98\x86\x98\x86\x98\x86'
b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'
b'\x1d\x98\x86\x98\x86\x98\x86\x98\x86\x98\x86\x98\x86\x98\x86\x98\x86\x98\x86\x86\x98\x86\x98\x86\x98\x86\x98\x86\x98\x86'
...
```

### Same over wired serial, 9600
```
$ stty -F /dev/ttyUSB0 9600 raw
$ cat /dev/ttyUSB0
IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII...
```

### 2.4G, 115200
```
...
b'\x1dIIIIIIIIIIIIIIIIIIIIIIIIIIII'
b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'
b'\x1dIIIIIIIIIIIIIIIIIIIIIIIIIIII'
...
```

## Refs
- <https://github.com/Ted-CAcert/mymbot/wiki/mBot-2.4G-Wireless-Serial>
- <http://docs.makeblock.com/diy-platform/en/electronic-modules/main-control-boards/images/mcore_Mcore.png>
- <https://cdn.sparkfun.com/datasheets/Dev/Arduino/Other/CH340DS1.PDF>
- <https://trezor.github.io/cython-hidapi/examples.html#finding-devices>
- <https://docs.rs/hidapi/latest/hidapi/struct.HidApi.html>
- <http://docs.makeblock.com/diy-platform/en/electronic-modules/communicators/2-4g-wireless-serial.html>
