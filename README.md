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

### Same, but back to 8N1

```
...
b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'
b"\x1d0123456789:;<=>?@A'()*+,-./0"
b'\x1d56789:;<=>?@ABCDEF*+,-./0123'
b'\x1d56789:;<=>?@ABCDEF/012345678'
...
```

### enumerate.py change --- back to just reading all bytes

```
...
b'\x1dNOPQRSTUVWXYZ[\\]^_CDEFGHIJKLM'
b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'
b'\x1dNOPQRSTUVWXYZ[\\]^_EFGHIJKLMNO'
b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'
b'\x1dPQRSTUVWXYZ[\\]^_`aHIJKLMNOPQR'
b'\x1dSTUVWXYZ[\\]^_`abcdHIJKLMNOPQR'
...
```

### Back to 8N2

```
...
b'\x1dabcdefghijklmnopqr<=>?@ABCDEF'
b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'
b'\x1dGHIJKLMNOPQRSTUVWX56789:;<=>?'
...
```

Looking pretty close!

### trying some changes to enumerate.py

```
...
Sync up
.b'\x1dXYZ[\\]^_`abcdefghiDEFGHIJKLM'
Sync up
.b'\x1dGHIJKLMNOPQRSTUVWX456789:;<='
...
```

### Trying enumerate.py change: always read fixed-sized buffer

```
...
b'\x1dwxyz{|}~!"#$%&\'()*cdefghijklm'
b'wxyz{|}~!"#$%&\'()*cdefghijklm'
b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'
b''
b'\x1dnopqrstuvwxyz{|}~![\\]^_`abcde'
...
```

There never seems to be anything else in the buffer except zeros:
```
$ sudo ./enumerate.py |grep '^Left: b...'
Left: b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'
Left: b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'
Left: b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'
```

### Slowing down the Arduino's character rate even more!

Delay 9us -> 20us

```
...
b'tuvwxyz{|}~!"#$%&\'PQRSTUVWXYZ'
b'[\\]^_`abcdefghijkl./012345678'
b'9:;<=>?@ABCDEFGHIJjklmnopqrst'
b'uvwxyz{|}~!"#$%&\'(@ABCDEFGHIJ'
b"KLMNOPQRSTUVWXYZ[\\'()*+,-./01"
b'23456789:;<=>?@ABC~!"#$%&\'()*'
b'+,-./0123456789:;<defghijklmn'
b'opqrstuvwxyz{|}~!"vwxyz{|}~!"'
...
```

### Trying back on 8N1

Seems to be repeated data:
```
Read the data
b'@ABCDEFGHIJKLMNOPQ6789:;<=>?@'
b'ABCDEFGHIJKLMNOPQR6789:;<=>?@'
b'ABCDEFGHIJKLMNOPQR9:;<=>?@ABC'
b'DEFGHIJKLMNOPQRSTU9:;<=>?@ABC'
b'DEFGHIJKLMNOPQRSTU:;<=>?@ABCD'
b'EFGHIJKLMNOPQRSTUV;<=>?@ABCDE'
b'FGHIJKLMNOPQRSTUVW;<=>?@ABCDE'
b'FGHIJKLMNOPQRSTUVW<=>?@ABCDEF'
```

## Two-way comms

### Test 1 - echo char

Read one char at a time and echo it back - works fine over USB
with `stty -F /dev/ttyUSB0 9600 raw`.

### Test 2 - echo str

```
# in terminal 1
$ cat /dev/ttyUSB0
Hello, world!
Hello, world!^J^PHello, world!^J^P^PHello, world!^J^P^P^PHello, world!^J^P^P^P^PHello, world!^J^P^P^P^P^P^C
```

### Try adding a prompt

```
~/proj/mbot$ stty -F /dev/ttyUSB0 9600 raw
~/proj/mbot$ cat /dev/ttyUSB0
>=>
>==>^J>
>===>^J>^J>
>====>^J>^J>^J>
>=====>^J>^J>^J>^J>
>======>^J>^J>^J>^J>^J>
>=======>^J>^J>^J>^J>^J>^J>
>========>^J>^J>^J>^J>^J>^J>^J>
>=========>^J>^J>^J>^J>^J>^J>^J>^J>
>==========>^J>^J>^J>^J>^J>^J>^J>^J>^J>
>===========>^J>^J>^J>^J>^J>^J>^J>^J>^J>^J>
>============>^J>^J>^J>^J>^J>^J>^J>^J>^J>^J>^J>
>=============>^J>^J>^J>^J>^J>^J>^J>^J>^J>^J>^J>^J>
>==============>^J>^J>^J>^J>^J>^J>^J>^J>^J>^J>^J>^J>^J>
>^C
```

??!!

### Next

Tried stubbing out serialEventRun() per
<https://forum.arduino.cc/t/unexpected-input-from-serialeventrun/591470/3>
but got infinite spam.

### Trying serialEvent

I saw the prompt but no other output.  But at least it didn't spam me
with prompts!

### Trying serialEvent sample

Same issue --- newlines seem to not get consumed.

### Hmm...

I noticed that `readBytes` was syntax-highlighted.  I changed it to
`bytesRead` and am no longer getting spammed.

## Refs
- <https://github.com/Ted-CAcert/mymbot/wiki/mBot-2.4G-Wireless-Serial>
- <http://docs.makeblock.com/diy-platform/en/electronic-modules/main-control-boards/images/mcore_Mcore.png>
- <https://cdn.sparkfun.com/datasheets/Dev/Arduino/Other/CH340DS1.PDF>
- <https://trezor.github.io/cython-hidapi/examples.html#finding-devices>
- <https://docs.rs/hidapi/latest/hidapi/struct.HidApi.html>
- <http://docs.makeblock.com/diy-platform/en/electronic-modules/communicators/2-4g-wireless-serial.html>
