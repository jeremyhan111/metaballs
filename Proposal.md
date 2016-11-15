# Final Project Proposal




 For our final project, we would like to create a two-dimensional implementation of metaballs. Metaballs is a visually interesting implementation of two-dimensional circles displayed in an organic-looking way to form a blobby pattern as they collide. Using a polynomial to combine various circle components, we will create an animation that demonstrates the concept of metaballs. We want to code it using both CUDA and OpenGL because we are curious about how these interfaces compare with respect to implementation specifics and speed. We hope to time how long each implementation requires to run a fixed number of iterations of our metaballs animation.If time permits, we will also implement a three dimensional version of metaballs.




# Course Concepts Used


 Through this project, we hope to develop our understanding of many of the concepts that were introduced to us in class. We will explore openGL in relation to CUDA by implementing metaballs using both of these platforms. We will rely on our understanding of the openGL pipeline learned in class, and we will build on our understanding of previous implementations of circles and spheres. In order to understand the metaball combination function, we will rely on the intuition developed while exploring the concept of noise in class. In the event that we implement a three dimensional version, we also hope to use lighting tools from class to make a more realistic looking model. In implementing metaballs, we will build upon graphical and mathematical tools introduced in class.




# Other software Tools


We will be using using Cuda and OpenGL, as well as various timing mechanisms. 


# Goals


 We have a couple short term goals that we hope to reach. The first goal is to refamiliarize ourselves with the software tools that we will be using, and that we can compile and get an animation working for a simple object. The second short term goal is to make sure that we understand the math behind metaballs, such as the threshold function, and develop a better understanding of linear interpolation. Our third short term goal is to be able to input an arbitrary number of circles and make sure that the animation runs smoothly. 


In the long term, we want to make sure that we can implement a grid-like object that will keep track of which square corners are inside a circle. Then, using math we can use linear interpolation to draw the lines that represent the locations of the circles. 


If we have time, we would also like to create a three dimensional version of metaballs.




# References


> http://jamie-wong.com/2014/08/19/metaballs-and-marching-squares


> https://en.wikipedia.org/wiki/Metaballs


# Assessment


 We will know that we have coded a successful implementation of metaballs when the balls collide and split in a fluid-like manner. Some reach goals that we think could be feasible is using other shapes instead of circles. Another possible reach goal is extending this project so that we use three-dimensional shapes instead of two-dimensional shapes.





