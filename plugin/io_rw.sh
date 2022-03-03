#/bin/bash

DT=1
DISK=nvme1n1
FILE_OLD_WRT=`awk '{print $7}' /sys/block/$DISK/stat` # First field is number of read I/Os processed,and seventh field is sectors written
BIN_OLD_WRT=`iostat -m |rg $DISK | awk '{print $7 }' `
while true; do
    sleep $DT
    FILE_NEW_WRT=`awk '{print $7}' /sys/block/$DISK/stat`
    BIN_NEW_WRT=`iostat -m |rg $DISK | awk '{print $7 }' `
    echo "$DISK : file $((($FILE_NEW_WRT-$FILE_OLD_WRT)*512/1024/1024/$DT)) ,bin $(($BIN_NEW_WRT-$BIN_OLD_WRT/$DT)) "
    # echo $((($FILE_NEW_WRT-$FILE_OLD_WRT)/$DT))
    # echo $((($BIN_NEW_WRT-$BIN_OLD_WRT)/$DT))
    FILE_OLD_WRT=$FILE_NEW_WRT
    BIN_OLD_WRT=$BIN_NEW_WRT
done

# FILE_WRT=`awk '{print $7}' /sys/block/nvme1n1/stat` # First field is number of read I/Os processed,and seventh field is sectors written
# BIN_WRT=`iostat -m | awk '{print $7 }' | head -9 | tail -1`
# echo " $((($FILE_WRT)*512/1024/1024)) ======= $BIN_WRT "

# /sys/block/*/stat 各字段详解
# Name            units         description
# ----            -----         -----------
# read I/Os       requests      number of read I/Os processed
# read merges     requests      number of read I/Os merged with in-queue I/O
# read sectors    sectors       number of sectors read
# read ticks      milliseconds  total wait time for read requests
# write I/Os      requests      number of write I/Os processed
# write merges    requests      number of write I/Os merged with in-queue I/O
# write sectors   sectors       number of sectors written
# write ticks     milliseconds  total wait time for write requests
# in_flight       requests      number of I/Os currently in flight
# io_ticks        milliseconds  total time this block device has been active
# time_in_queue   milliseconds  total wait time for all requests
# discard I/Os    requests      number of discard I/Os processed
# discard merges  requests      number of discard I/Os merged with in-queue I/O
# discard sectors sectors       number of sectors discarded
# discard ticks   milliseconds  total wait time for discard requests

# 可以像直接读取sysfs一样/sys/block/sda/stat。字段＃1给出读取总数，字段＃5是写入总数，字段＃9是正在进行的I / O操作数

# /dev/nvme1n1 written
# iostat |  awk '{print $1" "$6" "$7 }'  |head -9 | tail -2
# BIN_OLD_WRT=`iostat |  awk '{print $7 }'  |head -9 | tail -1`
# sleep 1
# BIN_NEW_WRT=`iostat |  awk '{print $7 }'  |head -9 | tail -1`
# echo "($BIN_NEW_WRT - $BIN_OLD_WRT)/1024" |bc
# echo $(( BIN_NEW_WRT - BIN_OLD_WRT ))

