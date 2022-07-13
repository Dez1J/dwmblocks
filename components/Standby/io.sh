watch --difference=cummulative --interval=1 '(echo device read_IOs read_merges read_sectors read_ticks write_IOs write_merges write_sectors write_ticks in_flight io_ticks time_in_queue; for file in /sys/block/*/stat; do echo -n $file; cat $file; done) | column -t'

iostat -kx 2

vmstat 2 10

mpstat 2 10

dstat --top-io --top-bio

dstat --time --io --mem --net --load --fs --vm --disk-util --disk-tps --freespace --top-io --top-bio-adv
