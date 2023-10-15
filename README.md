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

### 2.4G, 115200, outputting ASCIITable text

```
...
b'\x1d1101\r\n~, dec: 126,5, oct: 45'
b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'
b'\x1d 39, oct: 71, bin:t: 122, bi'
b'\x1d dec: 89, hex: 59, oct: 153,'
...
```

### 2.4G, 115200 **8N2**, outputting ASCIITable text

```
...
b'\x1d 40, oct: 100, binB, dec: 66'
b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'
b'\x1ddec: 70, hex: 46, in: 100011'
b'\x1d1001\r\nJ, dec: 74, t: 113, bi'
...
```

Definitely better.  Ted's comment

> The mBot's serial interface speed of 115200 is faster than the RF
> transmission. So, if sending bigger blocks of data, make sure to put in some
> delay statements, to avoid loosing data because of buffer overflows.

may be the issue.

### Same, but with hackish delay between chars

```
...
b'\x1d, oct: , bin: \x10,, \xb8\x06\x07\x07\xb8\xb8\xb8, b'
b'\x1dbin: \xb8\xb8\xb8\xb8\xb8!\xb8\xb8\x06\x07\x07\xb8\xb8\x06\x07\x07\xb8\xb8\xb8, bi'
b'\x1d\xb8\xb8\x06\x07\x07\xb8\xb8\xb8\x100, dec: \xb8\xb8\x06\x07\x07\xb8\xb8\xb8\x101,'
b'\x1d hex: \xb8\xb8\xb8\x06\x07\x07\xb8\xb8\xb8, o\x07\xb8\xb8\xb8, oct:'
b'\x1dec: \x06\x07\x07\xb8\xb8\xb8, hex: \x06\xb8, oct: \x07\x07'
...
```

### Same, but just print the chars

```
...
'\x1d123456789:;<=>?@ABABCDEFGHIJ'
b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'
b'\x1d=>?@ABCDEFGHIJKLMNwxyz{|}~!"'
...
```

## Refs
- <https://github.com/Ted-CAcert/mymbot/wiki/mBot-2.4G-Wireless-Serial>
- <http://docs.makeblock.com/diy-platform/en/electronic-modules/main-control-boards/images/mcore_Mcore.png>
- <https://cdn.sparkfun.com/datasheets/Dev/Arduino/Other/CH340DS1.PDF>
- <https://trezor.github.io/cython-hidapi/examples.html#finding-devices>
- <https://docs.rs/hidapi/latest/hidapi/struct.HidApi.html>
- <http://docs.makeblock.com/diy-platform/en/electronic-modules/communicators/2-4g-wireless-serial.html>
