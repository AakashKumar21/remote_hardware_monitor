#include "hw_info.h"

int main()
{
	thinger_device thing(USER_ID, DEVICE_ID, DEVICE_CREDENTIAL);
	thing["Memory"] >> [](pson &out) {
		hw_stat.refresh();
		out["MemTotal"] = hw_stat.meminfo[0] / 1024;
		out["MemFree"] = hw_stat.meminfo[1] / 1024;
		out["MemAvailable"] = hw_stat.meminfo[2] / 1024;
		out["Cached"] = hw_stat.meminfo[3] / 1024;
		out["SwapTotal"] = hw_stat.meminfo[4] / 1024;
		out["SwapFree"] = hw_stat.meminfo[5] / 1024;
	};

	thing["CPU"] >> [](pson &out) {
		out["Usage"] = hw_stat.cpu_usage;
		out["Temp(C)"] = hw_stat.cpu_temp / 1000;
		out["Frequency(Mhz)"] = hw_stat.cpu_freq / 1000;
	};

	thing.start();
	return 0;
}