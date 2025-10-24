#ifndef GNOMEGRUB_H
#define GNOMEGRUB_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>

class GnomeGrubMigration {
private:
    const std::string RED = "\033[0;31m";
    const std::string GREEN = "\033[0;32m";
    const std::string CYAN = "\033[0;36m";
    const std::string YELLOW = "\033[1;33m";
    const std::string BLUE = "\033[0;34m";
    const std::string NC = "\033[0m";
    const std::string BOLD = "\033[1m";

public:
    void printStatus(const std::string& message) {
        std::cout << GREEN << "[✓] " << NC << message << std::endl;
    }

    void printWarning(const std::string& message) {
        std::cout << YELLOW << "[!] " << NC << message << std::endl;
    }

    void printError(const std::string& message) {
        std::cout << RED << "[✗] " << NC << message << std::endl;
    }

    void printInfo(const std::string& message) {
        std::cout << CYAN << "[i] " << NC << message << std::endl;
    }

    void printSection(const std::string& message) {
        std::cout << BLUE << BOLD << "[=== " << message << " ===]" << NC << std::endl;
    }

    void displayBanner() {
        std::cout << RED;
        std::cout << " ░█████╗░██╗░░░░░░█████╗░██║░░░██╗██████╗░███████╗███╗░░░███╗░█████╗░██████╗░░██████╗" << std::endl;
        std::cout << " ██╔══██╗██║░░░░░██╔══██╗██║░░░██║██╔══██╗██╔════╝████╗░████║██╔══██╗██╔══██╗██╔════╝" << std::endl;
        std::cout << " ██║░░╚═╝██║░░░░░███████║██║░░░██║██║░░██║█████╗░░██╔████╔██║██║░░██║██║░░██║╚█████╗░" << std::endl;
        std::cout << " ██║░░██╗██║░░░░░██╔══██║██║░░░██║██║░░██║██╔══╝░░██║╚██╔╝██║██║░░██║██║░░██║░╚═══██╗" << std::endl;
        std::cout << " ╚█████╔╝███████╗██║░░██║╚██████╔╝██████╔╝███████╗██║░╚═╝░██║╚█████╔╝██████╔╝██████╔╝" << std::endl;
        std::cout << " ░╚════╝░╚══════╝╚═╝░░░░░░╚═════╝░╚══════╝░╚══════╝╚═╝░░░░░╚═╝░╚════╝░╚═════╝░╚═════╝░" << std::endl;
        std::cout << NC;

        std::cout << CYAN;
        std::cout << "           claudemods Vanilla Arch Gnome Grub to CachyOS Gnome Grub v1.0 24-10-2025" << std::endl;
        std::cout << NC;
        std::cout << "================================================================================" << std::endl;
        std::cout << std::endl;
    }

    bool checkVirtManager() {
        printSection("Checking for virt-manager");
        if (std::system("pacman -Qi virt-manager &>/dev/null") == 0) {
            printInfo("virt-manager found, removing it...");
            if (std::system("sudo pacman -Rns --noconfirm virt-manager") != 0) {
                printError("Failed to remove virt-manager");
                return false;
            }
            printStatus("virt-manager removed successfully");
        } else {
            printInfo("virt-manager not installed, proceeding...");
        }
        return true;
    }

    bool setupCachyosRepos() {
        printSection("Step 1: Setting up CachyOS Repositories");

        printInfo("Downloading CachyOS repository package...");
        if (std::system("curl https://mirror.cachyos.org/cachyos-repo.tar.xz -o cachyos-repo.tar.xz") != 0) {
            printError("Failed to download CachyOS repositories");
            return false;
        }
        printStatus("Download completed");

        printInfo("Extracting repository files...");
        if (std::system("tar xvf cachyos-repo.tar.xz && cd cachyos-repo") != 0) {
            printError("Failed to extract repository files");
            return false;
        }
        printStatus("Extraction completed");

        printInfo("Running CachyOS repository setup...");
        if (std::system("sudo ./cachyos-repo.sh") != 0) {
            printError("Failed to setup CachyOS repositories");
            return false;
        }
        printStatus("Repository setup completed");

        return true;
    }

