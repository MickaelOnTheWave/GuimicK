# GuimicK

A set of tools to schedule and run recurrent tasks on your computer, even on your absence.
Originally designed to automate backups, GuimicK has been extended to support system maintenance, remote management, and more.


## ✨ Features

- Schedule and run recurring tasks automatically
- Generate execution reports (stored locally or sent to the user)
- Control both local and remote machines
- Graphical interface for configuration
- Cross-platform: Linux and Windows

### Supported Task Types

- File backups (raw copy, rsync, zip/tar, and more)
- Git repository backups
- Disk checks
- Custom command-line commands
- Remote commands via SSH
- Wake-on-LAN for remote machines
- Automatic shutdown (local or remote)


### Configuration Modes

A configuration file defines which tasks run and where:

- **Standalone** — runs tasks locally on the same machine  
- **Server** — runs tasks on a remote client machine  
- **Client** — exposes this machine to be controlled by a remote server

When creating a new configuration in the GUI tool, you will be prompted to choose one of these modes.

## ⚙️ Tools

GuimicK provides two executables:

- **`TaskTool`** — background runner  
  Executes tasks as defined in a configuration file.  
  Can be run manually, or added to your scheduler (Windows Task Scheduler / Linux `cron`).  

- **`ConfigurationEditingTool`** — graphical configuration editor  
  Provides an intuitive way to define tasks, test them, and generate configuration files.  
  (You may also edit configuration files manually if preferred.)


## 🖥️ Platform Notes

### 🪟 Windows
- Only **Standalone** mode tested  
- Tutorial available: [Installation guide](https://mickaelcguimaraes.wixsite.com/guimick/services)

### 🐧 Linux
- All modes tested (Standalone, Server, Client)  
- No direct GUI integration with `cron` — add entries manually  
- After building, executables are located in:
  - `TaskTool/` → command-line runner
  - `ConfigurationEditingTool/` → GUI editor


## Dependencies
- [CMake](https://cmake.org/download/) (≥ 3.22)
- [Qt](https://www.qt.io/download) (≥ 6, Qt Widgets module required)
- A C++17-compatible compiler (e.g., GCC ≥ 9, MSVC ≥ 2019, Clang ≥ 10)

## How to build it

1. Clone the repository with submodules::

```bash
git clone --recursive https://github.com/MickaelOnTheWave/GuimicK.git
```

2. Configure and build :

```bash
cd GuimicK
mkdir build
cd build
cmake ../
cmake --build .
```

3. Run from the build directory:

- `ConfigurationEditingTool` — GUI configuration editor

- `TaskTool` — background runner
  

## ⚠️ Limitations

- `Maintenance`- The project was dormant for several years; some features may be broken.
- `Feedback to user`- Email reporting is mostly unusable due to modern SMTP restrictions. A Telegram bot was partially implemented but not fully tested.
- `Automated tests`- Unit tests exist (Catch2, some migrated from QTest), but external backup service tests are still in oldSrc/ and oldInclude/.

## 🤝 Contributing
GuimicK was designed to make it easy to add new task types.  
If you’d like to extend it, feel free to fork the repository and submit a pull request!
