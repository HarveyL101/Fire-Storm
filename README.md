# Operation Fire Storm Solution

## Brief Description

This program listens on port 5733 for incoming traffic, forwards the non-CTFS payloads to port 5744 (untouched). CTFS relevant traffic is removed from the stream for sequence-order construction into a whole PDF file.

## Building and Running the Program

Building and running the program can be achieved in multiple ways, depending on your Desktop setup, if unsure this can be found by running `cat /etc/os-release` in your terminal (Linux only).

1. [Prerequisites](#prerequisites)
2. [Ubuntu 24.04 Systems](#running-on-ubuntu-2404)
3. [Non Ubuntu 24.04 Systems](#running-on-non-ubuntu-distros-using-podman)
4. [Windows-based Systems](#running-on-windows)

### Prerequisites [#](#building-and-running-the-program)

Ensure that all necessary dependencies of the program are installed and verified. These softwares are critical in compiling the program, as well as creating useful abstractions (make) to simplify the process of compiling & running the project.

Be sure to download the test script from CoreTech @[Operation FIRESTORM](https://github.com/CoreTechSecurity/fire-storm)
The following commands will require a terminal window to carry out.

#### Ubuntu 24.04 Example
```bash
sudo apt update
sudo apt install -y build-essential make

g++ --version
make --version
```

#### Non Ubuntu Example (Using `dnf` package manager)
```bash
sudo dnf install gcc-c++ make

gcc --version
g++ --version
make --version
```

#### Containerisation installation (Using `podman` on non-Ubuntu systems)
```bash
dnf install podman

podman --version
```

### Running on Ubuntu 24.04 [#](#building-and-running-the-program)

To build and run the project on the target Linux distribution (Ubuntu 24.04), follow these steps:

1. Open a terminal and navigate to the project directory via `cd <path_to_directory>`, if using Visual Studio Code (VSC) this can be done by pressing `<Ctrl + J>`.
```bash
cd <path_to_directory>
```
2. In the terminal, run the command `make` to compile the program.
```bash
make
```
3. Following this, using `make run` the program will begin to listen for incoming traffic.
```bash
make run
```
4. From here the test script can be run in a new terminal to start the flow of network traffic, being processed by the program. After a brief pause the test script will exit complete once all traffic has been sent.
5. View the constructed pdf file labelled `transmission.pdf` within the generated `build/` folder.

### Running on Non-Ubuntu Distros (Using Podman) [#](#building-and-running-the-program)

> [!IMPORTANT]
> The following code snippets are written in the context of a Fedora distro (dnf package manager), if using a different package manager some commands may require tweaking to adapt to different software environments.

To build and run the project on a Linux distribution external to Ubuntu 24.04, containerisation can be used to emulate the target environment. The following example uses [podman](https://podman.io/) to achieve this effect, but feel free to use any containerisation software you are confident with.

> [!NOTE]
> This program has only been tested with podman, so success cannot be guaranteed with other softwares adjacent to this.

1. Open a terminal window and navigate to the project directory on your system using `cd <path_to_directory>`.
```bash
# E.g. cd ~/Documents/Github/FIRE_STORM
cd <path_to_directory>
```
2. Create and run the Ubuntu 24.04 container using `container.sh`.
    1. Use the premade command `make container` in the terminal to begin emulating the Ubuntu dev environment. This script will also install the necessary toolchain versions for proper use if necessary.
    2. A common error faced when attemping this is "permission denied". Overcome this by permitting execution of the target file (assuming *step 1* has been followed):
```bash
# Permits execution on the target file.
chmod +x container.sh
```
3. Once installation is complete, run both `make clean` and `make` to compile the program, the `firestorm.exe` output is stored in the generated `/build` folder.
```bash
make clean
```
```bash
make
```
4. Run the program using `make run`. Running the test script in a new terminal after this will produce a complete and valid PDF file name `transmission.pdf` in your build folder.
```bash
# To run the FIRE_STORM program. 
make run
```
```bash
# To run the test.py file.
python3 test.py
```

### Running on Windows. [#](#building-and-running-the-program)

Since this program utilises [POSIX](https://en.wikipedia.org/wiki/POSIX) System headers e.g. `sys/socket.h`, a family of standards designed to increase portability across [Unix](https://en.wikipedia.org/wiki/Unix)-like Operating Systems. Window's inadherence to these standards will warrants extra support to get it running.

Since the [Operation FIRESTORM](https://www.coretech-security.com/take-the-challenge/operation-fire-storm/) spec specifically asks for a Ubuntu-24 friendly solution, this lack of compatibility is intentional. However this can be overcome via tools such as the Windows Subsystem for Linux ([WSL 2](https://learn.microsoft.com/en-us/windows/wsl/install)) or the [MSYS2](https://www.msys2.org/) platform to try and circumvent this shortcoming.

> [!WARNING]
> These alternatives for Non-POSIX compliant OS's have not been tested and may be subject to unexpected behaviour.

## Verifying Output Integrity

Since the output is a valid PDF file, any PDF viewing software should (in theory) be a valid option to check the integrity of the file. 

I personally used [Okular](https://okular.kde.org/en-gb/), which displayed the contents perfectly. In the event that a faulty PDF is produced, the file should not be openable by PDF viewing media.

Since PDF viewers can be tolerant to poorly constructed PDFs, running the following command on your `transmission.pdf` file will more securely flag whether there are any errors in construction.
```bash
pdfinfo build/transmission.pdf
```

## Design decisions and tradeoffs

C++20 was selected to increase the stability of the system, ensuring that compiler default behaviours did not interfere (E.g. Fedora's gcc v16 & Ubuntu's v13) with the software whilst also benefiting from a modern & vast toolkit to build upon.

In adherence to the problem spec's rule of 'No third-party libraries' external to the language's standard library, it has been assumed that the POSIX `sys/socket.h` header is permissible for its critical nature to C++ network programming.

A more detailed description of design decisions can be found in the [Design.md](/docs/Design.md) file.

## Known Limitations

* Handles one connection, although the mission references "multiple unverified sources".
* The program assumes non-CTFS traffic is Ethernet/IPv4
* The highest sequence number is treated as the end marker.
* The output filename is fixed and overwritten on re-compilation.
* Packet upper bounds are set to 64KB + header sizes.

## AI/LLM Declaration

Claude was used throughout this development cycle as a review and debugging tool. This mainly included explanation during the analysis of the network traffic and packet structure. This tool was also used to confirm thoughts and ideas surrounding system architecture, applying guard-rails that ensure I was not digging myself a hole that would cripple the scalability/ performance of the solution.

Additionally, smaller peripheral parts of the project were aided by AI, my primary example of this is using containerisation to test the project works on Ubuntu-24.04 since I am building this on Fedora. These areas where I knew exactly what i wanted to accomplish with the software but did not want to waste time on learning the syntax (E.g. a four line Containerfile and a `podman` cmd with 10 options) AI was used to bridge that gap and *accelerate* my progress, not be the main driving force.

It also identified specific bugs in my drafts and suggested short example snippets, such as the `PacketFramer` enum-return value, which i adapted and tested before implementing.

## Conclusion/ Closing Comments

Although pressured by exterior concerns (Work, other applications etc.), I thoroughly enjoyed this challenge and how its constraints pushed towards the proper usage of tools to guarantee a positive outcome. I regret that I was not able to complete the 'optional' extras despite enjoying the new ground of using C++ in a networking context, this is something that I would love to incorporate into my future personal projects, thank you very much!

> Author: Harvey Lopez | HarveyLopez101@gmail.com | [Github Account](https://www.github.com/HarveyL101) | [LinkedIn](https://www.linkedin.com/in/harvey-lopez-2009b52b4)