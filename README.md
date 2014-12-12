## Lab 8 - Robot Maze

Objective - to get the robot to maneuver through the maze using the onboard IR sensors.

### Design

My hardware design is the same as it was in Lab 7. The "logic" layout of the robot and relevant connections/pins is below.

![alt text](https://raw.githubusercontent.com/ChrisMKiernan/ECE382_Lab8/master/HardwarePinDiagram.png "The hardware layout of my robot's circuits, in logic format")

I designed the code for my robot from my existing code from Labs 6 and 7. The general flow of the code is as follows.

Make the robot follow the right wall
getSensorReadFromRightWall()
isItClose()
  if(close){
    smallTurnLeft()
  }else if(not close){
    smallTurnRight()
  }
getSensorReadFromCenterWall
isItClose()
  if(close){
    tankTurnLeft()
  }else if(not close){
    continue()
    nothing()
    moveForward()
  }

The robot would basically poll each sensor, switching back and forth every few milliseconds to test distances from the walls and correct if needed. I decided not to use a high and low value for distance from the wall, but instead use one value. This means that there was no acceptable median range, but instead the robot was always correcting to stay on a line. The reason for this is that if there was a range and the robot was turned too far from centerline, the correcting turns may not work as planned. With the "line" technique, the robot didn't have any "wiggle room."

### Debugging

My robot encountered several bugs and was never fully reliable. However, I think I solved most bugs as follows.

In my first implementation of the code, I used dead reckoning turns. That is, the tank turns and wide right turns were timed based on my duty cycle and the desired degree of turn. However, this proved unreliable, as if the robot started into a wide turn, it would continue into a wall without stopping. To remedy this, I made the tank turn work like the correcting turns of the wall-following technique. Using this method still turned the robot if it was too close to a wall, but did not move the robot out of control.

Another issue I had was that the robot would work and successfully complete the maze, but then after sitting in the locker overnight, would suddenly fail to complete the maze. After spending a significant amount of time adjusting sensor values to no avail, I noticed that the wheels did not seem to spin as fast or as aggressively as they had before. I decided to change the battery I was using for a new one, and discovered that the bot was suddenly very sensitive and very fast. Unfortunately, there was no time revert the sensor values back to the original by the time M40 class ended, and I never fully debugged the issue. My assumption is that even though there was charge enough to run the robot, the charge in the battery was significantly degraded to the point of being unreliable in supply voltages to the sensors and motors, causing the subsequent receiving pins to be unreliable.
