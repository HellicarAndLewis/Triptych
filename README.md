IntelMirrors
============

Triptych of Digital Mirrors for Intel to show the qualities of their Ivy Bridge Processor


Intel_Security
==============
Security visualization. You can use the following keys in the application:

 - g: **Flocking:** toggle gui
 - s: **Flocking:** save settings
 - l: **Flocking:** load settings
 - f: Toggle fullscreen
 - d: Toggle debug draw
 - spacebar: **Kinect** toggle gui 
 
 ## Flocking sliders
 
  - **Flock radius SQ:** The radius squared (SQ) in which boids interact with 
    each other (i.e. when **Flock align energy**, **Flock separate energy** and 
    **Flock attract energy** have influence.
  - **Flock high threshold** Basically a toggle to switch between alignment and
     separation. *Default value: 0.6*
  - **Flock low threshold** See 'high'. *Default value: 0.4*
  - **Flock align energy:** Amount of alignment between boids. *Default value: 0.0070*
  - **Flock separate energy:** Amount of separation between boids. *Default value: 0.0048*
  - **Flock attract energy:** Amount of attraction between boids. *Default value: 0.0034*
  - **Flock center energy:** How much are the boids attracted to the center point. *Default value: 0.4167*
  - **Flock sphere size:** Works with center energy. The radius of the sphere where boids are allowed to fly around. *Default value:5.9633*
  - **Boid trail duration (millis)** **CURRENTLY NOT USED**
  - **Boid glow duration (ms)** **CURRENTLY NOT USED**
  - **Boid glow size:** The size of glow around a boid when it's in attack mode. *Default value: 0.246*
  - **Boid scale:** The scale of a boid. *Default value: 0.0933*
  - **Boid trail width:** The trail width when a boid attacks. *Default value: 0.2120*
  - **Boid trail length:** :-) 
  - **[x] Boid create trails:** **DO NOT USE THIS FOR NOW, SET LENGTH TO ZERO IF YOU DONT WANT TRAILS**
  - **Boid attract to user energy** **UPDATE:** used as force for user input now
  - **Boid attract to user radius** **UPDATE:** "push boids away" radius .. 
  - **Boid attack energy** How "fast" a boid goes to a point of attack. *Default value: 0.0793*
  - **Boid attack duration min**:  We take a random value between duration min and max. As long as "duration" the boid will attack **Value in millis**. *Default value: 366*.
  - **Boid attack duration max**: See Boid attack duration min.
  - **Boid attack delay min (ms)**: We don't want all boids to attack so they have a random delay between min and max. *Default value: 6233.xxx*
  - **Boid attack delay max (ms)**: See min. *Default value: 1533.xxx*
  - **[x] Record kinect** Used to record kinect while testing. **DO NOT USE**
  - **Save kinect recording:** Used to recor kinect while testing. **DO NOT USE**
  - **[x] Draw boid glows:** Draw glows when a boid attack.
  - **[x] Draw grid:** Used while testing, for alignment. Draw an axis + grid.
  - **[x] Draw flock:** Draw the boids.
  - **[x] DebugDraw attackers: in debug draw mode (the ugly version), draw the attackers magenta)
  - **Kinect scale:** Scale of triangulated kinect image.
  - **Percentage of visible boids**: From the 300 boids possible in total, only use this percentage
  - **Percentage of attack boids**: Percentage from the visible boids which are **responsive** to user input.
  - **Update number of visible boids** After changing the percentages (two lines above), press this button.
  
 *Example of values which work well*
 
 <img src="http://upload.roxlu.com/server/php/files/Screen%20shot%202012-07-10%20at%2011.20.21%20AM.png">
 <img src="http://upload.roxlu.com/server/php/files/Screen%20shot%202012-07-10%20at%205.20.42%20PM.png">
 