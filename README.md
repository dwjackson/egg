# Egg Timer

Egg Timer is, as you might expect, a simple (egg) timer program.

## Usage

Timer is used as follows:

```sh
egg [FLAGS] [DURATION]
```

The duration can be specified using the following syntax:

```txt
XhYmZs
```

where X, Y, and Z are numbers. All parts are optional. If you are just doing
seconds then you don't need the trailing "s."

Note that, by defauly, when the timer runs out it will chime until the user
kills the program via a CTRL+C.

## Flags

The `egg` program supports the following flags:

* `-s` Count down in seconds vs hh:mm:ss
* `-m [MESSAGE]` write a message when the timer completes
* `-e [SECONDS]` stop the timer's end-chime after the given number of seconds
* `-q` do not print anything ("quiet mode")

## Building & Installing

To build `timer`, run

```sh
make
```

To install to the default location of `/usr/local`, run

```sh
make install
```

To install to somewhere else on your system, specify the `PREFIX` variable as
follows:

```
make install PREFIX=/some/dir
```

## License

Timer is licensed under the MIT license, a copy of which should be included.