    bool installPackages() {
        printSection("Step 2: Installing Packages");
        printInfo("Starting installation (this will take a while)...");
        printInfo("Please be patient as this process may take 30-60 minutes...");

        std::string packageCommand = "sudo pacman -S --needed a52dec aalib abseil-cpp accountsservice acl adobe-source-han-sans-cn-fonts adobe-source-han-sans-jp-fonts adobe-source-han-sans-kr-fonts adwaita-cursors adwaita-fonts adwaita-icon-theme adwaita-icon-theme-legacy alacritty alsa-card-profiles alsa-firmware alsa-lib alsa-plugins alsa-topology-conf alsa-ucm-conf alsa-utils amd-ucode ananicy-cpp aom apache appstream apr apr-util archlinux-keyring aribb24 aribb25 at-spi2-core atkmm attr audit autoconf automake avahi awesome-terminal-fonts baobab base base-devel bash bash-completion bat bind binutils bison bluez bluez-hid2hci bluez-libs bluez-obex bluez-utils bolt bpf brltty brotli btop btrfs-progs bubblewrap bzip2 ca-certificates ca-certificates-mozilla ca-certificates-utils cachyos-ananicy-rules cachyos-fish-config cachyos-grub-theme cachyos-hello cachyos-hooks cachyos-kernel-manager cachyos-keyring cachyos-micro-settings cachyos-mirrorlist cachyos-packageinstaller cachyos-plymouth-bootanimation cachyos-rate-mirrors cachyos-settings cachyos-v3-mirrorlist cachyos-v4-mirrorlist cachyos-wallpapers cachyos-zsh-config cairo cairomm cairomm-1.16 cantarell-fonts cdparanoia chromaprint chwd cifs-utils clang colord colord-gtk-common colord-gtk4 colord-sane compiler-rt composefs confuse coreutils cpupower cracklib cryptsetup cups-pk-helper curl dav1d db5.3 dbus dbus-broker dbus-broker-units dbus-units dconf debugedit decibels default-cursors desktop-file-utils device-mapper dhclient diffutils ding-libs djvulibre dmidecode dmraid dnsmasq dnssec-anchors dosfstools dotconf double-conversion duf duktape e2fsprogs editorconfig-core-c efibootmgr efitools efivar egl-wayland eglexternalplatform ell enchant eog epiphany ethtool evince evolution-data-server exempi exfatprogs exiv2 expac expat eza f2fs-tools faac faad2 fakeroot fastfetch fd ffmpeg ffmpeg4.4 ffmpegthumbnailer fftw file file-roller filesystem findutils firefox fish fish-autopair fish-pure-prompt fisher flac flashrom flatpak flex fluidsynth fmt folks fontconfig freeglut freerdp freetype2 fribidi fsarchiver ftgl fuse-common fuse3 fwupd fwupd-efi fzf gawk gc gcc gcc-libs gcr gcr-4 gd gdbm gdk-pixbuf2 gdm gedit geoclue geocode-glib-2 geocode-glib-common gettext gexiv2 ghostscript giflib git gjs glances glew glib-networking glib2 glibc glibmm glibmm-2.68 glm glslang glu glycin gmp gnome-app-list gnome-autoar gnome-backgrounds gnome-bluetooth-3.0 gnome-calculator gnome-calendar gnome-characters gnome-clocks gnome-color-manager gnome-connections gnome-console gnome-contacts gnome-control-center gnome-desktop gnome-desktop-4 gnome-desktop-common gnome-disk-utility gnome-font-viewer gnome-keybindings gnome-keyring gnome-logs gnome-maps gnome-menus gnome-music gnome-nettool gnome-online-accounts gnome-power-manager gnome-remote-desktop gnome-screenshot gnome-session gnome-settings-daemon gnome-shell gnome-software gnome-system-monitor gnome-terminal gnome-text-editor gnome-themes-extra gnome-tour gnome-tweaks gnome-usage gnome-user-docs gnome-user-share gnome-weather gnulib-l10n gnupg gnutls gobject-introspection-runtime gom gperftools gpgme gpgmepp gpm graphene graphite grep grilo grilo-plugins groff grub grub-hook gsettings-desktop-schemas gsettings-system-schemas gsfonts gsm gsound gspell gssdp gssproxy gst-devtools-libs gst-editing-services gst-libav gst-plugin-gtk gst-plugin-gtk4 gst-plugin-pipewire gst-plugins-bad gst-plugins-bad-libs gst-plugins-base gst-plugins-base-libs gst-plugins-good gst-plugins-ugly gst-python gstreamer gtest gtk-update-icon-cache gtk-vnc gtk3 gtk4 gtkmm-4.0 gtkmm3 gtksourceview4 gtksourceview5 guile gupnp gupnp-av gupnp-dlna gupnp-igd gvfs gvfs-afc gvfs-dnssd gvfs-goa gvfs-google gvfs-gphoto2 gvfs-mtp gvfs-nfs gvfs-onedrive gvfs-smb gvfs-wsdd gzip harfbuzz harfbuzz-icu haveged hdparm hicolor-icon-theme hidapi highway hwdata hwdetect hwinfo hyphen iana-etc ibus icu iio-sensor-proxy ijs imagemagick imath imlib2 inetutils inxi iproute2 iptables-nft iputils iso-codes iw iwd jack2 jansson jbig2dec jbigkit jemalloc jfsutils jq js140 json-c json-glib kbd kcolorscheme kconfig kcoreaddons kcrash kdbusaddons keyutils kguiaddons ki18n kmod knotifications krb5 kwallet kwidgetsaddons kwindowsystem l-smash lame lcms2 ldb leancrypto less libadwaita libaio libao libarchive libass libassuan libasyncns libatasmart libavc1394 libavif libavtp libb2 libblockdev libblockdev-crypto libblockdev-fs libblockdev-loop libblockdev-mdraid libblockdev-nvme libblockdev-part libblockdev-swap libbluray libbpf libbs2b libbsd libbytesize libcaca libcacard libcanberra libcap libcap-ng libcbor libcddb libcdio libcdio-paranoia libcloudproviders libcolord libcue libcups libdaemon libdatrie libdc1394 libdca libde265 libdecor libdeflate libdisplay-info libdmapsharing libdnet libdovi libdrm libdv libdvbpsi libdvdcss libdvdnav libdvdread libebml libebur128 libedit libei libelf libepoxy libevdev libevent libexif libfdk-aac libffi libfontenc libfreeaptx libftdi libfyaml libgcrypt libgdata libgdm libgedit-amtk libgedit-gfls libgedit-gtksourceview libgedit-tepl libgee libgexiv2 libgirepository libgit2 libglvnd libgme libgoa libgoom2 libgpg-error libgphoto2 libgsf libgtop libgudev libgusb libgweather-4 libgxps libhandy libheif libibus libical libice libidn libidn2 libiec61883 libieee1284 libimobiledevice libimobiledevice-glue libinih libinput libiptcdata libisl libjcat libjpeg-turbo libjxl libkate libksba liblc3 libldac libldap liblouis liblqr liblrdf libltc libmad libmalcontent libmanette libmatroska libmaxminddb libmbim libmd libmediaart libmfx libmicrodns libmm-glib libmnl libmodplug libmpc libmpcdec libmpeg2 libmspack libmtp libmysofa libnautilus-extension libndp libnetfilter_conntrack libnewt libnfnetlink libnfs libnftnl libnghttp2 libnghttp3 libnice libnl libnm libnma libnma-common libnma-gtk4 libnotify libnsl libnvme liboauth libogg liboggz libopenmpt libopenraw libosinfo libp11-kit libpaper libpcap libpciaccess libpeas libpgm libphonenumber libpipeline libpipewire libplacebo libplist libpng libportal libportal-gtk3 libportal-gtk4 libproxy libpsl libpulse libpwquality libqmi libqrtr-glib libraqm libraw1394 librest librsvg libsamplerate libsasl libseccomp libsecret libshout libshumate libsigc++ libsigc++-3.0 libsm libsndfile libsodium libsoup libsoup3 libsoxr libspectre libspeechd libspelling libsrtp libssh libssh2 libstemmer libsynctex libsysprof-capture libtasn1 libtatsu libteam libthai libtheora libtiff libtiger libtirpc libtool libunibreak libunistring libunwind libupnp liburcu liburing libusb libusb-compat libusbmuxd libuv libva libvdpau libverto libvlc libvncserver libvorbis libvpl libvpx libwacom libwbclient libwebp libwireplumber libwnck3 libx11 libx86emu libxau libxcb libxcomposite libxcrypt libxcursor libxcvt libxdamage libxdmcp libxext libxfixes libxfont2 libxft libxi libxinerama libxkbcommon libxkbcommon-x11 libxkbfile libxml2 libxmlb libxmu libxpm libxrandr libxrender libxres libxshmfence libxslt libxss libxt libxtst libxv libxxf86vm libyaml libyuv licenses lilv linux-api-headers linux-cachyos linux-cachyos-headers linux-cachyos-lts linux-cachyos-lts-headers linux-firmware linux-firmware-amdgpu linux-firmware-atheros linux-firmware-broadcom linux-firmware-cirrus linux-firmware-intel linux-firmware-mediatek linux-firmware-nvidia linux-firmware-other linux-firmware-radeon linux-firmware-realtek linux-firmware-whence lirc live-media lld llhttp llvm llvm-libs lm_sensors lmdb localsearch logrotate loupe lsb-release lsscsi lua lv2 lvm2 lz4 lzo m4 mailcap make malcontent man-db man-pages md4c mdadm meld mesa mesa-utils micro mjpegtools mkinitcpio mkinitcpio-busybox mobile-broadband-provider-info mod_dnssd modemmanager mpdecimal mpfr mpg123 msgraph mtdev mtools mutter nano nano-syntax-highlighting nautilus ncurses neon net-snmp netctl nettle networkmanager networkmanager-openvpn networkmanager-vpn-plugin-openvpn nfs-utils nfsidmap nftables nilfs-utils noto-color-emoji-fontconfig noto-fonts noto-fonts-cjk noto-fonts-emoji npth nspr nss nss-mdns ntp numactl ocl-icd octopi oh-my-zsh-git oniguruma open-vm-tools openal opencore-amr opendesktop-fonts openexr openh264 openjpeg2 openssh openssl openvpn opus orc orca os-prober osinfo-db ostree p11-kit pacman pacman-contrib pacman-mirrorlist pacutils pahole pam pambase pango pangomm pangomm-2.48 papers parallel parted paru passim patch pavucontrol pciutils pcre pcre2 pcsclite perl perl-clone perl-encode-locale perl-error perl-file-listing perl-html-parser perl-html-tagset perl-http-cookiejar perl-http-cookies perl-http-daemon perl-http-date perl-http-message perl-http-negotiate perl-io-html perl-libwww perl-lwp-mediatypes perl-mailtools perl-net-http perl-timedate perl-try-tiny perl-uri perl-www-robotrules perl-xml-parser perl-xml-writer phodav pinentry pipewire pipewire-alsa pipewire-audio pipewire-pulse pixman pkcs11-helper pkgconf pkgfile plocate plymouth polkit poppler poppler-data poppler-glib popt portaudio power-profiles-daemon powerline-fonts ppp procps-ng projectm protobuf protobuf-c psmisc pv python python-annotated-types python-argcomplete python-cairo python-dasbus python-dbus python-defusedxml python-gobject python-orjson python-packaging python-psutil python-pydantic python-pydantic-core python-pyxdg python-setproctitle python-typing-inspection python-typing_extensions python-wxpython qca-qt6 qemu-guest-agent qrencode qt-sudo qt6-5compat qt6-base qt6-declarative qt6-multimedia qt6-multimedia-ffmpeg qt6-shadertools qt6-svg qt6-translations qt6-wayland qtermwidget raptor rate-mirrors rav1e readline rebuild-detector reflector ripgrep rpcbind rsync rtkit rtmpdump rubberband run-parts rygel s-nail sane sbc scx-manager scx-scheds sd sdl12-compat sdl2-compat sdl2_ttf sdl3 sdl_image sed serd sg3_utils shaderc shadow shared-mime-info showtime simple-scan slang smartmontools smbclient snappy snapshot sof-firmware sord sound-theme-freedesktop soundtouch spandsp spdlog speech-dispatcher speex speexdsp spice-gtk spice-protocol spice-vdagent spirv-tools sqlite sratom srt startup-notification sudo sushi svt-av1 svt-hevc sysfsutils systemd systemd-libs systemd-resolvconf systemd-sysvcompat taglib talloc tar tcl tdb tealdeer tecla tevent texinfo thin-provisioning-tools tinysparql totem totem-pl-parser tpm2-tss tslib ttf-bitstream-vera ttf-dejavu ttf-fantasque-nerd ttf-liberation ttf-meslo-nerd ttf-opensans twolame tzdata uchardet udisks2 ufw unrar unzip upower uriparser usb_modeswitch usbmuxd usbredir usbutils util-linux util-linux-libs v4l-utils vapoursynth vi vid.stab vim vim-runtime virtualbox-guest-utils vlc-plugin-a52dec vlc-plugin-aalib vlc-plugin-alsa vlc-plugin-aom vlc-plugin-archive vlc-plugin-aribb24 vlc-plugin-aribb25 vlc-plugin-ass vlc-plugin-avahi vlc-plugin-bluray vlc-plugin-caca vlc-plugin-cddb vlc-plugin-chromecast vlc-plugin-dav1d vlc-plugin-dbus vlc-plugin-dbus-screensaver vlc-plugin-dca vlc-plugin-dvb vlc-plugin-dvd vlc-plugin-faad2 vlc-plugin-ffmpeg vlc-plugin-firewire vlc-plugin-flac vlc-plugin-fluidsynth vlc-plugin-freetype vlc-plugin-gme vlc-plugin-gnutls vlc-plugin-gstreamer vlc-plugin-inflate vlc-plugin-jack vlc-plugin-journal vlc-plugin-jpeg vlc-plugin-kate vlc-plugin-kwallet vlc-plugin-libsecret vlc-plugin-lirc vlc-plugin-live555 vlc-plugin-lua vlc-plugin-mad vlc-plugin-matroska vlc-plugin-mdns vlc-plugin-modplug vlc-plugin-mpeg2 vlc-plugin-mpg123 vlc-plugin-mtp vlc-plugin-musepack vlc-plugin-nfs vlc-plugin-notify vlc-plugin-ogg vlc-plugin-opus vlc-plugin-png vlc-plugin-pulse vlc-plugin-quicksync vlc-plugin-samplerate vlc-plugin-sdl vlc-plugin-sftp vlc-plugin-shout vlc-plugin-smb vlc-plugin-soxr vlc-plugin-speex vlc-plugin-srt vlc-plugin-svg vlc-plugin-tag vlc-plugin-theora vlc-plugin-twolame vlc-plugin-udev vlc-plugin-upnp vlc-plugin-vorbis vlc-plugin-vpx vlc-plugin-x264 vlc-plugin-x265 vlc-plugin-xml vlc-plugin-zvbi vlc-plugins-all vlc-plugins-base vlc-plugins-extra vlc-plugins-video-output vlc-plugins-visualization vmaf volume_key vte-common vte3 vte4 vulkan-icd-loader vulkan-mesa-device-select vulkan-virtio wavpack wayland webkit2gtk-4.1 webkitgtk-6.0 webp-pixbuf-loader webrtc-audio-processing-1 wget which whois wildmidi wireless-regdb wireplumber woff2 wpa_supplicant wsdd wxwidgets-common wxwidgets-gtk3 x264 x265 xcb-proto xcb-util xcb-util-cursor xcb-util-image xcb-util-keysyms xcb-util-renderutil xcb-util-wm xdg-dbus-proxy xdg-desktop-portal xdg-desktop-portal-gnome xdg-desktop-portal-gtk xdg-user-dirs xdg-user-dirs-gtk xdg-utils xf86-input-libinput xf86-input-vmmouse xfsprogs xkeyboard-config xl2tpd xmlsec xorg-fonts-encodings xorg-server xorg-server-common xorg-setxkbmap xorg-xauth xorg-xdpyinfo xorg-xinit xorg-xinput xorg-xkbcomp xorg-xkill xorg-xmodmap xorg-xprop xorg-xrandr xorg-xrdb xorg-xset xorg-xwayland xorgproto xvidcore xxhash xz yelp yelp-xsl yyjson zbar zeromq zimg zix zlib-ng zlib-ng-compat zram-generator zsh zsh-autosuggestions zsh-completions zsh-history-substring-search zsh-syntax-highlighting zsh-theme-powerlevel10k zstd zvbi zxing-cpp";

        if (std::system(packageCommand.c_str()) != 0) {
            printError("Package installation failed");
            return false;
        }

        printStatus("Package installation completed");
        return true;
    }

