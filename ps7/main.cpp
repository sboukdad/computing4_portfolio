// Copyright Salma Boukdad 2026

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include "boost/date_time/posix_time/posix_time.hpp"

using boost::posix_time::ptime;
using boost::posix_time::time_duration;
using boost::posix_time::time_from_string;
using std::cerr;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::regex;
using std::regex_search;
using std::smatch;
using std::string;
using std::vector;

// a single service start/completion during a boot sequence
struct ServiceEntry {
    string name;
    int startLine;
    int endLine;
    int elapsedMs;
};

// a single boot sequence with all its services
struct BootEntry {
    int startLine;
    int endLine;            // set to 0 if incomplete
    string startTime;
    string endTime;
    int64_t bootMs;
    bool completed;
    vector<ServiceEntry> services;
};

int
main(int argc, char *argv[]) {
    // make sure log file arg was provided
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <log_filename>" << endl;
        return 1;
    }

    // extract the filename from the full path
    string filepath = argv[1];
    string filename = filepath;

    size_t slashPos = filepath.find_last_of("/\\");
    if (slashPos != string::npos) {
        filename = filepath.substr(slashPos + 1);
    }

    // open input log file for reading
    ifstream iFile(filepath);
    if (!iFile.is_open()) {
        cerr << "Error: Could not open file " << filepath << endl;
        return 1;
    }

    // output report file has the same name as the log with .rpt appended

    // regex for "server started", matches the device boot start message
    regex startRegex(
    R"(^(\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}):\s*\(log\.c\.166\) server started)");

    // Regex for boot completion
    regex successRegex(R"(^(\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2})[\.\d]*:INFO:oejs\.AbstractConnector:Started SelectChannelConnector)");

    // regex to capture the service name
    regex serviceStartRegex(R"(^Starting Service\.\s+(\S+)\s+\S+)");

    // regex to capture the service name and elapsed ms
    regex serviceEndRegex(
        R"(^Service started successfully\.\s+(\S+)\s+\S+\s+\((\d+) ms\))");

    string line;
    int lineNum = 0;
    int totalLines = 0;
    bool isBooting = false;     // track whether a boot sequence is in progress
    ptime startTime;            // timestamp of the current boot start

    // int startLine = 0;          // line number of the current boot start
    // string startTimestamp;      // string timestamp of the current

    vector<BootEntry> boots;
    BootEntry currentBoot;

    vector<std::pair<string, int>> pendingServices;

    while (getline(iFile, line)) {
        lineNum++;

        // strip window-style carriage returns so regex anchors work correctly
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        smatch match;

        // check for startup message
        if (regex_search(line, match, startRegex)) {
            // error handle if a new boot starts before previous finishes
            if (isBooting) {
                // oFile << "**** Incomplete boot ****" << endl;
                // oFile << endl;
                currentBoot.completed = false;
                currentBoot.endLine = 0;
                boots.push_back(currentBoot);
                pendingServices.clear();
            }

            currentBoot = BootEntry();
            currentBoot.startLine = lineNum;
            currentBoot.startTime = match[1];
            currentBoot.completed = false;
            currentBoot.endLine = 0;
            currentBoot.bootMs = 0;
            startTime = time_from_string(currentBoot.startTime);
            isBooting = true;

        } else if (isBooting && regex_search(line, match, successRegex)) {
            // boot completed, gets elapsed time
            currentBoot.endTime = match[1];
            currentBoot.endLine = lineNum;
            ptime endTime = time_from_string(currentBoot.endTime);
            time_duration elapsed = endTime - startTime;
            currentBoot.bootMs = elapsed.total_seconds() * 1000;
            currentBoot.completed = true;
            boots.push_back(currentBoot);
            pendingServices.clear();
            isBooting = false;
        } else if (isBooting && regex_search(line, match, serviceStartRegex)) {
            // get service start and store name + line num
            pendingServices.push_back({match[1], lineNum});
        } else if (isBooting && regex_search(line, match, serviceEndRegex)) {
            // match the completion to the most recent pending entry
            // with the same name
            string svcName = match[1];
            int elapsedMs = std::stoi(match[2]);

            // search pending services in reverse for the most
            // recent matching start
            for (int i = pendingServices.size() - 1; i >= 0; i--) {
                if (pendingServices[i].first == svcName) {
                    ServiceEntry svc;
                    svc.name = svcName;
                    svc.startLine = pendingServices[i].second;
                    svc.endLine = lineNum;
                    svc.elapsedMs = elapsedMs;
                    currentBoot.services.push_back(svc);
                    pendingServices.erase(pendingServices.begin() + i);
                    break;
                }
            }
        }
    }

    // report failure if the file ends while booting
    if (isBooting) {
       // oFile << "**** Incomplete boot ****" << endl;
       currentBoot.completed = false;
       currentBoot.endLine = 0;
       boots.push_back(currentBoot);
    }

    totalLines = lineNum;

    // count the completed boots for summary header
    int completedCount = 0;
    for (const auto& b : boots) {
        if (b.completed) {
            completedCount++;
        }
    }

    // write the report file
    ofstream oFile(filepath + ".rpt");

    // summary header
    oFile << "Device Boot Report" << endl << endl;
    oFile << "InTouch log file: " << filename << endl;
    oFile << "Lines Scanned: " << totalLines << endl << endl;
    oFile << "Device boot count: initiated = " << boots.size()
          << ", completed: " << completedCount << endl << endl;


    // write each boot entry
    for (const auto& boot : boots) {
        oFile << "=== Device boot ===" << endl;
        oFile << boot.startLine << "(" << filename << "): "
              << boot.startTime << " Boot Start" << endl;

        if (boot.completed) {
            oFile << boot.endLine << "(" << filename << "): "
                  << boot.endTime << " Boot Completed" << endl;
            oFile << "\tBoot Time: " << boot.bootMs << "ms" << endl << endl;

            // write service entries
            if (!boot.services.empty()) {
                oFile << "Services" << endl;
                for (const auto& svc : boot.services) {
                    oFile << "\t" << svc.name << endl;
                    oFile << "\t\t Start: " << svc.startLine
                          << "(" << filename << ")" << endl;
                    oFile << "\t\t Completed: " << svc.endLine
                          << "(" << filename << ")" << endl;
                    oFile << "\t\t Elapsed Time: " << svc.elapsedMs
                          << " ms" << endl;
                }
                oFile << endl;
            }
        } else {
            oFile << "**** Incomplete boot ****" << endl << endl;
        }
    }

    iFile.close();
    oFile.close();

    return 0;
}
