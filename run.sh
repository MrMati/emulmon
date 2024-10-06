#!/bin/sh
(make && sudo rmmod emulmon >/dev/null 2>&1 || true) && sudo insmod emulmon.ko