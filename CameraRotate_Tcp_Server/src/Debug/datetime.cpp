#include "datetime.h"

std::string datetime::DataZ = "to_timestamp('01-01-2000 00:00:00', 'dd-mm-yyyy hh24:mi:ss')";
long int datetime::cntTime=0;

std::string datetime::utcExample()
{     
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S.%MS");
    auto str = oss.str();

     return str;

}


char* strptime(const char* s,
               const char* f,
               struct tm* tm) {
     // Isn't the C++ standard lib nice? std::get_time is defined such that its
     // format parameters are the exact same as strptime. Of course, we have to
     // create a string stream first, and imbue it with the current C locale, and
     // we also have to make sure we return the right things if it fails, or
     // if it succeeds, but this is still far simpler an implementation than any
     // of the versions in any of the C standard libraries.
     std::istringstream input(s);
     input.imbue(std::locale(setlocale(LC_ALL, nullptr)));
     input >> std::get_time(tm, f);
     if (input.fail()) {
         return nullptr;
     }
     return (char*)(s + input.tellg());
}

long int datetime::string2timestamp(const string &str)
{
    time_t t;
    struct tm tm;
    //setlocale(LC_ALL,"/QSYS.LIB/EN_US.LOCALE");
    if(strptime(str.c_str(), "%Y-%m-%d %H:%M:%S.%MS",&tm) == NULL)
          return -1; 
    return mktime(&tm);
}
// cout<<string2timestamp(string{"2017-12-07 11:09:15.451"})<<"\n";

string datetime::unixTimeToHumanReadable(long int seconds)
{

    // Save the time in Human
    // readable format
    string ans = "";

    // Number of days in month
    // in normal year
    int daysOfMonth[] = { 31, 28, 31, 30, 31, 30,
                          31, 31, 30, 31, 30, 31 };

    long int currYear, daysTillNow, extraTime, extraDays,
        index, date, month, hours, minutes, secondss,
        flag = 0;

    // Calculate total days unix time T
    daysTillNow = seconds / (24 * 60 * 60);
    extraTime = seconds % (24 * 60 * 60);
    currYear = 1970;

    // Calculating current year
    while (true) {
        if (currYear % 400 == 0
            || (currYear % 4 == 0 && currYear % 100 != 0)) {
            if (daysTillNow < 366) {
                break;
            }
            daysTillNow -= 366;
        }
        else {
            if (daysTillNow < 365) {
                break;
            }
            daysTillNow -= 365;
        }
        currYear += 1;
    }
    // Updating extradays because it
    // will give days till previous day
    // and we have include current day
    extraDays = daysTillNow + 1;

    if (currYear % 400 == 0
        || (currYear % 4 == 0 && currYear % 100 != 0))
        flag = 1;

    // Calculating MONTH and DATE
    month = 0, index = 0;
    if (flag == 1) {
        while (true) {

            if (index == 1) {
                if (extraDays - 29 < 0)
                    break;
                if(month<12)month += 1;
                extraDays -= 29;
            }
            else {
                if (extraDays - daysOfMonth[index] < 0) {
                    break;
                }
                if(month<12)month += 1;
                extraDays -= daysOfMonth[index];
            }
            index += 1;
        }
    }
    else {
        while (true) {

            if (extraDays - daysOfMonth[index] < 0) {
                break;
            }
            if(month<12)month += 1;
            extraDays -= daysOfMonth[index];
            index += 1;
        }
    }

    // Current Month
    if (extraDays > 0) {
        if(month<12)month += 1;
        date = extraDays;
    }
    else {
        if (month == 2 && flag == 1)
            date = 29;
        else {
            date = daysOfMonth[month - 1];
        }
    }

    // Calculating HH:MM:YYYY
    hours = extraTime / 3600;
    minutes = (extraTime % 3600) / 60;
    secondss = (extraTime % 3600) % 60;

    ans += to_string(date);
    ans += "/";
    ans += to_string(month);
    ans += "/";
    ans += to_string(currYear);
    ans += " ";
    ans += to_string(hours);
    ans += ":";
    ans += to_string(minutes);
    ans += ":";
    ans += to_string(secondss);

    // Return the time
    return ans;
}


std::string datetime::GenerateTime()
{
    long int ms = string2timestamp(utcExample())+3600*24*100+
    (cntTime++)*(3600*100);
    return "to_timestamp('"
           +unixTimeToHumanReadable(ms)
           +"', 'dd/mm/yyyy hh24:mi:ss')";
}

std::string datetime::GenerateTime_sqlite()
{
    long int ms = string2timestamp(utcExample())+3600*24*100+
                  (cntTime++)*(3600*100);
    return "'"+ unixTimeToHumanReadable(ms)+"'";
}

std::string datetime::GenerateTime_exp()
{
    return "' '";
}


