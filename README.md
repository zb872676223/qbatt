# qbatt
Qt application which shows the information about laptop's battery in the system tray.

I'm using i3 window manager and for really long time I used various plugins for
i3blocks bar (and I'm still using it). Battery status was implemented as a script
which i3blocks executes and reads its output which is later used in its status
bar. I just was thinking about how to make the information about the battery more
convenient. This is how qbatt project came.

For the present moment it uses sysfs to read the information about the battery/ac
adapter, but I plan to stick to something more reliable in the future.
