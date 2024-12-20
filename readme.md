


# Jura Impressa F-series UART decoder and ESPHOME controller

This repository is a advanced version of the basic esphome configuration from Rynald Alden. Its tailort for use with a Jura Impressa F7 coffeemachine.
In this repository you will find a lot of additions for this Jura Impressa series. This way we managed to decipher more codes from the UART messages of the jura.

The following counters are now available:

- Single espresso
- Double espresso
- Single Coffee
- Double coffee
- Single Ristretto
- Double Ristretto
- Brew-unit movements
- Cleanings
- num of coffee grounds in tray

## reference

Orginal sourcecode is based on: https://github.com/ryanalden/esphome-jura-component / https://community.home-assistant.io/t/control-your-jura-coffee-machine/26604

# Hardware connections

Hardware is a Wemos D1 Mini connected to the 7-pin Jura service port via a 3.3V<->5V logic level converter.\
The D1 mini is powered from the Jura.

<img src="images/seven-pin-interface.jpg" alt="Jura 7-pin interface">

Above image taken from [here](https://community.home-assistant.io/t/control-your-jura-coffee-machine/26604).

<img src="images/connection-diagram.png" alt="Jura 7-pin interface">

If you have diffuculty, try swapping the TX/RX pins.
