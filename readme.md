
# Jura Impressa F-series UART decoder and ESPHOME controller

![coding_example](/images/coding_example.jpg)

This repository is a advanced version of the basic esphome configuration from Rynald Alden. Its tailort for use with a Jura Impressa F7 coffeemachine.
In this repository you will find a lot of additions for this Jura Impressa series. This way we managed to decipher more codes from the UART messages of the jura.

The following counters are now available:

- Single espresso
- Double espresso
- Single Coffee
- Double coffee
- Single cappucino (*)
- Single Ristretto (*)
- Double Ristretto (*)
- Brew-unit movements (*)
- Cleanings performed (*)
- Descalings performed (*)
- num of coffee grounds in tray (*)

(*) _all these sensors are new compared to the original esphome interaction by [ryanalden]_

<div style="background: green; padding: 10px;">
  <p><strong>Warning:</strong> There is an extensive WIKI on this GitHub in development where the information below is described in more detail and better! 
  
  <h1><a href="https://github.com/alco28/Jura-F7-ESPHOME/wiki">CLICK HERE FOR THE WIKI</a></p></h1>
</div>

# Hardware connections

Hardware is a Wemos D1 Mini connected to the 7-pin Jura service port via a 3.3V<->5V logic level converter. The D1 mini is powered from the Jura.

![ueart2](./images/uart_pinout.png)
The first pin is on the right and number 8 is on the left
You need:

| pin-out | function    |
| :------ | :--------- |
| (2)     | TX         |
| (3)     | Ground     |
| (4)     | RX         |
| (6)     | **5** Volt |

## wire-connections

![connections](images/connection-diagram.png)

If you have difficulty, try swapping the TX/RX pins.
Be careful to place the logic leveler otherwise you can damage the Jura!

In my scripts you will also find a piece of extra hardware, namely a One-wire temperature sensor (Dallas 18B), this measures my kitchen temperature. If you do not have this, remove it from my scripts at the marked points.

# Jura UART port protocols

I have not deciphered the protocol myself, I have based this mainly on Ryan's earlier (collected) work. But the file jura_coffee.h contains all the C++ logic to understand the Jura commands and read the EEPROM. I tried to keep the file as readable as possible. The most important parts are the ``counts[0] = parseHexSubstring(data, 3, 7);   // Single espresso`` parts (line 125+), those decode the string from the Jura. 

There a still bits un decoded! the are noted as ``unknown_fields[*]`` in the script!
There is a good documentation reference at the [Jura Proto project][Jura Proto github]

# Home Assistant connection and dashboard

Here is a example of a home-assistant dashboard that you can make with the provided functions and counters:

![dashboard](images/HA-dashboard.png)

Here your can find an [example YAML file](../Home-assistant%20dashboard%20example.yaml) from my home-assistant implementation.

# ESPhome webserver examples

This is a version 2 dashboard example of the integrated webserver from the ESPhome:
![esphome webserver example ](/images/esphome-webcounters.png)

----

## reference

Orginal sourcecode is based on:

- [Ryan Alden Github][ryanalden]
- [home assistant forum]
- [Jura Proto project][Jura Proto github]

[ryanalden]: https://github.com/ryanalden/esphome-jura-component
[home assistant forum]: https://community.home-assistant.io/t/control-your-jura-coffee-machine/26604
[Jura Proto github]: https://github.com/Jutta-Proto/protocol-cpp?tab=readme-ov-file