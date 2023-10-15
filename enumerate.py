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

    def sync():
        # It always seems to output 0x1d as a length byte.  Since we're only
        # sending ASCII, use that as a sync byte.
        print("Sync up")
        while True:
            print(".", end="")
            sys.stdout.flush()
            byte = h.read(1)
            if not byte:
                time.sleep(0.05)
            elif byte == b"\x1d":
                break

    # read back the answer
    print("Read the data")
    try:
        while True:
            #sync()
            #d = h.read(0x1D)
            d = h.read(65)
            #print(d)
            if d:
                count = d[0]
                print(d[1:count+1])
                print('Left:', d[count+1:])
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
