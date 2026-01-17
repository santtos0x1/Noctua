#ifndef UTILS_H
#define UTILS_H

// Terminal debug colors
#define CLR_RED     "\033[0;31m"
#define CLR_GREEN   "\033[0;32m"
#define CLR_YELLOW  "\033[0;33m"
#define CLR_RESET   "\033[0m"

// wifi_scan.cpp
#define CONN_TIMEOUT_MS       (10 * 1000)
#define GET_RSSI_QUALITY(rssi) ( \
    (rssi >= -50) ? "Strong" :  \
    (rssi >= -70) ? "Medium" :  \
    (rssi >= -85) ? "Weak"   : "V.Weak" \
)

#endif // !UTILS_h
