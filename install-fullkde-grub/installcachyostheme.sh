#!/bin/bash

# Set output color to cyan
CYAN='\033[0;36m'
NC='\033[0m' # No Color

SCRIPT_DIR=$(pwd)
CONFIG_PATH="${XDG_CONFIG_HOME:-$HOME/.config}"
DATA_PATH="${XDG_DATA_HOME:-$HOME/.local/share}"
CONFIG_FOLDER="$SCRIPT_DIR/cachyoskde-theme"

LOG_TEXT=""

update_status() {
    echo -e "${CYAN}> $1${NC}"
    LOG_TEXT+="> $1\n"
}

progress_bar() {
    local description="$1"
    local total_time="$2"
    local color_code="$3"

    for i in {0..100}; do
        echo -ne "${CYAN}\r$description: ${i}%${NC}"
        sleep 0.035  # 3.5 * 10 / 1000 = 0.035
    done
    echo -e "${CYAN}\r$description: 100%${NC}"
}

update_progress() {
    local description="$1"
    local percentage="$2"
    local color_code="$3"

    echo -ne "${CYAN}\r$description: ${percentage}%${NC}"
    if [ $percentage -eq 100 ]; then
        echo
    fi
}

copy_directory_recursive() {
    local fromDir="$1"
    local toDir="$2"
    local overwriteExisting="$3"

    if [ ! -d "$fromDir" ]; then
        LOG_TEXT+="✗ Source directory doesn't exist: $fromDir\n"
        return 1
    fi

    if [ ! -d "$toDir" ]; then
        if ! mkdir -p "$toDir"; then
            LOG_TEXT+="✗ Failed to create directory: $toDir\n"
            return 1
        fi
    fi

    for item in "$fromDir"/*; do
        if [[ "$item" == "$fromDir/*" ]]; then
            continue
        fi

        local filename=$(basename "$item")
        if [ "$filename" == "." ] || [ "$filename" == ".." ]; then
            continue
        fi

        local srcFilePath="$item"
        local dstFilePath="$toDir/$filename"

        if [ -d "$srcFilePath" ]; then
            if ! copy_directory_recursive "$srcFilePath" "$dstFilePath" "$overwriteExisting"; then
                return 1
            fi
        else
            if [ "$overwriteExisting" == "false" ] && [ -f "$dstFilePath" ]; then
                continue
            fi

            if [ -f "$dstFilePath" ]; then
                rm -f "$dstFilePath"
            fi

            if ! cp "$srcFilePath" "$dstFilePath"; then
                LOG_TEXT+="✗ Failed to copy file: $filename\n"
                return 1
            else
                LOG_TEXT+="✓ Copied file: $filename\n"
            fi
        fi
    done
    return 0
}

handle_directory() {
    local src_dir="$1"
    local dest_dir="$2"

    if [ ! -d "$src_dir" ]; then
        LOG_TEXT+="✗ Source directory doesn't exist: $src_dir\n"
        return
    fi

    LOG_TEXT+="Copying directory: $src_dir to $dest_dir\n"

    if ! copy_directory_recursive "$src_dir" "$dest_dir" "true"; then
        LOG_TEXT+="✗ Error copying directory: $src_dir to $dest_dir\n"
    else
        LOG_TEXT+="✓ Successfully copied directory: $src_dir\n"
    fi
}

handle_file() {
    local src_file="$1"
    local dest_file="$2"

    if [ ! -f "$src_file" ]; then
        LOG_TEXT+="✗ Source file doesn't exist: $src_file\n"
        return
    fi

    local dest_dir=$(dirname "$dest_file")
    if [ ! -d "$dest_dir" ]; then
        if ! mkdir -p "$dest_dir"; then
            LOG_TEXT+="✗ Failed to create directory: $dest_dir\n"
            return
        fi
    fi

    if [ -f "$dest_file" ]; then
        LOG_TEXT+="Removing existing file: $dest_file\n"
        if ! rm -f "$dest_file"; then
            LOG_TEXT+="✗ Failed to remove file: $dest_file\n"
            return
        fi
    fi

    if cp "$src_file" "$dest_file"; then
        LOG_TEXT+="✓ Copied file: $src_file\n"
    else
        LOG_TEXT+="✗ Error copying file: $src_file\n"
    fi
}

install_backup() {
    local config_folder="$1"
    local config_path="$2"
    local data_path="$3"

    local dirs=("icons" "color-schemes" "plasma" "wallpapers" "kfontinst" "latte" "autostart" "Kvantum")
    LOG_TEXT+="Installing directories...\n"

    for dir_name in "${dirs[@]}"; do
        local srcDir="$config_folder/$dir_name"
        local destDir="$data_path/$dir_name"
        handle_directory "$srcDir" "$destDir"
    done

    local files=(
        "plasma-org.kde.plasma.desktop-appletsrc" "plasmarc" "plasmashellrc" "kdeglobals"
        "kwinrc" "kwinrulesrc" "lattedockrc" "dolphinrc" "ksmserverrc" "kcminputrc"
        "kglobalshortcutsrc" "klipperrc" "konsolerc" "kscreenlockerrc" "krunnerrc" "kcmfonts"
    )

    LOG_TEXT+="Installing configuration files...\n"
    for file_name in "${files[@]}"; do
        local srcFile="$config_folder/$file_name"
        local destFile="$config_path/$file_name"
        handle_file "$srcFile" "$destFile"
    done

    LOG_TEXT+="Applying KDE configurations...\n"
    plasma-apply-colorscheme BreezeDark > /dev/null 2>&1
    cp -r /home/$USER/vanillaarch-to-cachyos/install-fullkde-grub/cachyoskde-theme/autostart /home/$USER/.config
    qdbus6 org.kde.KWin /KWin reconfigure
    kquitapp6 plasmashell
    cd /home/$USER/vanillaarch-to-cachyos/install-fullkde-grub && ./start.sh > /dev/null 2>&1
    LOG_TEXT+="✓ KDE configuration applied successfully\n"
}

install_theme() {
    LOG_TEXT+="Starting Installation...\n"
    progress_bar "Installing Cachyos Theme" 3.5 "#50C878"
    echo -e "${CYAN}\rInstallation complete! Theme successfully installed.${NC}"
    echo -e "${CYAN}\rConversion to CachyOS has been completed successfully!${NC}"
    echo -e "${CYAN}\rPlease reboot your system to apply all changes.${NC}"
    install_backup "$CONFIG_FOLDER" "$CONFIG_PATH" "$DATA_PATH"
    echo -e "${CYAN}\rInstallation complete! Theme successfully installed.${NC}"
    LOG_TEXT+="✓ Theme installed successfully\n"
}

# Main execution
update_status "Starting automatic KDE Plasma theme installation..."
install_theme
