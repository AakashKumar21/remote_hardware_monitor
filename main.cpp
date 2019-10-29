#include "main.h"

int main()
{
	thinger_device thing(USER_ID, DEVICE_ID, DEVICE_CREDENTIAL);
	thing["Memory"] >> [](pson& out) {
		hw_stat.refresh();
		out["Total"] = hw_stat.meminfo[0];
		out["Free"] = hw_stat.meminfo[1];
		out["Cached"] = hw_stat.meminfo[2];
		out["Swap Total"] = hw_stat.meminfo[3];
		out["Swap Free"] = hw_stat.meminfo[4];
	};

	thing["CPU"] >> [](pson& out) {
		out["Usage"] = hw_stat.cpu[0];
	};

	// Temperatures
	thing["temp"] >> [](pson& out) {
		out["cpu"] = 54.5;
		out["hdd"] = 88.9;
	};
	thing.start();
	return 0;
}