    bool configureSystem() {
        printSection("Step 3: System Configuration");

        printInfo("Configuring GRUB bootloader...");
        if (std::system("sudo cp -r /home/$USER/vanillaarch-to-cachyos/install-fullkde-grub/grub /etc/default") != 0) {
            printError("Failed to copy GRUB configuration");
            return false;
        }
        printStatus("GRUB configuration copied");

        printInfo("Generating new GRUB configuration...");
        if (std::system("sudo grub-mkconfig -o /boot/grub/grub.cfg") != 0) {
            printError("Failed to generate GRUB configuration");
            return false;
        }
        printStatus("GRUB configuration updated");

        printInfo("Setting Plymouth boot animation...");
        if (std::system("sudo plymouth-set-default-theme -R cachyos-bootanimation") != 0) {
            printError("Failed to set Plymouth theme");
            return false;
        }
        printStatus("Plymouth theme configured");

        printInfo("Configuring Fish shell...");
        if (std::system("mkdir -p /home/$USER/.config/fish") != 0 ||
            std::system("cp -r /home/$USER/vanillaarch-to-cachyos/install-fullkde-grub/config.fish /home/$USER/.config/fish/config.fish") != 0 ||
            std::system("cp -r /home/$USER/vanillaarch-to-cachyos/install-fullkde-grub/.zshrc /home/$USER/.zshrc") != 0 ||
            std::system("sudo chmod +x /home/$USER/.config/fish/config.fish") != 0 ||
            std::system("chsh -s $(which fish)") != 0) {
            printError("Failed to configure Fish shell");
        return false;
            }
            printStatus("Fish configuration applied");

            return true;
    }

