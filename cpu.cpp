#include "cpu.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring> //strtok
#include <string>
#include <cstdlib>
#include <sstream>
#include <QString>

cpu::cpu(int A)
{
    n_coreNumber = A;
    updateGovernorList();
}

void cpu::setOnline(bool online)
{
    if (n_coreNumber > 0) //если ядро не основное (не 0)
    {
        std::stringstream sstm;

        sstm << "/sys/devices/system/cpu/cpu" << n_coreNumber << "/online\0";
        // Склейка пути
        std::ofstream fon; // Открываем файл и записываем в переменную
        fon.open(sstm.str().c_str());
        if(fon.fail())
        {
            std::cout << "Unable to open online file for writing." << std::endl;
            return;
        }
        fon << online;
        fon.close();
        b_isOnline = online;
    }
}
void cpu::setMaxScalingFreq(int maxScaingFreq)
{
    if (b_isOnline == true) //если ядро в сети
    {
        std::stringstream sstm;

        sstm << "/sys/devices/system/cpu/cpu" << n_coreNumber << "/cpufreq/scaling_max_freq\0";
        // Склейка пути
        std::ofstream fon; // Открываем файл и записываем в переменную
        fon.open(sstm.str().c_str());
        if(fon.fail())
        {
            std::cout << "Unable to open maxScalingFreq file for writing." << std::endl;
            return;
        }
        fon << maxScaingFreq;
        fon.close();
    }
}
void cpu::setMinScalingFreq(int minScaingFreq)
{
    if (b_isOnline == true) //если ядро в сети
    {
        std::stringstream sstm;

        sstm << "/sys/devices/system/cpu/cpu" << n_coreNumber << "/cpufreq/scaling_min_freq\0";
        // Склейка пути
        std::ofstream fon; // Открываем файл и записываем в переменную
        fon.open(sstm.str().c_str());
        if(fon.fail())
        {
            std::cout << "Unable to open minScaingFreq file for writing." << std::endl;
            return;
        }
        fon << minScaingFreq;
        fon.close();
    }
}
void cpu::setGovernor(QString governor)
{
    if (b_isOnline == true) //если ядро в сети
    {
        std::stringstream sstm;

        sstm << "/sys/devices/system/cpu/cpu" << n_coreNumber << "/cpufreq/scaling_governor\0";
        // Склейка пути
        std::ofstream fon; // Открываем файл и записываем в переменную
        fon.open(sstm.str().c_str());
        if(fon.fail())
        {
            std::cout << "Unable to open governor file for writing." << std::endl;
            return;
        }
        fon << governor.toStdString() ;
        fon.close();
    }
}

