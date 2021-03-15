# Project 3 Computer Graphics

## Compilation

This project has only been tested on MacOS. It is not guaranteed to work
on other platforms.

- Install `googletest` (`brew install googletest` on MacOS)
- Install `boost`
- Go to project root directory `mkdir build`
- `cd build`
- `cmake ..`
- `make`

## Running
`./project3 {input_file}`

## Sample Scene 1
We start with 50 samples per pixel on the file [spheres1.txt](.github/spheres1.txt)

<p align="center">

![Sample Scene 1](.github/spheres1.png)

</p>

# Features

## Scene Setup

### Camera Placement
<p align="center">

[The viewpoint is adjusted slightly](.github/spheres1-vp.txt)
```text
camera_pos: -6 3 -2
```

![Sample Scene 1](.github/spheres1-vp.png)

</p>

### Film Resolution (and Aspect Ratio)

<p align="center">

![Sample Scene 2](.github/spheres1-res.png)

</p>

### Background Colors

<p align="center">

![Sample Scene 4](.github/spheres1-bg.png)

</p>

### PNG Output
- This should be clear


## Primitives
### Spheres
- This should be clear

## Lighting
### Ambient Lights
- This should be apparent by the default image

### Point light sources
- This should be apparent by the default image

### Multiple light sources

<p align="center">

![Sample Scene 4](.github/spheres2.png)

</p>

### Shadows
- This should be apparent by the default image

## Sampling 
- I added a custom command `samples: {#}` to determine the number of samples
- The first sample is always at the center of the pixels
- The rest are randomly distributed between the pixel boundary
- This makes 1 sample equivalent to basic sampling and `n > 1` samples equivalent to
jittered sampling
  
### Basic Sampling
- results in jagged edges
<p align="center">

![Sample Scene 5](.github/spheres-samples1.png)

</p>

### Sampling with `n=100`
- smoother result like box-filtering.

<p align="center">

![Sample Scene 6](.github/spheres-samples100.png)

</p>

## Materials
### Color & Specularity
- This should be apparent by the default image

# Implementation
The code is implemented in C++. Some interesting features

## General
- I really like immutable data, so I made a macro `#define let const auto`
- There are no switch statements for strings in C++. This is an issue for parsing commands.
To get around this I made an unordered map of string to enum and then used the switch
  statement on the enum.
- I use modern C++ 17 features such as destructuring and default nested struct values

## Ray Tracing
- Most of the logic occurs in `Scene` class
- Collision uses the quadratic formula
- Collision which occurs with a `t-value` less than 0.001 is ignored (to prevent shadow acne)
- For each sample loops over all lights and sees if can reach it (to do this needs to iterate over all spheres)
- Shades with appropriate formulas

## Sampling
- See the `Features/Sampling` section

# Challenges
- The switch statements described in the previous 

# Issues
- So far, I do not see any issues in my code (apart from missing features)
- If there is an issue, feel free to create a GitHub issue.


# Extra-Credit
- I completed 55 points worth of features instead of 50. Consider jittered supersampling
to be my extra credit.
  

