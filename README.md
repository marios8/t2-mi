# t2-mi
This is a commandline T2-MI baseband frame extractor.  
It will read a MPEG TS containing a T2-MI data stream from STDIN and output the extracted MPEG TS to STDOUT  
Writing of this tool was inspired by newspaperman  

Usage:
1) Use a tool like tune-s2 to tune a transponder
2) Find out the Data PID that contains the T2-MI stream or tool will tell You PID found in the stream
3) Use dvbsnoop and pipe to t2-mi and pipe to your video-player

For example:  
./dvbsnoop -b -s ts 4096 | ./t2-mi 4096 0 | vlc -  
curl -s [TVH Transponder URL] | ./t2-mi 4096 0 | vlc -  

Documentation:  
ts_102773v010301p.pdf - T2-MI packet format  
en_302755v010401a.pdf - BBFrame stream format for High Efficiency Mode for TS  
Wiki MPEG transport stream
