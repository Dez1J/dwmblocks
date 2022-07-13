#/bin/bash

# cpu, cpu-core 0-7
# paste <(ls /sys/class/*/hwmon*/name) <(cat /sys/class/*/hwmon*/name) <(ls /sys/class/*/hwmon*/temp*_input) <(cat /sys/class/*/hwmon*/temp*_input)
paste <(cat /sys/class/hwmon/hwmon6/*label) <(cat /sys/class/hwmon/hwmon6/*input) | sed 's/\(.\)..$/.\1°C/' 
#paste <(cat /sys/class/nvme/nvme*/model) <(cat /sys/class/nvme/nvme*/hwmon*/temp1_input) | sed 's/\(.\)..$/.\1°C/'
paste <(ls /sys/class/nvme) <(cat /sys/class/nvme/nvme*/hwmon*/temp1_input) | column -s $'\t' -t | sed 's/\(.\)..$/.\1°C/'
paste <(cat /sys/class/thermal/thermal_zone*/type) <(cat /sys/class/thermal/thermal_zone*/temp) | column -s $'\t' -t | sed 's/\(.\)..$/.\1°C/'
