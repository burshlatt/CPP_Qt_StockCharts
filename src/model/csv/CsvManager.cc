#include <regex>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

#include "CsvManager.h"

namespace fs = std::filesystem;

namespace s21 {
int CsvManager::GetYear(const std::string& date) {
    return std::stoi(date.substr(0, 4));
}

int CsvManager::GetMonth(const std::string& date) {
    return std::stoi(date.substr(5, 2));
}

int CsvManager::GetDay(const std::string& date) {
    return std::stoi(date.substr(8, 2));
}

std::map<long, double> CsvManager::GetData() {
    if (data_.empty()) {
        throw std::runtime_error("Error: Data is not parsed yet.");
    }

    return data_;
}

bool CsvManager::ParseCsv(std::string_view filename) {
    std::ifstream file(fs::path(filename), std::ios_base::in);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;

        return false;
    }

    ReadCsv(file);

    return true;
}

void CsvManager::ReadCsv(std::ifstream& file) {
    data_.clear();

    std::string line;

    std::getline(file, line); // Для скипа первой строки в csv файле

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string dateStr;
        double value;
        
        if (std::getline(ss, dateStr, ',') && IsValidDate(dateStr) && ss >> value) {
            boost::gregorian::date date = ParseDate(dateStr);
            data_[DateToSeconds(date)] = value;
        } else {
            std::cerr << "Error parsing line: " << line << std::endl;
        }
    }
}

bool CsvManager::IsValidDate(const std::string& date) {
    static const std::regex dateRegex("\\d{4}-\\d{2}-\\d{2}");

    if (!std::regex_match(date, dateRegex)) {
        return false;
    }

    try {
        boost::gregorian::date(boost::gregorian::greg_year(GetYear(date)), GetMonth(date), GetDay(date));
    } catch (const std::exception& e) {
        return false;
    }

    return true;
}

boost::gregorian::date CsvManager::ParseDate(const std::string& dateStr) {
    static const std::locale loc(std::locale::classic(), new boost::gregorian::date_input_facet("%Y-%m-%d"));
    std::istringstream is(dateStr);

    is.imbue(loc);

    boost::gregorian::date date;

    is >> date;

    return date;
}

int CsvManager::DateToDay(const boost::gregorian::date& time) {
    static const boost::gregorian::date epoch(1970, 1, 1);
    boost::gregorian::date_duration day = time - epoch;

    return day.days();
}

long CsvManager::DateToSeconds(const boost::gregorian::date& time) {
    static const boost::gregorian::date epoch(1970, 1, 1);
    boost::gregorian::date_duration days = time - epoch;
    long seconds = days.days() * 24LL * 60LL * 60LL;

    return seconds;
}

long long CsvManager::DateToMilliseconds(const boost::gregorian::date& time) {
    static const boost::gregorian::date epoch(1970, 1, 1);
    boost::gregorian::date_duration days = time - epoch;
    long long milliseconds = days.days() * 24LL * 60LL * 60LL * 1000LL;

    return milliseconds;
}
} // namespace s21
