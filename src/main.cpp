#include "hw_info.h"

int main()
{
	thinger_device thing(USER_ID, DEVICE_ID, DEVICE_CREDENTIAL);
	thing["Memory"] >> [](pson &out) {
		hw_stat.refresh();
		out["Total"] = hw_stat.meminfo[0] / 1024;
		out["Free"] = hw_stat.meminfo[1] / 1024;
		out["Cached"] = hw_stat.meminfo[2] / 1024;
		out["Swap Total"] = hw_stat.meminfo[3] / 1024;
		out["Swap Free"] = hw_stat.meminfo[4] / 1024;
	};

	thing["CPU"] >> [](pson &out) {
		out["Usage"] = hw_stat.cpu[0];
	};

	// Temperatures
	thing["temp"] >> [](pson &out) {
		out["cpu"] = hw_stat.cpu_temp / 1000;
		out["hdd"] = -1;
	};
	thing.start();
	return 0;
}