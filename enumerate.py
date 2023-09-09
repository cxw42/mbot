# Modified from
# https://trezor.github.io/cython-hidapi/examples.html#finding-devices
import hidapi
import time

device=None
for device in hidapi.enumerate():
    #print(device)
    #dir(device)
    #keys = []# list(device_dict.keys())
    #keys.sort()
    #for key in keys:
    #    print("%s : %s" % (key, device_dict[key]))

    print(f'path: {device.path}')
    print(f'vendor: {device.vendor_id:04x}')
    print(f'product: {device.product_id:04x}')
    print()
    if device.vendor_id==0x0416:
        break

if device is None:
    print("No device")
    sys.exit(0)

try:
    print("Opening the device")

    h = hidapi.Device(info=device, blocking=False)

    print("Manufacturer: %s" % h.get_manufacturer_string())
    print("Product: %s" % h.get_product_string())
    print("Serial No: %s" % h.get_serial_number_string())

    # enable non-blocking mode
    #h.set_nonblocking(1)

    # write some data to the device
    print("Write the data")
    h.write(bytes([0, 63, 35, 35] + [0] * 61 + [0x0a]))

    # wait
    #time.sleep(0.05)

    # read back the answer
    print("Read the data")
    while True:
        d = h.read(64)
        if d:
            print(d)
        else:
            time.sleep(1)

    print("Closing the device")
    h.close()

except IOError as ex:
    print(ex)
    print("You probably don't have the hard-coded device.")
    print("Update the h.open() line in this script with the one")
    print("from the enumeration list output above and try again.")

print("Done")
