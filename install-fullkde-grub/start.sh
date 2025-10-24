#!/bin/bash
nohup /usr/bin/plasmashell
print_section "CachyOS Conversion Complete!"
echo -e "${GREEN}${BOLD}"
echo "Conversion to CachyOS has been completed successfully!"
sudo rm -rf /home/$USER/vanillaarch-to-cachyos
echo "Please reboot your system to apply all changes."
echo -e "${NC}"
