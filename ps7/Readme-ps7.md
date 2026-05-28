# PS7: Kronos Log Parsing

## Contact
Name: Salma Boukdad
Section: 203
Time to Complete: 3 days


## Description
Explain what the project does.

This program parses Kronos InTouch device log files to report boot sequences.
It reads a log file line by line, identifies boot start and completion events
using regular expressions, and writes a formatted report (.rpt) describing
each boot: whether it completed successfully, how long it took, or whether it
failed to complete.

### Features
Describe what your major decisions were and why you did things that way.

- Uses std::regex to match boot start and completion log entries
- Uses Boost posix_time to parse timestamps and calculate elapsed boot time
- Handles incomplete boots, if a new boot starts before the previous one finishes,
    or if the file ends mid-boot, the boot is reported as incomplete
- Strips Window-style carriage returns ('\r') to ensure regex anchors work
    correctly on cross-platform log files
- Extracts just the filename from the full path so report lines are clean
- Stores all boot and service data in structs before writing, enabling the summary
    header to be written first with accurate totals
- Tracks individual services per boot, matching the start and completion lines
    by service name to report start line, completion line, and elapsed time


### Approach
Describe your overall approach to solving this problem.

The program reads the log file line by line, maintaining a boolean 'isBooting'
flag to track whether a boot sequence is currently in progress. When a boot
start line is detected, it records the line number and timestamp. When a
completion line is detected while booting, it calculates the elapsed time and
writes the result. If a new boot start is detected while already booting,
the previous boot is marked complete. Any boot still in progress at the end is
also marked complete. Service start lines are stored in a pending list during a boot,
when a service completion line is found, it's matched to the most recent pending entry with the same name and saved to the current boot's service list. All boot entries are collected first,
then the report is written allowing the summary header to be written before the individual boot entries.

### Regex
What regular expressions did you use?  Explain what each one does.

Boot Start:
    ^(\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}):\s*(log.c.166) server started
This matches lines like '2013-05-04 05:28:13: (log.c.172) server started'.
It captures the timestamp, allows any 'log.c' line number,
and allows for optional whitespace before the parenthesis.

Boot Completion:
    ^(\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2})[.\d]*:INFO:oejs.AbstractConnector:Started SelectChannelConnector
This matches the lines like '2014-01-26 09:58:04.362:INFO:oejs.AbstractConnector@0.0.0:9080'.
It captures only the HH:MM:SS portion of the timestamp excluding milliseconds so
it can be parsed correctly by Boost 'time_from_string'.

Service Start:
    ^Starting Service.\s+(\S+)\s+\S+
Matches lines like 'Starting Service. Logging 1.0'. This captures the service name
before the version number.

Service Completion:
    ^Service started successfully.\s+(\S+)\s+\S+\s+((\d+) ms)
Matches lines like 'Service started successfully. Logging 1.0 (268 ms)'.
It captures the service name and elapsed ms.

### Issues
What did you have trouble with?  What did you learn?  What doesn't work?  Be honest.  You might be penalized if you claim something works and it doesn't.

The biggest issue was that the log file uses windows-style line ending ('\r\n'),
which caused the '^' regex anchor to fail silently, the file was being read but nothing
matched. Stipping the trailing '\r' from each line fixed this.
Another early issue was the the boot start regex initially hardcoded 'log.c.166',
but the actual log files use different line number like 'log.c.172', requiring
the pattern to be generalized to '\d+'.
A subtle debugging issue also occurred where a colon was accidentally included 
in a debug output statement, making the captured timestamp appear incorrect
when it was actually being captured correctly all along.

### Extra Credit
Anything special you did.  This is required to earn bonus points.

- Summary header: the report begins with 'Device Boot Report',
'InTouch log file', 'Lines Scanned', and 'Device boot count' showing total
initiated and completed boots.
- Service logging: each completed boot includes a 'Services' section listing
every service with its start line, completion line, and elapsed time in ms.

## Acknowledgements
List all sources of help including the instructor or TAs, classmates, and web pages.

- Boost date_time documentation: https://www.boost.org/doc/libs/1_57_0/doc/html/date_time.html
- C++ regex reference: https://en.cppreference.com/w/cpp/regex
