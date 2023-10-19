# Step 1: Install WSL and a "distro"

## Enable WSL

Windows Features -> Windows Subsystem for Linux

or using powershell:

    dism.exe /online /enable-feature /featurename:Microsoft-Windows-Subsystem-Linux /all

## WSL 2 vs WSL 1

### WSL 2 drawbacks

- It does not map serial "COM" ports to /dev/ttyS* and there is no
simple workaround as for openocd (pyterm is not working in python for
windows).
- It depends on hardware virtualisation so I could not use it in the
windows vm that I installed.

### WSL 1 drawback

- 32bits binary is not working (but you can use qemu as workaround).
- tap driver is not working (so you can not use "native" board for
networking apps).

### The choice

If you need full "native" board support, you must use WSL 2. You can surely
find a windows serial terminal app to replace pyterm, socat.exe from
cygwin for example. You can also try to passthrough USB from windows to
linux: http://usbip.sourceforge.net/ so there would be no need to
workaround anything other.

If you only want to provide a way to build your RIOT project to windows
users, WSL 1 is OK.

## Set the default install version to WSL 1

Run cmd.exe as administrator then issue the command:

    C:\Windows\system32>wsl --set-default-version 1

If you plane to use WSL 2 for future "distro" installation, you can
revert back after install with:

    C:\Windows\system32>wsl --set-default-version 2

If you want to use an already installed WSL 2 distro, you can convert it
with:

    C:\Windows\system32>wsl --set-version <Distro> 1

## install a "distro" from windows store

Choose Ubuntu 20.04 or Debian (lightweight).

The distro language is english whatever that of windows, you can change
it with:

    sudo dpkg-reconfigure locales

Before anything else you need to update apt:

    sudo apt update

### If you have chosen debian 

As debian distro is very minimal you can install vim and bash-completion
:
    sudo apt install vim bash-completion

then uncomment the relevent lines in `/etc/bash.bashrc` to enable
bash completion:

    # enable bash completion in interactive shells
    if ! shopt -oq posix; then
      if [ -f /usr/share/bash-completion/bash_completion ]; then
        . /usr/share/bash-completion/bash_completion
      elif [ -f /etc/bash_completion ]; then
        . /etc/bash_completion
      fi
    fi

to enable it immediately you can do:

    source /etc/bash_completion

# Step 2: setup

## in short

Run the following commands in your WSL terminal:

    sudo dpkg --add-architecture i386
    sudo apt update
    sudo apt -y dist-upgrade
    sudo apt -y install libc6-dev-i386 libc6-dbg:i386 build-essential \
    pkg-config uml-utilities bridge-utils git unzip gcc-arm-none-eabi \
    python3-serial wget p7zip
    wget https://sysprogs.com/getfile/1180/openocd-20200729.7z
    p7zip -d openocd-20200729.7z 
    sudo mv OpenOCD-20200729-0.10.0 /opt/openocd_pe32
    sudo chmod +x /opt/openocd_pe32/bin/openocd.exe 
    sudo chmod 777 /dev/ttyS*
    echo "export OPENOCD=/opt/openocd_pe32/bin/openocd.exe" >> .bashrc
    echo "export PORT_LINUX=/dev/ttyS3" >> .bashrc
    echo "export DISPLAY=:0" >> .bashrc
    source .bashrc
    ln -s /mnt/c`cmd.exe /c "echo %homepath%" 2>/dev/null | tr -d "\r\n" | tr '\\\\' '/'`/Desktop

Download and install the windows st-link proprietary driver from
`https://www.st.com/en/development-tools/stsw-link009.htm`

Identify the COM port of your stlink in Windows Device Manager (COM3 in
my case) then replace `export PORT_LINUX=/dev/ttyS3` by the good one in
`/home/<username>/.bashrc`.

## in detail

### Enable multiarch support for RIOT native architecture

    sudo dpkg --add-architecture i386 && apt update

### Install dependencies:

    sudo apt install libc6-dev-i386 libc6-dbg:i386 build-essential \
    pkg-config uml-utilities bridge-utils git unzip gcc-arm-none-eabi \
    python3-serial wget

### OpenOCD

WSL do not support USB so we must install the windows (pe32) binary
insteed of the Linux (elf) one:

    sudo apt install p7zip
    wget https://sysprogs.com/getfile/1180/openocd-20200729.7z
    p7zip -d openocd-20200729.7z
    sudo mkdir /opt
    sudo mv OpenOCD-20200729-0.10.0 /opt/openocd_pe32
    sudo chmod +x /opt/openocd_pe32/bin/openocd.exe
    echo "export OPENOCD=/opt/openocd_pe32/bin/openocd.exe" >> .bashrc
    source .bashrc

Then download and install the windows st-link proprietary driver from
`https://www.st.com/en/development-tools/stsw-link009.htm`

### pyterm serial terminal

First you must identify the COM port of your stlink in Windows Device
Manager (COM3 in my case). Then you can set the PORT_LINUX environement
variable to the corresponding linux device (`/dev/ttyS3` for COM3 in my
case):

    sudo chmod 777 /dev/ttyS*
    echo "export PORT_LINUX=/dev/ttyS3" >> .bashrc
    source .bashrc

## native (32bit elf) support

    sudo apt install qemu-user-static
    sudo update-binfmts --install i386 /usr/bin/qemu-i386-static --magic '\x7fELF\x01\x01\x01\x03\x00\x00\x00\x00\x00\x00\x00\x00\x03\x00\x03\x00\x01\x00\x00\x00' --mask '\xff\xff\xff\xff\xff\xff\xff\xfc\xff\xff\xff\xff\xff\xff\xff\xff\xf8\xff\xff\xff\xff\xff\xff\xff'

## GUI apps

To be able to use GUI apps from WSL you must install an X11 server in
windows as vcxsrv or xming.

`https://sourceforge.net/projects/vcxsrv/files/latest/download`

Install it and run XLaunch, clic [Next] two times, then in the "Extra
settings" dialog check "Disable access control".

You must also set the DISPLAY environnement variable in WSL :

    echo "export DISPLAY=:0" >> .bashrc
    source .bashrc

Now you can install and run GUI apps from WSL.

## Auto-start VcXsrv with WSL

Save configuration from XLaunch to
`\\wsl$\Ubuntu-20.04\home\<username>\.config.xlaunch`

Add the following lines to your `.bashrc`:
    if ! xset q &>/dev/null; then
        /mnt/c/Program\ Files/VcXsrv/xlaunch.exe -run .config.xlaunch
    fi

## Accessing windows storage from WSL

Windows storage is mounted on `/mnt/c`. You can add a symlink to your
windows desktop :

    ln -s /mnt/c`cmd.exe /c "echo %homepath%" 2>/dev/null | tr -d "\r\n" | tr '\\\\' '/'`/Desktop

WARNING: do not use windows storage for source tree, git, etc.

## Accessing WSL storage from windows

Distro's filesystem is accessible as a network share at `\\wsl$`
