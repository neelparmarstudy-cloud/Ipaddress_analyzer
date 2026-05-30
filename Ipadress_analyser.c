#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

// Function to check if a string is a valid number (for IPv4)
bool isValidNumber(const char *str) {
    if (strlen(str) == 0) return false;
    for (int i = 0; str[i]; i++) {
        if (str[i] < '0' || str[i] > '9') return false;
    }
    return true;
}

// Function to validate IPv4 address
bool isValidIPv4(const char *ip) {
    if (strlen(ip) > 15) {  // Max length for IPv4 (e.g., "255.255.255.255")
        printf("Error: IPv4 address too long!\n");
        return false;
    }

    int dots = 0, num = 0, i = 0, start = 0;
    char octet[4];
    int octetIndex = 0;

    while (ip[i]) {
        if (ip[i] == '.') {
            if (octetIndex == 0 || octetIndex > 3) {
                printf("Error: Invalid octet length!\n");
                return false;
            }
            octet[octetIndex] = '\0';
            if (!isValidNumber(octet)) {
                printf("Error: Non-numeric octet!\n");
                return false;
            }
            num = atoi(octet);
            if (num < 0 || num > 255) {
                printf("Error: Octet value out of range (0-255)!\n");
                return false;
            }
            if (octetIndex > 1 && octet[0] == '0') {
                printf("Error: Leading zeros not allowed!\n");
                return false;
            }
            dots++;
            octetIndex = 0;
            start = i + 1;
        } else {
            if (octetIndex >= 3) {
                printf("Error: Octet too long!\n");
                return false;
            }
            octet[octetIndex++] = ip[i];
        }
        i++;
    }

    // Process last octet
    if (octetIndex == 0 || octetIndex > 3) {
        printf("Error: Invalid last octet length!\n");
        return false;
    }
    octet[octetIndex] = '\0';
    if (!isValidNumber(octet)) {
        printf("Error: Non-numeric last octet!\n");
        return false;
    }
    num = atoi(octet);
    if (num < 0 || num > 255) {
        printf("Error: Last octet value out of range!\n");
        return false;
    }
    if (octetIndex > 1 && octet[0] == '0') {
        printf("Error: Leading zeros not allowed in last octet!\n");
        return false;
    }
    dots++;

    if (dots != 4) {
        printf("Error: IPv4 address must have exactly 4 octets!\n");
        return false;
    }
    return true;
}

// Function to validate IPv6 address
bool isValidIPv6(const char *ip) {
    if (strlen(ip) > 39) {  // Max length for IPv6
        printf("Error: IPv6 address too long!\n");
        return false;
    }

    char ipCopy[40];
    strncpy(ipCopy, ip, sizeof(ipCopy));
    ipCopy[sizeof(ipCopy) - 1] = '\0';

    int colons = 0, doubleColon = 0, segments = 0;
    for (int i = 0; ipCopy[i]; i++) {
        if (ipCopy[i] == ':') {
            colons++;
            if (i > 0 && ipCopy[i-1] == ':' && ipCopy[i+1] != '\0') {
                doubleColon++;
            }
        }
    }

    if (colons < 2 || colons > 7 || doubleColon > 1) {
        printf("Error: Invalid colon count or multiple double colons in IPv6 address!\n");
        return false;
    }

    char *token = strtok(ipCopy, ":");
    while (token != NULL) {
        segments++;
        if (strlen(token) > 4 || strlen(token) == 0) {
            printf("Error: Invalid segment length in IPv6 address!\n");
            return false;
        }
        for (int i = 0; token[i]; i++) {
            char c = tolower(token[i]);
            if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'))) {
                printf("Error: Invalid hexadecimal character in IPv6 address!\n");
                return false;
            }
        }
        token = strtok(NULL, ":");
    }

    if (doubleColon == 0 && segments != 8) {
        printf("Error: IPv6 address must have 8 segments without double colon!\n");
        return false;
    }
    if (doubleColon == 1 && (segments < 2 || segments > 7)) {
        printf("Error: Invalid segment count with double colon in IPv6 address!\n");
        return false;
    }

    return true;
}

// Function to analyze IPv6 address
void analyzeIPv6(const char *ip) {
    printf("%-20s: %s\n", "IP Address", ip);

    // Check for loopback
    if (strcmp(ip, "::1") == 0) {
        printf("%-20s: %s\n", "Type", "Loopback");
        return;
    }

    // Check for unspecified address
    if (strcmp(ip, "::") == 0) {
        printf("%-20s: %s\n", "Type", "Unspecified");
        return;
    }

    // Check for link-local
    char ipCopy[40];
    strncpy(ipCopy, ip, sizeof(ipCopy));
    ipCopy[sizeof(ipCopy) - 1] = '\0';
    char *firstSegment = strtok(ipCopy, ":");
    if (firstSegment && strncmp(firstSegment, "fe80", 4) == 0) {
        printf("%-20s: %s\n", "Type", "Link-Local");
        return;
    }

    // Assume global unicast for others
    printf("%-20s: %s\n", "Type", "Global Unicast (assumed)");
}

// IPv4 utility functions
char getIPClass(int firstOctet) {
    if (firstOctet >= 1 && firstOctet <= 126) return 'A';
    if (firstOctet >= 128 && firstOctet <= 191) return 'B';
    if (firstOctet >= 192 && firstOctet <= 223) return 'C';
    if (firstOctet >= 224 && firstOctet <= 239) return 'D';
    return 'E';
}

