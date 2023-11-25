#!/usr/bin/env python3
# Modified from
# https://trezor.github.io/cython-hidapi/examples.html#finding-devices
import hidapi
import time
import sys


def find_device():
    for device in hidapi.enumerate():
        # print(device)
        # dir(device)
        # keys = []# list(device_dict.keys())
        # keys.sort()
        # for key in keys:
        #    print("%s : %s" % (key, device_dict[key]))

        print(f"path: {device.path}")
        print(f"vendor: {device.vendor_id:04x}")
        print(f"product: {device.product_id:04x}")
        print()
        if device.vendor_id == 0x0416:
            return device

    raise Exception("No device")


device = find_device()


def read_and_print(h):
    # read back the answer
    print("Read the data")
    d = h.read(65)
    if not d:
        print("<no data>")

    else:
        count = d[0]
        if not count:
            # Empty packet
            print("<Empty packet>")
            return
        print(d[1 : count + 1])

        # Print extra bytes we got if there are any nonzeros.
        left = d[count + 1 :]
        if any(b for b in left):
            print("Left:", left)


def write_str(h, s):
    data = s.encode("utf-8")
    data = bytes([len(data)]) + data
    print(f"Writing {data!r}")
    h.write(data)


try:
    print("Opening the device")

    h = hidapi.Device(info=device, blocking=True)

    print("Manufacturer: %s" % h.get_manufacturer_string())
    print("Product: %s" % h.get_product_string())
    print("Serial No: %s" % h.get_serial_number_string())

    write_str(h, "Yowza")
    read_and_print(h)
    time.sleep(2)
    write_str(h, "Yowza") # stop
    read_and_print(h)
    sys.exit(0)

    while True:
        try:
            # write some data to the device
            write_str(h, "Hello, world!\n")

            # wait
            time.sleep(0.05)

            read_and_print(h)

            time.sleep(0.05)

        except KeyboardInterrupt:
            break

    print("Closing the device")
    h.close()

except IOError as ex:
    print(ex)
    print("Try with `sudo` if you aren't root.")
    print("Otherwise, you probably don't have the hard-coded device.")
    print("Update the h.open() line in this script with the one")
    print("from the enumeration list output above and try again.")

print("Done")
