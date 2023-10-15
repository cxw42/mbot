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

try:
    print("Opening the device")

    h = hidapi.Device(info=device, blocking=True)

    print("Manufacturer: %s" % h.get_manufacturer_string())
    print("Product: %s" % h.get_product_string())
    print("Serial No: %s" % h.get_serial_number_string())

    ## write some data to the device
    # print("Write the data")
    # h.write(bytes([0, 63, 35, 35] + [0] * 61 + [0x0a]))

    # wait
    # time.sleep(0.05)

    # read back the answer
    print("Read the data")
    try:
        while True:
            count = int.from_bytes(h.read(1), sys.byteorder)
            if count == 0:
                time.sleep(0.05)
                continue

            d = h.read(count)
            if d:
                print(d)
    except KeyboardInterrupt:
        pass

    print("Closing the device")
    h.close()

except IOError as ex:
    print(ex)
    print("Try with `sudo` if you aren't root.")
    print("Otherwise, you probably don't have the hard-coded device.")
    print("Update the h.open() line in this script with the one")
    print("from the enumeration list output above and try again.")

print("Done")
