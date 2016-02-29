#ifndef CPU_H
#define CPU_H
#include <vector>
#include <string>
#include <QString>

// Класс ядра процессора.
class cpu
{
public:
    double f_maxFreq, f_minFreq, f_curFreq; // Переменные для макс мин и текущей частоты ядра.
    double f_scalingMaxFreq, f_scalingMinFreq; // Переменные для макс и мин частоты governor.
    int n_coreNumber; // Номер ядра.
    bool b_isOnline; // В сети ли ядро.
    char str_governor[60];
    std::vector<std::string> vec_governorList;

    cpu(int A);

private:
    void split(const std::string &s, const char* delim, std::vector<std::string> & v);
    void updateGovernor(void);
    void updateGovernorList(void);
    // Обновляем состояние ядра(в "сети" или нет)
    void updateOnline(void);
    // Обновляем максимальную частоту ядра
    void updateMaxFreq(void);
    // Обновляем минимальную частоту ядра
    void updateMinFreq(void);
    // Обновляем текущую частоту ядра
    void updateCurFreq(void);
    // Обновляем минимальную частоту ядра в governour
    void updateScalingMinFreq(void);
    // Обновляем максимальную частоту ядра в governour
    void updateScalingMaxFreq(void);
public:
    void updateInformation(void);
    void DEBUGprintInformation(void);
    void drawProgressBar(void);
    void printShit(void);
    void setOnline(bool online);
    void setMaxScalingFreq(int maxScaingFreq);
    void setMinScalingFreq(int minScaingFreq);
    void setGovernor(QString governor);
};

#endif // CPU_H
