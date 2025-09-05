# GuimicK

A set of tools to schedule and run recurrent tasks on your computer, even on your absence.
The usages that motivated it were :
- Running backups periodically and automatically
- Running system updates
- Running antivirus scans
- Having a report sent to the user detailing what happened to each task after each run

## Usage

A tutorial is available on how to download/install it on Windows : 
https://mickaelcguimaraes.wixsite.com/guimick/services

## Dependencies
- [CMake](https://cmake.org/download/) (≥ 3.10 recommended)
- [Qt](https://www.qt.io/download) (≥ 6, Qt Widgets module required)
- A C++17-compatible compiler (e.g., GCC ≥ 9, MSVC ≥ 2019, Clang ≥ 10)

## How to build it

1. Open a terminal and run:

```bash
git clone --recursive https://github.com/MickaelOnTheWave/GuimicK.git
```

GuimicK uses several submodules. You need the --recursive option to clone them too.

2. Generate the build files and compile it :

```bash
cd GuimicK
mkdir build
cd build
cmake ../
cmake --build .
```

This will compile all the tools necessary to run GuimicK.
GuimicK uses Cmake and depends on QT. Make sure you have them installed.

3. Ready to be used!

In your build directory, you will find three folders with executables :

- `ConfigurationEditingTool` — GUI tool to define and test tasks

  This is the tool you will use to define the tasks you want to run, configure them and schedule them.
  You can also run them to test the output.
  It is a Graphical Interface and it will create the configuration that will be used by the background tool.
  This is an easy and intuitive way to create the configuration file that defines your tasks, but you can also
  write the configuration file yourself in a text editor if you prefer.

- `TaskTool` — background runner (add to scheduler/cron)

  This is the tool that runs in the background and executes the tasks you configured. You can run it manually, and this is
  the tool you will need to configure in your scheduler to run your tasks periodically.
  
- `TaskToolLauncher` — Windows-only helper for admin privileges

  It is used internally on Windows to handle administrator priviledges when necessary. You should not need to use it yourself.

## Limitations

### Maintenance
This software is old and has not been maintained for some years. It has only recently been uploaded to GitHub and migrated to Cmake.
It as been untested for a while and some features might be broken.

### Feedback to user
It was originally developed around the idea of sending an email to the user with a report of all tasks. Although the code still works, today
most email providers (at least gmail, which I used at the time) don't allow anymore external SMTP access. As such, the email reporting feature is now
unusable for most users.
A Telegram bot has been added later, but I didn't finish the development of it. It has some basic features but full production runs were not tested.

### Automated tests
There were several automated tests developed around QTest. I migrated some of them to Catch2 and moved to test/ folders, however all of the main tests
on external backup services haven't been migrated yet. Their code is still present in oldSrc/ and oldInclude/ for a future migration.

## Platforms

It was first designed to run on Linux, and then generalized for Windows. Mac is untested.
It runs on both platforms, although not all features are available everywhere :

Tested on Windows only :
- Setting scheduled runs directly from the configuration tool

Tested on Linux only :
- Waking remote machines and taking control of them
- Shutting down automatically


