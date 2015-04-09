#Receive the Packets and Send them to the NUC
This code can be used to read the packets from /dev/cc2520_1 and send them over the socket connection to the NUC for processing.

    gcc read.c -o read
    ./read 192.168.7.1 <portnumber>
