#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <getopt.h>
#include <fmt/core.h>
#include <fmt/chrono.h>

// Option class to parse command-line arguments
class Option {
public:
    int year;
    int month;
    int before;
    int after;
    int row_num;

    Option() : year(0), month(0), before(0), after(0), row_num(3) {}

    void parseArgs(int argc, char* argv[]) {
        int opt;
        while ((opt = getopt(argc, argv, "A:B:d:r:m:")) != -1) {
            switch (opt) {
                case 'A':
                    after = std::stoi(optarg);
                    break;
                case 'B':
                    before = std::stoi(optarg);
                    break;
                case 'd': {
                    std::string date = optarg;
                    size_t pos = date.find('-');
                    if (pos != std::string::npos) {
                        year = std::stoi(date.substr(0, pos));
                        month = std::stoi(date.substr(pos + 1));
                    }
                    break;
                }
                case 'r':
                    row_num = std::stoi(optarg);
                    break;
                case 'm':
                    month = std::stoi(optarg);
                    break;
                default:
                    std::cerr << "Usage: cal [-A months] [-B months] [-d yyyy-mm] [-r rownum] [-m month] [yyyy]\n";
                    exit(1);
            }
        }
        if (optind < argc) {
            year = std::stoi(argv[optind]);
            month = 0; // Display full year
        }
        if (year == 0) {
            std::time_t t = std::time(nullptr);
            std::tm* now = std::localtime(&t);
            year = now->tm_year + 1900;
            if (month == 0) month = now->tm_mon + 1;
        }
    }
};

// Calendar class to generate and print the calendar
class Calendar {
public:
    int year;
    int month;
    int before;
    int after;
    int row_num;

    Calendar(const Option& opt) : year(opt.year), month(opt.month), before(opt.before), after(opt.after), row_num(opt.row_num) {}

    std::vector<std::string> generateMonth(int y, int m) {
        std::vector<std::string> lines;

        // Set up std::tm for the first day of the month
        std::tm tm = {0, 0, 0, 1, m - 1, y - 1900}; // tm_mon is 0-based, m is 1-based
        std::mktime(&tm);

        // Get month name using fmt::format
        std::string month_name = fmt::format("{:%B}", tm);
        lines.push_back(fmt::format("{:^20}", month_name + " " + std::to_string(y)));
        lines.push_back("Su Mo Tu We Th Fr Sa");

        // Get the starting weekday (0 = Sunday, 6 = Saturday)
        int start_day = tm.tm_wday;

        // Calculate days in the month
        std::tm tm_last = tm;
        tm_last.tm_mon += 1; // Move to next month
        tm_last.tm_mday = 0; // Day 0 is the last day of the previous month
        std::mktime(&tm_last);
        int days_in_month = tm_last.tm_mday;

        // Build the calendar lines
        std::string week;
        for (int i = 0; i < start_day; ++i) {
            week += "   "; // 3 spaces for alignment
        }
        for (int day = 1; day <= days_in_month; ++day) {
            week += fmt::format("{:>2} ", day);
            if ((start_day + day) % 7 == 0 || day == days_in_month) {
                lines.push_back(week);
                week.clear();
            }
        }
        while (lines.size() < 8) {
            lines.push_back(""); // Pad with empty lines if needed
        }

        return lines;
    }

    std::vector<std::vector<std::string>> generateMultiMonths() {
        std::vector<std::vector<std::string>> months;
        if (month == 0) { // Full year
            for (int m = 1; m <= 12; ++m) {
                months.push_back(generateMonth(year, m));
            }
        } else {
            int start_month = month - before;
            int end_month = month + after;
            for (int m = start_month; m <= end_month; ++m) {
                int y = year + (m - 1) / 12;
                int cm = (m - 1) % 12 + 1;
                if (cm <= 0) {
                    y -= 1;
                    cm += 12;
                }
                months.push_back(generateMonth(y, cm));
            }
        }
        return months;
    }

    void printCalendar() {
        auto months = generateMultiMonths();
        int total_months = months.size();
        for (int i = 0; i < total_months; i += row_num) {
            int end = std::min(i + row_num, total_months);
            for (int line = 0; line < 8; ++line) {
                for (int j = i; j < end; ++j) {
                    std::cout << months[j][line] << "  ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
    }
};

int main(int argc, char* argv[]) {
    Option opt;
    opt.parseArgs(argc, argv);
    Calendar cal(opt);
    cal.printCalendar();
    return 0;
}