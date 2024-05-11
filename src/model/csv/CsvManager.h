#ifndef A8_ALGORITHMIC_TRADING_SRC_MODEL_CSV_CSV_MANAGER_H
#define A8_ALGORITHMIC_TRADING_SRC_MODEL_CSV_CSV_MANAGER_H

#include <map>
#include <string>
#include <string_view>
#include <boost/date_time.hpp>

namespace s21 {

class CsvManager {
public:
    std::map<long, double> GetData();

    bool ParseCsv(std::string_view filename);

private:
    int GetDay(const std::string& date);
    int GetYear(const std::string& date);
    int GetMonth(const std::string& date);

    void ReadCsv(std::ifstream& file);

    bool IsValidDate(const std::string& date);

    boost::gregorian::date ParseDate(const std::string& dateStr);

    int DateToDay(const boost::gregorian::date& time);
    long DateToSeconds(const boost::gregorian::date& time);
    long long DateToMilliseconds(const boost::gregorian::date& time);

private:
    std::map<long, double> data_;
};

}  // namespace s21

#endif  // A8_ALGORITHMIC_TRADING_SRC_MODEL_CSV_CSV_MANAGER_H
