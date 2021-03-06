# 3D_water_depth_logger
A simple Arduino based logger for depth mapping of water bodies. Using cheap and simple hardware like a GPS module, an SD card and a standard NMEA capable echolot sounder. The data is then transformed into a depth map with depth contours using python and matplotlib.


The harbour I mapped has an area of around 40.000m^2 and the water level changes 4 meters every 6 hours with the tides. As the measurement took around 1,5 hours, I had to take this tidal change into account and I used data from a Wasser&Schiffahrsamt water level. The logger writes depth data every 1 second but checks for valid GPS/depth synchronization before. This led to some missed datapoints in the log. Anyway the result is ok and shows the shallow spots in the harbour.


## The process looks like this:
1. Take measurements with the logging device of the water body
2. Take tidal data from a near by Wasser&Schiffahrsamt water level and bring it into a format like the example
3. Take the data from SD card and import it in the python script.
4. adapt the normal water level to get a calibrated output (like it in official marine maps)
5. run the script and save the map (the route of the test-vehicle can be disabled in line 70)

## Improvements:
1. some minor glitches in the Arduino code stop the logger from logging to the SD
2. the map interpolation is not 100% right and needs some tweaking
3. the map interpolation needs a zero line round the measurements to prevent false data in the border areas
4. the Wasser&Schiffahrtsampt water level data should be read automatically OR even better should be replaced with an on-site reference level


## Results:
![result](/images/animation.gif)

## Video:
[![LINK TO VIDEO](https://img.youtube.com/vi/nWLPmjaNJ6I/0.jpg)](https://www.youtube.com/watch?v=nWLPmjaNJ6I)


## Some images of the setup
![PCB Backside](/images/boat_setup.JPG)
![PCB Backside](/images/device_on_boat.JPG)
![PCB Backside](/images/device.JPG)
![PCB Backside](/images/device2.JPG)
![PCB Backside](/images/device3.JPG)
![PCB Backside](/images/map.png)
![PCB Backside](/images/map2.png)


## Some blogs wrote about this project:
https://blog.arduino.cc/2021/09/07/diy-3d-water-depth-mapping-with-arduino/

https://www.hackster.io/news/create-topographical-maps-of-water-depths-with-this-unique-project-9df50369b685

https://hackaday.com/2021/09/06/homebrew-sounder-maps-the-depths-in-depth/
