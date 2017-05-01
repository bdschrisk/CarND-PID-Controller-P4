# Proportional-Integral-Differential Controller Project
Self-Driving Car Engineer Nanodegree Program

---

## Overview
In this project I present a variant of the PID controller model which I've termed "Stochastic-Temporal-Update-Proportional-Integral-Differential" controller - or STUPID for short.  The STUPID model uses short term temporal differences which is optimised using stochastic gradient descent, to push the PID values to their optimum.
Depending on the initialisation strategy the STUPID controller is gauranteed to converge, and in less epochs than a traditional PID controller.

To initialise the starting weights [Wp, Wi, Wd], a uniform distribution with partial ranges is utilised, where;
 - Wp = 0.4 < x < 0.7
 - Wi = 0.0 < x < 0.4
 - Wd = 0.7 < x < 5.0
*The above initialisation strategy ensures that the integral doesn't cause the gradients to explode or saturate the other gain values.*

The update process is robust enough that symmetry breaking during initialisation is not required, however, it will cause delayed convergence.  Tuning the convergence speed is done by modifying the learning rate (alpha).

Because the STUPID model doesn't require extensive parameter search for initialisation, it can be a used in an online learning setting.  This is a big advantage over traditional PID controllers where proper tuning of parameters is paramount to their successes.
It is also complete in that additional process noise induced during operation will not cause it to diverge in the case of other types of controllers.

### Optimisation

Below are comparisons between the weight initialisation stategies for the STUPID controller in reference to traditional PID controllers.

![STUPID Optimisation using symmetric vs. standard PID controllers](/images/SGD-PID-1.png)
*Symmetric initialisation of 0.1*

![STUPID Optimisation weight strategy vs. standard PID controllers](/images/SGD-PID-2.png)
*Non-symmetric initialisation using above strategy*

## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets) == 0.13, but the master branch will probably work just fine
  * Follow the instructions in the [uWebSockets README](https://github.com/uWebSockets/uWebSockets/blob/master/README.md) to get setup for your platform. You can download the zip of the appropriate version from the [releases page](https://github.com/uWebSockets/uWebSockets/releases). Here's a link to the [v0.13 zip](https://github.com/uWebSockets/uWebSockets/archive/v0.13.0.zip).
  * If you run OSX and have homebrew installed you can just run the ./install-mac.sh script to install this
* Simulator. You can download these from the [project intro page](https://github.com/udacity/CarND-PID-Control-Project/releases) in the classroom.

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`. 

## Project Instructions and Rubric

Note: regardless of the changes you make, your project must be buildable using
cmake and make!

More information is only accessible by people who are already enrolled in Term 2
of CarND. If you are enrolled, see [the project page](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/f1820894-8322-4bb3-81aa-b26b3c6dcbaf/lessons/e8235395-22dd-4b87-88e0-d108c5e5bbf4/concepts/6a4d8d42-6a04-4aa6-b284-1697c0fd6562)
for instructions and the project rubric.
