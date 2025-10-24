#ifndef TTYSYSTEMDBOOT_H
#define TTYSYSTEMDBOOT_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>

class TTYSystemdBootMigration {
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
        std::cout << CYAN;
        std::cout << "           claudemods Vanilla Arch Tty Systemd-Boot to CachyOS Tty Systemd-Boot v1.0 24-10-2025" << std::endl;
        std::cout << NC;
        std::cout << "================================================================================" << std::endl;
        std::cout << std::endl;
    }

    bool checkRoot() {
        if (geteuid() == 0) {
            printError("This script should not be run as root. Please run as regular user.");
            return false;
        }
        return true;
    }

    bool confirmProceed() {
        std::cout << YELLOW << BOLD << std::endl;
        std::cout << "WARNING: This script will perform major system modifications including:" << std::endl;
        std::cout << "• Adding CachyOS repositories" << std::endl;
        std::cout << "• Modifying bootloader configuration" << std::endl;
        std::cout << "• Changing system themes and configurations" << std::endl;
        std::cout << NC << std::endl;
        std::cout << RED << BOLD << "Ensure you have backups and understand the risks before proceeding!" << NC << std::endl;
        std::cout << std::endl;

        std::string confirm;
        std::cout << "Do you want to continue? (yes/no): ";
        std::cin >> confirm;

        if (confirm != "yes") {
            printError("Installation cancelled by user.");
            return false;
        }
        return true;
    }

    void removeVirtManager() {
        printSection("Checking for virt-manager");
        if (std::system("pacman -Qi virt-manager &>/dev/null") == 0) {
            printInfo("virt-manager found, removing it...");
            std::system("sudo pacman -Rns --noconfirm virt-manager");
            printStatus("virt-manager removed successfully");
        } else {
            printInfo("virt-manager not installed, proceeding...");
        }
    }

    void setupCachyosRepos() {
        printSection("Step 1: Setting up CachyOS Repositories");
        printInfo("Downloading CachyOS repository package...");
        std::system("curl https://mirror.cachyos.org/cachyos-repo.tar.xz -o cachyos-repo.tar.xz");
        printStatus("Download completed");

        printInfo("Extracting repository files...");
        std::system("tar xvf cachyos-repo.tar.xz && cd cachyos-repo");
        printStatus("Extraction completed");

        printInfo("Running CachyOS repository setup...");
        std::system("sudo ./cachyos-repo.sh");
        printStatus("Repository setup completed");
    }

    void installPackages() {
        printSection("Step 2: Installing Packages");
        printInfo("Starting installation (this will take a while)...");
        printInfo("Please be patient as this process may take 30-60 minutes...");

        std::string packageCommand = "sudo pacman -S --needed a52dec aalib abseil-cpp accountsservice acl adobe-source-han-sans-cn-fonts adobe-source-han-sans-jp-fonts adobe-source-han-sans-kr-fonts adwaita-cursors adwaita-fonts adwaita-icon-theme adwaita-icon-theme-legacy alacritty alsa-card-profiles alsa-firmware alsa-lib alsa-plugins alsa-topology-conf alsa-ucm-conf alsa-utils ananicy-cpp aom archlinux-keyring aribb24 aribb25 at-spi2-core atkmm attr audit autoconf automake avahi awesome-terminal-fonts base base-devel bash bash-completion bat bind binutils bison bluez bluez-hid2hci bluez-libs bluez-utils bpf brotli btop btrfs-progs bubblewrap bzip2 ca-certificates ca-certificates-mozilla ca-certificates-utils cachyos-ananicy-rules cachyos-fish-config cachyos-hello cachyos-hooks cachyos-kernel-manager cachyos-keyring cachyos-micro-settings cachyos-mirrorlist cachyos-packageinstaller cachyos-plymouth-bootanimation cachyos-rate-mirrors cachyos-settings cachyos-v3-mirrorlist cachyos-v4-mirrorlist cachyos-wallpapers cachyos-zsh-config cairo cairomm cairomm-1.16 cantarell-fonts chromaprint chwd cifs-utils clang compiler-rt confuse coreutils cpupower cryptsetup curl dav1d db5.3 dbus dbus-broker dbus-broker-units dbus-units dconf debugedit default-cursors desktop-file-utils device-mapper dhclient diffutils ding-libs dmidecode dmraid dnsmasq dnssec-anchors dosfstools double-conversion duf duktape e2fsprogs efibootmgr efitools efivar ell ethtool exfatprogs expac expat eza f2fs-tools faac faad2 fakeroot fastfetch fd ffmpeg ffmpeg4.4 ffmpegthumbnailer fftw file filesystem findutils firefox fish fish-autopair fish-pure-prompt fisher flac flex fluidsynth fmt fontconfig freeglut freetype2 fribidi fsarchiver ftgl fuse-common fuse3 fzf gawk gc gcc gcc-libs gcr-4 gdbm gdk-pixbuf2 gettext giflib git glances glew glib-networking glib2 glibc glibmm glibmm-2.68 glm glslang glu glycin gmp gnulib-l10n gnupg gnutls gobject-introspection-runtime gperftools gpgme gpgmepp gpm graphene graphite grep groff gsettings-desktop-schemas gsettings-system-schemas gsm gssdp gssproxy gst-libav gst-plugin-pipewire gst-plugins-bad gst-plugins-bad-libs gst-plugins-base-libs gst-plugins-ugly gstreamer gtest gtk-update-icon-cache gtk3 gtk4 gtkmm-4.0 gtkmm3 gtksourceview4 guile gupnp gupnp-igd gzip harfbuzz haveged hdparm hicolor-icon-theme hidapi highway hwdata hwdetect hwinfo iana-etc icu imagemagick imath imlib2 inetutils inxi iproute2 iptables-nft iputils iso-codes iw iwd jack2 jansson jbigkit jemalloc jfsutils jq json-c json-glib kbd kcolorscheme kconfig kcoreaddons kcrash kdbusaddons keyutils kguiaddons ki18n kmod knotifications krb5 kwallet kwidgetsaddons kwindowsystem l-smash lame lcms2 ldb leancrypto less libaio libarchive libass libassuan libasyncns libavc1394 libavtp libb2 libbluray libbpf libbs2b libbsd libcaca libcanberra libcap libcap-ng libcddb libcdio libcloudproviders libcolord libcups libdaemon libdatrie libdc1394 libdca libde265 libdecor libdeflate libdnet libdovi libdrm libdv libdvbpsi libdvdcss libdvdnav libdvdread libebml libebur128 libedit libelf libepoxy libevdev libevent libfdk-aac libffi libfontenc libfreeaptx libftdi libgcrypt libgirepository libgit2 libglvnd libgme libgoom2 libgpg-error libgsf libgudev libice libidn2 libiec61883 libimobiledevice libimobiledevice-glue libinih libinput libisl libjpeg-turbo libjxl libkate libksba liblc3 libldac libldap liblqr liblrdf libltc libmad libmatroska libmaxminddb libmbim libmd libmfx libmicrodns libmm-glib libmnl libmodplug libmpc libmpcdec libmpeg2 libmspack libmtp libmysofa libndp libnetfilter_conntrack libnewt libnfnetlink libnfs libnftnl libnghttp2 libnghttp3 libnice libnl libnm libnma-common libnma-gtk4 libnotify libnsl libnvme libogg liboggz libopenmpt libopenraw libp11-kit libpcap libpciaccess libpgm libpipeline libpipewire libplacebo libplist libpng libproxy libpsl libpulse libqmi libqrtr-glib libraqm libraw1394 librsvg libsamplerate libsasl libseccomp libsecret libshout libsigc++ libsigc++-3.0 libsm libsndfile libsodium libsoup3 libsoxr libsrtp libssh libssh2 libstemmer libsysprof-capture libtasn1 libtatsu libteam libthai libtheora libtiff libtiger libtirpc libtool libunibreak libunistring libunwind libupnp liburcu liburing libusb libusb-compat libusbmuxd libuv libva libvdpau libverto libvlc libvorbis libvpl libvpx libwacom libwbclient libwebp libwireplumber libwnck3 libx11 libx86emu libxau libxcb libxcomposite libxcrypt libxcursor libxcvt libxdamage libxdmcp libxext libxfixes libxfont2 libxft libxi libxinerama libxkbcommon libxkbcommon-x11 libxkbfile libxml2 libxmu libxrandr libxrender libxres libxshmfence libxslt libxss libxt libxtst libxv libxxf86vm licenses lilv linux-api-headers linux-cachyos linux-cachyos-headers linux-cachyos-lts linux-cachyos-lts-headers linux-firmware linux-firmware-amdgpu linux-firmware-atheros linux-firmware-broadcom linux-firmware-cirrus linux-firmware-intel linux-firmware-mediatek linux-firmware-nvidia linux-firmware-other linux-firmware-radeon linux-firmware-realtek linux-firmware-whence lirc live-media lld llhttp llvm llvm-libs lm_sensors lmdb logrotate lsb-release lsscsi lua lv2 lvm2 lz4 lzo m4 mailcap make man-db man-pages md4c mdadm meld mesa mesa-utils micro mjpegtools mkinitcpio mkinitcpio-busybox mobile-broadband-provider-info modemmanager mpdecimal mpfr mpg123 mtdev mtools nano nano-syntax-highlighting ncurses neon netctl nettle networkmanager networkmanager-openvpn networkmanager-vpn-plugin-openvpn nfs-utils nfsidmap nftables nilfs-utils noto-color-emoji-fontconfig noto-fonts noto-fonts-cjk noto-fonts-emoji npth nspr nss nss-mdns ntp numactl ocl-icd octopi oh-my-zsh-git oniguruma open-vm-tools openal opencore-amr opendesktop-fonts openexr openh264 openjpeg2 openssh openssl openvpn opus orc p11-kit pacman pacman-contrib pacman-mirrorlist pacutils pahole pam pambase pango pangomm pangomm-2.48 parallel paru patch pavucontrol pciutils pcre pcre2 pcsclite perl perl-clone perl-encode-locale perl-error perl-file-listing perl-html-parser perl-html-tagset perl-http-cookiejar perl-http-cookies perl-http-daemon perl-http-date perl-http-message perl-http-negotiate perl-io-html perl-libwww perl-lwp-mediatypes perl-mailtools perl-net-http perl-timedate perl-try-tiny perl-uri perl-www-robotrules perl-xml-parser perl-xml-writer pinentry pipewire pipewire-alsa pipewire-audio pipewire-pulse pixman pkcs11-helper pkgconf pkgfile plocate plymouth polkit poppler poppler-glib popt portaudio power-profiles-daemon powerline-fonts ppp procps-ng projectm protobuf psmisc pv python python-annotated-types python-cairo python-defusedxml python-gobject python-orjson python-packaging python-psutil python-pydantic python-pydantic-core python-typing-inspection python-typing_extensions python-wxpython qca-qt6 qemu-guest-agent qrencode qt-sudo qt6-5compat qt6-base qt6-declarative qt6-multimedia qt6-multimedia-ffmpeg qt6-shadertools qt6-translations qtermwidget raptor rate-mirrors rav1e readline rebuild-detector reflector ripgrep rpcbind rsync rtkit rtmpdump rubberband run-parts s-nail sbc scx-manager scx-scheds sd sdl12-compat sdl2-compat sdl3 sdl_image sed serd sg3_utils shaderc shadow shared-mime-info slang smartmontools smbclient snappy sof-firmware sord sound-theme-freedesktop soundtouch spandsp spdlog speex speexdsp spice-vdagent spirv-tools sqlite sratom srt startup-notification sudo svt-av1 svt-hevc sysfsutils systemd systemd-boot-manager systemd-libs systemd-resolvconf systemd-sysvcompat taglib talloc tar tcl tdb tealdeer tevent texinfo thin-provisioning-tools tinysparql tpm2-tss tslib ttf-bitstream-vera ttf-dejavu ttf-fantasque-nerd ttf-liberation ttf-meslo-nerd ttf-opensans twolame tzdata ufw unrar unzip upower uriparser usb_modeswitch usbutils util-linux util-linux-libs v4l-utils vapoursynth vi vid.stab vim vim-runtime virtualbox-guest-utils vlc-plugin-a52dec vlc-plugin-aalib vlc-plugin-alsa vlc-plugin-aom vlc-plugin-archive vlc-plugin-aribb24 vlc-plugin-aribb25 vlc-plugin-ass vlc-plugin-avahi vlc-plugin-bluray vlc-plugin-caca vlc-plugin-cddb vlc-plugin-chromecast vlc-plugin-dav1d vlc-plugin-dbus vlc-plugin-dbus-screensaver vlc-plugin-dca vlc-plugin-dvb vlc-plugin-dvd vlc-plugin-faad2 vlc-plugin-ffmpeg vlc-plugin-firewire vlc-plugin-flac vlc-plugin-fluidsynth vlc-plugin-freetype vlc-plugin-gme vlc-plugin-gnutls vlc-plugin-gstreamer vlc-plugin-inflate vlc-plugin-jack vlc-plugin-journal vlc-plugin-jpeg vlc-plugin-kate vlc-plugin-kwallet vlc-plugin-libsecret vlc-plugin-lirc vlc-plugin-live555 vlc-plugin-lua vlc-plugin-mad vlc-plugin-matroska vlc-plugin-mdns vlc-plugin-modplug vlc-plugin-mpeg2 vlc-plugin-mpg123 vlc-plugin-mtp vlc-plugin-musepack vlc-plugin-nfs vlc-plugin-notify vlc-plugin-ogg vlc-plugin-opus vlc-plugin-png vlc-plugin-pulse vlc-plugin-quicksync vlc-plugin-samplerate vlc-plugin-sdl vlc-plugin-sftp vlc-plugin-shout vlc-plugin-smb vlc-plugin-soxr vlc-plugin-speex vlc-plugin-srt vlc-plugin-svg vlc-plugin-tag vlc-plugin-theora vlc-plugin-twolame vlc-plugin-udev vlc-plugin-upnp vlc-plugin-vorbis vlc-plugin-vpx vlc-plugin-x264 vlc-plugin-x265 vlc-plugin-xml vlc-plugin-zvbi vlc-plugins-all vlc-plugins-base vlc-plugins-extra vlc-plugins-video-output vlc-plugins-visualization vmaf vulkan-icd-loader vulkan-mesa-device-select vulkan-virtio wayland webrtc-audio-processing-1 wget which wildmidi wireless-regdb wireplumber wpa_supplicant wxwidgets-common wxwidgets-gtk3 x264 x265 xcb-proto xcb-util xcb-util-cursor xcb-util-image xcb-util-keysyms xcb-util-renderutil xcb-util-wm xdg-user-dirs xdg-utils xf86-input-libinput xf86-input-vmmouse xfsprogs xkeyboard-config xl2tpd xmlsec xorg-fonts-encodings xorg-server xorg-server-common xorg-setxkbmap xorg-xauth xorg-xdpyinfo xorg-xinit xorg-xinput xorg-xkbcomp xorg-xkill xorg-xmodmap xorg-xprop xorg-xrandr xorg-xrdb xorg-xset xorgproto xvidcore xxhash xz yyjson zbar zeromq zimg zix zlib-ng zlib-ng-compat zram-generator zsh zsh-autosuggestions zsh-completions zsh-history-substring-search zsh-syntax-highlighting zsh-theme-powerlevel10k zstd zvbi zxing-cpp";

        std::system(packageCommand.c_str());
        printStatus("Package installation completed");
    }

    void configureSystem() {
        printSection("Step 3: System Configuration");

        printInfo("Setting Plymouth boot animation...");
        std::system("sudo plymouth-set-default-theme -R cachyos-bootanimation");
        printStatus("Plymouth theme configured");

        printInfo("Configuring Fish shell...");
        std::system("mkdir -p /home/$USER/.config/fish");
        std::system("cp -r /home/$USER/vanillaarch-to-cachyos/install-fullkde-grub/config.fish /home/$USER/.config/fish/config.fish");
        std::system("cp -r /home/$USER/vanillaarch-to-cachyos/install-fullkde-grub/.zshrc /home/$USER/.zshrc");
        std::system("sudo chmod +x /home/$USER/.config/fish/config.fish");
        std::system("chsh -s $(which fish)");
        printStatus("Fish configuration applied");
    }

    void runMigration() {
        displayBanner();

        if (!checkRoot()) return;
        if (!confirmProceed()) return;

        removeVirtManager();
        setupCachyosRepos();
        installPackages();
        configureSystem();

        printSection("CachyOS Conversion Complete!");
        std::cout << GREEN << BOLD;
        std::cout << "Conversion to CachyOS has been completed successfully!" << std::endl;
        std::system("sudo rm -rf /home/$USER/vanillaarch-to-cachyos");
        std::cout << "Please reboot your system to apply all changes." << std::endl;
        std::cout << NC;
    }
};

#endif
