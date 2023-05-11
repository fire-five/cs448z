# CS 488Z: Physically Based Animation & Sound

## Assignment #0: Audiovisual code warm-up

OpenGL + OpenAL

A simple audiovisual simulation of three balls in a box. 


## Homework #1: Real-time Rigid-Body Dynamics with Acceleration Noise

OpenGL + OpenAL

An interactive audiovisual simulation of a simple rigid-body system which includes realistic physics-based acceleration noise.

- I use the simple AABB trick to speed up the collision detections
- Calculate pressure at position x, time t using the analytical approximation formula, then sample pressure in collision time tao with SAMPLE_RATE to get the sound.
