#!/bin/bash

VENDER_ID=0x04d9 #YOU NEED TO CHANGE HERE

PRODUCT_ID=0x4545 #YOU NEED TO CHANGE HERE

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

COMMAND_ATTACH="kextunload /System/Library/Extensions/AppleUSBTopCase.kext/Contents/PlugIns/AppleUSBTCKeyboard.kext/"

COMMAND_DETACH="kextload /System/Library/Extensions/AppleUSBTopCase.kext/Contents/PlugIns/AppleUSBTCKeyboard.kext/"

sudo $DIR/usbattwatch \
    -i \
    -a "$COMMAND_ATTACH" \
    -d "$COMMAND_DETACH" \
    $VENDER_ID $PRODUCT_ID

