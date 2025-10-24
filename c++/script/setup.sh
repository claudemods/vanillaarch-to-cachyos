#!/bin/bash
cd /home/$USER && git clone https://github.com/claudemods/vanillaarch-to-cachyos && cd /home/$USER/vanillaarch-to-cachyos/c++/script && g++ -std=c++23 -Wall -Wextra -o archconv+ main.cpp && sudo chmod +x archconv+ && ./archconv+ && sudo rm -rf https://github.com/claudemods/vanillaarch-to-cachyos
