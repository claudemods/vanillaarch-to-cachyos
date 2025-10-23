#!/bin/bash

# Color codes
RED='\033[38;2;255;0;0m'
CYAN='\033[38;2;0;255;255m'
NC='\033[0m' # No Color

# Clear screen and display header
clear
echo -e "${RED}
 ░█████╗░██╗░░░░░░█████╗░██║░░░██╗██████╗░███████╗███╗░░░███╗░█████╗░██████╗░░██████╗
 ██╔══██╗██║░░░░░██╔══██╗██║░░░██║██╔══██╗██╔════╝████╗░████║██╔══██╗██╔══██╗██╔════╝
 ██║░░╚═╝██║░░░░░███████║██║░░░██║██║░░██║█████╗░░██╔████╔██║██║░░██║██║░░██║╚█████╗░
 ██║░░██╗██║░░░░░██╔══██║██║░░░██║██║░░██║██╔══╝░░██║╚██╔╝██║██║░░██║██║░░██║░╚═══██╗
 ╚█████╔╝███████╗██║░░██║╚██████╔╝██████╔╝███████╗██║░╚═╝░██║╚█████╔╝██████╔╝██████╔╝
 ░╚════╝░╚══════╝╚═╝░░╚═╝░╚═════╝░╚═════╝░╚══════╝╚═╝░░░░░╚═╝░╚════╝░╚═════╝░╚═════╝░
${NC}"

echo -e "${CYAN}claudemods Vanilla Arch to CachyOS Migration Script${NC}"
echo -e "${CYAN}===================================================${NC}"
echo ""

# Function to display menu
show_menu() {
    echo -e "${CYAN}Please select an installation option:${NC}"
    echo ""
    echo -e "${CYAN}GRUB Bootloader Options:${NC}"
    echo -e "  ${CYAN}1${NC}) TTY Only (No Desktop) - GRUB"
    echo -e "  ${CYAN}2${NC}) Full KDE Plasma - GRUB"
    echo -e "  ${CYAN}3${NC}) Full GNOME - GRUB"
    echo ""
    echo -e "${CYAN}systemd-boot Options:${NC}"
    echo -e "  ${CYAN}4${NC}) TTY Only (No Desktop) - systemd-boot"
    echo -e "  ${CYAN}5${NC}) Full KDE Plasma - systemd-boot"
    echo -e "  ${CYAN}6${NC}) Full GNOME - systemd-boot"
    echo ""
    echo -e "  ${CYAN}0${NC}) Exit"
    echo ""
}

# Function to execute selected option
execute_option() {
    local choice=$1
    
    case $choice in
        1)
            echo -e "${CYAN}Installing TTY Only with GRUB...${NC}"
            bash -c "$(curl -fsSL https://raw.githubusercontent.com/claudemods/vanillaarch-to-cachyos/refs/heads/main/install-fulltty-grub/install-from-github.sh)"
            ;;
        2)
            echo -e "${CYAN}Installing Full KDE Plasma with GRUB...${NC}"
            bash -c "$(curl -fsSL https://raw.githubusercontent.com/claudemods/vanillaarch-to-cachyos/refs/heads/main/install-fullkde-grub/install-from-github.sh)"
            ;;
        3)
            echo -e "${CYAN}Installing Full GNOME with GRUB...${NC}"
            bash -c "$(curl -fsSL https://raw.githubusercontent.com/claudemods/vanillaarch-to-cachyos/refs/heads/main/install-fullgnome-grub/install-from-github.sh)"
            ;;
        4)
            echo -e "${CYAN}Installing TTY Only with systemd-boot...${NC}"
            bash -c "$(curl -fsSL https://raw.githubusercontent.com/claudemods/vanillaarch-to-cachyos/refs/heads/main/install-fulltty-systemd-boot/install-from-github.sh)"
            ;;
        5)
            echo -e "${CYAN}Installing Full KDE Plasma with systemd-boot...${NC}"
            bash -c "$(curl -fsSL https://raw.githubusercontent.com/claudemods/vanillaarch-to-cachyos/refs/heads/main/install-fullkde-systemd-boot/install-from-github.sh)"
            ;;
        6)
            echo -e "${CYAN}Installing Full GNOME with systemd-boot...${NC}"
            bash -c "$(curl -fsSL https://raw.githubusercontent.com/claudemods/vanillaarch-to-cachyos/refs/heads/main/install-fullgnome-systemd-boot/install-from-github.sh)"
            ;;
        0)
            echo -e "${CYAN}Exiting. No changes made.${NC}"
            exit 0
            ;;
        *)
            echo -e "${CYAN}Invalid option! Please try again.${NC}"
            return 1
            ;;
    esac
}

# Main loop
while true; do
    show_menu
    echo -e -n "${CYAN}Enter your choice [0-6]: ${NC}"
    read choice
    
    # Validate input
    if [[ $choice =~ ^[0-6]$ ]]; then
        if [ $choice -eq 0 ]; then
            echo -e "${CYAN}Goodbye!${NC}"
            exit 0
        fi
        
        # Confirm before executing
        echo ""
        echo -e "${CYAN}WARNING: This will modify your system configuration.${NC}"
        echo -e "${CYAN}Make sure you have backups of important data!${NC}"
        echo -e -n "${CYAN}Proceed with installation? [y/N]: ${NC}"
        read confirm
        
        if [[ $confirm =~ ^[Yy]$ ]]; then
            echo ""
            execute_option $choice
            break
        else
            echo -e "${CYAN}Installation cancelled.${NC}"
            echo ""
        fi
    else
        echo -e "${CYAN}Invalid input! Please enter a number between 0 and 6.${NC}"
        echo ""
        sleep 2
    fi
done
