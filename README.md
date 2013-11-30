# Mac Built-in Keyboard Disabler

This disable(/re-enable) macbook's built-in keyboard smarty __when your external keyboard connected(/disconnected)__.

![my macbook and hhkbpro2](https://syeygw.bn1301.livefilestore.com/y2p1O8cDq2aAw0ftSKuQ6XdEtFWH3-J9xQzadbdOXajhWzlG4i1uvOhSpm72CYtTq0twBdh9Epf9z2rDbhnOKdgfMRPiVczzidoW_uNmvG8rFY/1385784561950.jpg?psid=1)

So, you can put your favorite keyboard on your Macbook.

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

Next, in `./keyboard-disabler.sh`, edit `VENDER_ID` and `PRODUCT_ID` for your keyboard's one (You can get them by executing `$ system_profiler SPUSBDataType`):

```
#!/bin/bash

VENDER_ID=0x04d9 #YOU NEED TO CHANGE HERE

PRODUCT_ID=0x4545 #YOU NEED TO CHANGE HERE
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

Because I'm using the trick presented in following thread.

> [Can I disable internal keyboard/trackpad on MBP? - MacRumors Forums](http://forums.macrumors.com/showthread.php?t=433407)

This problem depends on this trick. But I don't know other way to disable built-in keyboard. We only can be careful.

### The integrated keyboard is not re-enabled when I without typing a built-in keyboard even once.

To avoid it, please type something before disconnect external keyboard.

