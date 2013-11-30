# Mac Built-in Keyboard Disabler

This disable(/re-enable) smarty macbook's built-in keyboard __when your external keyboard connected(/disconnected)__.

## Setup

This have dependency for [libusbx](http://libusbx.org/) to watch hotplug. If you don't have it:

```
$ brew install libusbx
```

Next, clone this repository:

```
$ git clone https://github.com/kumar8600/mac-builtin-keyboard-disabler
$ cd mac-builtin-keyboard-disabler
```

Next, in `./keyboard-disabler.sh`, edit `VENDER\_ID` and `PRODUCT\_ID` for your keyboard's one (It can get by executing `$ system_profiler SPUSBDataType`):

```
#!/bin/bash

VENDER_ID=0x04d9 #YOU NEED TO CHANGE HERE

PRODUCT_ID=0x4545 #YOU NEED TO CHANGE HERE

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
.
.
.
```

Finally, make and install:

```
$ make
$ sudo make install
```

Done. Let's connect your external keyboard!!

## Known Issues

### When external keyboard disconnect (re-enable built-in keyboard), prints all that was typed on the built-in keyboard.

I'm using the trick presented in following thread.

> [Can I disable internal keyboard/trackpad on MBP? - MacRumors Forums](http://forums.macrumors.com/showthread.php?t=433407)

This problem depends on this trick. But I don't know other way to disable built-in keyboard. We only can be careful.

### The integrated keyboard is not re-enabled when without typing a built-in keyboard even once.

Please type something before disconnect external keyboard.

