#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <termios.h>
#include <unistd.h>
#include "grubtty.h"
#include "kdegrub.h"
#include "gnomegrub.h"
#include "ttysystemd-boot.h"
#include "kdesystemd-boot.h"
#include "gnomesystemd-boot.h"

class MigrationScript {
private:
    const std::string RED = "\033[38;2;255;0;0m";
    const std::string CYAN = "\033[38;2;0;255;255m";
    const std::string RESET = "\033[0m";

    struct MenuOption {
        int choice;
        std::string description;
        std::string url;
        bool useInternal;
    };

    std::vector<MenuOption> options = {
        {1, "TTY Only (No Desktop) - GRUB", "", true},
        {2, "Full KDE Plasma - GRUB", "", true},
        {3, "Full GNOME - GRUB", "", true},
        {4, "TTY Only (No Desktop) - systemd-boot", "", true},
        {5, "Full KDE Plasma - systemd-boot", "", true},
        {6, "Full GNOME - systemd-boot", "", true}  // Now uses internal implementation
    };

    int currentSelection = 1;
    bool exitRequested = false;

public:
    void enableRawMode() {
        termios term;
        tcgetattr(STDIN_FILENO, &term);
        term.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &term);
    }

    void disableRawMode() {
        termios term;
        tcgetattr(STDIN_FILENO, &term);
        term.c_lflag |= (ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &term);
    }

    char getKey() {
        char c;
        read(STDIN_FILENO, &c, 1);
        if (c == '\033') {
            read(STDIN_FILENO, &c, 1);
            read(STDIN_FILENO, &c, 1);
            return c;
        }
        return c;
    }

    void clearScreen() {
        std::system("clear");
    }

    void displayBanner() {
        std::cout << RED;
        std::cout << " ░█████╗░██╗░░░░░░█████╗░██╗░░░██╗██████╗░███████╗███╗░░░███╗░█████╗░██████╗░░██████╗" << std::endl;
        std::cout << " ██╔══██╗██║░░░░░██╔══██╗██║░░░██║██╔══██╗██╔════╝████╗░████║██╔══██╗██╔══██╗██╔════╝" << std::endl;
        std::cout << " ██║░░╚═╝██║░░░░░███████║██║░░░██║██║░░██║█████╗░░██╔████╔██║██║░░██║██║░░██║╚█████╗░" << std::endl;
        std::cout << " ██║░░██╗██║░░░░░██╔══██║██║░░░██║██║░░██║██╔══╝░░██║╚██╔╝██║██║░░██║██║░░██║░╚═══██╗" << std::endl;
        std::cout << " ╚█████╔╝███████╗██║░░██║╚██████╔╝██████╔╝███████╗██║░╚═╝░██║╚█████╔╝██████╔╝██████╔╝" << std::endl;
        std::cout << " ░╚════╝░╚══════╝╚═╝░░░░░░╚═════╝░╚══════╝░╚══════╝╚═╝░░░░░╚═╝░╚════╝░╚═════╝░╚═════╝░" << std::endl;
        std::cout << RESET;

        std::cout << CYAN;
        std::cout << "claudemods Vanilla Arch to CachyOS Migration Script" << std::endl;
        std::cout << "===================================================" << std::endl;
        std::cout << std::endl;
        std::cout << "Please select an installation option:" << std::endl;
        std::cout << std::endl;
        std::cout << "GRUB Bootloader Options:" << std::endl;
        std::cout << RESET;
    }

    void displayMenu() {
        for (int i = 1; i <= 3; i++) {
            if (currentSelection == i) {
                std::cout << CYAN << "  > " << i << ") " << options[i-1].description << RESET << std::endl;
            } else {
                std::cout << CYAN << "    " << i << ") " << options[i-1].description << RESET << std::endl;
            }
        }

        std::cout << CYAN << std::endl;
        std::cout << "systemd-boot Options:" << std::endl;
        std::cout << RESET;

        for (int i = 4; i <= 6; i++) {
            if (currentSelection == i) {
                std::cout << CYAN << "  > " << i << ") " << options[i-1].description << RESET << std::endl;
            } else {
                std::cout << CYAN << "    " << i << ") " << options[i-1].description << RESET << std::endl;
            }
        }

        std::cout << CYAN << std::endl;
        std::cout << "   0) Exit" << std::endl;
        std::cout << std::endl;
        std::cout << "Use arrow keys to navigate, Enter to select" << RESET << std::endl;
    }

    void handleNavigation() {
        char key = getKey();

        switch(key) {
            case 'A':
                currentSelection--;
                if (currentSelection < 1) currentSelection = 6;
                break;
            case 'B':
                currentSelection++;
                if (currentSelection > 6) currentSelection = 1;
                break;
            case '\n':
                if (currentSelection == 0) {
                    exitRequested = true;
                } else {
                    executeScript(currentSelection);
                    exitRequested = true;
                }
                break;
            case '0':
                exitRequested = true;
                break;
            case '1' ... '6':
                executeScript(key - '0');
                exitRequested = true;
                break;
        }
    }

    void executeScript(int choice) {
        if (choice == 0) {
            std::cout << CYAN << "Exiting..." << RESET << std::endl;
            return;
        }

        for (const auto& option : options) {
            if (option.choice == choice) {
                std::cout << CYAN << "Executing: " << option.description << RESET << std::endl;

                if (option.useInternal) {
                    if (choice == 1) {
                        GrubTTYMigration migration;
                        migration.runMigration();
                    } else if (choice == 2) {
                        KdeGrubMigration migration;
                        migration.runMigration();
                    } else if (choice == 3) {
                        GnomeGrubMigration migration;
                        migration.runMigration();
                    } else if (choice == 4) {
                        TTYSystemdBootMigration migration;
                        migration.runMigration();
                    } else if (choice == 5) {
                        KdeSystemdBootMigration migration;
                        migration.runMigration();
                    } else if (choice == 6) {
                        GnomeSystemdBootMigration migration;
                        migration.runMigration();
                    }
                } else {
                    std::string command = "bash -c \"$(curl -fsSL " + option.url + ")\"";
                    std::system(command.c_str());
                }
                return;
            }
        }
    }

    void run() {
        enableRawMode();

        while (!exitRequested) {
            clearScreen();
            displayBanner();
            displayMenu();
            handleNavigation();
        }

        disableRawMode();
        std::cout << CYAN << "Goodbye!" << RESET << std::endl;
    }
};

int main() {
    MigrationScript script;
    script.run();
    return 0;
}
