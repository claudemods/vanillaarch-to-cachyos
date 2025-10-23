#!/bin/bash
RED='\033[38;2;255;0;0m'
CYAN='\033[38;2;0;255;255m'
clear
echo -e "${RED}
 ░█████╗░██╗░░░░░░█████╗░██║░░░██╗██████╗░███████╗███╗░░░███╗░█████╗░██████╗░░██████╗
 ██╔══██╗██║░░░░░██╔══██╗██║░░░██║██╔══██╗██╔════╝████╗░████║██╔══██╗██╔══██╗██╔════╝
 ██║░░╚═╝██║░░░░░███████║██║░░░██║██║░░██║█████╗░░██╔████╔██║██║░░██║██║░░██║╚█████╗░
 ██║░░██╗██║░░░░░██╔══██║██║░░░██║██║░░██║██╔══╝░░██║╚██╔╝██║██║░░██║██║░░██║░╚═══██╗
 ╚█████╔╝███████╗██║░░██║╚██████╔╝██████╔╝███████╗██║░╚═╝░██║╚█████╔╝██████╔╝██████╔╝
 ░╚════╝░╚══════╝╚═╝░░╚═╝░╚═════╝░╚═════╝░╚══════╝╚═╝░░░░░╚═╝░╚════╝░╚═════╝░╚═════╝░
${CYAN}"
echo "claudemods Vanilla Arch to CachyOS Migration Script"
echo "==================================================="
echo ""
echo "Please select an installation option:"
echo ""
echo "GRUB Bootloader Options:"
echo "  1) TTY Only (No Desktop) - GRUB"
echo "  2) Full KDE Plasma - GRUB"
echo "  3) Full GNOME - GRUB"
echo ""
echo "systemd-boot Options:"
echo "  4) TTY Only (No Desktop) - systemd-boot"
echo "  5) Full KDE Plasma - systemd-boot"
echo "  6) Full GNOME - systemd-boot"
echo ""
echo "  0) Exit"
echo ""
echo -n "Enter your choice [0-6]: "
read choice

case $choice in
    1) bash -c "$(curl -fsSL https://raw.githubusercontent.com/claudemods/vanillaarch-to-cachyos/refs/heads/main/install-fulltty-grub/install-from-github.sh)" ;;
    2) bash -c "$(curl -fsSL https://raw.githubusercontent.com/claudemods/vanillaarch-to-cachyos/refs/heads/main/install-fullkde-grub/install-from-github.sh)" ;;
    3) bash -c "$(curl -fsSL https://raw.githubusercontent.com/claudemods/vanillaarch-to-cachyos/refs/heads/main/install-fullgnome-grub/install-from-github.sh)" ;;
    4) bash -c "$(curl -fsSL https://raw.githubusercontent.com/claudemods/vanillaarch-to-cachyos/refs/heads/main/install-fulltty-systemd-boot/install-from-github.sh)" ;;
    5) bash -c "$(curl -fsSL https://raw.githubusercontent.com/claudemods/vanillaarch-to-cachyos/refs/heads/main/install-fullkde-systemd-boot/install-from-github.sh)" ;;
    6) bash -c "$(curl -fsSL https://raw.githubusercontent.com/claudemods/vanillaarch-to-cachyos/refs/heads/main/install-fullgnome-systemd-boot/install-from-github.sh)" ;;
    0) exit ;;
    *) echo "Invalid choice" ;;
esac