    void runMigration() {
        displayBanner();

        // Warning message
        std::cout << YELLOW << BOLD;
        std::cout << "WARNING: This script will perform major system modifications including:" << std::endl;
        std::cout << "• Adding CachyOS repositories" << std::endl;
        std::cout << "• Installing 1000+ packages" << std::endl;
        std::cout << "• Modifying bootloader configuration" << std::endl;
        std::cout << "• Changing system themes and configurations" << std::endl;
        std::cout << NC;
        std::cout << RED << BOLD << "Ensure you have backups and understand the risks before proceeding!" << NC << std::endl;
        std::cout << std::endl;

        std::string confirm;
        std::cout << "Do you want to continue? (yes/no): ";
        std::cin >> confirm;

        if (confirm != "yes") {
            printError("Installation cancelled by user.");
            return;
        }

        printSection("Starting CachyOS Conversion Process");

        if (!checkVirtManager()) return;
        if (!setupCachyosRepos()) return;
        if (!installPackages()) return;
        if (!configureSystem()) return;

        // Cleanup
        std::system("sudo rm -rf /home/$USER/vanillaarch-to-cachyos");

        printSection("CachyOS Conversion Complete!");
        std::cout << GREEN << BOLD;
        std::cout << "Conversion to CachyOS has been completed successfully!" << std::endl;
        std::cout << "Please reboot your system to apply all changes." << std::endl;
        std::cout << NC;
    }
};

#endif
