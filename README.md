# OTLS - MODELS
This repository contains multiple libraries that model a transmission line, as
well as provide engineering calculations. Each library has its own subfolder to
distinguish it from others.

## BASE
This library provides base functionality needed for most libraries.
* Unit conversions
* Polynomials
* Geometric shapaes

## TRANSMISSION LINE
This library contains classes/methods that model a transmission line.
* Cables
  * Catenary position/tension
  * Weathercases

## SAG-TENSION
This library contains classes/methods that model cable sag-tension behavior.
* Experimental Plastic Elongation (EPE) Cable Model
  * Individual shell/core components
  * Tension/compression extrapolation
* Inclined (3D) catenary geometry
* Load-based stretch
* Resolving multiple tension constraints

## GET INVOLVED!
This is an open source effort to create transmission line design software. This
software is released in the public domain (see UNLICENSE file), so we can
contribute to this collectively without worrying about intellectual property.
