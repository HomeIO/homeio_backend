HomeIO backend
==============

Super fast and resource friendly version of HomeIO backend!
-----------------------------------------------------------

[HomeIO](http://homeio.org.pl/) is system for remote monitor and control of
various systems. I have created it about year 2005 and have been using various
technologies. This is the last iteration because it is written in C++ for
**maximum performance**.

Performance
-----------

I am using it for **small wind turbine** since 2014 while I moved components
written in higher level language (Ruby).

System is deployed on [Raspberry Pi](https://www.raspberrypi.org/products/model-b/)
with average 15-20% CPU usage. Memory usage depends on measurement buffer.

[Frontend](https://github.com/HomeIO/homeio_frontend) is written in Go as HTTP server,
and most of operation is performed client side.

How it works?
-------------

Please take a moment and look this sample video.

[![HomeIO demo](https://i.vimeocdn.com/video/528816542.webp?mw=600&mh=400)](https://vimeo.com/135051782)

Scenario:

* 0:07 - list of measurements
* 0:15 - current measurements graph
* 0:53 - remote shutdown of wind turbine
* 1:20 - effects of shutdown, gradually reduce charge current
* 1:37 - start - turn of turbine brake
* 2:00 - turbine slowly starts up


Features
--------

* IoServer - bridge between UART/RS to rest of modules via TCP socket
* Measurement fetching - fetch measurements periodically. In wind turbine example it
  fetches about *10 types of measurements every 100ms*!
* Measurement buffer - large optimized buffer. It is much better solution that Redis because
  every measurement is stored as `unsigned int` for minimum RAM usage.
* TCP server - allow communication with [frontend](https://github.com/HomeIO/homeio_frontend):
  current measurements, graph, history, stats, execute actions, ...
* Actions - all possible actions are predefined and can be executed by `Overseer` classes,
  by `Addon` classes or manually via frontend.
* Overseer - simple control rules, example: shutdown wind turbine if charge current is
  higher than 10A
* File storage - store measurements in CSV file format which you can easily process and
  store
* Addons - custom addons which can create and store advanced stats, example: hourly
  generated energy, time utilization. Or you can put there dedicated control algorithms.
* Ncurses UI - most crucial information are accessible from console      


Am I missing something?
-----------------------

I am only author of this system so everything is abvious for me, but you may
find that a lot of is missing. Please do not hesitate, [post issue](https://github.com/HomeIO/homeio_backend/issues/new)
or [email me](mailto:homeio@interia.eu).
