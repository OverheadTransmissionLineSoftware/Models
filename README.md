## OTLS-Models
This repository contains multiple libraries that model a transmission line and
provide engineering calculations. Each library has its own subfolder to
separate it from others.

## Libraries
### Base
This library provides base functionality needed for most libraries.
* Unit conversions
* Polynomials
* Geometric shapes

### TransmissionLine
This library contains classes/methods that model a transmission line in 3D.
* Alignments
  * Station
  * Line angles
* Structures
  * Rotation, offset, height
  * Multiple attachments
  * Hardware assemblies
* Cables
  * Catenary position/tension
  * Weathercases

### SagTension
This library contains classes/methods that model cable sag-tension behavior.
* Experimental Plastic Elongation (EPE) Cable Model
  * Individual shell/core components
  * Tension/compression extrapolation
* Load and creep stretch
* Resolving multiple tension constraints

### ThermalRating
This library contains classes/methods that model cable thermal behavior.
* Steady-state current and temperature
* Transient temperature

## Branches
The master branch contains stable code most of the time, but it's best to use
specific [releases](https://github.com/OverheadTransmissionLineSoftware/Models/releases)
 of the master branch whenever possible.

## Get Involved!
This is an open source effort to create transmission line design software. This
software is [licensed](https://github.com/OverheadTransmissionLineSoftware/Models/blob/master/LICENSE.md)
 in the public domain, so we can contribute to this collectively without
worrying about intellectual property.
