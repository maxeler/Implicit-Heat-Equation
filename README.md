=============================
Implicit-Heat-Equation
=============================

Simulate heat spreading according to implicit methods.


Requirements
------------

This application requires MaxGenFD library.


Usage
-----

Set environment variable RUNRULE to be Simulation or DFE.

To compile:

cd APP/CPUCode

make

To run:

cd App/Runrules/$RunRule/bin/ImplicitHeatEquation <path to parmfile>

A parameter file can be generated with APP/CPUCode/generate_parmfile.py

