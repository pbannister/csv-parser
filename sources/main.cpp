#include "parser/reader.h"
#include "parser/scanner.h"
#include "base/string.h"
#include "base/timer.h"

#include "common.h"
#include <getopt.h>

int nWorkers = 1;
const char* sRunner = "/usr/bin/perl";
const char* sScript = 0;

long asMB(double n) {
    return (long)(n / (1024 * 1024));
}

int rateMB(double r, long dt) {
    if (0 == dt) {
        return 0;
    }
    r /= dt; // bytes per millisecond
    r *= 1000; // bytes per second
    r /= 1024 * 1024; // megabytes per second
    return (int)r; // fraction is not significant
}

void usage() {
    fprintf(stderr,
        "Usage: parse-csv [ options ] files...\n"
        "Where options are:\n"
        "  -n N        number of worker processes\n"
        "  -r run      name of executable to run as a worker (default is /usr/bin/perl)\n"
        "  -s script   name of script (passed as argument to worker)\n"
        "The lines read from given list of files are distributed equally among the workers.\n"
        );
    exit(3);
}

const char* now() {
    time_t t = time(0);
    char* s = ctime(&t);
    char* p = strchr(s,'\n');
    *p = 0;
    return s;
}

int main(int ac, char** av) {
	TRACE_FN(main);
    for (;;) {
        int c = getopt(ac, av, "n:r:s:");
        if (EOF == c) break;
        switch (c) {
        case 'n': nWorkers = atoi(optarg); break;
        case 'r': sRunner = optarg; break;
        case 's': sScript = optarg; break;
        default: usage();
        }
    }
    fprintf(stderr,"TIME %s\n",now());
    base::timer_o t;
    csv_parser::scanner_o scanner;
    VERIFY(scanner.open_to(nWorkers,sRunner,sScript));
    if (ac <= optind) {
        csv_parser::reader_o reader;
        scanner.on_file("STDIN");
        reader.file_scan(scanner);
    } else {
        csv_parser::reader_o::files_scan(scanner,optind,ac,av);
    }
    scanner.workers_close();
    t.split();
    fprintf(stderr,"TIME %s\n",now());
    fprintf(
        stderr,
        "Elapsed (ms): %ld"
        ", total (MB): %ld"
        "\n",
        (long)t.elapsed(),
        asMB(scanner.n_bytes_total)
        );
    int v = rateMB(scanner.n_bytes_total, t.elapsed());
    // Report rates only if somewhat meaningful
    if (1 < v) {
        fprintf(stderr,"Scanned %d MB/s\n",v);
    }
    return 0;
}
