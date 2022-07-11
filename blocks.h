//Modify this file to change what commands output to your statusbar, and recompile using the make command.
static const Block blocks[] = {
	/*Icon*/	/*Command*/		                                        /*Update Interval*/	/*Update Signal*/

    {"", NULL,				                                            0,		            12, getmpdstat},

    {"", NULL,				                                            1,		            0,  get_netusage},

//	{"", "/home/jiang/.dwmbar/statusbar/scrobbler",						0,		            13, NULL},

//    {"", "/home/jiang/.scripts/pacupdate",						        28800,		        0, NULL},
    
//	{"", "/home/jiang/.dwmbar/statusbar/release",					    0,		            0, NULL},
	
	{"", "/home/jiang/.dwmbar/statusbar/BTBattery",                     30,		            0, NULL},

	{"", "/home/jiang/.dwmbar/statusbar/volume",                        0,		            10, NULL},

	{"", "xbacklight |awk -F . '{printf \" ^c#FFFF00^ %s%^d^ \\n\",$1}'",			0,		            11, NULL},

	{"", "/home/jiang/.dwmbar/statusbar/cpu_usage",                     2,		            0, NULL},

	{"", "/home/jiang/.dwmbar/statusbar/memory",                        1,		            0, NULL},

    {" ", "sensors | awk '/^temp1:/{print $2}'|sed -n '3p'",		    3,		            0, NULL},

//	{"", "/home/jiang/.dwmbar/statusbar/battery",						5,		            0, NULL},
    {"", NULL,				                                        60,		            0,  getbattery},

	{"", "/home/jiang/.dwmbar/statusbar/clock",						    60,		            0, NULL},

//    {"", "/home/jiang/.dwmbar/statusbar/weather",						18000,	            5, NULL},
//    {"", "/home/jiang/.dwmbar/statusbar/mpdstatus",					1,		            12, NULL},
//    {"", "/home/jiang/.dwmbar/statusbar/torrent",						20,		            15, NULL},
//    {"", "cat /tmp/lrc.tmp",						                    1,		            0, NULL},

};

//sets delimeter between status commands. NULL character ('\0') means no delimeter.
static char delim = ' ';
