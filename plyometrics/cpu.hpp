#pragma once

namespace plyometrics
{

void warn_on_cpu_freq_scaling()
{
    std::size_t cpu_number = 0;

    while (true)
    {
        std::stringstream path;
        path << "/sys/devices/system/cpu/cpu" << cpu_number << "/cpufreq/scaling_governor";
        auto f = std::ifstream(path.str());

        if (!f)
            return;

        std::string governor;
        f >> governor;
        if (governor != "performance")
            std::cerr << "cpu" << cpu_number << " is running on '" << governor << "' governor" << std::endl;

        cpu_number++;
    }
}

std::string cpu_model()
{
    utsname buf;
    uname(&buf);
    return buf.machine;
}

}
