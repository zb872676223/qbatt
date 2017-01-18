# qbatt
Qt application which shows the information about laptop's battery in the system tray.

I'm using i3 window manager and for really long time I used various plugins for
i3blocks bar (and I'm still using it). Battery status was implemented as a script
which i3blocks executes and reads its output which is later used in its status
bar. I just was thinking about how to make the information about the battery more
convenient. This is how qbatt project came.

Qbatt supports two data collection mechanisms: D-Bus and SysFS. By-default it tries
to use D-Bus as most preferrable method, otherwise qbatt uses SysFS method.

TODO:
    - add option for builder which will allow to use one collection mechanism only.
    
Thoughts:
    - implement gathering the data from attached UPS using D-Bus.
