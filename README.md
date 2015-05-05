HomeIO backend
==============

Super fast and resource friendly version of HomeIO backend!

Features
--------

* IoServer - bridge between UART/RS to rest via TCP
* Meas fetching - fetch measurements
* Meas buffer - very big buffer, it's better than redis list
* TCP server - access buffer, execute actions, get info about internals
* Actions
* Overseer - special thread to implement simple control rules
* File storage - store measurements in CSV fil in format similar to used in DB