void getDefaultSubnetMask(char ipClass, char *subnetMask) {
    switch (ipClass) {
        case 'A': strncpy(subnetMask, "255.0.0.0", 16); break;
        case 'B': strncpy(subnetMask, "255.255.0.0", 16); break;
        case 'C': strncpy(subnetMask, "255.255.255.0", 16); break;
        default: strncpy(subnetMask, "N/A", 16); break;
    }
}

void calculateNetworkAndHost(const char *ip, char *networkAddr, char *hostAddr, char ipClass) {
    int octets[4];
    sscanf(ip, "%d.%d.%d.%d", &octets[0], &octets[1], &octets[2], &octets[3]);

    switch (ipClass) {
        case 'A':
            snprintf(networkAddr, 16, "%d.0.0.0", octets[0]);
            snprintf(hostAddr, 16, "0.%d.%d.%d", octets[1], octets[2], octets[3]);
            break;
        case 'B':
            snprintf(networkAddr, 16, "%d.%d.0.0", octets[0], octets[1]);
            snprintf(hostAddr, 16, "0.0.%d.%d", octets[2], octets[3]);
            break;
        case 'C':
            snprintf(networkAddr, 16, "%d.%d.%d.0", octets[0], octets[1], octets[2]);
            snprintf(hostAddr, 16, "0.0.0.%d", octets[3]);
            break;
        default:
            strncpy(networkAddr, "N/A", 16);
            strncpy(hostAddr, "N/A", 16);
            break;
    }
}

bool isLoopback(const char *ip) {
    int firstOctet;
    sscanf(ip, "%d", &firstOctet);
    return firstOctet == 127;
}

bool isPrivate(const char *ip) {
    int octets[4];
    sscanf(ip, "%d.%d.%d.%d", &octets[0], &octets[1], &octets[2], &octets[3]);
    if (octets[0] == 10) return true;
    if (octets[0] == 172 && octets[1] >= 16 && octets[1] <= 31) return true;
    if (octets[0] == 192 && octets[1] == 168) return true;
    return false;
}

void getCIDRNotation(char ipClass, char *cidr) {
    switch (ipClass) {
        case 'A': strncpy(cidr, "/8", 5); break;
        case 'B': strncpy(cidr, "/16", 5); break;
        case 'C': strncpy(cidr, "/24", 5); break;
        default: strncpy(cidr, "N/A", 5); break;
    }
}

int main() {
    char ip[40], subnetMask[16], networkAddr[16], hostAddr[16], cidr[5];
    int choice;
    char continueChoice;

    do {
        printf("\nIP Address Analyzer\n");
        printf("------------------\n");
        printf("1. Analyze IPv4 Address\n");
        printf("2. Analyze IPv6 Address\n");
        printf("Enter your choice (1 or 2): ");
        if (scanf("%d", &choice) != 1) {
            printf("Error: Invalid input for choice!\n");
            while (getchar() != '\n');
            continue;
        }
        
        while (getchar() != '\n');

        switch (choice) {
            case 1: // IPv4 Analysis
                printf("\nEnter an IPv4 address (e.g., 192.168.1.1): ");
                if (fgets(ip, sizeof(ip), stdin) == NULL) {
                    printf("Error: Failed to read input!\n");
                    continue;
                }
                ip[strcspn(ip, "\n")] = '\0';
                if (strlen(ip) == 0) {
                    printf("Error: Empty input is not allowed!\n");
                    continue;
                }

                if (!isValidIPv4(ip)) {
                    continue;
                }

                printf("\nIPv4 Address Analysis:\n");
                printf("----------------------------------------\n");
                printf("%-20s: %s\n", "IP Address", ip);

                int firstOctet;
                sscanf(ip, "%d", &firstOctet);
                char ipClass = getIPClass(firstOctet);
                printf("%-20s: %c\n", "Class", ipClass);

                getDefaultSubnetMask(ipClass, subnetMask);
                printf("%-20s: %s\n", "Default Subnet Mask", subnetMask);

                calculateNetworkAndHost(ip, networkAddr, hostAddr, ipClass);
                printf("%-20s: %s\n", "Network Address", networkAddr);
                printf("%-20s: %s\n", "Host Address", hostAddr);
                printf("%-20s: %s\n", "Subnet ID", networkAddr);

                getCIDRNotation(ipClass, cidr);
                printf("%-20s: %s%s\n", "CIDR Notation", ip, cidr);
                printf("%-20s: %s\n", "Loopback Address", isLoopback(ip) ? "Yes" : "No");
                printf("%-20s: %s\n", "Public/Private", isPrivate(ip) ? "Private" : "Public");
                break;

            case 2: // IPv6 Analysis
                printf("\nEnter an IPv6 address (e.g., 2001:0db8::1): ");
                if (fgets(ip, sizeof(ip), stdin) == NULL) {
                    printf("Error: Failed to read input!\n");
                    continue;
                }
                ip[strcspn(ip, "\n")] = '\0';
                if (strlen(ip) == 0) {
                    printf("Error: Empty input is not allowed!\n");
                    continue;
                }

                if (!isValidIPv6(ip)) {
                    continue;
                }

                printf("\nIPv6 Address Analysis:\n");
                printf("----------------------------------------\n");
                analyzeIPv6(ip);
                break;

            default:
                printf("Error: Invalid choice! Please select 1 or 2.\n");
                continue;
        }

        printf("\nDo you want to analyze another address? (y/n): ");
        continueChoice = getchar();
        while (getchar() != '\n');
    } while (tolower(continueChoice) == 'y');

    printf("Thank you for using IP Address Analyzer!\n");
    return 0;
}