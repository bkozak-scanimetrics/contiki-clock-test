Simple Contiki Clock Test
=========================

This simple Contiki clock test is intended to test for huge arcurracy problems
in the clock time fuctions. This test was intended to run on cc26xx and this is
the default TARGET for builds.

To perform the test simply press the select button on srf06 (left button on
sensortag) and use a stop watch to ensure that the test results are printed
to the serial port after (roughly) 60 seconds.

Building
========

To build run ```make CONTIKI=path/to/your/contiki/source/root```
