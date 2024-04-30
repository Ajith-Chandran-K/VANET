# VANET

During the past decade, the automobile industry has seen a surge in the use of
advanced technologies, such as state-of-the-art electronic devices, in order to
improve automobile safety. Sadly, however, the fatalities and injuries caused
due to automobile accidents have remained at an alarming level.
A major cause of accidents is the slow response time of drivers to stopped
traffic i.e., the average time a driver takes to hit the brake after a preceding car
has stopped. The cumulative response times for the leading vehicles play the
main role in the collision probability of the upstream vehicles, potentially
resulting in domino-style collisions. To reduce the drivers‘ response time to
accidents, recent research and development in the automobile industry has
introduced collision warning systems to be installed on modern automobiles.
Collision warning systems are capable of cautioning about critical, time
sensitive incidents such as crashes or traffic jams etc., with the advancements in
Vehicular Ad Hoc Networks (VANET) recent research suggests the use of
VANETs to improve the effectiveness of collision warning systems. VANETs
allow for cross-communication between cars within a close proximity of each
other, which can enable them to efficiently and reliably communicate sensitive
traffic messages such as crash-relevant information.
The Federal Communications Commission has allocated 75 MHz of spectrum in
the 5.9 GHz band for Dedicated Short Range Communications (DSRC). To
serve as the groundwork for DSRC, the IEEE 802.11p standard was published 
in the year 2010 for Wireless Access in Vehicular Environments (WAVE).The
75 MHz spectrum of DSRC is divided into seven10 MHz-wide channels.
One channel is called the control channel (CCH) and serves exclusively for
safety messages. The other channels are called service channels (SCH) and are
reserved for commercial applications. Safety messages are either event-driven
or periodic. Each vehicle sends periodic messages in a single hop regularly in
order to inform other vehicles inside its given neighbourhood of important
information such as location, speed, and acceleration while it sends event-driven
messages to warn other vehicles of a collision. A portion of CCH is usually left
unused for event-driven messages which waste bandwidth. In this project, we
focus on exploiting periodic messages intelligently for collision avoidance.
In order to ameliorate drivers‘ safety using the personalized vehicular
communications, first we need to know the delay requirements of the safety
applications. In general, the difference between the communication delay and
the sum of perception reaction times of drivers in a chain plays the main role in
reducing the average collision probability of the vehicles.
Perception reaction time (PRT) is the time needed for a driver to perceive that
something has happened and react to it. It is desirable to reduce this delay as
much as possible by lessening the interference caused by other vehicles. Our
proposed algorithm tunes this transmission probability of each vehicle based on
the individual characteristics of drivers and the traffic conditions around the
vehicles.
