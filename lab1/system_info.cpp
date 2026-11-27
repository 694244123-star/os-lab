#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <sys/statvfs.h>
#include <dirent.h>
#include <cctype>

using namespace std;

void showCPUInfo() {
    ifstream cpuinfo("/proc/cpuinfo");
    string line;
    cout << "1. CPU Name: ";
    while (getline(cpuinfo, line)) {
        if (line.find("model name") != string::npos) {
            cout << line.substr(line.find(":") + 2) << endl;
            return;
        }
    }
    cout << "Unknown CPU" << endl;
}

void showMemoryInfo() {
    struct sysinfo info;
    if (sysinfo(&info) == 0) {
        double totalRAM = (info.totalram * info.mem_unit) / (1024.0 * 1024.0 * 1024.0);
        double freeRAM = (info.freeram * info.mem_unit) / (1024.0 * 1024.0 * 1024.0);
        cout << fixed << setprecision(2);
        cout << "2. Memory (RAM): " << totalRAM << " GB (Free: " << freeRAM << " GB)" << endl;
    }
}

void showOSInfo() {
    ifstream osRelease("/etc/os-release");
    string line, osName = "Linux";
    while (getline(osRelease, line)) {
        if (line.find("PRETTY_NAME=") == 0) {
            osName = line.substr(13, line.length() - 14);
            break;
        }
    }

    struct utsname buffer;
    uname(&buffer);
    cout << "3. OS: " << osName << " (Kernel: " << buffer.release << ")" << endl;
}

void showStorageInfo() {
    struct statvfs stat;
    if (statvfs("/", &stat) == 0) {
        double totalGB = (stat.f_blocks * stat.f_frsize) / (1024.0 * 1024.0 * 1024.0);
        double freeGB = (stat.f_bavail * stat.f_frsize) / (1024.0 * 1024.0 * 1024.0);
        cout << fixed << setprecision(2);
        cout << "4. Storage (/): Total: " << totalGB << " GB | Free: " << freeGB << " GB" << endl;
    }
}

void showRunningProcesses() {
    cout << "5. Running Processes:" << endl;
    DIR* dir = opendir("/proc");
    if (!dir) return;

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        bool isPid = true;
        for (int i = 0; entry->d_name[i] != '\0'; i++) {
            if (!isdigit(entry->d_name[i])) {
                isPid = false;
                break;
            }
        }

        if (isPid) {
            string pid = entry->d_name;
            ifstream commFile("/proc/" + pid + "/comm");
            string procName;
            if (getline(commFile, procName)) {
                cout << "   [PID: " << pid << "] " << procName << endl;
            }
        }
    }
    closedir(dir);
}

int main() {
    cout << "=== Computer Information (Linux) ===" << endl << endl;
    showCPUInfo();
    cout << endl;
    showMemoryInfo();
    cout << endl;
    showOSInfo();
    cout << endl;
    showStorageInfo();
    cout << endl;
    showRunningProcesses();

    return 0;
}

