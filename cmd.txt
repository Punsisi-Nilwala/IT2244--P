@echo off
:: Displays the current system date.
echo %date%

:: Extract Year from system date.
set current_year=%date:~10,4%
echo Current year is %current_year%

:: Extract Month from system date.
set current_month=%date:~4,2%
echo Current month is %current_month%

:: Extract Date from system date.
set current_date=%date:~7,2%
echo Current date is %current_date%

:: Extract Day Name.
set day_name=%date:~0,3%
echo Today is %day_name%

:: calculation of the day of the year.
set /a current_day_of_year = ((%current_month%-1)*30) + %current_date%
echo Current day of year is %current_day_of_year%

:: Calculate week of the year.
set /a week_of_year = (%current_day_of_year% + 6) / 7
echo Week of Year is %week_of_year%

:: Calculate week of the month.
set /a week_of_month = (%current_date% + 6) / 7
echo Week of Month is %week_of_month%

pause