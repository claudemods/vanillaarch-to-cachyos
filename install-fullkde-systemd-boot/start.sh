#!/bin/bash
nohup /usr/bin/plasmashell > /dev/null 2>&1 &
sleep 1
sudo rm -rf /home/$USER/vanillaarch-to-cachyos
