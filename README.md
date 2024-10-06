Emulmon is a naive attempt at creating a hwmon device which reports fake readings. It is useful for testing purposes.

Currently it creates two attributes with custom labels:
- temperature
- fan speed

The reported values are constant.