// Обновляем состояние ядра(в "сети" или нет)
void cpu::updateOnline(void)
{
    if (n_coreNumber != 0) //если ядро не основное (не 0)
    {
        char str_path[70]; // Переменная для пути к файлу
        sprintf(str_path,"%s%d%s", "/sys/devices/system/cpu/cpu", n_coreNumber, "/online\0");
        // Склейка пути
        std::ifstream fin; // Открываем файл и записываем в переменную
        fin.open(str_path);
        if(fin.fail())
        {
            std::cout << "Unable to open online file." << std::endl;
            b_isOnline = false;
            return;
        }
        fin >> b_isOnline;
        fin.close();
    }
    else
        b_isOnline = true;
}
void cpu::updateGovernor(void)
{
    if (b_isOnline) //если ядро работает
    {
        char str_path[70]; // Переменная для пути к файлу
        sprintf(str_path,"%s%d%s", "/sys/devices/system/cpu/cpu", n_coreNumber, "/cpufreq/scaling_governor\0");
        // Склейка пути
        std::ifstream fin; // Открываем файл и записываем в переменную
        fin.open(str_path);
        if(fin.fail())
        {
            std::cout << "Unable to open maxfreq file." << std::endl;
            f_maxFreq = 0.0f;
            return;
        }
        fin >> str_governor;
        fin.close();
    }
    else
        sprintf(str_governor, "%s", "Offline\0");
}
void cpu::split(const std::string &s, const char* delim, std::vector<std::string> & v)
{
    // to avoid modifying original string
    // first duplicate the original string and return a char pointer then free the memory
    char * dup = strdup(s.c_str());
    char * token = strtok(dup, delim);
    while(token != NULL){
        v.push_back(std::string(token));
        // the call is treated as a subsequent calls to strtok:
        // the function continues from where it left in previous invocation
        token = strtok(NULL, delim);
    }
    std::free(dup);
}
void cpu::updateGovernorList(void)
{
    vec_governorList.clear();
    std::string str_governorList;
    char str_path[70]; // Переменная для пути к файлу
    sprintf(str_path,"%s%d%s", "/sys/devices/system/cpu/cpu", n_coreNumber, "/cpufreq/scaling_available_governors\0");
    // Склейка пути
    std::ifstream fin; // Открываем файл и записываем в переменную
    fin.open(str_path);
    if(fin.fail())
    {
        std::cout << "Unable to open governorList file." << std::endl;
        // f_maxFreq = 0.0f;
        return;
    }
    std::getline(fin, str_governorList);
    split(str_governorList, " ", vec_governorList);
    fin.close();
}
// Обновляем максимальную частоту ядра
void cpu::updateMaxFreq(void)
{
    if (b_isOnline) //если ядро работает
    {
        char str_path[70]; // Переменная для пути к файлу
        sprintf(str_path,"%s%d%s", "/sys/devices/system/cpu/cpu", n_coreNumber, "/cpufreq/cpuinfo_max_freq\0");
        // Склейка пути
        std::ifstream fin; // Открываем файл и записываем в переменную
        fin.open(str_path);
        if(fin.fail())
        {
            std::cout << "Unable to open maxfreq file." << std::endl;
            f_maxFreq = 0.0f;
            return;
        }
        fin >> f_maxFreq;
        fin.close();
    }
    else
        f_maxFreq = 0.0f;
}
// Обновляем минимальную частоту ядра
void cpu::updateMinFreq(void)
{
    if (b_isOnline) //если ядро работает
    {
        char str_path[70]; // Переменная для пути к файлу
        sprintf(str_path,"%s%d%s", "/sys/devices/system/cpu/cpu", n_coreNumber, "/cpufreq/cpuinfo_min_freq\0");
        // Склейка пути
        std::ifstream fin; // Открываем файл и записываем в переменную
        fin.open(str_path);
        if(fin.fail())
        {
            std::cout << "Unable to open minfreq file." << std::endl;
            f_minFreq = 0.0f;
            return;
        }
        fin >> f_minFreq;
        fin.close();
    }
    else
        f_minFreq = 0.0f;
}
// Обновляем текущую частоту ядра
void cpu::updateCurFreq(void)
{
    if (b_isOnline) //если ядро работает
    {
        char str_path[70]; // Переменная для пути к файлу
        sprintf(str_path,"%s%d%s", "/sys/devices/system/cpu/cpu", n_coreNumber, "/cpufreq/scaling_cur_freq");
        // Склейка пути
        std::ifstream fin; // Открываем файл и записываем в переменную
        fin.open(str_path);
        if(fin.fail())
        {
            std::cout << "Unable to open curfreq file." << std::endl;
            f_curFreq = 0.0f;
            return;
        }
        fin >> f_curFreq;
        fin.close();
    }
    else
        f_curFreq = 0.0f;
}
// Обновляем минимальную частоту ядра в governour
void cpu::updateScalingMinFreq(void)
{
    if (b_isOnline) //если ядро работает
    {
        char str_path[70]; // Переменная для пути к файлу
        sprintf(str_path,"%s%d%s", "/sys/devices/system/cpu/cpu", n_coreNumber, "/cpufreq/scaling_min_freq\0");
        // Склейка пути
        std::ifstream fin; // Открываем файл и записываем в переменную
        fin.open(str_path);
        if(fin.fail())
        {
            std::cout << "Unable to open scalingMinFreq file." << std::endl;
            f_scalingMinFreq = 0.0f;
            return;
        }
        fin >> f_scalingMinFreq;
        fin.close();
    }
    else
        f_scalingMinFreq = 0.0f;
}
// Обновляем максимальную частоту ядра в governour
void cpu::updateScalingMaxFreq(void)
{
    if (b_isOnline) //если ядро работает
    {
        char str_path[70]; // Переменная для пути к файлу
        sprintf(str_path,"%s%d%s", "/sys/devices/system/cpu/cpu", n_coreNumber, "/cpufreq/scaling_max_freq\0");
        // Склейка пути
        std::ifstream fin; // Открываем файл и записываем в переменную
        fin.open(str_path);
        if(fin.fail())
        {
            std::cout << "Unable to open scalingMaxFreq file." << std::endl;
            f_scalingMaxFreq = 0.0f;
            return;
        }
        fin >> f_scalingMaxFreq;
        fin.close();
    }
    else
        f_scalingMaxFreq = 0.0f;
}
//public:
void cpu::updateInformation(void)
{
    updateOnline();
    updateGovernor();
    updateScalingMaxFreq();
    updateScalingMinFreq();
    updateCurFreq();
    updateMaxFreq();
    updateMinFreq();
}
void cpu::DEBUGprintInformation(void)
{
    std::cout << "CPU" << n_coreNumber << " is online: " << b_isOnline << std::endl;
    std::cout << "CPU" << n_coreNumber << " maximum freq: " << f_maxFreq/1000 << "MHz" << std::endl;
    std::cout << "CPU" << n_coreNumber << " minimum freq: " << f_minFreq/1000 << "MHz" << std::endl;
    std::cout << "CPU" << n_coreNumber << " current freq: " << f_curFreq/1000 << "MHz" << std::endl;
    std::cout << "CPU" << n_coreNumber << " governor minimum freq: " << f_scalingMinFreq/1000 << "MHz" << std::endl;
    std::cout << "CPU" << n_coreNumber << " governor maximum freq: " << f_scalingMaxFreq/1000 << "MHz" << std::endl;
}
void cpu::printShit(void)
{
    updateInformation();
    DEBUGprintInformation();
}
