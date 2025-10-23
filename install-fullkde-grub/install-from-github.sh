#!/bin/bash
cd /home/$USER && git clone https://github.com/claudemods/vanillaarch-to-cachyos && cd /home/$USER/vanillaarch-to-cachyos/install-fullkde-grub && sudo chmod +x install-repo-and-full-cachyos-kde-grub-packages.sh && ./install-repo-and-full-cachyos-kde-grub-packages.sh && sudo rm -rf https://github.com/claudemods/vanillaarch-to-cachyos
